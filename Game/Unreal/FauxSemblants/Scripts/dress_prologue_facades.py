"""Premier habillage réversible des façades existantes, sans reconstruire le niveau.

Fenêtres et encadrements sont plaqués à moins de 3 cm des façades, sans collision.
Ce passage ne constitue pas le décor photoréaliste final.
Exécuter après setup_prologue.py dans L_Prologue ; relançable.
"""
import json
from pathlib import Path
import unreal

TAG = "CodexFacadeDetail"
BUILDINGS = {"school_main", "square_shop_west", "alley_corner_mask",
             "alley_corner_south_mask", "alley_house_1900", "alley_house_1930",
             "alley_house_1970", "alley_house_recent", "alley_house_south_1",
             "alley_house_south_2", "alley_house_south_3"}


def plan(nodes):
    """Retourne des détails déterministes plaqués sur chaque bâtiment, sans ouverture réelle."""
    result = []
    for node in nodes:
        if node.get("name") not in BUILDINGS:
            continue
        name = node["name"]
        x, y, z = node["translation"]
        width, height, depth = node["scale"]
        columns = max(1, int(width / 3.0))
        rows = max(1, int(height / 3.0))
        for side in (-1, 1):
            # Façade parallèle à X : le masque opaque source reste entier.
            face = z + side * (depth / 2 + 0.015)
            for row in range(rows):
                wy = y - height / 2 + 1.55 + row * 2.65
                for col in range(columns):
                    wx = x - width / 2 + (col + 0.5) * width / columns
                    prefix = f"{name}_{side}_{row}_{col}"
                    result.append((prefix + "_glass", (wx, wy, face), (0.9, 1.25, 0.02), "glass"))
                    result.append((prefix + "_sill", (wx, wy - 0.65, face), (1.10, 0.08, 0.025), "stone"))
                    for edge in (-1, 1):
                        result.append((prefix + f"_frame_{edge}", (wx + edge * 0.49, wy, face),
                                       (0.07, 1.35, 0.025), "stone"))
    return result


def main():
    project = Path(unreal.Paths.project_dir()).resolve()
    repo = project.parents[2]
    gltf = json.loads((repo / "Game/Blockout/ouverture-centre-ville.gltf").read_text(encoding="utf-8"))
    world = unreal.get_editor_subsystem(unreal.UnrealEditorSubsystem).get_editor_world()
    if world.get_path_name().split(":")[0].split(".")[0] != "/Game/Maps/L_Prologue":
        raise RuntimeError("Ouvrir L_Prologue avant l'habillage ; aucune autre carte ne sera modifiée.")
    actors = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    present = {str(a.get_actor_label()): a for a in actors.get_all_level_actors()}
    missing = BUILDINGS - set(present)
    if missing:
        raise RuntimeError("Bâtiments source manquants : " + ", ".join(sorted(missing)))
    # Refuser un habillage sur une géométrie déplacée depuis la maquette.
    for node in gltf["nodes"]:
        if node.get("name") not in BUILDINGS:
            continue
        location = present[node["name"]].get_actor_location()
        x, y, z = node["translation"]
        if max(abs(location.x - 100*x), abs(location.y + 100*z), abs(location.z - 100*y)) > 0.1:
            raise RuntimeError("Ancrage modifié : " + node["name"])
        scale = present[node["name"]].get_actor_scale3d()
        sx, sy, sz = node["scale"]
        if max(abs(scale.x-sx), abs(scale.y-sz), abs(scale.z-sy)) > 0.001:
            raise RuntimeError("Dimensions modifiées : " + node["name"])
    cube = unreal.load_asset("/Engine/BasicShapes/Cube.Cube")
    base = unreal.load_asset("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial")
    if not cube or not base:
        raise RuntimeError("Meshes ou matériau Engine manquants.")
    materials = {}
    for name, color in {"glass": (0.08, 0.12, 0.14), "stone": (0.83, 0.75, 0.62)}.items():
        folder = "/Game/Materials/CodexFacadeDetails"
        path = folder + "/MI_" + name
        mi = unreal.load_asset(path)
        if not mi:
            mi = unreal.AssetToolsHelpers.get_asset_tools().create_asset(
                "MI_" + name, folder, unreal.MaterialInstanceConstant,
                unreal.MaterialInstanceConstantFactoryNew())
            unreal.MaterialEditingLibrary.set_material_instance_parent(mi, base)
        unreal.MaterialEditingLibrary.set_material_instance_vector_parameter_value(
            mi, "Color", unreal.LinearColor(*color, 1.0))
        unreal.EditorAssetLibrary.save_loaded_asset(mi)
        materials[name] = mi
    # Uniquement nos propres détails, jamais les acteurs de Claude ou les masques source.
    for actor in actors.get_all_level_actors():
        if unreal.Name(TAG) in actor.tags:
            actors.destroy_actor(actor)
    details = plan(gltf["nodes"])
    for label, (x, y, z), (sx, sy, sz), mat in details:
        actor = actors.spawn_actor_from_class(unreal.StaticMeshActor,
            unreal.Vector(100*x, -100*z, 100*y), unreal.Rotator(0, 0, 0))
        actor.set_actor_label("DETAIL_" + label)
        actor.set_editor_property("tags", [unreal.Name(TAG)])
        actor.static_mesh_component.set_static_mesh(cube)
        actor.static_mesh_component.set_material(0, materials[mat])
        actor.static_mesh_component.set_collision_enabled(unreal.CollisionEnabled.NO_COLLISION)
        actor.set_actor_scale3d(unreal.Vector(sx, sz, sy))
    unreal.get_editor_subsystem(unreal.LevelEditorSubsystem).save_current_level()
    unreal.log(f"{len(details)} détails de façade ajoutés ; implantation et collisions source conservées.")


if __name__ == "__main__":
    main()
