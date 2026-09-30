"""Construit le niveau L_Prologue et copie les données de mission, dans l'éditeur Unreal.

À lancer dans l'éditeur : menu Outils (Tools) › Exécuter un script Python (Execute Python Script…),
puis choisir ce fichier. Relançable sans risque : le niveau est reconstruit à chaque fois.

Sources (aucune donnée recopiée à la main) :
- Game/Blockout/ouverture-centre-ville.gltf : maquette de Codex (PR #3), mètres, X est, Y haut, Z nord ;
- GameData/missions/01/*.json et GameData/dialogues/01-ouverture.json : mission du prologue (PR #4).
Conversion vers Unreal (cm, X est, Y sud, Z haut) : [x, y, z] -> [100x, -100z, 100y], comme
Game/Unreal/opening_assembly_plan.json.

Statut : proposition de Claude, écrite sans pouvoir lancer Unreal (à tester sur le PC d'Audrey).
"""
import json
import math
import os
import shutil

import unreal

PROJECT = os.path.abspath(unreal.Paths.project_dir())
REPO = os.path.abspath(os.path.join(PROJECT, "..", "..", ".."))
GLTF = os.path.join(REPO, "Game", "Blockout", "ouverture-centre-ville.gltf")
MISSION = os.path.join(REPO, "GameData", "missions", "01")
DIALOGUES = os.path.join(REPO, "GameData", "dialogues", "01-ouverture.json")
TELEPHONE = os.path.join(REPO, "GameData", "telephone", "01-prologue.json")
DATA_OUT = os.path.join(PROJECT, "Content", "Data", "M01")
AUDIO = "/Game/Audio"
LEVEL = "/Game/Maps/L_Prologue"
MATERIALS = "/Game/Materials"

# Volumes de la maquette remplacés par des acteurs animés ou jouables.
DYNAMIC = {"heroine_place_placeholder", "heroine_alley_placeholder", "ariane_place_placeholder",
           "ariane_alley_placeholder", "child_school_placeholder", "child_alley_placeholder",
           "k2_placeholder", "van_placeholder", "scent_object_marker", "scent_clue_marker"}

actors_sub = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
level_sub = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
cube = unreal.load_asset("/Engine/BasicShapes/Cube.Cube")
base_material = unreal.load_asset("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial")


def to_ue(x, y, z):
    return unreal.Vector(100.0 * x, -100.0 * z, 100.0 * y)


def material(name, rgb):
    path = f"{MATERIALS}/MI_{name}"
    if unreal.EditorAssetLibrary.does_asset_exist(path):
        mi = unreal.load_asset(path)
    else:
        mi = asset_tools.create_asset(f"MI_{name}", MATERIALS, unreal.MaterialInstanceConstant,
                                      unreal.MaterialInstanceConstantFactoryNew())
        unreal.MaterialEditingLibrary.set_material_instance_parent(mi, base_material)
    unreal.MaterialEditingLibrary.set_material_instance_vector_parameter_value(
        mi, "Color", unreal.LinearColor(rgb[0], rgb[1], rgb[2], 1.0))
    unreal.EditorAssetLibrary.save_loaded_asset(mi)
    return mi


def box(label, centre, size, mi, movable=False, tags=()):
    """centre et size en mètres dans le repère de la maquette glTF."""
    actor = actors_sub.spawn_actor_from_class(unreal.StaticMeshActor, to_ue(*centre), unreal.Rotator(0, 0, 0))
    actor.set_actor_label(label)
    comp = actor.static_mesh_component
    if movable:
        comp.set_mobility(unreal.ComponentMobility.MOVABLE)
    comp.set_static_mesh(cube)
    comp.set_material(0, mi)
    actor.set_actor_scale3d(unreal.Vector(size[0], size[2], size[1]))
    if "PorteCles" in tags or "Bracelet" in tags:
        comp.set_collision_enabled(unreal.CollisionEnabled.NO_COLLISION)
    if tags:
        actor.set_editor_property("tags", [unreal.Name(t) for t in tags])
    return actor


def source_position(gltf, name):
    """Position exacte du repère : le glTF reste la source de vérité."""
    return next(node["translation"] for node in gltf["nodes"] if node.get("name") == name)


def copy_data():
    os.makedirs(DATA_OUT, exist_ok=True)
    for name in os.listdir(MISSION):
        if name.endswith(".json"):
            shutil.copy(os.path.join(MISSION, name), os.path.join(DATA_OUT, name))
    shutil.copy(DIALOGUES, os.path.join(DATA_OUT, "dialogues.json"))
    if os.path.exists(TELEPHONE):
        shutil.copy(TELEPHONE, os.path.join(DATA_OUT, "telephone.json"))
    unreal.log(f"Données de mission copiées dans {DATA_OUT}")


