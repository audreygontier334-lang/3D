"""Generate a small, self-contained glTF blockout of the opening streets.

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

# glTF uses metres here: X east, Y up, Z north. The school gate is (0, 0, 0).
# A cube is shared by every object; dimensions and positions live on nodes.
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
    "sol": [0.40, 0.40, 0.39, 1],
    "trottoir": [0.64, 0.60, 0.53, 1],
    "batiment": [0.80, 0.76, 0.66, 1],
    "toit": [0.49, 0.35, 0.29, 1],
    "ecole": [0.83, 0.72, 0.55, 1],
    "fourgon": [0.84, 0.84, 0.79, 1],
    "heroine": [0.16, 0.29, 0.52, 1],
    "ariane": [0.62, 0.44, 0.25, 1],
    "fillette": [0.70, 0.34, 0.37, 1],
    "vegetation": [0.22, 0.37, 0.22, 1],
    "repere": [0.91, 0.67, 0.12, 1],
}
materials = [
    {"name": name, "pbrMetallicRoughness": {"baseColorFactor": rgba, "metallicFactor": 0, "roughnessFactor": 1}}
    for name, rgba in palette.items()
]
material_index = {name: i for i, name in enumerate(palette)}
meshes = []
for name in palette:
    meshes.append({
        "name": "cube_" + name,
        "primitives": [{
            "attributes": {"POSITION": 0, "NORMAL": 1},
            "indices": 2,
            "material": material_index[name],
        }],
    })

nodes = []


def box(name, centre, size, material, note=None):
    node = {"name": name, "mesh": material_index[material], "translation": centre, "scale": size}
    if note:
        node["extras"] = {"role": note}
    nodes.append(node)


# A compact, walkable town centre: a square by the school, an eastbound road,
# a side street, low shop fronts and a sightline broken by the street corner.
box("ground", [12, -.18, -10], [130, .30, 90], "sol")
box("school_sidewalk", [-3, .02, -1], [42, .12, 4], "trottoir")
box("south_sidewalk", [25, .02, -21], [95, .12, 4], "trottoir")
box("east_road", [26, -.02, -11], [100, .06, 12], "sol")
box("cross_street", [43, -.02, -2], [10, .06, 37], "sol")
box("school_main", [-7, 4, 9], [24, 8, 11], "ecole", "sortie sur la place; entrée côté sud")
box("school_roof", [-7, 8.45, 9], [25, .9, 12], "toit")
box("school_gate_left", [-2.5, .8, 1], [.3, 1.6, .3], "repere")
box("school_gate_right", [2.5, .8, 1], [.3, 1.6, .3], "repere")
box("west_shop", [-31, 3.2, -24], [15, 6.4, 9], "batiment")
box("west_shop_roof", [-31, 6.7, -24], [16, .6, 10], "toit")
box("corner_house", [21, 3.4, 1], [12, 6.8, 10], "batiment", "masque partiellement la sortie est")
box("corner_roof", [21, 7.1, 1], [13, .7, 11], "toit")
box("east_shop", [57, 3.2, -23], [14, 6.4, 10], "batiment")
box("east_shop_roof", [57, 6.7, -23], [15, .6, 11], "toit")
box("tree_trunk_1", [-20, 2.2, -3], [.6, 4.4, .6], "vegetation")
box("tree_crown_1", [-20, 5.6, -3], [4.2, 3.6, 4.2], "vegetation")
box("tree_trunk_2", [37, 2.2, -24], [.6, 4.4, .6], "vegetation")
box("tree_crown_2", [37, 5.6, -24], [4.2, 3.6, 4.2], "vegetation")
box("van_placeholder", [29, 1.1, -11], [5.2, 2.2, 2.1], "fourgon", "couleur provisoire; identité ouverte")
box("heroine_placeholder", [8, .83, -19], [.5, 1.66, .36], "heroine", "volume de test seulement")
box("ariane_placeholder", [10, .42, -17.8], [1.05, .84, .48], "ariane", "libre; aucun collier ni laisse")
box("child_placeholder", [1, .62, -.5], [.36, 1.24, .30], "fillette", "silhouette fictive de test")
box("scent_object_marker", [26, .13, -6], [.35, .26, .35], "repere", "objet à définir avec Audrey; après départ du fourgon")
box("scent_clue_marker", [33, .13, -2], [.35, .26, .35], "repere", "indice olfactif provisoire")


def camera_rotation(eye, target):
    # glTF camera looks along local -Z with local +Y up.
    forward = [target[i] - eye[i] for i in range(3)]
    magnitude = math.sqrt(sum(v * v for v in forward))
    forward = [v / magnitude for v in forward]
    right = [forward[2], 0, -forward[0]]
    length = math.sqrt(sum(v * v for v in right))
    right = [v / length for v in right]
    up = [right[1] * forward[2] - right[2] * forward[1],
          right[2] * forward[0] - right[0] * forward[2],
          right[0] * forward[1] - right[1] * forward[0]]
    matrix = [right, up, [-v for v in forward]]
    # Rotation matrix columns -> quaternion.
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
for name, eye, target, fov in [
    ("CAM_SHOULDER", [8.4, 2.15, -21], [13, 1.15, -8], 72),
    ("CAM_WIDE", [6, 4.7, -27], [14, 1.1, -8], 65),
    ("CAM_FIRST", [8, 1.61, -19], [12.5, 1.1, -8], 80),
]:
    cameras.append({"name": name, "type": "perspective", "perspective": {"yfov": math.radians(fov), "znear": .1, "zfar": 200}})
    nodes.append({"name": name, "camera": len(cameras) - 1, "translation": eye, "rotation": camera_rotation(eye, target),
                  "extras": {"role": "point de contrôle visuel statique; pas une caméra jouable"}})

gltf = {
    "asset": {"version": "2.0", "generator": "Tools/build_opening_blockout.py",
              "extras": {"status": "maquette spatiale de travail, non photoréaliste, non jouable"}},
    "scene": 0,
    "scenes": [{"name": "P0-P3 centre-ville", "nodes": list(range(len(nodes)))}],
    "nodes": nodes,
    "cameras": cameras,
    "meshes": meshes,
    "materials": materials,
    "buffers": [{"byteLength": len(buffer), "uri": "data:application/octet-stream;base64," + base64.b64encode(buffer).decode("ascii")}],
    "bufferViews": [
        {"buffer": 0, "byteOffset": 0, "byteLength": len(position_bytes), "target": 34962},
        {"buffer": 0, "byteOffset": len(position_bytes), "byteLength": len(normal_bytes), "target": 34962},
        {"buffer": 0, "byteOffset": len(position_bytes) + len(normal_bytes), "byteLength": len(index_bytes), "target": 34963},
    ],
    "accessors": [
        {"bufferView": 0, "componentType": 5126, "count": 24, "type": "VEC3", "min": [-.5, -.5, -.5], "max": [.5, .5, .5]},
        {"bufferView": 1, "componentType": 5126, "count": 24, "type": "VEC3"},
        {"bufferView": 2, "componentType": 5123, "count": 36, "type": "SCALAR", "min": [0], "max": [23]},
    ],
    "extras": {"axis": "X east, Y up, Z north", "unit": "metre", "school_gate": [0, 0, 0],
               "scene_notes": "centre-ville côtier, sans port ni mer visible; route et indices provisoires"},
}

OUTPUT.parent.mkdir(parents=True, exist_ok=True)
OUTPUT.write_text(json.dumps(gltf, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
print(f"{OUTPUT}: {len(nodes)} nodes, {len(cameras)} cameras, {len(buffer)} embedded bytes")
