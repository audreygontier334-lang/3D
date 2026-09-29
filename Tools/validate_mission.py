#!/usr/bin/env python3
"""Validateur de cohérence narrative pour les missions de « Faux-semblants ».

Usage :
    python3 Tools/validate_mission.py [GameData/missions/01]

Vérifie, sans dépendance externe :
  1. unicité et préfixes des identifiants ;
  2. références croisées (indices, lieux, interactions, dialogues, branches...) ;
  3. que chaque indice a au moins une source ;
  4. que chaque énigme a une solution et produit quelque chose d'atteignable ;
  5. que les hypothèses obligatoires sont atteignables dans TOUS les scénarios du
     prologue, y compris si le joueur échoue à toutes les énigmes facultatives
     (les rattrapages doivent suffire) ;
  6. que le parcours de référence est jouable et laisse la marge pour toutes les erreurs ;
  7. que toutes les branches rejoignent une issue prévue ;
  8. que les IDs de dialogue référencés existent ;
  9. des contrôles de contenu (position du soleil pour PZ_09, arithmétique de PZ_07).

Code de sortie 0 si aucune erreur, 1 sinon.
"""
from __future__ import annotations

import itertools
import json
import math
import re
import sys
from dataclasses import dataclass, field
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

PREFIXES = {
    "clues": "CLU_", "locations": "LOC_", "zones": "Z_", "interactions": "INT_",
    "events": "EVT_", "deductions": "DED_", "hypotheses": "H_", "branches": "BR_",
    "puzzles": "PZ_", "scenes": "DLG_", "lines": "DLG_", "ui": "UI_", "actions": "ACT_",
}


# --------------------------------------------------------------------------- utils

def hm(s: str) -> int:
    h, m = s.split(":")
    return int(h) * 60 + int(m)


def fmt(t: int) -> str:
    return f"{t // 60:02d}:{t % 60:02d}"


@dataclass
class Report:
    errors: list[str] = field(default_factory=list)
    warnings: list[str] = field(default_factory=list)
    info: list[str] = field(default_factory=list)

    def err(self, msg: str) -> None:
        self.errors.append(msg)

    def warn(self, msg: str) -> None:
        self.warnings.append(msg)

    def ok(self) -> bool:
        return not self.errors


class Mission:
    """Charge une mission et indexe tous ses identifiants."""

    def __init__(self, mission_dir: Path):
        self.dir = Path(mission_dir)
        self.meta = json.loads((self.dir / "mission.json").read_text(encoding="utf-8"))
        f = self.meta["files"]

        def load(key):
            return json.loads((self.dir / f[key]).read_text(encoding="utf-8"))

        loc = load("locations")
        self.locations = {x["id"]: x for x in loc["locations"]}
        self.zones = {x["id"]: x for x in loc["zones"]}
        self.clues = {x["id"]: x for x in load("clues")["clues"]}
        self.interactions = {x["id"]: x for x in load("interactions")["interactions"]}
        self.events = {x["id"]: x for x in load("events")["events"]}
        self.deductions = {x["id"]: x for x in load("deductions")["deductions"]}
        hyp = load("hypotheses")
        self.axes = {x["id"]: x for x in hyp["axes"]}
        self.hypotheses = {x["id"]: x for x in hyp["hypotheses"]}
        self.branches = {x["id"]: x for x in load("branches")["branches"]}
        self.puzzles = {x["id"]: x for x in load("puzzles")["puzzles"]}
        dlg = load("dialogues")
        self.dialogue_doc = dlg
        self.scenes = {x["id"]: x for x in dlg["scenes"]}
        self.lines = {}
        self.choices = {}
        for sc in dlg["scenes"]:
            for ln in sc["lines"]:
                self.lines[ln["id"]] = (sc["id"], ln)
                for ch in ln.get("choices", []):
                    self.choices[ch["id"]] = (sc["id"], ch)
        self.ui = {x["id"]: x for x in dlg.get("ui", [])}
        self.hints = {}
        for pz in self.puzzles.values():
            for h in pz.get("hints", []):
                self.hints[h["id"]] = (pz["id"], h)
        self.prologue = self.meta.get("prologue")
        self.actions = {a["id"]: a for a in self.prologue["actions"]} if self.prologue else {}

    # IDs obtenables (indices + drapeaux) par source
    def flags_defined(self) -> set[str]:
        out = set()
        for src in list(self.interactions.values()) + list(self.events.values()) + list(self.branches.values()):
            for g in src.get("grants", []) + src.get("flags", []):
                if g.startswith("FLAG_"):
                    out.add(g)
            if src.get("fail_flag"):
                out.add(src["fail_flag"])
        if self.prologue:
            out |= set(self.prologue.get("player_flags", []))
        for st in self.meta.get("entry_states", []):
            out |= {g for g in st.get("grants", []) if g.startswith("FLAG_")}
        return out


# ------------------------------------------------------------------ 1. identifiants

