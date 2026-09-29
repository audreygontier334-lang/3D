# GameData — données de mission (proposition de contrat avec Codex)

> **Statut : proposition de Claude, à valider avec Codex et Audrey.** Aucun code Unreal ne dépend encore de ces fichiers. Le format est volontairement simple (JSON, sans logique exécutable) pour pouvoir être importé tel quel dans des *Data Tables* / *Data Assets* Unreal ou lu par un script d'import.

## Arborescence

```
GameData/
  schema/                 schémas JSON (draft 2020-12) de chaque type
  missions/01/
    mission.json          horloge, prologue, parcours de référence, issues, fichiers
    locations.json        lieux (LOC_) et zones de déplacement (Z_)
    clues.json            indices = faits observés (CLU_)
    interactions.json     modes d'obtention : coût en minutes, conditions, gains (INT_)
    events.json           événements horaires et rattrapages (EVT_)
    deductions.json       conclusions du joueur à partir des indices (DED_)
    hypotheses.json       tableau d'hypothèses en trois axes (H_)
    branches.json         conséquences des erreurs et des choix (BR_)
    puzzles.json          énigmes, solutions, aides à trois niveaux (PZ_, UI_HINT_)
  dialogues/
    01-ouverture.json     répliques (DLG_) et textes d'interface (UI_)
```

## Principes

- **Faits ≠ hypothèses.** Un `CLU_` décrit ce qui est constaté (`fact`) et propose des lectures (`interpretations`). Une conclusion n'existe que comme `DED_`, obtenue quand un ensemble de preuves est réuni. Le tableau (`H_`) s'appuie sur des `DED_`.
- **IDs stables.** Préfixes fixes, majuscules. Un ID publié ne change plus ; on en crée un nouveau.
- **Temps de jeu.** L'horloge n'avance que par les interactions (`cost`) et les changements de zone (`travel_cost_per_zone`). Les événements se déclenchent à heure fixe ; ceux marqués `fallback` sont des rattrapages qui garantissent l'accès aux preuves obligatoires.
- **Conditions.** `requires.all` (toutes) et `requires.any` (au moins une) portent sur des `CLU_`, `DED_` ou `FLAG_`. `blocked_by` rend une interaction indisponible.
- **Aucun échec bloquant.** Chaque hypothèse fausse pointe vers une branche qui coûte du temps, donne un résultat négatif et rejoint l'issue `RES_M01`. Une clôture automatique termine le chapitre.
- **Jetons de texte.** `{HEROINE}`, `{CHIENNE}`, `{heure}` etc. sont remplacés par le moteur.
- **Médias.** Les champs `assets` et `env` renvoient aux IDs du tableau d'assets de `docs/cases/01-ouverture.md` §8 et de `docs/DIRECTION_VISUELLE.md`. Aucun fichier média n'est livré ici.

## Points à arbitrer avec Codex

1. Import : Data Tables Unreal (une table par type) ou un plugin d'import JSON ? Les schémas facilitent les deux.
2. Représentation des conditions de dialogue (`condition` est aujourd'hui un texte lisible ; à normaliser en expressions sur `CLU_/FLAG_/ACT_` si Codex le souhaite).
3. Localisation : les textes sont en français dans les JSON ; une table de chaînes séparée par langue peut être extraite automatiquement si une traduction est prévue.
4. Sauvegarde locale : l'état de mission tient dans l'ensemble des IDs acquis + l'heure de jeu + les options exclues.

## Vérifier

```
python3 Tools/validate_mission.py
python3 -m unittest discover -s Tools/tests -v
python3 Tools/render_dialogues.py        # après toute modification des dialogues
```
