"""Generate the two-shot opening blockout: school square and adjacent alley.

This is spatial test geometry, not final art or a playable Unreal level.
Run: python3 Tools/build_opening_blockout.py
"""

import base64
import json
import math
import struct
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
OUTPUT = ROOT / "Game" / "Blockout" / "ouverture-centre-ville.gltf"

FACES = [
    ((0, 0, 1), [(-.5, -.5, .5), (.5, -.5, .5), (.5, .5, .5), (-.5, .5, .5)]),
    ((0, 0, -1), [(.5, -.5, -.5), (-.5, -.5, -.5), (-.5, .5, -.5), (.5, .5, -.5)]),
    ((1, 0, 0), [(.5, -.5, .5), (.5, -.5, -.5), (.5, .5, -.5), (.5, .5, .5)]),
    ((-1, 0, 0), [(-.5, -.5, -.5), (-.5, -.5, .5), (-.5, .5, .5), (-.5, .5, -.5)]),
    ((0, 1, 0), [(-.5, .5, .5), (.5, .5, .5), (.5, .5, -.5), (-.5, .5, -.5)]),
    ((0, -1, 0), [(-.5, -.5, -.5), (.5, -.5, -.5), (.5, -.5, .5), (-.5, -.5, .5)]),
]
positions, normals, indices = [], [], []
for normal, corners in FACES:
    first = len(positions) // 3
    for point in corners:
        positions.extend(point)
        normals.extend(normal)
    indices.extend([first, first + 1, first + 2, first, first + 2, first + 3])

position_bytes = struct.pack("<" + "f" * len(positions), *positions)
normal_bytes = struct.pack("<" + "f" * len(normals), *normals)
index_bytes = struct.pack("<" + "H" * len(indices), *indices)
buffer = position_bytes + normal_bytes + index_bytes

palette = {
    "sol": [0.40, 0.40, 0.39, 1], "trottoir": [0.64, 0.60, 0.53, 1],
    "creme": [0.83, 0.75, 0.62, 1], "rose": [0.76, 0.58, 0.55, 1],
    "bleu": [0.50, 0.66, 0.70, 1], "blanc": [0.84, 0.84, 0.80, 1],
    "toit": [0.48, 0.31, 0.24, 1], "ecole": [0.83, 0.72, 0.55, 1],
    "fourgon": [0.72, 0.72, 0.69, 1], "heroine": [0.16, 0.29, 0.52, 1],
    "ariane": [0.62, 0.44, 0.25, 1], "fillette": [0.70, 0.34, 0.37, 1],
    "k2": [0.35, 0.30, 0.42, 1], "vegetation": [0.22, 0.37, 0.22, 1],
    "repere": [0.91, 0.67, 0.12, 1],
}
materials = [{"name": n, "pbrMetallicRoughness": {"baseColorFactor": c, "metallicFactor": 0, "roughnessFactor": 1}}
             for n, c in palette.items()]
material_index = {name: i for i, name in enumerate(palette)}
meshes = [{"name": "cube_" + name, "primitives": [{"attributes": {"POSITION": 0, "NORMAL": 1},
          "indices": 2, "material": material_index[name]}]} for name in palette]
nodes = []


def box(name, centre, size, material, note=None):
    node = {"name": name, "mesh": material_index[material], "translation": centre, "scale": size}
    if note:
        node["extras"] = {"role": note}
    nodes.append(node)


# Plan A: school square. The alley begins after the eastern corner.
box("ground", [35, -.18, -20], [170, .30, 110], "sol")
box("school_square", [0, -.02, -8], [54, .06, 28], "sol")
box("school_sidewalk", [0, .02, -1], [46, .12, 4], "trottoir")
box("school_main", [-7, 4, 9], [24, 8, 11], "ecole", "école, plan A")
box("school_roof", [-7, 8.45, 9], [25, .9, 12], "toit")
box("school_gate_left", [-2.5, .8, 1], [.3, 1.6, .3], "repere")
box("school_gate_right", [2.5, .8, 1], [.3, 1.6, .3], "repere")
box("square_shop_west", [-28, 3.2, -20], [14, 6.4, 10], "creme")
box("square_shop_west_roof", [-28, 6.7, -20], [15, .6, 11], "toit")
box("alley_corner_mask", [27, 4.0, -3], [13, 8, 18], "rose",
    "masque le fourgon et l'abordage depuis la place")
