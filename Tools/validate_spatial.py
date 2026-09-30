#!/usr/bin/env python3
"""Vérifie GameData/missions/01/spatial_requirements.json contre la mission et la maquette 3D.

Contrôles :
  1. chaque zone et chaque repère existent dans la maquette (manifeste de Codex s'il est présent
     dans le dépôt, sinon la liste recopiée dans blockout_ref) ;
  2. chaque élément référence un ID connu de la mission, sans doublon ;
  3. la zone et le coût en temps concordent avec les données de mission (source unique de vérité) ;
  4. chaque élément déclare les trois vues du lieu où il se joue (plan A place / plan B ruelle) ;
     un élément commun aux deux lieux est déclaré `transversal`, avec sa justification ;
     tout élément obligatoire possède un repli spatial valable ;
  5. couverture : toutes les interactions, toutes les actions du prologue et tous les indices
     de fenêtre sont décrits.

Utilisation : python3 Tools/validate_spatial.py [dossier_de_mission] [--manifest CHEMIN | --commit REF]
  --commit REF lit le manifeste de la PR #3 dans git (ex. origin/codex/implantation-ouverture).
"""
from __future__ import annotations

import json
import subprocess
import sys
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
ROOT = TOOLS.parent
sys.path.insert(0, str(TOOLS))

import validate_mission as vm  # noqa: E402

MANIFEST = ROOT / "Game" / "Blockout" / "ouverture-centre-ville.manifest.json"
FALLBACK_TYPES = {"evenement_rattrapage", "indice_equivalent", "toujours_accessible", "partout", "automatique"}
KINDS = {"evenement", "action", "indice", "interaction"}
REQUIRED = ("id", "kind", "zone_id", "marker_id", "camera_independent", "fallback", "time_cost")


MANIFEST_PATH = "Game/Blockout/ouverture-centre-ville.manifest.json"


def manifest_from_git(ref: str) -> dict | None:
    """Manifeste de la maquette tel qu'il est dans le commit ou la branche `ref`, ou None s'il est introuvable."""
    try:
        out = subprocess.run(["git", "show", f"{ref}:{MANIFEST_PATH}"], cwd=ROOT, capture_output=True, text=True, check=True)
    except (OSError, subprocess.CalledProcessError):
        return None
    return json.loads(out.stdout)


def manifest_sets(man: dict) -> tuple[set[str], set[str], set[str], dict[str, set[str]]]:
    plans = {sh["id"]: set(sh["camera_nodes"]) for sh in man["validation"].get("shots", [])}
    return ({z["id"] for z in man["zones"]}, {e["id"] for e in man["event_markers"]},
            set(man["validation"]["camera_nodes"]), plans)


def blockout_sets(spec: dict, manifest: Path | dict, r: vm.Report):
    ref = spec["blockout_ref"]
    zones, markers, cams = set(ref["zones"]), set(ref["markers"]), set(ref["cameras"])
    plans = {pid: set(p["cameras"]) for pid, p in ref.get("plans", {}).items()}
    if not ref.get("commit"):
        r.err("[maquette] blockout_ref sans commit de référence de la PR #3")
    man = manifest if isinstance(manifest, dict) else (
        json.loads(manifest.read_text(encoding="utf-8")) if manifest.exists() else None)
    if man is not None:
        mz, mm, mc, mp = manifest_sets(man)
        if (mz, mm, mc, mp) != (zones, markers, cams, plans):
            r.err("[maquette] blockout_ref ne correspond plus au manifeste de la maquette : "
                  f"zones {sorted(mz ^ zones)}, repères {sorted(mm ^ markers)}, caméras {sorted(mc ^ cams)}, "
                  f"plans {sorted(set(mp) ^ set(plans)) or 'caméras des plans différentes' if mp != plans else '—'}")
        zones, markers, cams, plans = mz, mm, mc, mp
    else:
        r.info.append(f"manifeste de maquette absent de cette branche : contrôle sur la copie de blockout_ref (PR #3, {ref.get('commit')})")
    return zones, markers, cams, plans


