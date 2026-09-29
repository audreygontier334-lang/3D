# Faux-semblants — jeu PC 3D

Projet original d'enquête pour Windows, conçu par Audrey. La joueuse incarne une femme qui enquête avec sa chienne après avoir assisté à l'enlèvement d'une fillette dans un village du littoral landais.

## État du projet

Ce dépôt est le point de coordination du **nouveau jeu PC**. Il contient pour l'instant son cadre narratif et le mandat de rédaction pour Claude. Il ne contient pas encore de projet Unreal compilable, de modèles 3D riggés ni d'installateur Windows. Le prototype navigateur « Faux-semblants » créé auparavant est distinct et ne constitue pas le jeu photoréaliste demandé.

## Décisions et contributions

- Audrey décide en dernier ressort du scénario, des mécaniques et des visuels.
- Claude prépare l'enquête, les énigmes, les dialogues, les textes et, si utile, les données et outils de validation narratifs.
- Codex coordonne l'intégration, la direction visuelle, les modèles, textures, animations, audio, vidéo et le jeu PC.
- Les propositions vont sur une branche dédiée et en pull request. Aucune proposition ne devient canonique avant validation d'Audrey.

Lire [`docs/DECISIONS.md`](docs/DECISIONS.md) avant tout changement, puis [`docs/PROMPT_CLAUDE.md`](docs/PROMPT_CLAUDE.md) pour la mission narrative.

## Organisation prévue

- `docs/` : décisions, bible narrative, enquêtes et contrats de données.
- `Game/` : projet Unreal et code de jeu, à créer après choix technique validé.
- `Assets/` : contenus visuels et sonores avec source, auteur et licence documentés.
- `Tools/` : scripts de validation des données et des énigmes.

Le jeu final doit fonctionner **hors ligne**, sauvegarder localement et permettre de changer de caméra : troisième personne derrière l'épaule par défaut, troisième personne reculée et première personne facultative.

## Données personnelles

Le dépôt GitHub `audreygontier334-lang/3D` est actuellement **public**. Ne pas y ajouter les photos personnelles d'Audrey ou de sa chienne, ni des images générées à partir d'elles, sans décision explicite de sa part sur leur publication. Le brief décrit seulement les caractéristiques utiles au jeu. Le dépôt peut être rendu privé avant toute mise en commun de références photographiques.
