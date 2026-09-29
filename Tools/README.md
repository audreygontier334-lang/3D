# Tools — validation narrative

Outils Python 3 sans dépendance obligatoire (le paquet `jsonschema`, s'il est installé, active en plus la vérification des schémas).

| Outil | Rôle |
|---|---|
| `validate_mission.py [dossier]` | Vérifie une mission : IDs, références croisées, sources de chaque indice, solutions des énigmes, accessibilité des preuves obligatoires dans **tous** les scénarios du prologue (y compris si le joueur échoue à toutes les énigmes facultatives), marge de temps, branches, dialogues, et contrôles de contenu (soleil de PZ_09, horaires de PZ_07, fragments de plaque de PZ_05). |
| `render_dialogues.py [--check]` | Génère `docs/dialogues/01-ouverture.md` depuis le JSON ; `--check` échoue si le Markdown n'est pas à jour. |
| `tests/test_mission.py` | Tests unitaires : la mission est valide, et le validateur détecte bien des données cassées (rattrapage supprimé, indice sans source, dialogue inexistant, branche orpheline, fausse piste non réfutable, pénalités trop lourdes…). |

```
python3 Tools/validate_mission.py
python3 -m unittest discover -s Tools/tests -v
```
