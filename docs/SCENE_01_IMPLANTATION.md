# Scène 01 — implantation spatiale proposée

**Statut : blockout technique réversible, aligné sur le découpage de la PR #4.** Les options V4 « Jaune moutarde » pour Lila et V2 « Blanc usé » pour le fourgon sont des décisions d'Audrey ; les substituts géométriques ne préjugent pas du rendu photoréaliste final. La chorégraphie précise de Lila et le mouvement final des caméras restent à valider. Aucun autre choix narratif n'est fixé ici.

## Deux plans distincts

| Plan | Lieu | Fonction technique |
| --- | --- | --- |
| A | Place de l'école, en centre-ville inspiré d'Arcachon | Montrer la sortie d'école et le duo sans révéler le fourgon ni l'abordage. |
| B | Ruelle adjacente plus calme, orientée vers le front de mer | Tester séparément l'abordage, le fourgon, les indices et le départ de la piste. |

Un angle bâti sépare réellement les deux espaces. Le port reste hors de l'ouverture. Décision d'Audrey : une bande de mer se voit au loin au débouché de la ruelle, tandis que la chaussée du boulevard et le rond-point restent masqués.

## Direction du lieu validée

- Lumière basse et chaude de fin d'après-midi.
- Architecture arcachonnaise variée : époques, volumes, toitures, couleurs, retraits, murets, ferronneries, portails et haies différents ; pas de façades copiées-collées.
- Ariane libre, sans laisse ni collier, avec un foulard noir à motifs paisley blancs ; l'héroïne porte un bandeau assorti.
- Référence lumière vers 16 h 30, en fin d'après-midi.
- Baskets de l'héroïne noires et blanches, style Air Max, sans logo ni nom de marque.
- Cible finale photoréaliste. La géométrie actuelle reste un substitut sans ressemblance réelle.
- Photos et ressemblances privées exclues du dépôt sans accord explicite d'Audrey.

## Choix expressément réservés à Audrey

1. Densité et niveau d'animation de la place de l'école.
2. Déplacement précis de Lila entre l'école et la ruelle, ainsi que le mouvement final des caméras. Les images exploratoires ne fixent pas ce trajet.

Le porte-clés et le bracelet en perles sont les indices retenus. Le bracelet remplace la barrette et casse hors champ pendant la montée. Lila porte la variante validée « jaune moutarde » ; le fourgon est la variante validée « blanc usé ». Aucun des deux modèles de blockout n'utilise une ressemblance réelle.

## Contrat caméra

Chaque lieu dispose de trois contrôles statiques 16:9 : épaule, reculée et première personne.

- `CAM_PLACE_*` : Lila au portail et Ariane doivent être cadrées ; le fourgon et le substitut de Lila dans la ruelle doivent être invisibles.
- `CAM_RUELLE_*` : Lila, K2, le fourgon et Ariane doivent être cadrés ; le substitut de Lila resté à l'école doit être invisible.
- Les six orientations doivent conserver un axe vertical positif : une caméra renversée échoue désormais au test.
- `CAM_DEPART_COURT` : plan non interactif de secours de 4 s depuis l'entrée, plaque arrière lisible si la joueuse n'atteint pas la ruelle à temps.

La zone jouable du blockout reste à 68,0 m au plus de l'entrée de la ruelle, sous la limite de 110 m demandée par la règle de départ (retenue maximale : 30 s).

Le coude bâti est maintenant testé depuis le départ de la promenade, le portail et le banc de Dufau : Lila dans la ruelle, K2 et le fourgon doivent être occultés depuis toute la place. Le banc se trouve à moins de 30 m du portail ; Dufau voit l'angle de rue, pas l'intérieur. Le fourgon, Lila et K2 sont placés côté trottoir. Le débouché est à 79,62 m de la portière.

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
