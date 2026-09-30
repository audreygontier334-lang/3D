# Décor du prologue — validation et continuité

## Décision confirmée par Audrey le 1er octobre 2026

Audrey a confirmé l’option A de l’inventaire présenté par Codex : conserver les ambiances des concepts 13 (ruelle résidentielle arcachonnaise), 16 (centre-ville piéton) et 24 (place et façade scolaire).

Cette validation porte sur les ambiances architecturales. Elle ne valide ni les personnages des images, ni leurs actions, ni l’implantation représentée. Les références originales restent hors du dépôt public.

Repères de l’inventaire privé, pour éviter toute ambiguïté :
- 13 : exec-756c9d1f-251f-4ccf-ae22-c98dda52dafa.png
- 16 : exec-9fe6c633-3c45-4fd1-84fc-8622529f69d8.png
- 24 : exec-e3a942fd-87a7-4435-bdea-454d0b984cdc.png

## Exigence explicite : aucun faux raccord

Un seul décor 3D sert aux trois vues jouables et au plan court. Les arbres, grilles, poteaux, rues, bâtiments, clôtures, portails et haies conservent leur identité, leur position, leur orientation, leurs dimensions et leurs matériaux à chaque changement de caméra. Aucun décor alternatif propre à une caméra ne doit modifier la géographie.

La source d’implantation reste Game/Blockout/ouverture-centre-ville.gltf et les exigences spatiales existantes. Les détails architecturaux sont ajoutés sans déplacer les ancrages ni modifier les lignes de vue validées. Le port reste hors ouverture. L’état des propositions narratives reste celui de DECISIONS.md et CHOIX_VISUELS_V2_V4.md.

## Critères de contrôle du lot de décor

1. Attribuer des identifiants stables aux éléments fixes et enregistrer leurs transformations dans un manifeste unique ; le changement de caméra ne modifie aucune de ces transformations.
2. Conserver les tags narratifs, positions et ancrages existants, notamment Lila, K2, Fourgon, EntreeRuelle, PorteCles, Bracelet et CAM_DEPART_COURT.
3. Relancer Tools/validate_spatial.py selon ses options documentées et conserver le SHA des données effectivement contrôlées ; tester aussi dans Unreal les occultations réelles des nouveaux meshes et collisions.
4. Depuis les points de contrôle de la place, le fourgon et l’abordage restent occultés. L’entrée de ruelle reste repérable dans les trois vues. Les nouvelles haies, arbres et grilles ne masquent pas les indices requis.
5. Produire des captures des trois vues au même instant et à la même position du joueur ; comparer les repères fixes et les états des acteurs, véhicules et indices.
6. Vérifier collisions et parcours : trottoirs franchissables, façades bloquantes, absence d’obstacle ajouté sur les trajets nécessaires. Aucun succès en jeu n’est annoncé sans essai Unreal attesté.

## Coordination et état réel

Claude conserve l’écran titre, les objectifs, les sous-titres et le carnet sur claude/prologue-lisible. Ce lot ne recrée pas cette interface et ne modifie pas les textes narratifs.

Les personnages et le fourgon 3D restent à présenter à Audrey. Le niveau d’animation de la place et le trajet précis de Lila restent proposés. Aucune photo personnelle ou ressemblance privée n’est publiée.

Ce commit consigne une décision et les contrôles de production ; il ne livre pas encore le décor 3D, les animations ni les sons. La PR 6 doit être actualisée et testée séparément sur UE 5.8.3.