def build_level():
    if not os.path.exists(GLTF):
        raise RuntimeError(f"Maquette introuvable : {GLTF}. Récupérer la branche qui contient Game/Blockout/.")
    with open(GLTF, encoding="utf-8") as source:
        gltf = json.load(source)
    # Vérifier les sources et classes avant de remplacer le niveau existant.
    for name in ("van_placeholder", "scent_object_marker", "scent_clue_marker"):
        source_position(gltf, name)
    dog_class = unreal.load_class(None, "/Script/FauxSemblants.FSDogCharacter")
    director_class = unreal.load_class(None, "/Script/FauxSemblants.FSPrologueDirector")
    game_mode = unreal.load_class(None, "/Script/FauxSemblants.FSGameMode")
    if not (dog_class and director_class and game_mode):
        raise RuntimeError("Classes C++ introuvables : compiler le projet avant de construire le niveau.")
    if not level_sub.new_level(LEVEL):
        raise RuntimeError("Impossible de créer L_Prologue ; vérifier le Journal de sortie.")

    colors = {m["name"]: m["pbrMetallicRoughness"]["baseColorFactor"][:3] for m in gltf["materials"]}
    mats = {name: material(name, rgb) for name, rgb in colors.items()}
    mats["mer"] = material("mer", (0.32, 0.55, 0.66))
    count = 0
    for node in gltf["nodes"]:
        if "mesh" not in node or not node.get("scale") or node["name"] in DYNAMIC:
            continue
        mat_name = gltf["materials"][gltf["meshes"][node["mesh"]]["primitives"][0]["material"]]["name"]
        if node["name"].startswith("sea"):
            mat_name = "mer"
        # Tag = nom du volume dans la maquette (ex. dufau_placeholder), pour que le code retrouve les acteurs en jeu.
        box(node["name"], node["translation"], node["scale"], mats[mat_name], tags=[node["name"]])
        count += 1
    unreal.log(f"{count} volumes de maquette placés")

    # Figurants animés par AFSPrologueDirector (couleurs des choix V2 et V4 d'Audrey).
    box("Lila", (1.0, 0.65, 1.6), (0.36, 1.30, 0.30), material("lila_jaune_moutarde", (0.83, 0.63, 0.09)), True, ["Lila"])
    box("Sandrine", (47.4, 0.84, -16.1), (0.5, 1.68, 0.36), material("k2", (0.23, 0.21, 0.32)), True, ["K2"])
    box("Fourgon", source_position(gltf, "van_placeholder"), (5.2, 2.2, 2.1), material("fourgon_blanc_use", (0.86, 0.85, 0.80)), True, ["Fourgon"])
    box("PorteCles", source_position(gltf, "scent_object_marker"), (0.12, 0.05, 0.12), material("porte_cles", (0.88, 0.41, 0.11)), True, ["PorteCles"])
    box("Bracelet", source_position(gltf, "scent_clue_marker"), (0.12, 0.04, 0.12), material("bracelet", (0.95, 0.89, 0.96)), True, ["Bracelet"])

    entrance = actors_sub.spawn_actor_from_class(unreal.TargetPoint, to_ue(37.0, 0.0, -16.6), unreal.Rotator(0, 0, 0))
    entrance.set_actor_label("EntreeRuelle")
    entrance.set_editor_property("tags", [unreal.Name("EntreeRuelle")])

    # Plan court de repli : depuis l'entrée de la ruelle, vers l'arrière du fourgon.
    eye, van = (37.0, 1.7, -16.6), (66.0, 1.1, -18.6)
    yaw = math.degrees(math.atan2(-100 * (van[2] - eye[2]), 100 * (van[0] - eye[0])))
    cam = actors_sub.spawn_actor_from_class(unreal.CameraActor, to_ue(*eye), unreal.Rotator(0.0, -2.0, yaw))
    cam.set_actor_label("CAM_DEPART_COURT")
    cam.set_editor_property("tags", [unreal.Name("CAM_DEPART_COURT")])

    # Lumière de fin septembre vers 16 h 30 : soleil au sud-ouest, environ 30° de hauteur, qui éclaire vers le nord-est.
    sun = actors_sub.spawn_actor_from_class(unreal.DirectionalLight, unreal.Vector(0, 0, 3000), unreal.Rotator(0.0, -30.0, -45.0))
    sun.set_actor_label("Soleil_16h30")
    sun.light_component.set_intensity(8.0)
    sun.light_component.set_light_color(unreal.LinearColor(1.0, 0.86, 0.68, 1.0))
    sun.light_component.set_editor_property("atmosphere_sun_light", True)
    actors_sub.spawn_actor_from_class(unreal.SkyAtmosphere, unreal.Vector(0, 0, 0), unreal.Rotator(0, 0, 0))
    sky = actors_sub.spawn_actor_from_class(unreal.SkyLight, unreal.Vector(0, 0, 500), unreal.Rotator(0, 0, 0))
    sky.light_component.set_editor_property("real_time_capture", True)
    actors_sub.spawn_actor_from_class(unreal.ExponentialHeightFog, unreal.Vector(0, 0, 0), unreal.Rotator(0, 0, 0))

    # Départ de la joueuse (rue piétonne, face au nord) et Ariane, libre à côté d'elle.
    actors_sub.spawn_actor_from_class(unreal.PlayerStart, to_ue(8.0, 1.0, -18.0), unreal.Rotator(0.0, 0.0, -90.0))
    dog = actors_sub.spawn_actor_from_class(dog_class, to_ue(9.5, 0.4, -17.2), unreal.Rotator(0.0, 0.0, -90.0))
    dog.set_actor_label("Ariane")
    director = actors_sub.spawn_actor_from_class(director_class, unreal.Vector(0, 0, 0), unreal.Rotator(0, 0, 0))
    director.set_actor_label("PrologueDirector")

    world = unreal.get_editor_subsystem(unreal.UnrealEditorSubsystem).get_editor_world()
    world.get_world_settings().set_editor_property("default_game_mode", game_mode)
    level_sub.save_current_level()
    unreal.log("L_Prologue construit et enregistré. Lancer avec le bouton Jouer (Play).")