def check_ids(m: Mission, r: Report) -> None:
    groups = {
        "clues": m.clues, "locations": m.locations, "zones": m.zones,
        "interactions": m.interactions, "events": m.events, "deductions": m.deductions,
        "hypotheses": m.hypotheses, "branches": m.branches, "puzzles": m.puzzles,
        "scenes": m.scenes, "lines": {k: v for k, v in m.lines.items()}, "ui": m.ui,
        "actions": m.actions,
    }
    seen: dict[str, str] = {}
    for kind, d in groups.items():
        for i in d:
            if not i.startswith(PREFIXES[kind]):
                r.err(f"[ids] {kind}: « {i} » devrait commencer par {PREFIXES[kind]}")
            if i in seen and not (kind == "lines" and seen[i] == "scenes"):
                r.err(f"[ids] identifiant dupliqué « {i} » ({seen[i]} et {kind})")
            seen.setdefault(i, kind)
    for cid in m.choices:
        if cid in seen:
            r.err(f"[ids] identifiant de choix dupliqué « {cid} »")
        seen[cid] = "choices"
    for hid in m.hints:
        if not hid.startswith("UI_"):
            r.err(f"[ids] indice d'aide « {hid} » devrait commencer par UI_")
        if hid in seen:
            r.err(f"[ids] identifiant d'aide dupliqué « {hid} »")
        seen[hid] = "hints"
    # identifiants lisibles et stables
    pat = re.compile(r"^[A-Z][A-Z0-9_]*$")
    for i in seen:
        if not pat.match(i):
            r.err(f"[ids] format invalide « {i} » (majuscules, chiffres, _)")


# --------------------------------------------------------------- 2. références

def known_ids(m: Mission) -> set[str]:
    return set(m.clues) | set(m.deductions) | m.flags_defined()


def check_refs(m: Mission, r: Report) -> None:
    known = known_ids(m)
    scene_ids = set(m.scenes)

    def need(ids, where, allowed=None):
        pool = allowed if allowed is not None else known
        for i in ids:
            if i not in pool:
                r.err(f"[refs] {where} : identifiant inconnu « {i} »")

    for c in m.clues.values():
        need([c["location"]], f"indice {c['id']}", set(m.locations))
        for key in ("fact", "interpretations"):
            if not c.get(key):
                r.err(f"[refs] indice {c['id']} : champ « {key} » manquant ou vide")
        if c.get("availability") not in {"toujours", "fenetre", "branche"}:
            r.err(f"[refs] indice {c['id']} : disponibilité inconnue « {c.get('availability')} »")

    for loc in m.locations.values():
        need([loc["zone"]], f"lieu {loc['id']}", set(m.zones))
    for z in m.zones.values():
        need(z["neighbors"], f"zone {z['id']}", set(m.zones))
        for n in z["neighbors"]:
            if z["id"] not in m.zones.get(n, {}).get("neighbors", []):
                r.err(f"[refs] zones : voisinage non symétrique {z['id']} ↔ {n}")

    for it in m.interactions.values():
        w = f"interaction {it['id']}"
        need([it["location"]], w, set(m.locations))
        req = it.get("requires", {})
        need(req.get("all", []) + req.get("any", []) + it.get("blocked_by", []), w)
        need([g for g in it.get("grants", [])], w)
        if it.get("dialogue"):
            need([it["dialogue"]], w, scene_ids)
        else:
            r.err(f"[refs] {w} : aucun dialogue associé")
        if it.get("puzzle"):
            need([it["puzzle"]], w, set(m.puzzles))

    for ev in m.events.values():
        w = f"événement {ev['id']}"
        need(ev.get("grants", []) + ev.get("unless", []), w)
        if ev.get("dialogue"):
            need([ev["dialogue"]], w, scene_ids)

    for d in m.deductions.values():
        for s in d["any_of"]:
            need(s, f"déduction {d['id']}")
        if d.get("puzzle"):
            need([d["puzzle"]], f"déduction {d['id']}", set(m.puzzles))

    for h in m.hypotheses.values():
        w = f"hypothèse {h['id']}"
        need([h["axis"]], w, set(m.axes))
        need([h["label"]], w, set(m.ui))
        for s in h.get("requires_any_of", []):
            need(s, w)
        need(h.get("unlocked_by", []) + h.get("contradicted_by", []), w)
        if h["correct"] and not h.get("requires_any_of"):
            r.err(f"[refs] {w} : hypothèse correcte sans preuves requises")
        if not h["correct"]:
            if h.get("wrong_branch") not in m.branches:
                r.err(f"[refs] {w} : branche d'erreur inconnue « {h.get('wrong_branch')} »")
            if not h.get("contradicted_by"):
                r.err(f"[refs] {w} : hypothèse fausse sans preuve contradictoire (piste non loyale)")
    for a in m.axes.values():
        need([a["label"]], f"axe {a['id']}", set(m.ui))

    res_id = m.meta["resolution"]["id"]
    for b in m.branches.values():
        w = f"branche {b['id']}"
        need(b.get("grants", []), w)
        if b.get("dialogue"):
            need([b["dialogue"]], w, scene_ids)
        if b.get("rejoins") != res_id:
            r.err(f"[branches] {w} ne rejoint pas l'issue prévue {res_id}")
        for key in ("trigger", "effect"):
            if not b.get(key):
                r.err(f"[branches] {w} : champ « {key} » manquant")
        if "time_cost" not in b:
            r.err(f"[branches] {w} : effet sur le temps non précisé")

    all_refable = set(m.interactions) | set(m.events) | set(m.branches)
    for p in m.puzzles.values():
        w = f"énigme {p['id']}"
        need(p.get("inputs", []), w)
        need(p.get("produces", []), w)
        if p.get("interaction"):
            need([p["interaction"]], w, set(m.interactions))
        need(p.get("alt_interactions", []), w, set(m.interactions))
        need(p.get("recovery", []), w, all_refable)
        need(p.get("red_herrings", []), w, set(m.clues))
        wb = p.get("wrong", {}).get("branch")
        if wb and wb != "BR_ERR_*":
            need([wb], w, set(m.branches))

    for sid, ln in m.lines.values():
        w = f"réplique {ln['id']}"
        if ln["speaker"] not in m.dialogue_doc["speakers"]:
            r.err(f"[dialogues] {w} : locuteur inconnu « {ln['speaker']} »")
        for key in ("text", "intent", "emotion"):
            if not ln.get(key):
                r.err(f"[dialogues] {w} : champ « {key} » manquant")
        for ch in ln.get("choices", []):
            if ch["goto"] not in m.lines or m.lines[ch["goto"]][0] != sid:
                r.err(f"[dialogues] choix {ch['id']} : cible « {ch['goto']} » absente de la scène {sid}")
            eff = ch.get("effect")
            if eff and eff.startswith("BR_") and eff not in m.branches:
                r.err(f"[dialogues] choix {ch['id']} : branche inconnue « {eff} »")
    for sc in m.scenes.values():
        if not sc.get("lines"):
            r.err(f"[dialogues] scène {sc['id']} vide")
        if sc.get("location") and sc["location"] not in m.locations:
            r.err(f"[dialogues] scène {sc['id']} : lieu inconnu « {sc['location']} »")

    pro = m.prologue or {"dialogues": []}
    need(pro["dialogues"], "prologue", scene_ids)
    for a in m.actions.values():
        need(a["grants"], f"action {a['id']}")
        for other, g in a.get("grants_instead_if_with", {}).items():
            need([other], f"action {a['id']}", set(m.actions))
            need(g, f"action {a['id']}")
        need([a["dialogue"]], f"action {a['id']}", scene_ids)
    need(m.meta["reference_path"]["steps"], "parcours de référence", set(m.interactions))
    need([m.meta["resolution"]["dialogue"]], "résolution", scene_ids)

    # scènes jamais utilisées
    used = {it.get("dialogue") for it in m.interactions.values()}
    used |= {ev.get("dialogue") for ev in m.events.values()}
    used |= {b.get("dialogue") for b in m.branches.values()}
    used |= set(pro["dialogues"]) | {a["dialogue"] for a in m.actions.values()}
    used.add(m.meta["resolution"]["dialogue"])
    for sid in m.scenes:
        if sid not in used:
            r.warn(f"[dialogues] scène {sid} jamais déclenchée")


