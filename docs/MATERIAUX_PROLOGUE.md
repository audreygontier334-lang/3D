# Matières du prologue — import préparé

La palette demandée par Audrey le 01/10/2026 comprend les enduits blanc, crème et rosé, le bois naturel et les bardages bleu-gris, vert sauge, bordeaux et blanc. Le pack comprend aussi l'asphalte. Les neuf images sont des couleurs de base générées : aucune normal map ou carte de rugosité n'est fournie. La répartition sur les bâtiments et le rendu final restent à valider avec Audrey.

## Intégration pour Claude local

Le pack `FauxSemblants_Pack_Materiaux_Import.zip` contient les PNG, `manifest.json` et `import_prologue_materials.py`. Conserver le pack hors du clone Git. Dans Unreal 5.8.3, lancer avec le chemin réel du pack décompressé :

```python
import runpy
runpy.run_path(r'C:\CHEMIN_DU_PACK\import_prologue_materials.py', run_name='__main__')
```

Le script prépare `/Game/Codex/PrologueMaterials/Textures` et `/Game/Codex/PrologueMaterials/Materials`. Il vérifie les neuf noms et leurs empreintes avant l'import. Il crée un matériau et une instance par texture : `BaseColor`, `UVScale` et `Roughness` sont réglables, Metallic = 0. Les valeurs de départ de rugosité sont 0,85 pour l'enduit, 0,75 pour le bois et 0,90 pour l'asphalte. Une relance conserve les assets et réglages existants. Elle ne réimporte pas les textures existantes : un changement de PNG nécessite une réimportation explicite après validation.

Le script ne touche pas au niveau ni aux acteurs. Aucune surface n'est automatiquement attribuée aux maisons. Avant attribution, utiliser un panneau d'essai de 200 × 200 cm avec des UV 0–1 et UVScale = 1 ; puis un panneau répétant 3 × 3 fois le motif. Les UV des bâtiments doivent être adaptés à leur taille ; le matériau ne corrige pas leur étirement automatiquement. Orienter les lames verticalement sur le bardage, et le veinage suivant la longueur des charpentes.

## Contrôles demandés

- Compiler les matériaux ; signaler tout message Unreal, avec le SHA utilisé.
- Examiner coutures, largeur des lames, contraste et couleur sous la lumière de 16 h 30. Les coutures ne sont pas encore validées.
- Présenter deux ou trois répartitions de matières à Audrey avant application. Enregistrer ensuite acteur → matériau dans une correspondance stable, commune à toutes les caméras.
- Conserver l'implantation, les collisions, les volumes opaques, les tags et les caméras ; contrôler place → fourgon après chaque lot de meshes.
- Garder les réglages matériaux retouchés après une seconde exécution ; mesurer mémoire vidéo et temps de rendu sur GTX 1650.

## Preuves disponibles

Contrôles hors Unreal : syntaxe Python, neuf PNG décodables de 1254 × 1254 pixels, neuf empreintes conformes au manifeste, fichier altéré refusé et archive ZIP contrôlée. L'original intact du bois naturel a été retrouvé et recopié à l'identique pour remplacer une copie locale tronquée. Aucun test Unreal, capture de jeu, résultat de compilation shader ou attribution de bâtiment n'est attesté par ce lot.

API consultées : documentation officielle Epic, `MaterialEditingLibrary` et `AssetImportTask`, et exemples officiels Epic PythonSamples. Aucun portrait ni référence personnelle dans ce lot. Provenance des images : `Assets/prologue_materials_manifest.json`. Le statut global `MAT_OPENING_PBR` reste prévu dans le manifeste principal : ce lot n'est pas un pack PBR final.
