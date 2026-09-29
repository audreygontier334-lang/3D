#!/usr/bin/env python3
"""Génère docs/dialogues/<mission>.md à partir de GameData/dialogues/<mission>.json.

Usage :
    python3 Tools/render_dialogues.py            # écrit le fichier
    python3 Tools/render_dialogues.py --check    # échoue si le .md n'est pas à jour
"""
from __future__ import annotations

import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SOURCES = [("GameData/dialogues/01-ouverture.json", "docs/dialogues/01-ouverture.md")]


def esc(s: str) -> str:
    return s.replace("|", "\\|").replace("\n", " ")


def render(doc: dict) -> str:
    sp = doc["speakers"]
    out = [
        "# Dialogues et textes d'interface — Mission 01 (prologue et chapitre 1)",
        "",
        "> **Statut : proposition non canonique.** Fichier **généré** depuis "
        "`GameData/dialogues/01-ouverture.json` par `Tools/render_dialogues.py` : "
        "modifier le JSON, puis relancer le script. Ne pas éditer ce fichier à la main.",
        ">",
        "> `{HEROINE}` est remplacé par le moteur (nom de la protagoniste à décider). Colonnes : **intention** = ce que "
        "la ligne doit accomplir dans le jeu ; **émotion** = indication pour la voix et l'animation.",
        "",
        "## Locuteurs",
        "",
        "| ID | Personnage |",
        "|---|---|",
    ]
    out += [f"| `{k}` | {esc(v)} |" for k, v in sp.items()]
    out.append("")
    for sc in doc["scenes"]:
        out += [f"## `{sc['id']}`", "", f"*{esc(sc['context'])}* — lieu : `{sc.get('location', '—')}`", "",
                "| ID | Locuteur | Réplique | Intention | Émotion | Condition |", "|---|---|---|---|---|---|"]
        for ln in sc["lines"]:
            out.append(f"| `{ln['id']}` | {ln['speaker']} | {esc(ln['text'])} | {esc(ln['intent'])} | "
                       f"{esc(ln['emotion'])} | {esc(fmt_req(ln.get('requires')))} |")
            for ch in ln.get("choices", []):
                cond = fmt_req(ch.get("requires"))
                eff = ch.get("effect", "")
                extra = "; ".join(x for x in (f"si {cond}" if cond else "", f"effet : {eff}" if eff else "") if x)
                out.append(f"| `{ch['id']}` | ↳ choix | {esc(ch['text'])} | → `{ch['goto']}` | | {esc(extra)} |")
        out.append("")
    out += ["## Textes d'interface", "", "| ID | Texte | Contexte |", "|---|---|---|"]
    out += [f"| `{u['id']}` | {esc(u['text'])} | {esc(u['context'])} |" for u in doc.get("ui", [])]
    out += ["", "Les aides progressives des énigmes (`UI_HINT_*`) sont dans `GameData/missions/01/puzzles.json` "
            "et reprises dans `docs/cases/01-enigmes.md`.", ""]
    return "\n".join(out)


def fmt_req(req) -> str:
    """Condition structurée → texte lisible (la donnée reste la structure JSON)."""
    if not req:
        return ""
    parts = []
    if req.get("all"):
        parts.append(" et ".join(req["all"]))
    if req.get("any"):
        parts.append("l'un de : " + ", ".join(req["any"]))
    if req.get("none"):
        parts.append("aucun de : " + ", ".join(req["none"]))
    if req.get("selected"):
        parts.append("objet choisi = " + req["selected"])
    if req.get("outcome"):
        parts.append("résultat = " + req["outcome"])
    return " ; ".join(parts)


def main(argv: list[str]) -> int:
    check = "--check" in argv
    stale = []
    for src, dst in SOURCES:
        doc = json.loads((ROOT / src).read_text(encoding="utf-8"))
        text = render(doc)
        target = ROOT / dst
        if check:
            if not target.exists() or target.read_text(encoding="utf-8") != text:
                stale.append(dst)
        else:
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_text(text, encoding="utf-8")
            print(f"écrit : {dst}")
    if stale:
        print("À régénérer : " + ", ".join(stale))
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