box("alley_corner_roof", [27, 8.35, -3], [14, .7, 19], "toit")
box("tree_trunk_square", [-17, 2.2, -5], [.6, 4.4, .6], "vegetation")
box("tree_crown_square", [-17, 5.6, -5], [4.2, 3.6, 4.2], "vegetation")

# Transition and plan B: a narrower alley that turns away from the square.
box("alley_entry_road", [35, -.02, -15], [24, .06, 9], "sol")
box("alley_road", [64, -.02, -22], [62, .06, 9], "sol")
box("alley_sidewalk_north", [64, .02, -15.8], [62, .12, 2.8], "trottoir")
box("alley_sidewalk_south", [64, .02, -28.2], [62, .12, 2.8], "trottoir")
# Varied placeholder houses, deliberately different in size, colour and setback.
for args in [
    ("alley_house_1900", [43, 3.7, -8], [10, 7.4, 9], "creme"),
    ("alley_house_1930", [57, 3.0, -7], [9, 6.0, 8], "bleu"),
    ("alley_house_1970", [71, 2.5, -8], [13, 5.0, 9], "blanc"),
    ("alley_house_recent", [87, 3.4, -7], [11, 6.8, 10], "rose"),
    ("alley_house_south_1", [48, 3.2, -35], [12, 6.4, 9], "rose"),
    ("alley_house_south_2", [65, 3.8, -36], [10, 7.6, 10], "creme"),
    ("alley_house_south_3", [82, 2.6, -35], [15, 5.2, 9], "bleu"),
]:
    box(*args)
for name, x, z, length in [("hedge_north_1", 48, -13.7, 8), ("hedge_north_2", 67, -13.7, 12),
                           ("wall_south_1", 52, -30.0, 11), ("fence_south_2", 76, -30.0, 15)]:
    box(name, [x, .75, z], [length, 1.5, .5], "vegetation" if "hedge" in name else "blanc")

# Static checkpoints. Duplicates represent different instants, not simultaneous characters.
box("heroine_place_placeholder", [8, .83, -18], [.5, 1.66, .36], "heroine", "plan A checkpoint")
box("ariane_place_placeholder", [5.5, .42, -10.5], [1.05, .84, .48], "ariane", "plan A checkpoint")
box("child_school_placeholder", [1, .62, -.5], [.36, 1.24, .30], "fillette", "Lila au portail, plan A")
box("heroine_alley_placeholder", [37, .83, -21], [.5, 1.66, .36], "heroine", "entrée de ruelle, plan B")
box("ariane_alley_placeholder", [45, .42, -20.5], [1.05, .84, .48], "ariane", "entrée de ruelle, plan B")
box("child_alley_placeholder", [60, .62, -21], [.36, 1.24, .30], "fillette", "Lila devant le fourgon, plan B")
box("k2_placeholder", [61, .83, -20], [.5, 1.66, .36], "k2", "femme au badge, plan B")
box("van_placeholder", [66, 1.1, -22], [5.2, 2.2, 2.1], "fourgon", "apparence provisoire; plaque arrière vers l'entrée")
box("scent_object_marker", [44, .13, -19], [.35, .26, .35], "repere", "porte-clés de Lila; proposition")
box("scent_clue_marker", [63.5, .13, -19.9], [.35, .26, .35], "repere", "bracelet en perles au pied de la portière; proposition")
box("alley_exit_marker", [95, .25, -22], [.5, .5, .5], "repere", "débouché; mer/ciel/lumière à choisir par Audrey")


