#!/usr/bin/env python3
"""Génère docs/narrative/DECOUPAGE_SCENES.md depuis GameData/scenes/decoupage.json.

Usage :
    python3 Tools/render_decoupage.py            # écrit le fichier
    python3 Tools/render_decoupage.py --check    # échoue si le Markdown n'est pas à jour
"""
from __future__ import annotations

import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SRC = ROOT / "GameData" / "scenes" / "decoupage.json"
DST = ROOT / "docs" / "narrative" / "DECOUPAGE_SCENES.md"
MARK = {"audrey": "✅", "proposition": "🟡"}


def esc(s: str) -> str:
    return str(s).replace("|", "\\|").replace("\n", " ")


def m(x: dict) -> str:
    return MARK.get(x.get("statut", "proposition"), "🟡")


def render(doc: dict) -> str:
    o = ["# Découpage scène par scène — « Faux-semblants »", "",
         "> **Source pour la production des visuels.** Fichier **généré** depuis `GameData/scenes/decoupage.json` "
         "par `Tools/render_decoupage.py` : modifier le JSON puis relancer le script.",
         ">",
         "> **Légende** : ✅ = décision d'Audrey · 🟡 = proposition de Claude (modifiable). "
         "Rien dans ce document ne fixe la tenue, les cheveux ni le trajet précis de Lila, ni l'apparence du fourgon, "
         "ni l'animation de la place : ce sont des choix visuels ouverts (tableau ci-dessous).",
         ">",
         "> ⚠️ Contient des spoilers (chapitres 1 à 6).", "",
         "## Choix visuels ouverts (à trancher par Audrey)", "",
         "| ID | Question | Décidé par | Contrainte narrative à respecter |", "|---|---|---|---|"]
    for c in doc["choix_visuels_ouverts"]:
        o.append(f"| {c['id']} | {esc(c['question'])} | {esc(c['decide_par'])} | {esc(c['contrainte'])} |")
    o += ["", "## Sommaire", ""]
    for ch in doc["chapitres"]:
        o.append(f"- **{ch['titre']}** : " + ", ".join(f"`{s['id']}` {s['titre']}" for s in ch["scenes"]))
    for ch in doc["chapitres"]:
        o += ["", "---", "", f"## {ch['titre']}"]
        for s in ch["scenes"]:
            L, T = s["lieu"], s["moment"]
            o += ["", f"### `{s['id']}` — {s['titre']}", ""]
            if s.get("decisions_audrey"):
                o += ["**Décisions d'Audrey qui s'appliquent** ✅", ""] + [f"- ✅ {esc(x)}" for x in s["decisions_audrey"]] + [""]
            o += [f"- **Lieu** {m(L)} : {esc(L['texte'])} — décors `{'`, `'.join(L['env'])}`"
                  + (f" ; zones `{'`, `'.join(L['zones'])}`" if L["zones"] else ""),
                  f"- **Moment et lumière** {m(T)} : {T['jour']}, {T['debut']} → {T['fin']} ; {esc(T['lumiere'])} ; météo : {esc(T['meteo'])}",
                  "- **Personnages** : " + " · ".join(f"{m(p)} `{p['id']}` {esc(p['role'])}" for p in s["personnages"]),
                  "", "**Action**", ""]
            o += [f"{i}. {m(a)} {esc(a['texte'])}" for i, a in enumerate(s["action"], 1)]
            if s["deplacements"]:
                o += ["", "**Déplacements**", "", "| | Qui | De | Vers | Distance | Durée |", "|---|---|---|---|---|---|"]
                o += [f"| {m(d)} | {esc(d['qui'])} | {esc(d['de'])} | {esc(d['vers'])} | {esc(d['distance'])} | {esc(d['duree'])} |" for d in s["deplacements"]]
            if s["indices"]:
                o += ["", "**Indices observables**", "", "| | Indice | Ce qu'on voit | Point de vue | Distance | Caméras | Indispensable |",
                      "|---|---|---|---|---|---|---|"]
                o += [f"| {m(i)} | `{i['id']}` | {esc(i['ce_qu_on_voit'])} | {esc(i['point_de_vue'])} | {esc(i['distance'])} | "
                      f"{', '.join(c.replace('CAM_', '') for c in i['cameras'])} | {'**oui**' if i['indispensable'] else 'non'} |" for i in s["indices"]]
            o += ["", "**Caméras**", ""]
            o += [f"- `{k}` : {esc(v)}" for k, v in s["cameras"].items() if v]
            o += ["", "**Si un indice est manqué**", ""]
            o += [f"- {esc(r['si_manque'])} → {esc(r['alors'])}" for r in s["repli"]]
            o += ["", "**Éléments visuels à produire**", ""]
            o += [f"- {m(v)} `{v['id']}` ({v['type']}) : {esc(v['description'])}" for v in s["visuels"]]
            if s["continuite"]:
                o += ["", "**Continuité**", ""] + [f"- {esc(c)}" for c in s["continuite"]]
            if s["points_ouverts"]:
                o += ["", "**Points ouverts**", ""] + [f"- {esc(c)}" for c in s["points_ouverts"]]
            r = s["raccord_suivant"]
            o += ["", f"**Raccord** {m(r)} → `{r['vers']}` : {esc(r['texte'])}"]
            if s["refs"]:
                o += ["", "Références données : " + ", ".join(f"`{x}`" for x in s["refs"])]
    return "\n".join(o) + "\n"


def main(argv: list[str]) -> int:
    text = render(json.loads(SRC.read_text(encoding="utf-8")))
    if "--check" in argv:
        if not DST.exists() or DST.read_text(encoding="utf-8") != text:
            print(f"À régénérer : {DST.relative_to(ROOT)}")
            return 1
        return 0
    DST.write_text(text, encoding="utf-8")
    print(f"écrit : {DST.relative_to(ROOT)}")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
