# Scène 01 — implantation spatiale proposée

**Statut : proposition visuelle pour revue par Audrey et adaptation à l'ouverture de Claude.** C'est le plan d'un espace 3D jouable, pas le récit définitif ni une capture du jeu. Les distances sont des ordres de grandeur modifiables lorsque les actions et indices seront validés.

## Intention de lieu

Le village doit d'abord ressembler à un lieu habité : sortie d'école, banc, commerces, voitures ordinaires, chemin vers la mer. Le fourgon est visible parmi les véhicules courants. Après l'événement, le même espace devient une scène d'enquête : ce que le joueur pouvait observer avant l'incident prend un autre sens.

Audrey a précisé le cadre de l'ouverture : **centre-ville côtier inspiré du centre d'Arcachon**, en lumière chaude de fin d'après-midi. Le port peut exister ailleurs dans l'histoire mais n'est pas visible pendant P0–P3. L'école donne sur une place et une rue commerçante ; la mer n'est pas visible dans ces premiers plans. L'implantation définitive du village et les trajets d'enquête restent à accorder avec Claude.

## Zones et circulation

| Zone | Rôle jouable | Relation spatiale proposée |
| --- | --- | --- |
| `Z_PROMENADE` | Départ avec Ariane ; le joueur peut marcher, regarder, changer de vue. | Parcours piéton du centre-ville relié à la rue de l'école. |
| `Z_ECOLE` | Sortie de la fillette ; repère visuel constant. | Portail à environ 30 à 40 m du départ ; cheminement lisible sans téléportation. |
| `Z_CROISEMENT` | Rencontre des trajectoires piétonne et du fourgon ; premier lieu d'inspection. | Entre l'école et la rue de fuite, visible depuis au moins deux positions de promenade. |
| `Z_RUE_FUITE` | Départ du véhicule et question de direction. | Une bifurcation réelle, avec au moins deux destinations plausibles à départager par des preuves. |
| `Z_PLACE` | Espace de respiration et d'observation avant l'événement. | Relié à l'école et à la rue commerçante ; aucun port dans le champ P0–P3. |
| `Z_TEMOINS` | Plusieurs angles d'observation imparfaits et cohérents. | Commerçant, arrêt ou banc situés à des distances différentes du croisement. |

Le joueur doit pouvoir revenir dans toutes les zones accessibles après l'incident. Les objets nécessaires à la résolution ne disparaissent pas sans piste de remplacement. Le trajet de la fillette, la position du fourgon et la fuite devront être synchronisés avec la chronologie de `docs/cases/01-ouverture.md` quand Claude la publiera.

## Caméras à vérifier dans cette scène

| Vue | Usage | Vérification concrète |
| --- | --- | --- |
| Derrière l'épaule | Vue de départ proposée : on voit l'héroïne et la chienne. | Le corps ne masque ni le portail ni le mouvement du fourgon ; possibilité de changer d'épaule. |
| Reculée | Lire la rue, le duo et les chemins possibles. | Les toits, murs et arbres n'obstruent pas l'action ; caméra rapprochée automatiquement près des obstacles. |
| Première personne | Observer les détails et vivre la scène à hauteur humaine. | Les indices restent lisibles, la direction sonore du fourgon est cohérente, le joueur peut revenir aussitôt en troisième personne. |

Le changement de vue ne modifie ni les indices présents, ni le temps écoulé, ni l'état de la chienne. Les gros plans d'objets sont des interactions facultatives et quittables, pas une caméra imposée pendant toute l'enquête.

## Décisions visuelles récentes d'Audrey

1. **Lieu et lumière** : centre-ville côtier inspiré du centre d'Arcachon, lumière chaude de fin d'après-midi ; pas de port ni de promenade sur le front de mer visibles dans P0–P3.
2. **Duo** : la chienne se nomme **Ariane**. Elle se déplace librement dès le début, sans laisse ni collier. Elle porte un foulard noir à motifs paisley blancs ; l'héroïne porte un bandeau assorti. Leur comportement doit rester naturel.
3. **Premier indice olfactif** : après le départ du fourgon, Ariane sent un objet et mène à un indice. L'objet exact et la couleur du fourgon restent à décider ; la maquette les marque comme provisoires.
4. **Références privées** : la ressemblance d'Audrey et les images personnelles du duo restent hors du dépôt public tant qu'Audrey n'en autorise pas la publication.

## Maquette 3D de travail

`Game/Blockout/ouverture-centre-ville.gltf` est un fichier 3D glTF autonome, généré par `Tools/build_opening_blockout.py`. Il contient la place de l'école, la rue de fuite, des façades et volumes provisoires pour le fourgon, l'héroïne, Ariane et la fillette, ainsi que trois points de contrôle caméra. Les unités sont des mètres (X vers l'est, Y vers le haut, Z vers le nord ; portail de l'école à l'origine). Il sert à vérifier les dimensions et les angles avant l'assemblage dans Unreal. **Ce n'est ni une scène jouable, ni un rendu photoréaliste, ni un asset final.** Les volumes humains et canins sont de simples repères géométriques sans ressemblance réelle.

Contrôle reproductible des trois cadrages statiques, en format 16:9 : `python3 Tools/build_opening_blockout.py && python3 Tools/check_opening_sightlines.py`. Le script projette le centre de la fillette, du fourgon et d'Ariane dans chaque image, avec une marge de 5 %, puis vérifie qu'aucun volume de bâtiment ou tronc ne coupe les lignes de visée. Les trois cadrages passent ce contrôle. Il faudra encore vérifier dans Unreal les silhouettes entières, les mouvements, les autres rapports d'image, les obstacles dynamiques et la lisibilité des indices.

## Ce qui sera testé dans la vraie 3D

- Parcours à pied de l'école au croisement et de la promenade aux témoins.
- Visibilité du fourgon depuis chacune des vues, sans caméra qui devine à la place du joueur.
- Place d'Ariane à côté de la protagoniste, déplacement libre et collisions sans traverser les obstacles.
- Ambiance sonore spatiale : école, trafic, centre-ville et départ du fourgon ; ressac seulement si la distance réelle le justifie.
- Performance sur un PC cible à définir, avec modèles et matériaux en temps réel.

La prochaine étape est d'importer cette géométrie dans un niveau **3D navigable**, puis de tester les lignes de vue et le flair d'Ariane. Les vrais modèles, matériaux, animations et sons viendront après validation progressive. L'image de concept n'est pas cette maquette.