def camera_rotation(eye, target):
    forward = [target[i] - eye[i] for i in range(3)]
    magnitude = math.sqrt(sum(v * v for v in forward))
    forward = [v / magnitude for v in forward]
    # glTF cameras look along local -Z with local +Y up.  The right vector
    # must be forward × world-up; its opposite rolls the camera 180 degrees.
    right = [-forward[2], 0, forward[0]]
    length = math.sqrt(sum(v * v for v in right))
    right = [v / length for v in right]
    up = [right[1] * forward[2] - right[2] * forward[1], right[2] * forward[0] - right[0] * forward[2],
          right[0] * forward[1] - right[1] * forward[0]]
    matrix = [right, up, [-v for v in forward]]
    m00, m01, m02 = matrix[0][0], matrix[1][0], matrix[2][0]
    m10, m11, m12 = matrix[0][1], matrix[1][1], matrix[2][1]
    m20, m21, m22 = matrix[0][2], matrix[1][2], matrix[2][2]
    trace = m00 + m11 + m22
    if trace > 0:
        s = math.sqrt(trace + 1) * 2
        return [(m21 - m12) / s, (m02 - m20) / s, (m10 - m01) / s, s / 4]
    if m00 > m11 and m00 > m22:
        s = math.sqrt(1 + m00 - m11 - m22) * 2
        return [s / 4, (m01 + m10) / s, (m02 + m20) / s, (m21 - m12) / s]
    if m11 > m22:
        s = math.sqrt(1 + m11 - m00 - m22) * 2
        return [(m01 + m10) / s, s / 4, (m12 + m21) / s, (m02 - m20) / s]
    s = math.sqrt(1 + m22 - m00 - m11) * 2
    return [(m02 + m20) / s, (m12 + m21) / s, s / 4, (m10 - m01) / s]


cameras = []
camera_specs = [
    ("CAM_PLACE_SHOULDER", [8.4, 2.15, -20], [2, 1.0, -1], 58),
    ("CAM_PLACE_WIDE", [7, 4.7, -26], [1, 1.0, -2], 65),
    ("CAM_PLACE_FIRST", [8, 1.61, -18], [1.5, 1.0, -1], 67),
    ("CAM_RUELLE_SHOULDER", [37.4, 2.15, -23], [62, 1.0, -21], 58),
    ("CAM_RUELLE_WIDE", [35, 4.2, -29], [62, 1.0, -21], 65),
    ("CAM_RUELLE_FIRST", [37, 1.61, -21], [62, 1.0, -21], 67),
]
for name, eye, target, fov in camera_specs:
    cameras.append({"name": name, "type": "perspective", "perspective": {"yfov": math.radians(fov), "znear": .1, "zfar": 220}})
    nodes.append({"name": name, "camera": len(cameras) - 1, "translation": eye,
                  "rotation": camera_rotation(eye, target),
                  "extras": {"role": "point de contrôle statique; suffixe = mode de caméra"}})

gltf = {
    "asset": {"version": "2.0", "generator": "Tools/build_opening_blockout.py",
              "extras": {"status": "maquette spatiale de travail, non photoréaliste, non jouable"}},
    "scene": 0,
    "scenes": [{"name": "Ouverture plans A et B", "nodes": list(range(len(nodes)))}],
    "nodes": nodes, "cameras": cameras, "meshes": meshes, "materials": materials,
    "buffers": [{"byteLength": len(buffer), "uri": "data:application/octet-stream;base64," + base64.b64encode(buffer).decode("ascii")}],
    "bufferViews": [
        {"buffer": 0, "byteOffset": 0, "byteLength": len(position_bytes), "target": 34962},
        {"buffer": 0, "byteOffset": len(position_bytes), "byteLength": len(normal_bytes), "target": 34962},
        {"buffer": 0, "byteOffset": len(position_bytes) + len(normal_bytes), "byteLength": len(index_bytes), "target": 34963}],
    "accessors": [
        {"bufferView": 0, "componentType": 5126, "count": 24, "type": "VEC3", "min": [-.5, -.5, -.5], "max": [.5, .5, .5]},
        {"bufferView": 1, "componentType": 5126, "count": 24, "type": "VEC3"},
        {"bufferView": 2, "componentType": 5123, "count": 36, "type": "SCALAR", "min": [0], "max": [23]}],
    "extras": {"axis": "X east, Y up, Z north", "unit": "metre", "school_gate": [0, 0, 0],
               "scene_notes": "place et ruelle distinctes; apparences de Lila et du fourgon provisoires; débouché ouvert à décision"},
}
OUTPUT.parent.mkdir(parents=True, exist_ok=True)
OUTPUT.write_text(json.dumps(gltf, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
print(f"{OUTPUT}: {len(nodes)} nodes, {len(cameras)} cameras, {len(buffer)} embedded bytes")
