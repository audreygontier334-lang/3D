# Tools — validation narrative

Outils Python 3 sans dépendance obligatoire (le paquet `jsonschema`, s'il est installé, active en plus la vérification des schémas).

| Outil | Rôle |
|---|---|
| `validate_mission.py [dossier]` | Vérifie une mission : IDs, références croisées, sources de chaque indice, solutions des énigmes, accessibilité des preuves obligatoires dans **tous** les scénarios du prologue (y compris si le joueur échoue à toutes les énigmes facultatives), marge de temps, branches, dialogues, et contrôles de contenu (soleil de PZ_09, horaires de PZ_07, fragments de plaque de PZ_05) ; `check_open_visuals` : aucune hypothèse obligatoire ne dépend d'un visuel encore ouvert (fourgon, animation de la place, apparence de Lila). |
| `validate_spatial.py [dossier]` | Vérifie `spatial_requirements.json` : zones, repères et caméras de la maquette 3D (manifeste de la PR #3 s'il est présent), concordance des zones et des coûts avec la mission, repli spatial de chaque élément obligatoire, jouabilité dans les trois vues, couverture de toutes les interactions et actions du prologue. |
| `render_decoupage.py [--check]` | Génère `docs/narrative/DECOUPAGE_SCENES.md` depuis `GameData/scenes/decoupage.json`. |
| `export_unreal.py [--out DOSSIER]` | Exporte GameData/ au format d'import JSON des Data Tables Unreal (`build/unreal/M01/`, non versionné). Contrat : `docs/INTEGRATION_UNREAL.md`. |
| `render_dialogues.py [--check]` | Génère `docs/dialogues/01-ouverture.md` depuis le JSON ; `--check` échoue si le Markdown n'est pas à jour. |
| `tests/test_mission.py` | Tests unitaires : la mission est valide, et le validateur détecte bien des données cassées (rattrapage supprimé, indice sans source, dialogue inexistant, branche orpheline, fausse piste non réfutable, pénalités trop lourdes, photo nette et floue cumulées, état de fin tiré d'une déduction au lieu d'une hypothèse, condition de dialogue en prose, zone ou repère 3D inconnu, élément obligatoire sans repli…) ; et aucun texte ne contredit les décisions d'Audrey (laisse, front de mer, anciens identifiants). |

```
python3 Tools/validate_mission.py
python3 Tools/validate_spatial.py
python3 -m unittest discover -s Tools/tests -v
```
