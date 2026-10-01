"""Import des neuf matières du pack local, sans modifier L_Prologue.

Exécuter main(r'C:\\chemin\\du\\pack') depuis Python dans Unreal.
Les matériaux existants et leurs réglages sont conservés à la relance.
"""
import hashlib
import json
from pathlib import Path

NAMES = ('enduit_blanc', 'enduit_creme', 'enduit_rose', 'bois_naturel',
         'bois_bleu_gris', 'bois_vert_sauge', 'bois_bordeaux', 'bois_blanc', 'asphalte')
ROOT = '/Game/Codex/PrologueMaterials'


def validate_pack(pack):
    pack = Path(pack).resolve()
    manifest = json.loads((pack / 'manifest.json').read_text(encoding='utf-8'))
    entries = manifest['textures']
    if len(entries) != len(NAMES) or {e['name'] for e in entries} != set(NAMES):
        raise ValueError('Le pack doit contenir exactement les neuf matières attendues.')
    validated = []
    for entry in entries:
        name = entry['name']
        expected = f'Textures/T_{name}_BaseColor.png'
        if entry['file'] != expected or entry['type'] != 'BaseColor' or entry['srgb'] is not True:
            raise ValueError('Entrée de pack invalide : ' + name)
        source = pack / expected
        if not source.is_file() or hashlib.sha256(source.read_bytes()).hexdigest() != entry['sha256']:
            raise ValueError('Fichier manquant ou empreinte différente : ' + name)
        validated.append((name, source))
    return validated


def main(pack=None):
    import unreal
    if pack is None:
        pack = Path(__file__).resolve().parent
    entries = validate_pack(pack)  # Tout vérifier avant le premier import.
    editing = unreal.MaterialEditingLibrary
    assets = unreal.AssetToolsHelpers.get_asset_tools()

    def create(name, folder, cls, factory):
        result = assets.create_asset(name, folder, cls, factory)
        if result is None:
            raise RuntimeError('Création impossible : ' + folder + '/' + name)
        return result

    def connect(src, output, dst, input_name):
        if not editing.connect_material_expressions(src, output, dst, input_name):
            raise RuntimeError('Connexion de matériau impossible : ' + input_name)

    def property_link(src, output, prop):
        if not editing.connect_material_property(src, output, prop):
            raise RuntimeError('Connexion à la propriété impossible : ' + str(prop))

    completed = []
    for name, source in entries:
        texture_path = ROOT + '/Textures/T_' + name + '_BaseColor'
        texture = unreal.load_asset(texture_path)
        if texture is None:
            task = unreal.AssetImportTask()
            task.set_editor_property('filename', str(source))
            task.set_editor_property('destination_path', ROOT + '/Textures')
            task.set_editor_property('destination_name', 'T_' + name + '_BaseColor')
            task.set_editor_property('automated', True)
            task.set_editor_property('replace_existing', False)
            task.set_editor_property('save', True)
            assets.import_asset_tasks([task])
            if not task.get_editor_property('imported_object_paths'):
                raise RuntimeError('Import sans résultat : ' + name)
            texture = unreal.load_asset(texture_path)
            if not isinstance(texture, unreal.Texture2D):
                raise RuntimeError('Texture2D attendue : ' + texture_path)
            texture.set_editor_property('srgb', True)
            unreal.EditorAssetLibrary.save_loaded_asset(texture)
        elif not isinstance(texture, unreal.Texture2D) or not texture.get_editor_property('srgb'):
            raise RuntimeError('Asset existant incompatible ; conservé : ' + texture_path)

        # Un parent par matière évite d'avoir besoin d'une texture par défaut absente.
        # Aucune reconstruction des graphes existants : les retouches locales survivent.
        parent_path = ROOT + '/Materials/M_' + name
        parent = unreal.load_asset(parent_path)
        if parent is None:
            parent = create('M_' + name, ROOT + '/Materials', unreal.Material, unreal.MaterialFactoryNew())
            def expression(cls, x, y):
                return editing.create_material_expression(parent, cls, x, y)
            sample = expression(unreal.MaterialExpressionTextureSampleParameter2D, -200, -200)
            sample.set_editor_property('parameter_name', 'BaseColor')
            sample.set_editor_property('texture', texture)
            uv = expression(unreal.MaterialExpressionTextureCoordinate, -800, -200)
            tiling = expression(unreal.MaterialExpressionScalarParameter, -800, 0)
            tiling.set_editor_property('parameter_name', 'UVScale')
            tiling.set_editor_property('default_value', 1.0)
            multiply = expression(unreal.MaterialExpressionMultiply, -500, -200)
            connect(uv, '', multiply, 'A')
            connect(tiling, '', multiply, 'B')
            connect(multiply, '', sample, 'Coordinates')
            property_link(sample, 'RGB', unreal.MaterialProperty.MP_BASE_COLOR)
            roughness = expression(unreal.MaterialExpressionScalarParameter, -200, 100)
            roughness.set_editor_property('parameter_name', 'Roughness')
            roughness.set_editor_property('default_value', 0.9 if name == 'asphalte' else 0.75 if name.startswith('bois_') else 0.85)
            property_link(roughness, '', unreal.MaterialProperty.MP_ROUGHNESS)
            metallic = expression(unreal.MaterialExpressionConstant, -200, 250)
            metallic.set_editor_property('r', 0.0)
            property_link(metallic, '', unreal.MaterialProperty.MP_METALLIC)
            editing.recompile_material(parent)
            unreal.EditorAssetLibrary.save_loaded_asset(parent)
        elif not isinstance(parent, unreal.Material):
            raise RuntimeError('Material attendu ; asset existant conservé : ' + parent_path)

        instance_path = ROOT + '/Materials/MI_' + name
        instance = unreal.load_asset(instance_path)
        if instance is None:
            instance = create('MI_' + name, ROOT + '/Materials', unreal.MaterialInstanceConstant,
                              unreal.MaterialInstanceConstantFactoryNew())
            editing.set_material_instance_parent(instance, parent)
            unreal.EditorAssetLibrary.save_loaded_asset(instance)
        elif not isinstance(instance, unreal.MaterialInstanceConstant):
            raise RuntimeError('MaterialInstanceConstant attendue : ' + instance_path)
        completed.append(instance_path)
    unreal.log('Neuf matières préparées. Aucun acteur ni niveau modifié. Vérifier coutures, UV et rendu avant attribution.')
    return completed


if __name__ == '__main__':
    main()
