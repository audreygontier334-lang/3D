"""Importe les références privées, sans modifier L_Prologue ni ses acteurs.

Dans Unreal : exécuter ce script depuis le pack privé décompressé.
Les PNG doivent être dans le sous-dossier references adjacent.
Ce sont des concepts 2D, pas des modèles 3D ni des textures de visage.
"""
from pathlib import Path
import unreal

FILES = (
    "T_REF_Decor_Ruelle.png", "T_REF_Decor_Centre.png",
    "T_REF_Decor_Ecole.png", "T_REF_Heroine_Ariane.png",
    "T_REF_Heroine_Iris.png", "T_REF_Ariane_Vues.png",
    "T_REF_Ariane_Attitudes.png",
)
DESTINATION = "/Game/Developers/PrivateVisualReferences"


def main():
    source = Path(__file__).resolve().parent / "references"
    missing = [name for name in FILES if not (source / name).is_file()]
    if missing:
        raise RuntimeError("Pack privé incomplet : " + ", ".join(missing))

    # Protection locale avant tout import. Ne modifie pas le .gitignore partagé.
    project = Path(unreal.Paths.project_dir()).resolve()
    repo = next((p for p in [project, *project.parents] if (p / ".git").exists()), None)
    if repo is None or not (repo / ".git").is_dir():
        raise RuntimeError("Clone Git classique requis pour protéger les références privées.")
    exclude = repo / ".git" / "info" / "exclude"
    exclude.parent.mkdir(parents=True, exist_ok=True)
    rules = ["/" + (project / "Content/Developers/PrivateVisualReferences").relative_to(repo).as_posix() + "/"]
    if source.is_relative_to(repo):
        rules.append("/" + source.parent.relative_to(repo).as_posix() + "/")
    previous = exclude.read_text(encoding="utf-8") if exclude.exists() else ""
    extra = [rule for rule in rules if rule not in previous.splitlines()]
    if extra:
        with exclude.open("a", encoding="utf-8") as output:
            output.write("\n# Références visuelles privées du prologue\n" + "\n".join(extra) + "\n")

    for name in FILES:
        asset_path = DESTINATION + "/" + Path(name).stem
        if unreal.EditorAssetLibrary.does_asset_exist(asset_path):
            unreal.log("Référence conservée : " + asset_path)
            continue
        task = unreal.AssetImportTask()
        task.set_editor_property("filename", str(source / name))
        task.set_editor_property("destination_path", DESTINATION)
        task.set_editor_property("automated", True)
        task.set_editor_property("replace_existing", False)
        task.set_editor_property("save", True)
        unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks([task])
        imported = task.get_editor_property("imported_object_paths")
        if not imported:
            raise RuntimeError("Import sans résultat : " + name)
        unreal.log("Référence importée : " + ", ".join(imported))
    unreal.log("7 références disponibles. Aucun acteur, collision, caméra ou niveau modifié.")


if __name__ == "__main__":
    main()
