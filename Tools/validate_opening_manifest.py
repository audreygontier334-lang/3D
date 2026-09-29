"""Validate the opening blockout contract and its camera sightlines.

This uses only Python's standard library so it can run before an Unreal
project exists. It does not replace an in-engine playtest.
"""

import json
import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
MANIFEST = ROOT / "Game/Blockout/ouverture-centre-ville.manifest.json"
EXPECTED_ZONES = {
    "Z_PROMENADE",
    "Z_ECOLE",
    "Z_CROISEMENT",
    "Z_RUE_FUITE",
    "Z_PLACE",
    "Z_TEMOINS",
}
EXPECTED_AUDIO = {
    "sortie_ecole",
    "circulation_locale",
    "oiseaux_urbains",
    "vent_dans_les_pins",
}


def require(condition, message):
    if not condition:
        raise SystemExit(f"Manifest check failed: {message}")


spec = json.loads(MANIFEST.read_text(encoding="utf-8"))
scene_path = ROOT / spec["source_gltf"]
scene = json.loads(scene_path.read_text(encoding="utf-8"))

require(spec["manifest_version"] == 1, "unsupported manifest version")
require(spec["coordinate_system"] == {"units": "metres", "up_axis": "Y"},
        "coordinate system must match the generated glTF")

node_list = scene["nodes"]
node_names = [node["name"] for node in node_list]
require(len(node_names) == len(set(node_names)), "glTF node names must be unique")
nodes = {node["name"]: node for node in node_list}

zone_ids = {zone["id"] for zone in spec["zones"]}
require(zone_ids == EXPECTED_ZONES, "the six P0-P3 gameplay zones must be declared")
for zone in spec["zones"]:
    require(len(zone["centre"]) == 3, f"{zone['id']} needs a 3D centre")

validation = spec["validation"]
for camera_name in validation["camera_nodes"]:
    require(camera_name in nodes, f"missing camera node {camera_name}")
    require("camera" in nodes[camera_name], f"{camera_name} is not linked to a glTF camera")
for subject_name in validation["required_subject_nodes"]:
    require(subject_name in nodes, f"missing required subject {subject_name}")
for marker in spec["event_markers"]:
    require(marker["node"] in nodes, f"{marker['id']} references missing node {marker['node']}")

width, height = validation["frame_aspect_ratio"]
require(width > 0 and height > 0, "frame aspect ratio must be positive")
require(0 <= validation["safe_frame_margin"] < 0.5,
        "safe frame margin must be between 0 and 0.5")

available_audio = set(spec["audio_policy"]["always_available"])
require(EXPECTED_AUDIO <= available_audio, "the validated urban ambience set is incomplete")
surf = next((item for item in spec["audio_policy"]["conditional"] if item["id"] == "ressac"), None)
require(surf is not None and surf["enabled_by_default"] is False,
        "surf must remain conditional until coastal distance is credible")

shoes = spec["decision_guards"]["protagonist_shoes"]
require(shoes["current_value"] == "baskets_retro_running_noires_blanches_sans_marque",
        "footwear must match Audrey's validated wardrobe choice")
require(shoes["status"] == "validated_by_Audrey",
        "validated footwear must not be marked as an unresolved proposal")
require(spec["decision_guards"]["personal_photos"]["allowed_in_repository"] is False,
        "personal photos must remain excluded from the repository")

check = subprocess.run(
    [sys.executable, str(ROOT / "Tools/check_opening_sightlines.py")],
    cwd=ROOT,
    text=True,
    capture_output=True,
)
print(check.stdout, end="")
if check.returncode:
    print(check.stderr, end="", file=sys.stderr)
    raise SystemExit(check.returncode)

print(
    "Manifest valid: 6 zones, 3 cameras, "
    f"{len(validation['required_subject_nodes'])} required subjects, audio policy guarded."
)
