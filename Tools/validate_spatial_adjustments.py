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
    if clue[0] < centre[0]:
        errors.append("le bracelet est à l'arrière, pas dans la moitié avant de la portière coulissante")

    exit_pos = nodes[accepted["alley_exit"]["node"]]["translation"]
    door_to_exit = math.dist((clue[0], clue[2]), (exit_pos[0], exit_pos[2]))
    if not 70 <= door_to_exit <= 100:
        errors.append(f"débouché à {door_to_exit:.3f} m de la portière, hors plage 70-100 m")
    entry = manifest["validation"]["departure_rule"]["alley_entry_m"]
    approach = math.dist((entry[0], entry[2]), (origin[0], origin[2]))
    if not 8 <= approach <= 12:
        errors.append(f"abordage à {approach:.3f} m de l'entrée, au lieu d'environ 10 m")
    gate = nodes["child_school_placeholder"]["translation"]
    bench = nodes["bench_dufau"]["translation"]
    bench_distance = math.dist((gate[0], gate[2]), (bench[0], bench[2]))
    if bench_distance >= 30:
        errors.append(f"banc de Dufau à {bench_distance:.3f} m du portail, doit être < 30 m")
    sidewalk_z = nodes["alley_sidewalk_north"]["translation"][2]
    sidewalk_half_width = nodes["alley_sidewalk_north"]["scale"][2] / 2
    for name in ("child_alley_placeholder", "k2_placeholder"):
        if abs(nodes[name]["translation"][2] - sidewalk_z) > sidewalk_half_width:
            errors.append(f"{name} n'est pas sur le trottoir nord")

    shots = {shot["id"]: shot for shot in manifest["validation"]["shots"]}
    separation = accepted["separate_shots"]
    if separation["place"] not in shots or separation["alley"] not in shots:
        errors.append("contrats place/ruelle absents")
    elif "van_placeholder" not in shots[separation["place"]]["must_be_occluded"]:
        errors.append("le fourgon doit être masqué depuis la place")

    guards = data["guards"]
    required = ("no_narrative_choice_canonicalized", "no_private_photo_reference",
                "lila_precise_route_open", "place_activity_open", "van_appearance_validated", "lila_appearance_validated")
    if not all(guards[name] for name in required):
        errors.append("une garde narrative, visuelle ou de vie privée est désactivée")
    if guards["opening_cameras"] != manifest["validation"]["camera_nodes"]:
        errors.append("les six caméras ne correspondent pas au manifeste")
    if guards["fallback_cameras"] != manifest["validation"]["fallback_camera_nodes"]:
        errors.append("la caméra de repli ne correspond pas au manifeste")
    exit_contract = accepted["alley_exit"]
    if exit_contract["status"] != "decision_Audrey_29_09" or exit_contract["sea_node"] not in nodes:
        errors.append("la décision de mer lointaine n'est pas appliquée")
    departure = accepted["departure_rule"]
    if departure != {"maximum_distance_to_alley_entry_m": 110, "hold_max_s": 30,
                      "fallback_camera": "CAM_DEPART_COURT", "fallback_duration_s": 4}:
        errors.append("le contrat de départ ne correspond pas à la PR #4")
    if "barrette" in json.dumps(data, ensure_ascii=False).lower():
        errors.append("la barrette ne doit plus être un indice")

    if errors:
        for error in errors:
            print("ERROR:", error)
        return 1
    print(f"Spatial adjustments valid: two shots, 6 gameplay cameras + fallback, scent path {distance:.3f} m, exit {door_to_exit:.3f} m.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
