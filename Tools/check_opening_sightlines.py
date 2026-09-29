"""Check the opening's static camera framing against its generated glTF.

Run after build_opening_blockout.py. This is a geometric check at 16:9, not
proof of visibility in Unreal with animation, lighting, or player movement.
"""

import json
import math
from pathlib import Path


SCENE = Path(__file__).resolve().parents[1] / "Game/Blockout/ouverture-centre-ville.gltf"
data = json.loads(SCENE.read_text(encoding="utf-8"))
nodes = {node["name"]: node for node in data["nodes"]}
subjects = ("child_placeholder", "van_placeholder", "ariane_placeholder")
occluders = tuple(node for node in data["nodes"] if node["name"].startswith(
    ("school_main", "west_shop", "corner_house", "east_shop", "tree_trunk")))


def dot(a, b):
    return sum(x * y for x, y in zip(a, b))


def cross(a, b):
    return (a[1] * b[2] - a[2] * b[1],
            a[2] * b[0] - a[0] * b[2],
            a[0] * b[1] - a[1] * b[0])


def rotate(q, vector):
    axis, scalar = q[:3], q[3]
    doubled = tuple(2 * n for n in cross(axis, vector))
    again = cross(axis, doubled)
    return tuple(vector[i] + scalar * doubled[i] + again[i] for i in range(3))


def intersects_box(origin, destination, box):
    """Whether the open segment from camera to subject crosses a solid box."""
    centre, size = box["translation"], box["scale"]
    low, high = 0.0, 1.0
    for i in range(3):
        minimum, maximum = centre[i] - size[i] / 2, centre[i] + size[i] / 2
        delta = destination[i] - origin[i]
        if abs(delta) < 1e-9:
            if not minimum <= origin[i] <= maximum:
                return False
            continue
        near, far = sorted(((minimum - origin[i]) / delta, (maximum - origin[i]) / delta))
        low, high = max(low, near), min(high, far)
        if low > high:
            return False
    return low < .999 and high > .001


failures = []
for camera in (nodes[name] for name in ("CAM_SHOULDER", "CAM_WIDE", "CAM_FIRST")):
    eye, q = camera["translation"], camera["rotation"]
    right = rotate(q, (1, 0, 0))
    up = rotate(q, (0, 1, 0))
    forward = rotate(q, (0, 0, -1))
    vertical = math.tan(data["cameras"][camera["camera"]]["perspective"]["yfov"] / 2)
    print(camera["name"])
    for name in subjects:
        target = nodes[name]["translation"]
        direction = tuple(target[i] - eye[i] for i in range(3))
        distance = dot(direction, forward)
        x = dot(direction, right) / (distance * vertical * 16 / 9) if distance > 0 else math.inf
        y = dot(direction, up) / (distance * vertical) if distance > 0 else math.inf
        blocked = [box["name"] for box in occluders if intersects_box(eye, target, box)]
        # Allow a 5% safety margin at the frame boundary for these event markers.
        visible = abs(x) <= .95 and abs(y) <= .95 and not blocked
        print(f"  {name}: x={x:+.2f}, y={y:+.2f}, " + ("clear" if visible else f"CHECK {blocked}"))
        if not visible:
            failures.append(f"{camera['name']}: {name}")

if failures:
    raise SystemExit("Sightline checks failed: " + ", ".join(failures))