# ------------------------------------------------------------ 3. sources des indices

def clue_sources(m: Mission) -> dict[str, list[str]]:
    src: dict[str, list[str]] = {c: [] for c in m.clues}
    for it in m.interactions.values():
        for g in it.get("grants", []):
            if g in src:
                src[g].append(it["id"])
    for ev in m.events.values():
        for g in ev.get("grants", []):
            if g in src:
                src[g].append(ev["id"])
    for b in m.branches.values():
        for g in b.get("grants", []):
            if g in src:
                src[g].append(b["id"])
    for a in m.actions.values():
        for g in a["grants"]:
            src[g].append(a["id"])
        for gs in a.get("grants_instead_if_with", {}).values():
            for g in gs:
                src[g].append(a["id"])
    for st in m.meta.get("entry_states", []):
        for g in st.get("grants", []):
            if g in src:
                src[g].append(st["id"])
    return src


def check_sources(m: Mission, r: Report) -> None:
    for cid, s in clue_sources(m).items():
        if not s:
            r.err(f"[indices] {cid} n'a aucune source (interaction, événement, action ou branche)")
        c = m.clues[cid]
        if c["availability"] == "fenetre" and not all(x.startswith("ACT_") for x in s):
            r.warn(f"[indices] {cid} marqué « fenetre » mais obtenable ailleurs")
        if c["availability"] == "branche" and not all(x.startswith("BR_") for x in s):
            r.warn(f"[indices] {cid} marqué « branche » mais obtenable ailleurs")


# ------------------------------------------------------------- 4-5. atteignabilité

def prologue_scenarios(m: Mission) -> list[tuple[tuple[str, ...], set[str]]]:
    """Situations de départ à vérifier : combinaisons d'actions du prologue, ou, pour une mission sans prologue,
    états hérités du chapitre précédent (`entry_states`)."""
    if not m.prologue:
        states = m.meta.get("entry_states", [])
        return [((st["id"],), set(st.get("grants", []))) for st in states] or [((), set())]
    acts = list(m.actions)
    k = m.meta["prologue"]["window_max_actions"]
    out = []
    for n in range(0, k + 1):
        for combo in itertools.combinations(acts, n):
            grants: set[str] = set()
            for a in combo:
                act = m.actions[a]
                special = [o for o in act.get("grants_instead_if_with", {}) if o in combo]
                if special:
                    for o in special:
                        grants |= set(act["grants_instead_if_with"][o])
                else:
                    grants |= set(act["grants"])
            out.append((combo, grants))
    return out


