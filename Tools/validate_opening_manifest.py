"""Validate the two-shot opening contract with standard-library Python."""

import json
import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
MANIFEST = ROOT / "Game/Blockout/ouverture-centre-ville.manifest.json"
EXPECTED_ZONES = {"Z_PROMENADE", "Z_ECOLE", "Z_CROISEMENT", "Z_RUE_FUITE", "Z_PLACE", "Z_TEMOINS"}
EXPECTED_AUDIO = {"sortie_ecole", "circulation_locale", "oiseaux_urbains", "vent_dans_les_pins"}


def require(condition, message):
    if not condition:
        raise SystemExit(f"Manifest check failed: {message}")


spec = json.loads(MANIFEST.read_text(encoding="utf-8"))
scene = json.loads((ROOT / spec["source_gltf"]).read_text(encoding="utf-8"))
nodes = {node["name"]: node for node in scene["nodes"]}

require(spec["manifest_version"] == 3, "unsupported manifest version")
require(spec["coordinate_system"] == {"units": "metres", "up_axis": "Y"}, "coordinate system mismatch")
require(len(nodes) == len(scene["nodes"]), "glTF node names must be unique")
require({zone["id"] for zone in spec["zones"]} == EXPECTED_ZONES, "the six gameplay zones must be declared")

validation = spec["validation"]
shots = validation["shots"]
require([shot["id"] for shot in shots] == ["PLAN_A_PLACE_ECOLE", "PLAN_B_RUELLE"],
        "place and alley must be separate validation shots")
all_cameras = [name for shot in shots for name in shot["camera_nodes"]]
require(all_cameras == validation["camera_nodes"], "flat camera list must match the two shot contracts")
require(len(set(all_cameras)) == 6, "three distinct cameras are required per shot")
fallback = validation["fallback_shot"]
require(validation["fallback_camera_nodes"] == [fallback["camera_node"]], "fallback camera contract mismatch")
require(fallback["duration_s"] == 4, "departure fallback must last 4 seconds")
require(fallback["camera_node"] in nodes and "camera" in nodes[fallback["camera_node"]], "missing departure fallback camera")
for name in fallback["required_subject_nodes"]:
    require(name in nodes, f"missing fallback subject {name}")
for shot in shots:
    require(len(shot["camera_nodes"]) == 3, f"{shot['id']} must expose three cameras")
    for name in shot["camera_nodes"]:
        require(name in nodes and "camera" in nodes[name], f"missing camera {name}")
    for name in shot["required_subject_nodes"] + shot.get("must_be_occluded", []):
        require(name in nodes, f"missing shot subject {name}")
for marker in spec["event_markers"]:
    require(marker["node"] in nodes, f"{marker['id']} references missing node")

width, height = validation["frame_aspect_ratio"]
require(width > 0 and height > 0, "frame aspect ratio must be positive")
require(0 <= validation["safe_frame_margin"] < .5, "invalid safe-frame margin")
require(EXPECTED_AUDIO <= set(spec["audio_policy"]["always_available"]), "urban ambience incomplete")
surf = next((item for item in spec["audio_policy"]["conditional"] if item["id"] == "ressac"), None)
require(surf and surf["enabled_by_default"] is False, "surf must remain conditional")

choices = spec["open_visual_choices"]
require(set(choices) == {"V1_place_animation"}, "only the school-square activity remains open in this blockout")
fixed = spec["validated_visual_constraints"]
require(fixed["V3_alley_exit"] == "Mer visible au loin; port, chaussee du boulevard et rond-point invisibles.",
        "Audrey's validated alley exit changed")
require(fixed["ariane_scarf"] == "noir_paisley_blanc", "Ariane's validated paisley scarf changed")
require(fixed["V2_van_appearance"].startswith("Fourgon blanc use"), "Audrey's validated van changed")
require(fixed["V4_lila_appearance"].startswith("Sweat jaune moutarde"), "Audrey's validated Lila appearance changed")
require("sea_horizon_band" in nodes and "boulevard_road_screen" in nodes, "validated sea/screen geometry missing")
guards = spec["decision_guards"]
require(guards["personal_photos"]["allowed_in_repository"] is False, "personal photos must stay private")
require(guards["lila_hair_or_clothing_reference"] is True, "Lila's approved appearance is missing")
require(guards["van_appearance_is_canonical"] is True, "approved van appearance is missing")
require(guards["lila_precise_route_is_canonical"] is False, "Lila's precise route must remain open")
require(guards["protagonist_shoes"]["current_value"] == "baskets_style_air_max_noires_blanches_sans_logo_ni_marque",
        "Audrey's validated footwear changed")

rule = validation["departure_rule"]
(min_x, min_z), (max_x, max_z) = rule["reachable_bounds_xz_m"]
entry_x, _, entry_z = rule["alley_entry_m"]
farthest = max(((x-entry_x)**2 + (z-entry_z)**2)**.5 for x in (min_x, max_x) for z in (min_z, max_z))
require(farthest <= rule["maximum_distance_to_entry_m"], "reachable blockout exceeds 110 m from alley entry")
require(rule["hold_max_s"] == 30 and rule["fallback_duration_s"] == 4, "departure timing changed")

check = subprocess.run([sys.executable, str(ROOT / "Tools/check_opening_sightlines.py")],
                       cwd=ROOT, text=True, capture_output=True)
print(check.stdout, end="")
if check.returncode:
    print(check.stderr, end="", file=sys.stderr)
    raise SystemExit(check.returncode)

print(f"Manifest valid: 2 separate shots, 6 gameplay cameras + 1 fallback, farthest point {farthest:.1f} m, route still open.")
