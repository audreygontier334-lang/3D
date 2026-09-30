# Consigne pour Codex — visuels et son du prologue jouable

> Rédigée par Claude le 01/10/2026 à la demande d'Audrey. Codex choisit l'implémentation technique ; **Audrey décide de tout ce qui touche aux décors et aux personnages**.

## Où en est le projet (vérifié le 01/10 sur le PC d'Audrey)

- `main` (commit `55ca414`, PR #7) compile avec **Unreal Engine 5.8.3** : Visual Studio 2022, MSVC 14.44, SDK Windows 10.0.26100.
- Le dépôt est cloné dans **`C:\Dev\3D`**. N'utilise pas `Documents\GitHub\3D` : ce dossier appartient à un autre compte Windows et n'est pas accessible en écriture. Git portable : `C:\Dev\outils\MinGit\cmd\git.exe`. **Git LFS n'est pas installé.**
- `Game/Unreal/FauxSemblants/Scripts/setup_prologue.py` construit `L_Prologue` sans intervention. Ce ne sont que des boîtes colorées aux bonnes dimensions, à partir de `Game/Blockout/ouverture-centre-ville.gltf`.
- PC d'Audrey : carte graphique d'entrée de gamme (GTX 1650, 4 Go) et 16 Go de RAM. Voir les réglages légers de `Config/DefaultEngine.ini`.
- Audrey a lancé le jeu. Elle a vu : pas d'histoire lisible, pas d'interface, pas de visuels, pas de son, pas d'objectif, pas de route. **L'objectif est qu'elle puisse jouer un prologue qu'on comprend et qui ressemble à un lieu réel.**

## Partage du travail

- **Claude** (branche `claude/prologue-lisible`) : écran titre, carton d'ouverture, objectif affiché, sous-titres des dialogues, carnet d'indices, écran de fin du prologue. Tout est dessiné par un `AHUD` C++ (`AFSHUD`), sans asset. **Ne duplique pas ces éléments.** Tu pourras plus tard les habiller (police, cadres, sons d'interface) sans changer leur logique.
- **Codex** (toi, branche `codex/...`) : tout ce qui se voit et s'entend dans le monde 3D, ainsi que son intégration dans le niveau.
- Les textes (répliques, indices, objectifs) viennent de `GameData/` : ne les recopie pas à la main.

## Règles

1. Une branche `codex/...` par lot, puis une PR. **Jamais de push direct sur `main`**, et aucune fusion sans l'accord d'Audrey.
2. **Consulte Audrey avant toute décision de décor ou de personnage** : apparence, couleurs, architecture, style, choix d'un modèle ou d'un pack. Présente 2 ou 3 options avec des images (captures dans Unreal de préférence), dis laquelle tu recommandes et pourquoi, puis attends sa réponse. Note ses choix dans `docs/DECISIONS.md` ou dans un document `CHOIX_VISUELS_*`, comme `docs/CHOIX_VISUELS_V2_V4.md`.
3. Le dépôt est **public** : aucune photo d'Audrey ni de sa chienne, aucune ressemblance sans son accord explicite. Chaque asset a sa source, son auteur et sa licence dans `Assets/asset_production_manifest.json` (packs gratuits Fab/Quixel, Poly Haven, CC0, MetaHuman… en respectant leur licence).
4. Parle à Audrey en français simple. Elle ne code pas : donne-lui au plus 2 ou 3 gestes à faire, et fais le reste toi-même.
5. Ne présente rien comme terminé sans l'avoir compilé et lancé sur son PC, captures à l'appui. Les données (`GameData/`) et les écarts avec le découpage se signalent à Claude plutôt que de les corriger toi-même.

## Travail demandé, dans l'ordre

### 0. Inventaire de ce que tu as déjà créé

Liste pour Audrey tous les visuels déjà produits : images de concept (rue arcachonnaise, héroïne, Ariane, Lila, fourgon), références, modèles ou textures éventuels, et où ils se trouvent. Pour chacun, indique s'il est validé par Audrey (voir `docs/DECISIONS.md`, `docs/CHOIX_VISUELS_V2_V4.md`) et comment tu comptes l'utiliser dans Unreal. Les images exploratoires ne sont pas des références validées : demande-lui lesquelles garder.

### 1. Un vrai lieu à la place des boîtes (`L_Prologue`)

- Sol, **route**, trottoirs, bordures et passages piétons ; la place de l'école, le square et son banc ; la façade de l'école ; la ruelle vers le front de mer.
- Maisons arcachonnaises variées dans la ruelle, avec clôtures, portails et haies (décision n° 4). La mer reste lointaine et le port hors champ (décision n° 3).
- **Garde exactement l'implantation, les lignes de vue et les repères de la maquette** : `Tools/validate_spatial.py`, `spatial_requirements.json`, et les tags d'acteurs que lit `AFSPrologueDirector` (`Lila`, `K2`, `Fourgon`, `EntreeRuelle`, `PorteCles`, `Bracelet`, `CAM_DEPART_COURT`). Depuis la place, l'abordage doit rester invisible ; depuis l'entrée de la ruelle, il doit être visible dans les trois vues.
- Lumière chaude de fin septembre vers 16 h 30, à régler pour la GTX 1650.
- Choisis entre continuer à construire le niveau par script (reproductible, sans LFS) et enregistrer un `.umap` avec Git LFS installé sur le PC d'Audrey. Explique ton choix.

### 2. Personnages et fourgon provisoires mais crédibles

- Héroïne (décision n° 5), Ariane (décision n° 6), Lila (V4 A), la femme au badge (K2), Dufau sur le banc, les enfants à la sortie, et le fourgon (V2 A : blanc usé, ombre de lettrage, feu arrière droit fendu, plaque lisible, portière latérale côté trottoir).
- Animations minimales : marche, course et repos pour l'héroïne ; marche libre, trot, flair et alerte pour Ariane ; marche pour Lila et K2 ; roulage et portière pour le fourgon. Les déplacements sont pilotés par `AFSPrologueDirector` : remplace les volumes sans casser les tags.
- **Montre chaque personnage à Audrey (face, profil, dos, en mouvement) avant de le considérer comme validé.**

### 3. Son

Ambiance de la place (enfants, oiseaux, vent dans les pins, circulation locale), sonnerie de l'école à 16 h 26, pot d'échappement qui cogne, portière et démarrage du fourgon, respiration, pas et aboiements d'Ariane. Les sons doivent être spatialisés et déclenchés par les événements du directeur (voir `docs/DIRECTION_VISUELLE.md`, section « Son et vidéo »).

### 4. PR n° 6 (`codex/prologue-evidence-window`)

Remets-la à jour sur le nouveau `main` (UE 5.8, et bientôt l'interface de Claude), compile-la sur le PC d'Audrey, puis fais les essais listés dans son README.

## Livraison attendue

Une PR par lot, avec :
- les étapes pour Audrey (2 ou 3 gestes au plus) ;
- des captures ou une courte vidéo ;
- la liste des décisions qu'Audrey a prises et de celles qui restent à prendre ;
- les licences mises à jour dans le manifeste.