def derive(m: Mission, have: set[str]) -> set[str]:
    have = set(have)
    changed = True
    while changed:
        changed = False
        for d in m.deductions.values():
            if d["id"] not in have and any(set(s) <= have for s in d["any_of"]):
                have.add(d["id"])
                changed = True
    return have


def requirements_met(it: dict, have: set[str]) -> bool:
    req = it.get("requires", {})
    if not set(req.get("all", [])) <= have:
        return False
    if req.get("any") and not (set(req["any"]) & have):
        return False
    return True


def reachable(m: Mission, start: set[str], adversarial: bool) -> set[str]:
    """Point fixe de ce que le joueur peut obtenir avant l'échéance.

    adversarial=True : le joueur échoue à toute énigme facultative, braque les témoins
    (interactions à fail_flag exclues) et rate toute fenêtre temporaire. Seuls les
    rattrapages et les interactions obligatoires le font avancer.
    """
    deadline = hm(m.meta["clock"]["deadline"])
    have = derive(m, start)
    usable = []
    for it in m.interactions.values():
        if adversarial:
            pz = m.puzzles.get(it.get("puzzle")) if it.get("puzzle") else None
            if pz is not None and not pz.get("mandatory"):
                continue
            if it.get("fail_flag") or it.get("available_until"):
                continue
        if it.get("available_from") and hm(it["available_from"]) >= deadline:
            continue
        usable.append(it)
    events = [e for e in m.events.values() if hm(e["time"]) <= deadline and not e.get("ends_chapter")]
    changed = True
    while changed:
        changed = False
        for it in usable:
            if requirements_met(it, have) and not set(it.get("grants", [])) <= have:
                have |= set(it.get("grants", []))
                changed = True
        for ev in events:
            if not set(ev.get("grants", [])) <= have:
                have |= set(ev.get("grants", []))
                changed = True
        new = derive(m, have)
        if new != have:
            have = new
            changed = True
    return have


def hypothesis_supported(h: dict, have: set[str]) -> bool:
    return any(set(s) <= have for s in h.get("requires_any_of", []))


def check_reachability(m: Mission, r: Report) -> None:
    required = [h for h in m.hypotheses.values()
                if h["correct"] and not h.get("optional") and m.axes[h["axis"]]["required_for_resolution"]]
    for axis in m.axes.values():
        if axis["required_for_resolution"] and not any(h["axis"] == axis["id"] for h in required):
            r.err(f"[solution] l'axe « {axis['id']} » n'a aucune hypothèse correcte obligatoire")
    n = 0
    for combo, grants in prologue_scenarios(m):
        n += 1
        for mode in (False, True):
            have = reachable(m, grants, adversarial=mode)
            label = f"scénario prologue {list(combo) or ['aucune action']}" + (" + échecs" if mode else "")
            for h in required:
                if not hypothesis_supported(h, have):
                    r.err(f"[solution] {label} : hypothèse obligatoire {h['id']} inatteignable")
    r.info.append(f"{n} scénarios de prologue × 2 modes vérifiés (normal / échecs systématiques)")

    # hypothèses facultatives : atteignables au moins dans le meilleur cas
    best = set().union(*(g for _, g in prologue_scenarios(m)))
    have_best = reachable(m, best, adversarial=False)
    for h in m.hypotheses.values():
        if h["correct"] and h.get("optional") and not hypothesis_supported(h, have_best):
            r.err(f"[solution] hypothèse facultative {h['id']} inatteignable même dans le meilleur cas")
        if not h["correct"]:
            if not set(h.get("contradicted_by", [])) & have_best:
                r.err(f"[solution] fausse piste {h['id']} : aucune contradiction atteignable")

    # énigmes : solution définie et production atteignable
    for p in m.puzzles.values():
        ans = p.get("answer", {})
        if not ans or not (ans.get("correct") or ans.get("check") or ans.get("pairs") or ans.get("conditions")):
            r.err(f"[énigmes] {p['id']} : aucune solution définie")
        if ans.get("type") == "choice" and ans.get("correct") not in ans.get("options", []):
            r.err(f"[énigmes] {p['id']} : la bonne réponse n'est pas parmi les options")
        if len(p.get("hints", [])) != 3:
            r.err(f"[énigmes] {p['id']} : il faut exactement trois niveaux d'aide")
        for prod in p.get("produces", []):
            if prod.startswith("FLAG_"):
                continue
            if prod not in have_best:
                r.err(f"[énigmes] {p['id']} : production {prod} inatteignable")
        if not p.get("recovery"):
            r.err(f"[énigmes] {p['id']} : aucune voie de rattrapage")
        if p.get("mandatory"):
            for prod in p.get("produces", []):
                if prod.startswith("FLAG_"):
                    continue
                for combo, grants in prologue_scenarios(m):
                    if prod not in reachable(m, grants, adversarial=True):
                        r.err(f"[énigmes] {p['id']} obligatoire : {prod} inatteignable dans le scénario {list(combo)} avec échecs")
                        break
    duo = [p for p in m.puzzles.values() if p.get("duo")]
    # Mandat initial : 8 énigmes dont 2 avec la chienne pour le premier acte jouable (M01) ;
    # les chapitres suivants déclarent leur propre minimum dans mission.json (`min_puzzles`, `min_duo_puzzles`).
    min_p, min_duo = m.meta.get("min_puzzles", 8), m.meta.get("min_duo_puzzles", 2)
    if len(m.puzzles) < min_p:
        r.err(f"[énigmes] {len(m.puzzles)} énigmes : il en faut au moins {min_p}")
    if len(duo) < min_duo:
        r.err(f"[énigmes] {len(duo)} énigmes liées à la chienne : il en faut au moins {min_duo}")


