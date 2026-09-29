# Scène 01 — implantation spatiale proposée

**Statut : proposition visuelle pour revue par Audrey et adaptation à l'ouverture de Claude.** C'est le plan d'un espace 3D jouable, pas le récit définitif ni une capture du jeu. Les distances sont des ordres de grandeur modifiables lorsque les actions et indices seront validés.

## Intention de lieu

Le village doit d'abord ressembler à un lieu habité : sortie d'école, banc, commerces, voitures ordinaires, chemin vers la mer. Le fourgon est visible parmi les véhicules courants. Après l'événement, le même espace devient une scène d'enquête : ce que le joueur pouvait observer avant l'incident prend un autre sens.

Le concept visuel actuel propose une lumière atlantique de fin d'après-midi, une école au bord d'une rue qui descend vers un petit port et un sentier de dunes sur le côté. La présence d'un port immédiatement ouvert sur l'océan est un choix de village fictif à valider ; nous pouvons privilégier un village landais sans port, plus proche des dunes et de la forêt.

## Zones et circulation

| Zone | Rôle jouable | Relation spatiale proposée |
| --- | --- | --- |
| `Z_PROMENADE` | Départ avec la chienne ; le joueur peut marcher, regarder, changer de vue. | Chemin piéton relié à la rue de l'école et au sentier côtier. |
| `Z_ECOLE` | Sortie de la fillette ; repère visuel constant. | Portail à environ 30 à 40 m du départ ; cheminement lisible sans téléportation. |
| `Z_CROISEMENT` | Rencontre des trajectoires piétonne et du fourgon ; premier lieu d'inspection. | Entre l'école et la rue de fuite, visible depuis au moins deux positions de promenade. |
| `Z_RUE_FUITE` | Départ du véhicule et question de direction. | Une bifurcation réelle, avec au moins deux destinations plausibles à départager par des preuves. |
| `Z_COTE` | Respiration, repère sonore et visuel ; piste éventuelle à confirmer par Claude. | Accessible à pied sans barrer le trajet principal. |
| `Z_TEMOINS` | Plusieurs angles d'observation imparfaits et cohérents. | Commerçant, arrêt ou banc situés à des distances différentes du croisement. |

Le joueur doit pouvoir revenir dans toutes les zones accessibles après l'incident. Les objets nécessaires à la résolution ne disparaissent pas sans piste de remplacement. Le trajet de la fillette, la position du fourgon et la fuite devront être synchronisés avec la chronologie de `docs/cases/01-ouverture.md` quand Claude la publiera.

## Caméras à vérifier dans cette scène

| Vue | Usage | Vérification concrète |
| --- | --- | --- |
| Derrière l'épaule | Vue de départ proposée : on voit l'héroïne et la chienne. | Le corps ne masque ni le portail ni le mouvement du fourgon ; possibilité de changer d'épaule. |
| Reculée | Lire la rue, le duo et les chemins possibles. | Les toits, murs et arbres n'obstruent pas l'action ; caméra rapprochée automatiquement près des obstacles. |
| Première personne | Observer les détails et vivre la scène à hauteur humaine. | Les indices restent lisibles, la direction sonore du fourgon est cohérente, le joueur peut revenir aussitôt en troisième personne. |

Le changement de vue ne modifie ni les indices présents, ni le temps écoulé, ni l'état de la chienne. Les gros plans d'objets sont des interactions facultatives et quittables, pas une caméra imposée pendant toute l'enquête.

## Deux décisions visuelles proposées à Audrey

1. **Type de village** : (A) petit port côtier et dunes, comme le premier concept ; (B) village landais sans port, avec école, forêt de pins et accès à une plage de dunes. Le choix change la silhouette du niveau, les sons et les futures pistes de transport.
2. **Lumière de l'ouverture** : (A) fin d'après-midi dorée et quotidienne, pour un contraste fort avec l'enlèvement ; (B) ciel gris atlantique, plus immédiatement inquiétant. Les deux restent photoréalistes, sans éclairage artificiel exagéré.

## Ce qui sera testé dans la vraie 3D

- Parcours à pied de l'école au croisement et de la promenade aux témoins.
- Visibilité du fourgon depuis chacune des vues, sans caméra qui devine à la place du joueur.
- Place de la chienne à côté de la protagoniste, laisse et collisions sans traverser les obstacles.
- Ambiance sonore spatiale : océan, école, trafic et départ du fourgon.
- Performance sur un PC cible à définir, avec modèles et matériaux en temps réel.

La prochaine étape après validation des deux décisions est une maquette **3D navigable** de ce lieu, suivie des vrais modèles, matériaux, animations et sons. L'image de concept n'est pas cette maquette.
