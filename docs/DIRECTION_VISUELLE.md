# Direction visuelle et audiovisuelle — ouverture

État : **document de production proposé à Audrey**, pas un ensemble d'assets terminés. La cible est une scène 3D en temps réel pour PC Windows. Les images de concept existantes servent uniquement de références de proportions, de costume et d'ambiance ; elles ne sont pas des modèles 3D jouables.

## Intention

Un centre-ville côtier inspiré du centre d'Arcachon, en lumière chaude de fin d'après-midi : rue d'école, commerces, place et circulation quotidienne. Le port peut exister ailleurs dans l'histoire mais reste hors champ de P0–P3 ; aucune promenade sur le front de mer dans l'ouverture. Atmosphère crédible et habitée, où le basculement vers le thriller provient de l'événement et du son. L'implantation exacte reste à coordonner avec Claude.

## Personnages prioritaires

| ID | Modèle 3D attendu | Animations minimales de la première scène | État |
| --- | --- | --- | --- |
| `CHAR_HEROINE` | Femme brune adulte, jean slim taille basse, tee-shirt, veste en jean, bandeau noir à motifs blancs, créoles, baskets rétro de running noires et blanches d'inspiration ancienne sans marque ; posture naturelle, visage expressif | Repos, marche, course, arrêt brusque, regard, geste d'alerte, interaction avec chienne/téléphone | À créer et faire valider |
| `CHAR_CHIENNE` | Ariane, chienne athlétique d'environ 35 kg, beige fauve, masque et oreilles noirs, blanc au poitrail et aux pattes, une oreille dressée et l'autre tombante ; foulard noir à paisley blanc, sans collier ni laisse | Repos, marche libre naturelle, trot, flair au sol, alerte, regard vers héroïne, rappel, passage assis/debout | À créer et faire valider |
| `CHAR_FILLETTE` | Enfant fictive, silhouette lisible à distance, tenue d'école ordinaire sans ressemblance avec une personne réelle | Marche, hésitation, surprise, accompagnement contraint non graphique | À créer |
| `CHAR_RAVISSEURS` | Deux adultes distincts, vêtements ordinaires ; identité assez lisible pour les déductions mais sans révélation automatique | Conduite, approche, interaction avec portière, départ | À créer après scénario |

La fidélité du visage d'Audrey et de la morphologie de sa chienne demandera une validation sur modèles 3D sous plusieurs angles et en mouvement. Les photos personnelles restent hors du dépôt public.

## Décor et véhicules

| ID | Besoin interactif ou narratif |
| --- | --- |
| `ENV_PROMENADE` | Parcours piéton du centre-ville navigable où le duo se déplace et où Ariane réagit à l'environnement. |
| `ENV_ECOLE` | Sortie, trajet visible de la fillette, points de vue et témoins cohérents. |
| `ENV_RUE_FUITE` | Rue de fuite du fourgon, obstacles de ligne de vue et bifurcation. Aucun port visible dans P0–P3. |
| `ENV_LITTORAL` | Décor éventuel d'une scène ultérieure ; aucun horizon maritime visible dans P0–P3. |
| `VEH_FOURGON` | Portières, habitacle, plaque partiellement observable, variante/trace identifiable sans gros plan forcé. |

Chaque indice visuel scénarisé par Claude doit être visible à la distance et dans la durée indiquées dans la mission. Sa lisibilité sera vérifiée dans **tous les modes de caméra**.

## Caméras et mise en scène

- `CAM_SHOULDER` : troisième personne proche, caméra derrière l'héroïne ; la chienne reste visible autant que possible.
- `CAM_WIDE` : troisième personne reculée, orbite et évitement des murs pour explorer le village.
- `CAM_FIRST` : première personne facultative, position anatomique crédible ; utile pour examiner mais jamais obligatoire.
- `CAM_INSPECT` : cadrage temporaire d'un objet, déclenché par interaction et quittable immédiatement.
- Changement de vue à tout moment hors transition très courte, sans réinitialiser l'état de l'enquête.
- Dans la séquence d'enlèvement, conserver une présence du joueur : mouvement, observation et choix d'action restent possibles ; les plans cinématiques brefs servent la compréhension spatiale.

## Son et vidéo

- Ambiances de l'ouverture : vent dans les pins, circulation locale, oiseaux urbains et sortie d'école ; mélange spatial et transitions selon la distance. Le ressac reste désactivé par défaut et ne sera ajouté que si la distance réelle à la côte et la propagation sonore le rendent crédible.
- Ariane : respiration, pas, foulard discret, flair, signaux d'alerte mesurés. Aucun son ne révèle seul une énigme obligatoire.
- Fourgon : approche, arrêt, portière, démarrage et direction sonore cohérente avec la position réelle du véhicule.
- Voix et musique : dialogues fournis par Claude avec identifiants stables ; tension musicale progressive après le départ du fourgon, sans masquer les indices audio.
- Cinématique d'ouverture courte, idéalement rendue avec les mêmes modèles et lieux que la scène jouable ; aucun faux trailer pré-rendu présenté comme gameplay.

## Critères de qualité pour le premier jalon

1. L'héroïne et la chienne sont visibles en mouvement, avec proportions, costume et robe validables de face, profil et dos.
2. Le village et le fourgon sont de vrais objets 3D visitables, avec matériaux, éclairage, ombres et collisions.
3. L'événement est compréhensible en vue épaule, reculée et première personne ; un indice important n'est jamais dissimulé par la caméra.
4. Sons spatialisés et animations suivent réellement les événements du jeu.
5. Les assets ont une provenance et une licence enregistrées avant livraison ; les références privées ne sont pas publiées.

## État réel des livrables

Le dépôt contient une première maquette spatiale glTF sur la PR #3. **Aucun** modèle photoréaliste articulé, aucune animation finale, aucun enregistrement sonore final et aucun installateur PC n'y sont encore livrés. La prochaine étape technique est un projet Unreal vérifiable avec une scène navigable, puis le remplacement progressif des objets de travail par les assets validés.