# ------------------------------------------------------------------ 6. le temps

def zone_distance(m: Mission, a: str, b: str) -> int:
    if a == b:
        return 0
    seen = {a}
    frontier = [a]
    d = 0
    while frontier:
        d += 1
        nxt = []
        for z in frontier:
            for n in m.zones[z]["neighbors"]:
                if n == b:
                    return d
                if n not in seen:
                    seen.add(n)
                    nxt.append(n)
        frontier = nxt
    raise ValueError(f"zones non connectées : {a} → {b}")


def simulate_path(m: Mission, steps: list[str], start: set[str] | None = None,
                  start_time: int | None = None) -> tuple[int, set[str], list[str]]:
    clock = m.meta["clock"]
    t = hm(clock["chapter_start"]) if start_time is None else start_time
    per_zone = clock["travel_cost_per_zone"]
    zone = m.meta["start_zone"]
    have = derive(m, set(start or set()))
    problems: list[str] = []
    fired: set[str] = set()
    call_time = None

    def fire_events(now: int) -> None:
        nonlocal have
        for ev in sorted(m.events.values(), key=lambda e: hm(e["time"])):
            if ev["id"] in fired or ev.get("ends_chapter"):
                continue
            et = hm(ev["time"])
            if ev["id"] == "EVT_GENDARMES_ARRIVENT":
                if call_time is None:
                    continue
                et = max(et, call_time + 13)
            if et <= now:
                fired.add(ev["id"])
                if set(ev.get("unless", [])) & have:
                    continue
                have |= set(ev.get("grants", []))
        have = derive(m, have)

    for sid in steps:
        it = m.interactions[sid]
        loc = m.locations[it["location"]]
        t += zone_distance(m, zone, loc["zone"]) * per_zone
        zone = loc["zone"]
        earliest = max(hm(it.get("available_from", "00:00")), hm(loc.get("available_from", "00:00")))
        if loc["id"] == "LOC_POSTE" or it["location"] == "LOC_POSTE":
            fire_events(t)
            if "FLAG_GENDARMES_SUR_PLACE" not in have and call_time is not None:
                earliest = max(earliest, max(hm(m.events["EVT_GENDARMES_ARRIVENT"]["time"]), call_time + 13))
        t = max(t, earliest)
        fire_events(t)
        if not requirements_met(it, have):
            problems.append(f"{sid} à {fmt(t)} : conditions non remplies")
        if set(it.get("blocked_by", [])) & have:
            problems.append(f"{sid} à {fmt(t)} : bloquée")
        t += it["cost"]
        if sid == "INT_APPEL_17":
            call_time = t
        have |= set(it.get("grants", []))
        fire_events(t)
    return t, have, problems


def timed_entry_states(m: Mission) -> list[dict]:
    """États d'entrée qui portent leur propre heure de départ (`start`), simulés séparément."""
    return [st for st in m.meta.get("entry_states", []) if st.get("start")]


def check_time(m: Mission, r: Report) -> None:
    deadline = hm(m.meta["clock"]["deadline"])
    steps = m.meta["reference_path"]["steps"]
    required = [h for h in m.hypotheses.values() if h["correct"] and not h.get("optional")]
    penalties = sum(m.branches[h["wrong_branch"]]["time_cost"]
                    for h in m.hypotheses.values() if not h["correct"])
    states = m.meta.get("entry_states", [])
    if states and len(timed_entry_states(m)) != len(states):
        r.err("[temps] chaque état d'entrée doit porter son heure de départ (start)")
    starts = [hm(st["start"]) for st in timed_entry_states(m)]
    if any(a >= b for a, b in zip(starts, starts[1:])):
        r.err("[temps] les heures de départ des états d'entrée doivent être strictement croissantes (A avant B avant C)")
    runs = [(st["id"], set(st.get("grants", [])), hm(st["start"])) for st in timed_entry_states(m)] or [(None, set(), None)]
    if runs[0][0] is not None:
        # pire parcours d'une mission à états : toutes les hypothèses fausses ET toutes les énigmes ratées
        penalties += sum(p.get("wrong", {}).get("time_cost", 0) for p in m.puzzles.values())
    for sid, grants, t0 in runs:
        end, have, problems = simulate_path(m, steps, start=grants, start_time=t0)
        label = f"parcours de référence ({sid}, départ {fmt(t0)})" if sid else "parcours de référence"
        for p in problems:
            r.err(f"[temps] {label} : {p}")
        for h in required:
            if not hypothesis_supported(h, have):
                r.err(f"[temps] {label} : {h['id']} non étayée à la fin du parcours")
        worst = end + penalties
        if sid:
            r.info.append(f"{sid} : départ {fmt(t0)}, parcours de référence terminé à {fmt(end)} ; "
                          f"pire parcours {fmt(worst)} (échéance {fmt(deadline)}, marge {deadline - worst} min)")
        else:
            r.info.append(f"parcours de référence terminé à {fmt(end)} ; avec toutes les erreurs : {fmt(worst)} (échéance {fmt(deadline)})")
        if end > deadline:
            r.err(f"[temps] {label} terminé à {fmt(end)}, après l'échéance {fmt(deadline)}")
        if worst > deadline:
            r.err(f"[temps] {label} + toutes les erreurs = {fmt(worst)} > {fmt(deadline)}")
    for it in m.interactions.values():
        if it.get("available_from") and hm(it["available_from"]) >= deadline:
            r.err(f"[temps] {it['id']} n'est disponible qu'après l'échéance")
    for ev in m.events.values():
        if ev.get("fallback") and hm(ev["time"]) >= deadline:
            r.err(f"[temps] rattrapage {ev['id']} trop tardif ({ev['time']})")


