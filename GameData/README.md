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
    spatial_requirements.json  lien avec la maquette 3D : zone, repère, caméras, repli, coût
  dialogues/
    01-ouverture.json     répliques (DLG_) et textes d'interface (UI_)
```

## Principes

- **Faits ≠ hypothèses.** Un `CLU_` décrit ce qui est constaté (`fact`) et propose des lectures (`interpretations`). Une conclusion n'existe que comme `DED_`, obtenue quand un ensemble de preuves est réuni. Le tableau (`H_`) s'appuie sur des `DED_`.
- **IDs stables.** Préfixes fixes, majuscules. Un ID publié ne change plus ; on en crée un nouveau.
- **Temps de jeu.** L'horloge n'avance que par les interactions (`cost`) et les changements de zone (`travel_cost_per_zone`). Les événements se déclenchent à heure fixe ; ceux marqués `fallback` sont des rattrapages qui garantissent l'accès aux preuves obligatoires.
- **Conditions.** `requires.all` (toutes) et `requires.any` (au moins une) portent sur des `CLU_`, `DED_` ou `FLAG_`. `blocked_by` rend une interaction indisponible. Les dialogues utilisent le même format (`requires` : `all` / `any` / `none`, plus `selected` pour l'objet choisi et `outcome` pour le résultat de l'interaction en cours) ; aucune condition en prose n'est acceptée par le validateur.
- **Déduction ≠ hypothèse présentée.** `DED_` = établi dans le carnet ; `H_` = présenté sur le tableau. États de fin et bonus dépendent de `H_` ; une `H_` facultative exige sa `DED_`.
- **Aucun échec bloquant.** Chaque hypothèse fausse pointe vers une branche qui coûte du temps, donne un résultat négatif et rejoint l'issue `RES_M01`. Une clôture automatique termine le chapitre.
- **Jetons de texte.** `{HEROINE}` (nom de la protagoniste, à décider), `{heure}` etc. sont remplacés par le moteur. La chienne s'appelle Ariane (décision d'Audrey) : son nom est écrit en clair.
- **Médias.** Les champs `assets` et `env` renvoient aux IDs du tableau d'assets de `docs/cases/01-ouverture.md` §8 et de `docs/DIRECTION_VISUELLE.md`. Aucun fichier média n'est livré ici.

## Intégration Unreal — accord Claude / Codex (29/09)

Proposé par Codex dans sa revue de la PR #4, accepté par Claude :
1. **Import** : les JSON restent la source éditable et validée ; la préparation du jeu génère des Data Assets / Data Tables Unreal.
2. **Conditions de dialogue** : structurées (`requires`), voir ci-dessus. Fait.
3. **Textes** : String Tables Unreal générées avant l'enregistrement des voix ; une table par langue si une traduction est prévue.
4. **Sauvegarde locale** : classe SaveGame contenant l'heure de jeu, les IDs acquis, les drapeaux, les options exclues et la version des données.

## Vérifier

```
python3 Tools/validate_mission.py
python3 Tools/validate_spatial.py
python3 -m unittest discover -s Tools/tests -v
python3 Tools/render_dialogues.py        # après toute modification des dialogues
```
