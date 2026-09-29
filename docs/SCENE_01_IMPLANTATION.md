# Scène 01 — implantation spatiale proposée

**Statut : blockout technique réversible, aligné sur le découpage de la PR #4.** Ce document ne valide ni la chorégraphie définitive, ni l'apparence de Lila ou du fourgon, ni un choix narratif supplémentaire.

## Deux plans distincts

| Plan | Lieu | Fonction technique |
| --- | --- | --- |
| A | Place de l'école, en centre-ville inspiré d'Arcachon | Montrer la sortie d'école et le duo sans révéler le fourgon ni l'abordage. |
| B | Ruelle adjacente plus calme, orientée vers le front de mer | Tester séparément l'abordage, le fourgon, les indices et le départ de la piste. |

Un angle bâti sépare réellement les deux espaces. Le port reste hors de l'ouverture. Le débouché de la ruelle comporte seulement un repère : Audrey choisira plus tard s'il montre de la lumière, du ciel ou une bande de mer.

## Direction du lieu validée

- Lumière basse et chaude de fin d'après-midi.
- Architecture arcachonnaise variée : époques, volumes, toitures, couleurs, retraits, murets, ferronneries, portails et haies différents ; pas de façades copiées-collées.
- Ariane libre, sans laisse ni collier, avec un foulard noir à motifs blancs.
- Cible finale photoréaliste. La géométrie actuelle reste un substitut sans ressemblance réelle.
- Photos et ressemblances privées exclues du dépôt sans accord explicite d'Audrey.

## Choix expressément réservés à Audrey

1. Densité et niveau d'animation de la place de l'école.
2. Silhouette, couleur, état et détails visibles du fourgon.
3. Déplacement, tenue et cheveux de Lila. Aucun faux raccord des images exploratoires ne devient une référence.
4. Traitement du débouché de la ruelle et visibilité éventuelle de la mer.

Le porte-clés et le bracelet en perles sont des **propositions d'indices**. Le bracelet remplace la barrette afin de ne pas dépendre d'une coiffure non validée.

## Contrat caméra

Chaque lieu dispose de trois contrôles statiques 16:9 : épaule, reculée et première personne.

- `CAM_PLACE_*` : Lila au portail et Ariane doivent être cadrées ; le fourgon et le substitut de Lila dans la ruelle doivent être invisibles.
- `CAM_RUELLE_*` : Lila, K2, le fourgon et Ariane doivent être cadrés ; le substitut de Lila resté à l'école doit être invisible.
- Les six orientations doivent conserver un axe vertical positif : une caméra renversée échoue désormais au test.

Ces contrôles géométriques ne valident pas les silhouettes entières en animation, les obstacles dynamiques, l'éclairage final ni la caméra jouable dans Unreal.

## Fichiers et vérification

- `Game/Blockout/ouverture-centre-ville.gltf` : blockout généré.
- `Game/Blockout/ouverture-centre-ville.manifest.json` : contrat des deux plans et choix ouverts.
- `Game/Unreal/opening_assembly_plan.json` : conversion déterministe en centimètres Unreal.

Commande de contrôle :

```bash
python3 Tools/build_opening_blockout.py
python3 Tools/validate_opening_manifest.py
python3 Tools/validate_spatial_adjustments.py
python3 Tools/build_unreal_opening_plan.py --check
python3 Tools/render_opening_plan_svg.py --check
```

La prochaine validation visuelle devra présenter à Audrey des vues comparables, puis une courte séquence si le mouvement compte, avant de finaliser les assets photoréalistes.
