# Intégration Unreal des données de mission — contrat Claude / Codex

> **Statut : proposition de Claude** pour Codex, qui choisit l'implémentation. Elle prolonge l'accord du 29/09 de `GameData/README.md` : les JSON de `GameData/` restent la seule source éditable et validée ; Unreal lit des Data Tables générées. Rien ici ne fixe un choix visuel ou narratif.

## 1. Chaîne de données

```
GameData/*.json ──validate_mission.py / validate_spatial.py / tests──► export_unreal.py ──► build/unreal/M01/*.json ──import──► Data Tables (Content/Data/M01/)
```

- `python3 Tools/export_unreal.py` écrit un fichier par table au format d'import JSON des Data Tables (`Name` = ID stable) et un `manifest.json` (version des données, nombre de lignes, type de ligne attendu).
- Import proposé : script d'éditeur (Python Unreal ou commandlet) qui recrée les Data Tables à partir de `build/unreal/M01/`. Refuser l'import si `manifest.json` ne correspond pas aux types de lignes compilés.
- Ne jamais éditer les Data Tables à la main : corriger le JSON, relancer validateurs et export.

| Data Table | Type de ligne proposé | Source |
|---|---|---|
| `DT_M01_Zones`, `DT_M01_Locations` | `FFSZoneRow`, `FFSLocationRow` | `locations.json` |
| `DT_M01_Clues` | `FFSClueRow` | `clues.json` |
| `DT_M01_Interactions` | `FFSInteractionRow` | `interactions.json` |
| `DT_M01_Events` | `FFSEventRow` | `events.json` |
| `DT_M01_Deductions` | `FFSDeductionRow` | `deductions.json` |
| `DT_M01_Axes`, `DT_M01_Hypotheses` | `FFSAxisRow`, `FFSHypothesisRow` | `hypotheses.json` |
| `DT_M01_Branches` | `FFSBranchRow` | `branches.json` |
| `DT_M01_Puzzles` | `FFSPuzzleRow` | `puzzles.json` |
| `DT_M01_DialogueLines`, `DT_M01_UIText` | `FFSDialogueLineRow`, `FFSUITextRow` | `dialogues/01-ouverture.json` |
| `DA_M01_Mission` (Data Asset) | `UFSMissionConfig` | `mission.json` (horloge, prologue, règle de départ, issues) |

Types communs : `FFSCondition { TArray<FName> All, Any, None; }` pour tous les champs `requires` ; les IDs sont des `FName`, jamais des chaînes libres.

## 2. Systèmes de jeu (côté Codex) et ce qu'ils lisent

| Système | Rôle | Données |
|---|---|---|
| `UFSMissionClock` | Heure de jeu (minutes). N'avance que par le coût des interactions et les changements de zone ; affichée « 16 h 31 ». | `mission.clock`, `Interactions.cost`, `travel_cost_per_zone` |
| `UFSEvidenceState` | Ensemble des IDs acquis (`CLU_`, `DED_`, `FLAG_`), options exclues. Dérive les `DED_` dès que `any_of` est satisfait. | Clues, Deductions |
| `UFSEventScheduler` | Déclenche les `EVT_` à leur heure ; ignore ceux dont `unless` est acquis ; `time_rule` spécifique pour les gendarmes (appel + 13 min). | Events |
| `UFSInteractionComponent` (sur les acteurs) | Interaction jouable : vérifie `requires` / `blocked_by`, applique `cost`, accorde `grants`, lance le dialogue. | Interactions, Locations |
| `UFSDialogueRunner` | Joue les répliques d'une scène `DLG_` dans l'ordre, filtre par `requires`, gère les choix (`goto`), remplace les jetons `{HEROINE}`, `{FOURGON_COULEUR}`. | DialogueLines, UIText |
| `UFSHypothesisBoard` | Tableau à trois axes ; présenter une `H_` : correcte → validée ; fausse → `BR_` (coût, résultat négatif, retour). | Axes, Hypotheses, Branches |
| `UFSPuzzleSystem` | Énigmes `PZ_` : aides à trois niveaux (`UI_HINT_`), mauvaise réponse → `wrong`, rattrapage. | Puzzles |
| `AFSPrologueDirector` | Séquence P0–P3 en temps réel (voir §3). | `mission.prologue` |
| `UFSSaveGame` | Sauvegarde locale : heure de jeu, IDs acquis, drapeaux, options exclues, scène, `data_version`. | — |

La logique de référence existe déjà en Python (`Tools/validate_mission.py` : `derive`, `requirements_met`, `simulate_path`) : à reproduire à l'identique. Un test d'intégration Unreal devrait rejouer `mission.reference_path` et retrouver la même heure de fin que le validateur (actuellement 17:22).

## 3. Prologue en temps réel (`AFSPrologueDirector`)

Source : `mission.json` → `prologue`, et `docs/narrative/DECOUPAGE_SCENES.md` (SC_P0_A → SC_P3).

1. Les événements `EVT_START` → `EVT_MARCHE_FOURGON` suivent la chronologie de référence (16 h 25 → ≈ 16 h 29). Devant la portière, `DLG_P_ABORDAGE_04–06` tournent en boucle (Lila refuse de monter, faux appel) jusqu'à l'alerte.
2. **Alerte** (`EVT_ALERTE`) : joueuse ou Ariane à moins de `departure_rule.alert_trigger_distance_m` de l'entrée de la ruelle, ou au plus tard à `alert_latest` (Ariane s'élance en aboyant).
3. **Fenêtre d'action** : `window_seconds` (8–12 s), au plus `window_max_actions` actions parmi `actions` ; `grants_instead_if_with` remplace les gains (photo en courant = floue). Option d'accessibilité `window_accessibility`.
4. **Départ** (`EVT_DEPART`) : retenu tant que la joueuse est à plus de `hold_until_player_within_m_of_entrance` de l'entrée, dans la limite de `hold_max_s` ; au-delà, plan `CAM_DEPART_COURT` non interactif (`fallback_shot`), sans action acquise.
5. `EVT_HORS_VUE` après `descent_to_turn_s`, puis `EVT_CH1_START` : l'horloge passe à `clock.chapter_start` (16 h 31) quel que soit le temps réel écoulé.

## 4. Espace 3D ↔ données

- Chaque acteur interactif porte un tag égal à son ID (`LOC_`, `INT_`, `CLU_`) ; chaque zone de navigation est un volume taggé `Z_`.
- Les repères de la maquette (`EVT_*`, `scent_*_marker`) et les exigences de `GameData/missions/01/spatial_requirements.json` (`elements`, `coordination_requests`) sont la liste de contrôle du niveau : `Tools/validate_spatial.py` vérifie déjà le manifeste de la maquette ; la même vérification pourra tourner sur un export des acteurs du niveau.
- Caméras : `CAM_SHOULDER` (par défaut), `CAM_WIDE`, `CAM_FIRST`, `CAM_INSPECT` (examen d'objet), `CAM_DEPART_COURT` (repli du prologue). Changer de vue ne modifie ni l'heure, ni les indices, ni l'état d'Ariane.

## 5. Points à trancher par Codex

- Import par Python d'éditeur ou par commandlet C++.
- Types de lignes en C++ (recommandé pour la sauvegarde et les tests) ou en Blueprint.
- Dossier exact des Data Tables dans le projet Unreal.
