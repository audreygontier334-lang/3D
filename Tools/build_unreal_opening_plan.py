"""Build a deterministic Unreal assembly plan from the opening manifest.

The output is an engine-neutral handoff in Unreal centimetres. It can be
checked in CI before an Unreal project exists, then consumed by an Editor
Python importer without duplicating scene coordinates.

Usage:
    python3 Tools/build_unreal_opening_plan.py
    python3 Tools/build_unreal_opening_plan.py --check
"""

import argparse
import hashlib
import json
import math
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
MANIFEST_PATH = ROOT / "Game/Blockout/ouverture-centre-ville.manifest.json"
OUTPUT_PATH = ROOT / "Game/Unreal/opening_assembly_plan.json"


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def unreal_position(vector):
    """glTF metres (east, up, north) -> Unreal cm (X east, Y south, Z up)."""
    east, up, north = vector
    return [round(east * 100, 4), round(-north * 100, 4), round(up * 100, 4)]


def unreal_size(vector):
    east, up, north = vector
    return [round(abs(east) * 100, 4), round(abs(north) * 100, 4), round(abs(up) * 100, 4)]


def collision_profile(name, kind):
    if kind == "event_marker":
        return "OverlapOnly"
    if kind == "gameplay_placeholder":
        return "QueryOnly"
    if name.startswith(("tree_crown", "school_roof", "west_shop_roof", "corner_roof", "east_shop_roof")):
        return "NoCollision"
    return "BlockAll"


def node_kind(name):
    if name.endswith("_marker"):
        return "event_marker"
    if name.endswith("_placeholder"):
        return "gameplay_placeholder"
    return "static_geometry"


def build_plan():
    manifest = json.loads(MANIFEST_PATH.read_text(encoding="utf-8"))
    gltf_path = ROOT / manifest["source_gltf"]
    gltf = json.loads(gltf_path.read_text(encoding="utf-8"))
    nodes = {node["name"]: node for node in gltf["nodes"]}

    actors = []
    for node in gltf["nodes"]:
        if "camera" in node:
            continue
        kind = node_kind(node["name"])
        actor = {
            "id": node["name"],
            "kind": kind,
            "location_cm": unreal_position(node.get("translation", [0, 0, 0])),
            "collision_profile": collision_profile(node["name"], kind),
        }
        if "scale" in node:
            actor["bounds_size_cm"] = unreal_size(node["scale"])
        if "extras" in node and "role" in node["extras"]:
            actor["role"] = node["extras"]["role"]
        actors.append(actor)

    cameras = []
    all_camera_ids = manifest["validation"]["camera_nodes"] + manifest["validation"]["fallback_camera_nodes"]
    for name in all_camera_ids:
        node = nodes[name]
        camera = gltf["cameras"][node["camera"]]
        quaternion = node["rotation"]
        length = math.sqrt(sum(value * value for value in quaternion))
        cameras.append({
            "id": name,
            "location_cm": unreal_position(node["translation"]),
            "source_quaternion_xyzw": [round(value / length, 8) for value in quaternion],
            "vertical_fov_degrees": round(math.degrees(camera["perspective"]["yfov"]), 4),
            "near_clip_cm": round(camera["perspective"]["znear"] * 100, 4),
            "far_clip_cm": round(camera["perspective"]["zfar"] * 100, 4),
            "orientation_import": "preserve_from_gltf",
        })

    zones = [{
        "id": zone["id"],
        "centre_cm": unreal_position(zone["centre"]),
        "role": zone["role"],
    } for zone in manifest["zones"]]

    markers = [{
        "id": marker["id"],
        "source_node": marker["node"],
        "location_cm": unreal_position(nodes[marker["node"]]["translation"]),
    } for marker in manifest["event_markers"]]

    return {
        "plan_version": 1,
        "scene_id": manifest["scene_id"],
        "status": "assembly_handoff_not_playable",
        "source": {
            "manifest": str(MANIFEST_PATH.relative_to(ROOT)),
            "manifest_sha256": digest(MANIFEST_PATH),
            "gltf": str(gltf_path.relative_to(ROOT)),
            "gltf_sha256": digest(gltf_path),
        },
        "coordinate_conversion": {
            "source": "gltf_metres_X_east_Y_up_Z_north",
            "target": "unreal_centimetres_X_east_Y_south_Z_up",
            "formula": "[x, y, z] -> [100*x, -100*z, 100*y]",
        },
        "actors": actors,
        "zones": zones,
        "event_markers": markers,
        "cameras": cameras,
        "runtime_view_cycle": {
            "ordered_camera_ids": manifest["validation"]["camera_nodes"],
            "must_not_mutate_gameplay_state": True,
        },
        "departure_fallback": manifest["validation"]["fallback_shot"],
        "audio_policy": manifest["audio_policy"],
        "decision_guards": manifest["decision_guards"],
        "limitations": [
            "No Unreal project or packaged Windows build exists yet.",
            "Camera orientation must be preserved by the glTF importer and checked in-engine.",
            "Collision profiles are an assembly proposal and require an Unreal playtest.",
            "Placeholders are not final photorealistic characters, vehicle or props."
        ],
    }


def serialise(plan):
    return json.dumps(plan, ensure_ascii=False, indent=2) + "\n"


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true", help="fail if the committed plan is stale")
    args = parser.parse_args()
    expected = serialise(build_plan())

    if args.check:
        if not OUTPUT_PATH.exists() or OUTPUT_PATH.read_text(encoding="utf-8") != expected:
            raise SystemExit("Unreal opening plan is missing or stale; rebuild it without --check")
        plan = json.loads(expected)
        print(
            f"Unreal plan valid: {len(plan['actors'])} actors, "
            f"{len(plan['zones'])} zones, {len(plan['cameras'])} cameras, "
            f"{len(plan['event_markers'])} event markers."
        )
        return

    OUTPUT_PATH.parent.mkdir(parents=True, exist_ok=True)
    OUTPUT_PATH.write_text(expected, encoding="utf-8")
    print(OUTPUT_PATH)


if __name__ == "__main__":
    main()
