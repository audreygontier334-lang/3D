"""Check separate school-square and alley camera checkpoints at 16:9."""

import json
import math
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
spec = json.loads((ROOT / "Game/Blockout/ouverture-centre-ville.manifest.json").read_text(encoding="utf-8"))
data = json.loads((ROOT / spec["source_gltf"]).read_text(encoding="utf-8"))
nodes = {node["name"]: node for node in data["nodes"]}
prefixes = tuple(spec["validation"]["occluder_prefixes"])
occluders = tuple(n for n in data["nodes"] if n["name"].startswith(prefixes))
aw, ah = spec["validation"]["frame_aspect_ratio"]
safe = 1 - spec["validation"]["safe_frame_margin"]


def dot(a, b): return sum(x * y for x, y in zip(a, b))
def cross(a, b): return (a[1]*b[2]-a[2]*b[1], a[2]*b[0]-a[0]*b[2], a[0]*b[1]-a[1]*b[0])
def rotate(q, v):
    axis, scalar = q[:3], q[3]
    twice = tuple(2*n for n in cross(axis, v))
    again = cross(axis, twice)
    return tuple(v[i] + scalar*twice[i] + again[i] for i in range(3))


def intersects_box(origin, destination, box):
    centre, size = box["translation"], box["scale"]
    low, high = 0.0, 1.0
    for i in range(3):
        minimum, maximum = centre[i]-size[i]/2, centre[i]+size[i]/2
        delta = destination[i]-origin[i]
        if abs(delta) < 1e-9:
            if not minimum <= origin[i] <= maximum: return False
            continue
        near, far = sorted(((minimum-origin[i])/delta, (maximum-origin[i])/delta))
        low, high = max(low, near), min(high, far)
        if low > high: return False
    return low < .999 and high > .001


def projection(camera, target):
    eye, q = camera["translation"], camera["rotation"]
    right, up, forward = rotate(q, (1,0,0)), rotate(q, (0,1,0)), rotate(q, (0,0,-1))
    direction = tuple(target[i]-eye[i] for i in range(3))
    distance = dot(direction, forward)
    vertical = math.tan(data["cameras"][camera["camera"]]["perspective"]["yfov"]/2)
    x = dot(direction, right)/(distance*vertical*aw/ah) if distance > 0 else math.inf
    y = dot(direction, up)/(distance*vertical) if distance > 0 else math.inf
    blocked = [b["name"] for b in occluders if intersects_box(eye, target, b)]
    return x, y, blocked


failures = []
for shot in spec["validation"]["shots"]:
    print(shot["id"])
    for camera_name in shot["camera_nodes"]:
        camera = nodes[camera_name]
        world_up = rotate(camera["rotation"], (0, 1, 0))
        upright = world_up[1] > .9
        print(f"  {camera_name} -> horizon: " + ("upright" if upright else f"CHECK up_y={world_up[1]:+.3f}"))
        if not upright:
            failures.append(f"{camera_name}: camera horizon is inverted or rolled")
        for subject_name in shot["required_subject_nodes"]:
            x, y, blocked = projection(camera, nodes[subject_name]["translation"])
            visible = abs(x) <= safe and abs(y) <= safe and not blocked
            print(f"  {camera_name} -> {subject_name}: x={x:+.2f}, y={y:+.2f}, " + ("clear" if visible else f"CHECK {blocked}"))
            if not visible: failures.append(f"{camera_name}: {subject_name} must be visible")
        for hidden_name in shot.get("must_be_occluded", []):
            x, y, blocked = projection(camera, nodes[hidden_name]["translation"])
            hidden = bool(blocked) or abs(x) > safe or abs(y) > safe
            print(f"  {camera_name} -> {hidden_name}: " + (f"hidden {blocked}" if hidden else "CHECK visible"))
            if not hidden: failures.append(f"{camera_name}: {hidden_name} must be hidden")

# The kidnapping must stay hidden from every declared public-square checkpoint,
# not just from cameras currently aimed at the school.
global_checks = spec["validation"]["global_occlusion_checks"]
for observer_name in global_checks["observer_nodes"]:
    observer = nodes[observer_name]
    for hidden_name in global_checks["must_hide"]:
        blocked = [b["name"] for b in occluders if intersects_box(observer["translation"], nodes[hidden_name]["translation"], b)]
        print(f"GLOBAL {observer_name} -> {hidden_name}: " + (f"hidden {blocked}" if blocked else "CHECK visible"))
        if not blocked:
            failures.append(f"{observer_name}: {hidden_name} visible from public square")

# Dufau sees the corner that the van passed, but not the inside of the alley.
dufau = nodes["dufau_placeholder"]
angle = nodes[global_checks["dufau_visible_target"]]
angle_blocked = [b["name"] for b in occluders if intersects_box(dufau["translation"], angle["translation"], b)]
if angle_blocked:
    failures.append(f"dufau_placeholder: alley angle hidden by {angle_blocked}")

fallback = spec["validation"]["fallback_shot"]
fallback_camera = nodes[fallback["camera_node"]]
fallback_up = rotate(fallback_camera["rotation"], (0, 1, 0))
if fallback_up[1] <= .9:
    failures.append("CAM_DEPART_COURT: camera horizon is inverted or rolled")
for subject_name in fallback["required_subject_nodes"]:
    x, y, blocked = projection(fallback_camera, nodes[subject_name]["translation"])
    visible = abs(x) <= safe and abs(y) <= safe and not blocked
    print(f"FALLBACK {fallback['camera_node']} -> {subject_name}: x={x:+.2f}, y={y:+.2f}, " +
          ("clear" if visible else f"CHECK {blocked}"))
    if not visible:
        failures.append(f"{fallback['camera_node']}: {subject_name} must be visible")
if failures:
    raise SystemExit("Sightline checks failed: " + ", ".join(failures))
