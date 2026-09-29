#!/usr/bin/env python3
"""Validate Claude/Codex spatial coordination before regenerating the blockout."""

import json
import math
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
CONTRACT = ROOT / "Game" / "Blockout" / "claude-spatial-adjustments.json"
SCENE = ROOT / "Game" / "Blockout" / "ouverture-centre-ville.gltf"
MANIFEST = ROOT / "Game" / "Blockout" / "ouverture-centre-ville.manifest.json"


def main() -> int:
    data = json.loads(CONTRACT.read_text(encoding="utf-8"))
    scene = json.loads(SCENE.read_text(encoding="utf-8"))
    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    nodes = {node["name"]: node for node in scene["nodes"]}
    zones = {zone["id"]: zone for zone in manifest["zones"]}
    errors = []
    accepted = data["accepted_adjustments"]
    origin = accepted["scent_origin"]["target_position"]
    clue = accepted["scent_clue"]["target_position"]
    distance = math.dist((origin[0], origin[2]), (clue[0], clue[2]))
    low, high = data["computed_targets"]["accepted_range_m"]
    if not low <= distance <= high:
        errors.append(f"piste olfactive {distance:.3f} m hors plage {low}-{high} m")
    for key in ("scent_origin", "scent_clue"):
        item = accepted[key]
        actual = nodes[item["node"]]["translation"]
        if actual != item["target_position"]:
            errors.append(f"{item['node']} n'applique pas la position cible {item['target_position']}")
    if zones[accepted["scent_origin"]["zone_id"]]["centre"] != [origin[0], 0, origin[2]]:
        errors.append("le centre de Z_CROISEMENT ne suit pas le départ de la piste")

    van = data["reference_vehicle"]
    centre, size = van["centre"], van["size"]
    half_x, half_z = size[0] / 2, size[2] / 2
    if not centre[0] - half_x <= clue[0] <= centre[0] + half_x:
        errors.append("EVT_PISTE n'est pas sous la longueur du fourgon")
    expected_side_z = centre[2] + half_z
    if abs(clue[2] - expected_side_z) > 0.05:
        errors.append("EVT_PISTE n'est pas au droit de la portière latérale nord")

    extension = accepted["chapter1_roundabout_extension"]
    if extension["minimum_route_length_m"] < 300 or extension["required_for_p0_p3"]:
        errors.append("extension du rond-point mal cadrée")
    sightline = accepted["perron_line_of_sight"]
    if sightline["must_hide"] != "rond_point" or len(sightline["occluders"]) < 2:
        errors.append("contrainte de ligne de vue du perron incomplète")

    guards = data["guards"]
    if not guards["no_narrative_choice_canonicalized"] or not guards["no_private_photo_reference"]:
        errors.append("garde narrative ou vie privée désactivée")
    if set(guards["opening_cameras_unchanged"]) != {"CAM_SHOULDER", "CAM_WIDE", "CAM_FIRST"}:
        errors.append("liste des trois vues modifiée")

    if errors:
        for error in errors:
            print(f"ERROR: {error}")
        return 1
    print(f"Spatial adjustments valid: scent path {distance:.3f} m; 4 coordination requests recorded.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
