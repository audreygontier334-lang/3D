# Consigne pour Codex — démarrer le projet Unreal (jalon 1 : prologue jouable)

> **Statut : proposition de Claude, validée dans son principe par Audrey (29/09) : démarrer Unreal maintenant, sans attendre la fin de l'écriture.** Codex choisit l'implémentation. Ne rien fusionner sans l'accord d'Audrey. Tout ce qui n'est pas marqué ✅ dans les documents reste une proposition.

## 0. À lire d'abord

- `docs/DECISIONS.md` (PR #3) et `docs/VALIDATION_AUDREY_ACTE1.md` (PR #4) : décisions d'Audrey.
- `docs/narrative/DECOUPAGE_SCENES.md`, scènes `SC_P0_A` à `SC_P3` : ce qui se passe, où, quand, dans quelles vues.
- `docs/INTEGRATION_UNREAL.md` : contrat données ↔ Unreal (Data Tables, systèmes, prologue en temps réel).
- `GameData/missions/01/mission.json` → `prologue` (fenêtre d'action, `departure_rule`) et `spatial_requirements.json` (plans, caméras, replis).
- Aperçu de référence de la chorégraphie (volumes, chronologie, trois vues) : artefact « Prologue de Faux-semblants » d'Audrey.

## 1. Poste et dépôt

- **Il faut un PC Windows avec Unreal Engine 5** (version récente et stable, à fixer par Codex et à noter dans `Game/Unreal/README.md`). Un conteneur Linux en ligne ne peut ni compiler ni ouvrir le projet.
- Projet dans `Game/Unreal/FauxSemblants/` (C++ + Blueprints).
- **Git LFS** obligatoire pour `*.uasset`, `*.umap` et les binaires de contenu ; `.gitignore` Unreal standard (`Binaries/`, `Intermediate/`, `Saved/`, `DerivedDataCache/`, `build/`).
- Le dépôt est **public** : aucune photo d'Audrey ni de sa chienne, aucune ressemblance, aucun asset sans licence documentée (`Assets/asset_production_manifest.json`).

## 2. Contenu du jalon 1

1. **Niveau `L_Prologue`** construit depuis la maquette glTF (`Game/Blockout/ouverture-centre-ville.gltf`, commit `1d0434d` ou suivant) : sol, place, square et banc, école, ruelle, maisons, haies, mer au loin, collisions et volumes de navigation.
2. **Personnages provisoires** (capsules ou mannequins neutres, couleurs de la maquette) : héroïne jouable (marche, course), Ariane qui suit librement à 1–3 m et revient au rappel, Lila, « Sandrine », Dufau assis, fourgon.
3. **Trois vues** changeables à tout moment sans rien réinitialiser : `CAM_SHOULDER` (par défaut, changement d'épaule), `CAM_WIDE` (reculée, rapprochée près des obstacles), `CAM_FIRST` (subjective, hauteur d'yeux). Plus `CAM_INSPECT` pour examiner un objet, et le plan court `CAM_DEPART_COURT`.
4. **`AFSPrologueDirector`** qui rejoue la chronologie du découpage : sonnerie 16 h 26, sortie de Lila 16 h 27, abordage 16 h 28, refus devant la portière (répliques `DLG_P_ABORDAGE_04–06` en boucle), alerte au plus tard 16 h 30, fenêtre d'action de 8 à 12 s (photo, courir, crier, envoyer Ariane ; deux actions au plus), départ retenu 30 s au plus tant que la joueuse n'est pas à l'entrée de la ruelle, sinon plan court de 4 s, fourgon hors de vue avant 16 h 31, puis « Appeler le 17 ».
5. **Données** : `python3 Tools/export_unreal.py` → import des Data Tables `DT_M01_*` et du Data Asset `DA_M01_Mission` ; aucun texte ni ID recopié à la main.
6. **Tags** : chaque acteur interactif porte son ID (`LOC_`, `INT_`, `CLU_`, `EVT_`), chaque zone de navigation son `Z_`.

Hors jalon 1 : modèles photoréalistes, animations finales, chapitre 1 complet, carte des déplacements, portrait-robot.

## 3. Critères d'acceptation (mesurables)

- Depuis toute position de la place (`Z_PROMENADE`, `Z_ECOLE`, `Z_PLACE`, `Z_TEMOINS`), l'abordage, Lila devant le fourgon et le fourgon sont **invisibles** ; depuis l'entrée de la ruelle, ils sont visibles dans les trois vues.
- La joueuse la plus éloignée atteint l'entrée de la ruelle avant la fin de la retenue du départ (règle vérifiée par `validate_mission.py`).
- Le fourgon reste visible au moins 4 s au départ, plaque arrière lisible, dans les trois vues.
- Changer de vue pendant la fenêtre d'action ne change ni le temps restant ni les indices obtenus.
- Les indices obtenus pendant la fenêtre correspondent à `prologue.actions` (photo nette **ou** floue, jamais les deux).
- Le niveau se lance hors ligne ; une sauvegarde locale contient l'heure de jeu, les IDs acquis et `data_version`.
- Livrer une courte vidéo de capture de l'écran (ou des captures) pour Audrey, **en indiquant clairement que ce sont des volumes provisoires**.

## 4. Livraison

- Branche dédiée (par exemple `codex/unreal-jalon-1`) et PR en brouillon, sans fusion.
- Dans la PR : version d'Unreal, étapes pour ouvrir le projet, résultats des critères ci-dessus, et écarts éventuels avec le découpage à signaler à Claude plutôt qu'à corriger dans les données.
