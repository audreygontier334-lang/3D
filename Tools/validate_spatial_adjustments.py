#!/usr/bin/env python3
"""Validate the two-shot Claude/Codex spatial coordination contract."""

import json
import math
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
CONTRACT = ROOT / "Game/Blockout/claude-spatial-adjustments.json"
SCENE = ROOT / "Game/Blockout/ouverture-centre-ville.gltf"
MANIFEST = ROOT / "Game/Blockout/ouverture-centre-ville.manifest.json"


def main():
    data = json.loads(CONTRACT.read_text(encoding="utf-8"))
    scene = json.loads(SCENE.read_text(encoding="utf-8"))
    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    nodes = {node["name"]: node for node in scene["nodes"]}
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
        if nodes[item["node"]]["translation"] != item["target_position"]:
            errors.append(f"{item['node']} n'applique pas sa position cible")

    van = data["reference_vehicle"]
    centre, size = van["centre"], van["size"]
    if nodes[van["node"]]["translation"] != centre:
        errors.append("position du fourgon différente du contrat")
    if not centre[0] - size[0] / 2 <= clue[0] <= centre[0] + size[0] / 2:
        errors.append("le bracelet n'est pas sous la longueur du fourgon")
    if abs(clue[2] - (centre[2] + size[2] / 2)) > .05:
        errors.append("le bracelet n'est pas au droit de la portière")

    shots = {shot["id"]: shot for shot in manifest["validation"]["shots"]}
    separation = accepted["separate_shots"]
    if separation["place"] not in shots or separation["alley"] not in shots:
        errors.append("contrats place/ruelle absents")
    elif "van_placeholder" not in shots[separation["place"]]["must_be_occluded"]:
        errors.append("le fourgon doit être masqué depuis la place")

    guards = data["guards"]
    required = ("no_narrative_choice_canonicalized", "no_private_photo_reference",
                "lila_movement_wardrobe_hair_open", "place_activity_open", "van_appearance_open")
    if not all(guards[name] for name in required):
        errors.append("une garde narrative, visuelle ou de vie privée est désactivée")
    if guards["opening_cameras"] != manifest["validation"]["camera_nodes"]:
        errors.append("les six caméras ne correspondent pas au manifeste")
    if "barrette" in json.dumps(data, ensure_ascii=False).lower():
        errors.append("la barrette ne doit plus être un indice")

    if errors:
        for error in errors:
            print("ERROR:", error)
        return 1
    print(f"Spatial adjustments valid: two shots, 6 cameras, scent path {distance:.3f} m; choices remain open.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