# ------------------------------------------------------------------- 7. branches

def check_branches(m: Mission, r: Report) -> None:
    referenced = {h["wrong_branch"] for h in m.hypotheses.values() if not h["correct"]}
    referenced |= {p.get("wrong", {}).get("branch") for p in m.puzzles.values()}
    referenced |= {ch.get("effect") for _, ch in m.choices.values()}
    referenced |= {"BR_APPEL_TARDIF", "BR_RESOLU", "BR_CLOTURE", "BR_PERE_CONFIANCE"}
    referenced |= {b["id"] for b in m.branches.values() if set(b.get("flags", [])) & {"FLAG_RESOLU", "FLAG_CLOTURE"}}
    for b in m.branches:
        if b not in referenced:
            r.warn(f"[branches] {b} n'est déclenchée par aucune hypothèse, énigme ou choix")
    if not any("FLAG_RESOLU" in b.get("flags", []) for b in m.branches.values()):
        r.err("[branches] aucune branche de résolution")
    if not any(e.get("ends_chapter") for e in m.events.values()):
        r.err("[branches] aucune clôture automatique : le chapitre pourrait ne jamais finir")


# ---------------------------------------------------------- 9. contrôles de contenu

def solar_azimuth(lat: float, lon: float, day_of_year: int, local_hm: str, utc_offset: float) -> tuple[float, float]:
    """Azimut (0 = nord, sens horaire) et hauteur du soleil, formule NOAA simplifiée."""
    h, mi = map(int, local_hm.split(":"))
    g = 2 * math.pi / 365 * (day_of_year - 1 + (h - 12) / 24)
    eqt = 229.18 * (0.000075 + 0.001868 * math.cos(g) - 0.032077 * math.sin(g)
                    - 0.014615 * math.cos(2 * g) - 0.040849 * math.sin(2 * g))
    decl = (0.006918 - 0.399912 * math.cos(g) + 0.070257 * math.sin(g) - 0.006758 * math.cos(2 * g)
            + 0.000907 * math.sin(2 * g) - 0.002697 * math.cos(3 * g) + 0.00148 * math.sin(3 * g))
    tst = h * 60 + mi + eqt + 4 * lon - 60 * utc_offset
    ha = math.radians(tst / 4 - 180)
    la = math.radians(lat)
    cos_zen = math.sin(la) * math.sin(decl) + math.cos(la) * math.cos(decl) * math.cos(ha)
    zen = math.acos(max(-1, min(1, cos_zen)))
    az = math.degrees(math.atan2(math.sin(ha), math.cos(ha) * math.sin(la) - math.tan(decl) * math.cos(la))) + 180
    return az % 360, 90 - math.degrees(zen)