def validate(mission_dir: Path, manifest: Path | dict = MANIFEST) -> vm.Report:
    r = vm.Report()
    m = vm.Mission(mission_dir)
    spec = json.loads((mission_dir / "spatial_requirements.json").read_text(encoding="utf-8"))
    zones, markers, cams, plans = blockout_sets(spec, manifest, r)
    ref_plans = spec["blockout_ref"].get("plans", {})
    zone_plan = {z: pid for pid, p in ref_plans.items() for z in p["zones"]}
    for z in zones - set(zone_plan):
        r.err(f"[plans] zone {z} rattachée à aucun plan (place ou ruelle)")

    mission_zones = set(m.zones)
    if mission_zones != zones:
        r.err(f"[zones] locations.json et la maquette diffèrent : {sorted(mission_zones ^ zones)}")

    known = set(m.interactions) | set(m.actions) | set(m.clues) | set(m.events) | set(m.deductions)
    known |= set(m.meta["prologue"]["events"])
    deadline = vm.hm(m.meta["clock"]["deadline"])
    seen: set[str] = set()

    for el in spec["elements"]:
        eid = el.get("id", "?")
        missing = [k for k in REQUIRED if k not in el]
        if missing:
            r.err(f"[format] {eid} : champ(s) manquant(s) {missing}")
            continue
        if eid in seen:
            r.err(f"[format] {eid} décrit deux fois")
        seen.add(eid)
        if el["kind"] not in KINDS:
            r.err(f"[format] {eid} : type {el['kind']} inconnu")
        if eid not in known:
            r.err(f"[ids] {eid} n'existe pas dans la mission")
        if el["zone_id"] not in zones:
            r.err(f"[maquette] {eid} : zone inconnue {el['zone_id']}")
        if el["marker_id"] is not None and el["marker_id"] not in markers:
            r.err(f"[maquette] {eid} : repère inconnu {el['marker_id']}")
        for c in el.get("cameras", []):
            if c not in cams:
                r.err(f"[caméras] {eid} : caméra inconnue {c}")
        plan = zone_plan.get(el["zone_id"])
        if plan in plans:
            if el.get("transversal"):
                if set(el.get("cameras", [])) != cams:
                    r.err(f"[caméras] {eid} : élément transversal, il doit déclarer les vues des deux lieux")
            elif set(el.get("cameras", [])) != plans[plan]:
                r.err(f"[caméras] {eid} : vues déclarées {sorted(el.get('cameras', []))} ≠ trois vues de son lieu "
                      f"({plan} : {sorted(plans[plan])}) ; un élément commun aux deux lieux doit être marqué transversal")

        # concordance avec les données de mission
        if eid in m.interactions:
            it = m.interactions[eid]
            zone = m.locations[it["location"]]["zone"]
            if el["zone_id"] != zone:
                r.err(f"[zones] {eid} : zone {el['zone_id']} ≠ zone du lieu {it['location']} ({zone})")
            if el["time_cost"] != it["cost"]:
                r.err(f"[temps] {eid} : time_cost {el['time_cost']} ≠ coût de l'interaction ({it['cost']})")
        elif eid in m.clues and m.clues[eid].get("location"):
            zone = m.locations[m.clues[eid]["location"]]["zone"]
            if el["zone_id"] != zone:
                r.err(f"[zones] {eid} : zone {el['zone_id']} ≠ zone du lieu de l'indice ({zone})")
        if eid.startswith(("ACT_", "EVT_")) and el["time_cost"] != 0:
            r.err(f"[temps] {eid} : le prologue est en temps réel, time_cost doit valoir 0")

        # repli spatial
        fb = el["fallback"]
        if el.get("mandatory"):
            if not el["camera_independent"] or len(el.get("cameras", [])) < 3:
                r.err(f"[caméras] {eid} est obligatoire mais pas jouable dans les trois vues de son lieu")
            if not isinstance(fb, dict):
                r.err(f"[repli] {eid} est obligatoire et n'a aucune solution de repli spatiale")
                continue
        if fb is None:
            continue
        if fb.get("type") not in FALLBACK_TYPES:
            r.err(f"[repli] {eid} : type de repli « {fb.get('type')} » inconnu")
            continue
        if not fb.get("note"):
            r.err(f"[repli] {eid} : repli sans explication (note)")
        ids = fb.get("ids", [])
        for x in ids:
            if x not in known:
                r.err(f"[repli] {eid} : repli vers un ID inconnu {x}")
        if fb["type"] in ("evenement_rattrapage", "indice_equivalent") and not ids:
            r.err(f"[repli] {eid} : repli « {fb['type']} » sans ID")
        if fb["type"] == "evenement_rattrapage":
            for x in ids:
                ev = m.events.get(x)
                if not ev or not (ev.get("fallback") or ev.get("ends_chapter")):
                    r.err(f"[repli] {eid} : {x} n'est pas un événement de rattrapage")
        if fb["type"] == "toujours_accessible":
            loc = m.locations.get(m.interactions.get(eid, {}).get("location") or m.clues.get(eid, {}).get("location", ""))
            if not loc:
                r.err(f"[repli] {eid} : « toujours_accessible » sans lieu identifiable")
            elif loc.get("available_until") and vm.hm(loc["available_until"]) < deadline:
                r.err(f"[repli] {eid} : {loc['id']} ferme à {loc['available_until']}, avant l'échéance")

    # couverture
    need = set(m.interactions) | set(m.actions)
    need |= {c for c, x in m.clues.items() if x["availability"] == "fenetre"}
    for x in sorted(need - seen):
        r.err(f"[couverture] {x} n'a pas d'exigence spatiale")
    for step in m.meta["reference_path"]["steps"]:
        el = next((e for e in spec["elements"] if e.get("id") == step), None)
        if el and not el.get("mandatory"):
            r.err(f"[couverture] {step} est sur le parcours de référence mais n'est pas marqué obligatoire")

    r.info.append(f"{len(seen)} éléments spatiaux, {sum(1 for e in spec['elements'] if e.get('mandatory'))} obligatoires")
    return r


def main(argv: list[str]) -> int:
    args = [a for a in argv[1:] if not a.startswith("--")]
    manifest: Path | dict = MANIFEST
    if "--manifest" in argv:
        manifest = Path(argv[argv.index("--manifest") + 1])
        args = [a for a in args if a != str(manifest)]
    if "--commit" in argv:
        ref = argv[argv.index("--commit") + 1]
        args = [a for a in args if a != ref]
        manifest = manifest_from_git(ref)
        if manifest is None:
            print(f"  ✗ manifeste introuvable dans git pour {ref} (faire un git fetch de la branche de la PR #3)")
            return 1
    target = Path(args[0]) if args else ROOT / "GameData" / "missions" / "01"
    r = validate(target, manifest)
    for i in r.info:
        print(f"  · {i}")
    for e in r.errors:
        print(f"  ✗ {e}")
    print(f"\n{len(r.errors)} erreur(s).")
    return 0 if r.ok() else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv))
