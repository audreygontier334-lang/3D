#!/usr/bin/env python3
"""Vérifie la couche « déplacements » de la campagne (GameData/campaign/itineraire.json).

Garanties vérifiées :
- à chaque étape, une seule bonne destination, déductible d'au moins deux preuves connues ;
- chaque fausse route a un leurre loyal, une réfutation disponible, une enquête sur place et une piste de retour ;
- temps de trajet plausibles pour le mode de transport ;
- même en visitant toutes les fausses routes, la bonne destination est atteinte avant l'échéance dure ;
- Lila est retrouvée une seule fois, sur une bonne destination (décision Q3 d'Audrey : chapitre 4) ;
- le portrait-robot est établi par la joueuse : aucun champ prérempli, chaque vrai trait figure parmi les options
  et a au moins une source connue, et chaque suspect a assez de traits pour l'action visée.

Usage : python3 Tools/validate_itineraire.py [chemin/itineraire.json]
"""
from __future__ import annotations

import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
DEFAULT = ROOT / "GameData" / "campaign" / "itineraire.json"
MISSION_CLUES = ROOT / "GameData" / "missions" / "01" / "clues.json"
JOURS = ["mardi", "mercredi", "jeudi", "vendredi", "samedi", "dimanche", "lundi"]
VITESSE_MAX = {"à pied": 6, "voiture": 100, "train": 200, "avion": 800, "avion + train": 800, "avion + voiture": 800}


def minutes(s: str, floor: int = 0) -> int:
    """« jeudi 06:30 » → minutes depuis le premier mardi 00:00 ; passe à la semaine suivante si l'heure
    tomberait avant `floor` (les étapes se suivent dans le temps)."""
    jour, hm = s.split()
    h, m = hm.split(":")
    t = JOURS.index(jour) * 1440 + int(h) * 60 + int(m)
    while t < floor:
        t += 7 * 1440
    return t


def validate(doc: dict, mission_clues: set[str]) -> tuple[list[str], list[str]]:
    errs, info = [], []
    jauges = {j["id"]: j for j in doc["jauges"]}
    known = set(mission_clues)
    lila = []
    prev_depart = -1
    for e in doc["etapes"]:
        sid = e["id"]
        known |= {c["id"] for c in e.get("indices_nouveaux", [])}
        depart = minutes(e["depart"]["heure_ref"], prev_depart)
        prev_depart = depart
        cands = e["candidates"]
        good = [c for c in cands if c["correct"]]
        if len(good) != 1:
            errs.append(f"{sid} : {len(good)} bonne(s) destination(s) au lieu d'une")
            continue
        if len(cands) < 3:
            errs.append(f"{sid} : moins de trois destinations proposées")
        g = good[0]
        if len(set(g["preuves"])) < 2:
            errs.append(f"{sid} : {g['id']} déductible d'une seule preuve")
        worst = depart + g["trajet_min"]
        for c in cands:
            cid = c["id"]
            speed = c["km"] / (c["trajet_min"] / 60)
            if speed > VITESSE_MAX.get(c["mode"], 0):
                errs.append(f"{sid}/{cid} : {c['km']} km en {c['trajet_min']} min en {c['mode']} ({speed:.0f} km/h) invraisemblable")
            for ref in c.get("preuves", []) + c.get("refutation", []):
                if ref not in known:
                    errs.append(f"{sid}/{cid} : indice inconnu {ref}")
            for jid, delta in c.get("effets", {}).items():
                if jid not in jauges:
                    errs.append(f"{sid}/{cid} : jauge inconnue {jid}")
                elif abs(delta) > jauges[jid]["max"]:
                    errs.append(f"{sid}/{cid} : effet {delta} hors bornes pour {jid}")
            if c.get("lila_retrouvee"):
                lila.append((sid, cid, c["correct"]))
            if c["correct"]:
                continue
            for k in ("leurre", "refutation", "sur_place_min", "retour_min", "scene_fausse_route"):
                if not c.get(k):
                    errs.append(f"{sid}/{cid} : fausse route sans « {k} »")
            worst += c["trajet_min"] + c.get("sur_place_min", 0) + c.get("retour_min", 0)
        deadline = minutes(e["deadline_dure"], depart)
        if worst > deadline:
            errs.append(f"{sid} : pire cas {worst - depart} min après le départ, au-delà de l'échéance ({e['deadline_dure']})")
        info.append(f"{sid} : meilleur cas {g['trajet_min']} min, pire cas {worst - depart} min "
                    f"(marge {deadline - worst} min avant {e['deadline_dure']})")
    if len(lila) != 1 or not lila[0][2]:
        errs.append(f"Lila doit être retrouvée exactement une fois, sur une bonne destination (trouvé : {lila})")

    pr = doc["portrait_robot"]
    seuils = pr["seuil_par_action"]
    for s in pr["suspects"]:
        if len(s["traits"]) < seuils[s["action"]]:
            errs.append(f"portrait {s['id']} : {len(s['traits'])} traits pour une {s['action']} qui en exige {seuils[s['action']]}")
        for t in s["traits"]:
            where = f"portrait {s['id']}/{t['champ']}"
            if set(t) - {"champ", "options", "vrai", "sources"}:
                errs.append(f"{where} : clé non prévue {sorted(set(t) - {'champ', 'options', 'vrai', 'sources'})} (aucun champ prérempli)")
            if t["vrai"] not in t["options"] or len(t["options"]) < 3:
                errs.append(f"{where} : la bonne valeur doit figurer parmi au moins trois options")
            if not t["sources"]:
                errs.append(f"{where} : aucune source")
            for ref in t["sources"]:
                if ref not in known:
                    errs.append(f"{where} : source inconnue {ref}")
    return errs, info


def main(argv: list[str]) -> int:
    path = Path(argv[1]) if len(argv) > 1 else DEFAULT
    doc = json.loads(path.read_text(encoding="utf-8"))
    clues = {c["id"] for c in json.loads(MISSION_CLUES.read_text(encoding="utf-8"))["clues"]}
    errs, info = validate(doc, clues)
    for i in info:
        print(f"  · {i}")
    for e in errs:
        print(f"  ✗ {e}")
    print(f"\n{len(errs)} erreur(s).")
    return 1 if errs else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