def check_content(m: Mission, r: Report) -> None:
    pz9 = m.puzzles.get("PZ_09")
    if pz9:
        lo, hi = pz9["answer"]["check"]["sun_azimuth_deg"]
        # fin septembre, côte landaise (≈ 44,3° N ; 1,2° O), photo vers 17 h 35 (UTC+2)
        az, alt = solar_azimuth(44.3, -1.2, 272, "17:35", 2)
        r.info.append(f"PZ_09 : soleil à 17 h 35 le 29/09 → azimut {az:.0f}°, hauteur {alt:.0f}°")
        if not (lo <= az <= hi):
            r.err(f"[contenu] PZ_09 : azimut calculé {az:.0f}° hors de la plage annoncée {lo}–{hi}°")
        if alt <= 5:
            r.err("[contenu] PZ_09 : le soleil serait trop bas ou couché")
        if not (225 <= az <= 315):
            r.err("[contenu] PZ_09 : le soleil n'est pas à l'ouest, la déduction « rive est » ne tient pas")
    pz7 = m.puzzles.get("PZ_07")
    if pz7:
        c = pz7["answer"]["check"]
        arrival = hm(c["departure"]) + c["travel_min"]
        if fmt(arrival) != c["earliest_arrival"]:
            r.err(f"[contenu] PZ_07 : arrivée calculée {fmt(arrival)} ≠ {c['earliest_arrival']}")
        if (arrival <= hm(c["reported"])) != c["compatible"]:
            r.err("[contenu] PZ_07 : la conclusion de compatibilité est fausse")
    pz5 = m.puzzles.get("PZ_05")
    if pz5:
        plate = pz5["answer"]["correct"]
        if not re.match(r"^[A-Z]{2}-\d{3}-[A-Z]{2}$", plate):
            r.err(f"[contenu] PZ_05 : plaque « {plate} » au mauvais format")
        photo = "GF-4?7-??"
        video = "??-?37-TR"
        for frag in (photo, video):
            if not all(a == b or a == "?" for a, b in zip(frag, plate)):
                r.err(f"[contenu] PZ_05 : fragment « {frag} » incompatible avec « {plate} »")
    # ligne de chronologie : vidéo d'Inès et caméra du Relais
    rel = m.clues.get("CLU_CCTV_RELAIS")
    if rel and "16 h 40" not in rel["fact"]:
        r.err("[contenu] CLU_CCTV_RELAIS doit rester à 16 h 40 (9 km depuis le rond-point à 16 h 31)")


# ------------------------------------------- 8 bis. photo exclusive, états DED_/H_, conditions de dialogue

def check_window_exclusive(m: Mission, r: Report) -> None:
    """Une action « remplacée » (ex. photo en course) ne doit jamais donner aussi son résultat normal."""
    if not m.prologue:
        return
    for combo, grants in prologue_scenarios(m):
        for a in combo:
            act = m.actions[a]
            for other in act.get("grants_instead_if_with", {}):
                if other in combo:
                    others = set()
                    for b in combo:
                        if b != a:
                            others |= set(m.actions[b]["grants"])
                    leaked = (set(act["grants"]) & grants) - others
                    if leaked:
                        r.err(f"[fenêtre] {'+'.join(combo)} : {', '.join(sorted(leaked))} obtenu(s) alors que "
                              f"{a} avec {other} doit le(s) remplacer")
    pro = m.prologue
    if not pro:
        return
    acc = pro.get("window_accessibility")
    if not acc or not acc.get("max_actions_unchanged"):
        r.err("[fenêtre] option d'accessibilité absente ou modifiant le nombre d'actions")


def check_states(m: Mission, r: Report) -> None:
    """DED_ = déduction établie dans le carnet ; H_ = hypothèse présentée sur le tableau. Ne pas les confondre."""
    res = m.meta["resolution"]
    for st in res.get("chapter2_states") or res.get("next_states", []):
        for x in st["condition"].get("requires", []):
            if x.startswith("DED_"):
                r.err(f"[états] {st['id']} dépend de {x} : un état de fin dépend du tableau présenté, utiliser l'hypothèse H_ correspondante")
            elif x not in m.hypotheses:
                r.err(f"[états] {st['id']} : hypothèse inconnue {x}")
    for b in res["bonuses"]:
        src = b["from"]
        if src.startswith("DED_"):
            r.err(f"[états] bonus {b['id']} tiré de {src} : utiliser l'hypothèse H_ présentée")
        elif src.startswith("H_"):
            h = m.hypotheses.get(src)
            if not h:
                r.err(f"[états] bonus {b['id']} : hypothèse inconnue {src}")
            elif not any(x.startswith("DED_") for grp in h.get("requires_any_of", []) for x in grp):
                r.err(f"[états] {src} donne un bonus sans exiger de déduction DED_ établie")
    for h in m.hypotheses.values():
        if h.get("bonus") and h["bonus"] not in {b["id"] for b in res["bonuses"]}:
            r.err(f"[états] {h['id']} : bonus {h['bonus']} absent de mission.json")


REQ_KEYS = {"all", "any", "none", "selected", "outcome"}
OUTCOMES = {"succes", "echec"}


def dialogue_nodes(m: Mission):
    for sc in m.scenes.values():
        for ln in sc["lines"]:
            yield sc["id"], ln
            for ch in ln.get("choices", []):
                yield sc["id"], ch


def check_dialogue_requires(m: Mission, r: Report) -> None:
    known = set(m.clues) | set(m.deductions) | set(m.hypotheses) | set(m.actions) | m.flags_defined()
    known |= {b["id"] for b in m.meta["resolution"]["bonuses"]}
    for sid, node in dialogue_nodes(m):
        where = f"{sid}/{node['id']}"
        if "condition" in node:
            r.err(f"[dialogues] {where} : condition en prose « {node['condition']} » ; utiliser requires")
        req = node.get("requires")
        if req is None:
            continue
        if not isinstance(req, dict) or not req or set(req) - REQ_KEYS:
            r.err(f"[dialogues] {where} : requires mal formé {req!r}")
            continue
        for k in ("all", "any", "none"):
            for x in req.get(k, []):
                if x not in known:
                    r.err(f"[dialogues] {where} : {k} référence un ID inconnu {x}")
        if "selected" in req and req["selected"] not in m.clues:
            r.err(f"[dialogues] {where} : objet sélectionné inconnu {req['selected']}")
        if "outcome" in req and req["outcome"] not in OUTCOMES:
            r.err(f"[dialogues] {where} : résultat {req['outcome']} hors de {sorted(OUTCOMES)}")