# --- Son : catégories de volume et sons d'interface ------------------------------------------------
# Classes de son SC_* (volumes du menu Son) et sons d'interface synthétisés ici (aucun fichier externe,
# aucune licence tierce). Codex pourra remplacer ces sons provisoires en gardant les mêmes noms.

SOUND_CLASSES = ["SC_Musique", "SC_Effets", "SC_Voix", "SC_Ambiance", "SC_Interface"]


def write_wav(path, samples, rate=44100):
    import struct
    import wave
    with wave.open(path, "wb") as w:
        w.setnchannels(1)
        w.setsampwidth(2)
        w.setframerate(rate)
        w.writeframes(b"".join(struct.pack("<h", int(max(-1.0, min(1.0, x)) * 32000)) for x in samples))


def tone(freqs, duration, rate=44100, attack=0.005, release=0.06, volume=0.35):
    n = int(duration * rate)
    out = []
    for i in range(n):
        t = i / rate
        env = min(1.0, t / attack) * min(1.0, (duration - t) / release)
        out.append(volume * env * sum(math.sin(2 * math.pi * f * t) for f in freqs) / len(freqs))
    return out


def shutter(rate=44100):
    import random
    rnd = random.Random(7)
    n = int(0.12 * rate)
    return [0.5 * rnd.uniform(-1, 1) * math.exp(-i / (0.018 * rate)) for i in range(n)]


def build_audio():
    master = unreal.load_asset("/Engine/EngineSounds/Master")
    classes = {}
    for name in SOUND_CLASSES:
        path = f"{AUDIO}/{name}"
        sc = unreal.load_asset(path) if unreal.EditorAssetLibrary.does_asset_exist(path) else \
            asset_tools.create_asset(name, AUDIO, unreal.SoundClass, unreal.SoundClassFactory())
        if master:
            try:
                sc.set_editor_property("parent_class", master)
            except Exception as exc:  # propriété protégée selon les versions : la catégorie reste utilisable
                unreal.log_warning(f"Classe {name} : parent non défini ({exc})")
        unreal.EditorAssetLibrary.save_loaded_asset(sc)
        classes[name] = sc

    tmp = os.path.join(unreal.Paths.project_saved_dir(), "SonsGeneres")
    os.makedirs(tmp, exist_ok=True)
    sounds = {
        "SFX_UI_Deplacer": (tone([880], 0.05), "SC_Interface"),
        "SFX_UI_Valider": (tone([660], 0.06) + tone([990], 0.09), "SC_Interface"),
        "SFX_UI_Retour": (tone([520], 0.06) + tone([390], 0.08), "SC_Interface"),
        "SFX_Notification": (tone([1175, 1568], 0.12) + tone([1568, 2093], 0.18), "SC_Interface"),
        "SFX_Photo": (shutter() + tone([2400], 0.03, volume=0.2), "SC_Effets"),
    }
    tasks = []
    for name, (samples, _) in sounds.items():
        wav = os.path.join(tmp, name + ".wav")
        write_wav(wav, samples)
        task = unreal.AssetImportTask()
        task.set_editor_property("filename", wav)
        task.set_editor_property("destination_path", AUDIO)
        task.set_editor_property("destination_name", name)
        task.set_editor_property("replace_existing", True)
        task.set_editor_property("automated", True)
        task.set_editor_property("save", False)
        tasks.append(task)
    asset_tools.import_asset_tasks(tasks)
    for name, (_, class_name) in sounds.items():
        wave_asset = unreal.load_asset(f"{AUDIO}/{name}")
        if wave_asset:
            wave_asset.set_editor_property("sound_class_object", classes[class_name])
            unreal.EditorAssetLibrary.save_loaded_asset(wave_asset)
    unreal.log(f"Son : {len(classes)} catégories et {len(sounds)} sons d'interface dans {AUDIO}")


copy_data()
try:
    build_audio()
except Exception as exc:  # le niveau doit se construire même si l'import audio échoue
    unreal.log_error(f"Sons d'interface non créés : {exc}")
build_level()
