#!/usr/bin/env python3
"""Exporte GameData/ au format d'import JSON des Data Tables Unreal.

Chaque table est un tableau d'objets dont la clé « Name » est l'ID stable (nom de ligne Unreal).
Les listes et objets imbriqués sont conservés : ils correspondent aux TArray / USTRUCT décrits dans
docs/INTEGRATION_UNREAL.md. Les JSON de GameData/ restent la seule source éditable ; la sortie est
régénérée à chaque préparation du jeu et n'est pas versionnée.

Usage :
    python3 Tools/export_unreal.py                 # écrit dans build/unreal/M01/
    python3 Tools/export_unreal.py --out DOSSIER
"""
from __future__ import annotations

import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
MISSION = ROOT / "GameData" / "missions" / "01"
DIALOGUES = ROOT / "GameData" / "dialogues" / "01-ouverture.json"
DATA_VERSION = 1

# (fichier source, clé de liste, nom de la Data Table, type de ligne Unreal)
TABLES = [
    ("locations.json", "zones", "DT_M01_Zones", "FFSZoneRow"),
    ("locations.json", "locations", "DT_M01_Locations", "FFSLocationRow"),
    ("clues.json", "clues", "DT_M01_Clues", "FFSClueRow"),
    ("interactions.json", "interactions", "DT_M01_Interactions", "FFSInteractionRow"),
    ("events.json", "events", "DT_M01_Events", "FFSEventRow"),
    ("deductions.json", "deductions", "DT_M01_Deductions", "FFSDeductionRow"),
    ("hypotheses.json", "axes", "DT_M01_Axes", "FFSAxisRow"),
    ("hypotheses.json", "hypotheses", "DT_M01_Hypotheses", "FFSHypothesisRow"),
    ("branches.json", "branches", "DT_M01_Branches", "FFSBranchRow"),
    ("puzzles.json", "puzzles", "DT_M01_Puzzles", "FFSPuzzleRow"),
]


def rows(items: list[dict]) -> list[dict]:
    out, seen = [], set()
    for it in items:
        rid = it["id"]
        if rid in seen:
            raise ValueError(f"ID en double : {rid}")
        seen.add(rid)
        row = {"Name": rid}
        row.update({k: v for k, v in it.items() if not k.startswith("_")})
        out.append(row)
    return out


def dialogue_rows(doc: dict) -> tuple[list[dict], list[dict]]:
    """Une ligne par réplique (DT_M01_DialogueLines) et une par texte d'interface (DT_M01_UIText)."""
    lines = []
    for sc in doc["scenes"]:
        for order, ln in enumerate(sc["lines"]):
            lines.append({"id": ln["id"], "scene": sc["id"], "order": order, "location": sc.get("location") or "",
                          **{k: v for k, v in ln.items() if k != "id"}})
    return rows(lines), rows(doc.get("ui", []))


def export(out: Path) -> dict[str, int]:
    out.mkdir(parents=True, exist_ok=True)
    counts: dict[str, int] = {}
    for fname, key, table, _struct in TABLES:
        data = json.loads((MISSION / fname).read_text(encoding="utf-8"))
        r = rows(data[key])
        (out / f"{table}.json").write_text(json.dumps(r, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
        counts[table] = len(r)
    dl, ui = dialogue_rows(json.loads(DIALOGUES.read_text(encoding="utf-8")))
    for table, r in (("DT_M01_DialogueLines", dl), ("DT_M01_UIText", ui)):
        (out / f"{table}.json").write_text(json.dumps(r, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
        counts[table] = len(r)
    mission = json.loads((MISSION / "mission.json").read_text(encoding="utf-8"))
    config = {k: v for k, v in mission.items() if k not in ("files",) and not k.startswith("_")}
    config["data_version"] = DATA_VERSION
    (out / "DA_M01_Mission.json").write_text(json.dumps(config, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    manifest = {"data_version": DATA_VERSION, "tables": counts,
                "row_structs": {t: st for _f, _k, t, st in TABLES}
                | {"DT_M01_DialogueLines": "FFSDialogueLineRow", "DT_M01_UIText": "FFSUITextRow"}}
    (out / "manifest.json").write_text(json.dumps(manifest, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    return counts


def main(argv: list[str]) -> int:
    out = Path(argv[argv.index("--out") + 1]) if "--out" in argv else ROOT / "build" / "unreal" / "M01"
    counts = export(out)
    for t, n in counts.items():
        print(f"  {t}: {n} lignes")
    print(f"écrit dans {out}")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