# ------------------------------------------------------------------------- main

# ------------------------------------------------ 10. détails visuels encore ouverts

OPEN_VISUALS = {"apparence_fourgon", "animation_place_ecole", "apparence_lila"}


def check_open_visuals(m: Mission, r: Report) -> None:
    """Décision d'Audrey (29/09) : aucun indice indispensable ne dépend d'un détail visuel encore ouvert.

    Les indices et déductions marqués `open_visual` sont retirés ; les hypothèses obligatoires doivent
    rester atteignables dans tous les scénarios du prologue, même si le joueur échoue au facultatif.
    """
    tagged = set()
    for store in (m.clues, m.deductions):
        for i, x in store.items():
            for tag in x.get("open_visual", []):
                if tag not in OPEN_VISUALS:
                    r.err(f"[visuels ouverts] {i} : étiquette inconnue « {tag} »")
                tagged.add(i)
    if not tagged:
        return
    saved = {i: m.deductions.pop(i) for i in list(m.deductions) if i in tagged}
    try:
        required = [h for h in m.hypotheses.values() if h["correct"] and not h.get("optional")]
        for combo, grants in prologue_scenarios(m):
            have = reachable(m, grants - tagged, adversarial=True) - tagged
            have = derive(m, have)
            for h in required:
                if not any(set(s) <= have for s in h.get("requires_any_of", [])):
                    r.err(f"[visuels ouverts] {h['id']} dépend d'un détail visuel encore ouvert "
                          f"(scénario {list(combo) or ['aucune action']})")
    finally:
        m.deductions.update(saved)
    r.info.append(f"{len(tagged)} indices/déductions liés à des visuels ouverts : hypothèses obligatoires atteignables sans eux")


def hms(s: str) -> int:
    parts = [int(x) for x in s.split(":")]
    return parts[0] * 3600 + parts[1] * 60 + (parts[2] if len(parts) > 2 else 0)


def check_departure_rule(m: Mission, r: Report) -> None:
    """La joueuse voit toujours le départ du fourgon, où qu'elle soit pendant le prologue.

    Après l'alerte, le départ est retenu tant que la joueuse n'est pas à l'entrée de la ruelle (hold_max_s au plus) ;
    au-delà, un plan court non interactif montre le départ. Le fourgon doit être hors de vue avant le chapitre 1.
    """
    pro = m.prologue
    if not pro:
        return
    rule = pro.get("departure_rule")
    if not rule:
        r.err("[prologue] departure_rule manquante : rien ne garantit que la joueuse voie le départ du fourgon")
        return
    win_min, win_max = pro["window_seconds"]
    reach = rule["max_prologue_distance_to_entrance_m"] / rule["run_speed_mps"]
    budget = win_min + rule["hold_max_s"]
    if reach > budget:
        r.err(f"[prologue] la joueuse la plus éloignée met {reach:.0f} s pour rejoindre la ruelle, "
              f"mais le départ n'est retenu que {budget} s après l'alerte")
    shot = rule.get("fallback_shot") or {}
    if shot.get("duration_s", 0) < rule["min_visible_departure_s"] or shot.get("interactive", True):
        r.err("[prologue] le plan de repli doit être non interactif et durer au moins min_visible_departure_s")
    if rule["descent_to_turn_s"] < rule["min_visible_departure_s"]:
        r.err("[prologue] le fourgon tourne trop vite pour être vu pendant min_visible_departure_s")
    end = hms(rule["alert_latest"]) + win_max + rule["hold_max_s"] + rule["descent_to_turn_s"]
    if end > hms(m.meta["clock"]["chapter_start"]):
        r.err(f"[prologue] le fourgon peut encore être dans la ruelle après le début du chapitre 1 "
              f"({end // 3600:02d}:{end % 3600 // 60:02d}:{end % 60:02d})")
    r.info.append(f"départ du fourgon : joueuse la plus éloignée à l'entrée en {reach:.0f} s ≤ {budget} s ; "
                  f"hors de vue au plus tard à {end // 3600:02d}:{end % 3600 // 60:02d}:{end % 60:02d}")


def validate(mission_dir: Path) -> Report:
    r = Report()
    m = Mission(mission_dir)
    for check in (check_ids, check_refs, check_sources, check_reachability, check_time, check_branches, check_content,
                  check_window_exclusive, check_states, check_dialogue_requires, check_open_visuals,
                  check_departure_rule):
        check(m, r)
    return r


def main(argv: list[str]) -> int:
    target = Path(argv[1]) if len(argv) > 1 else ROOT / "GameData" / "missions" / "01"
    r = validate(target)
    for i in r.info:
        print(f"  · {i}")
    for w in r.warnings:
        print(f"  ⚠ {w}")
    for e in r.errors:
        print(f"  ✗ {e}")
    print(f"\n{len(r.errors)} erreur(s), {len(r.warnings)} avertissement(s).")
    return 0 if r.ok() else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv))
