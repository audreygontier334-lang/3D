"""Habillage des façades du prologue, vérifié sans Unreal (module unreal simulé)."""
from __future__ import annotations

import importlib.util
import json
import subprocess
import sys
import types
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SCRIPT = ROOT / "Game/Unreal/FauxSemblants/Scripts/dress_prologue_facades.py"
GLTF = ROOT / "Game/Blockout/ouverture-centre-ville.gltf"
sys.modules.setdefault("unreal", types.ModuleType("unreal"))
spec = importlib.util.spec_from_file_location("dress_prologue_facades", SCRIPT)
dress = importlib.util.module_from_spec(spec)
spec.loader.exec_module(dress)
sys.path.insert(0, str(ROOT / "Tools"))


def inside(point, node):
    return all(abs(point[i] - node["translation"][i]) < node["scale"][i] / 2 for i in range(3))


def intersects_box(origin, destination, box):
    low, high = 0.0, 1.0
    for i in range(3):
        lo = box["translation"][i] - box["scale"][i] / 2
        hi = box["translation"][i] + box["scale"][i] / 2
        delta = destination[i] - origin[i]
        if abs(delta) < 1e-9:
            if not lo <= origin[i] <= hi:
                return False
            continue
        near, far = sorted(((lo - origin[i]) / delta, (hi - origin[i]) / delta))
        low, high = max(low, near), min(high, far)
        if low > high:
            return False
    return low < .999 and high > .001


class FacadeDressingTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.nodes = json.loads(GLTF.read_text(encoding="utf-8"))["nodes"]
        cls.by_name = {n["name"]: n for n in cls.nodes}
        cls.details = dress.plan(cls.nodes)

    def owner(self, label):
        return max((b for b in dress.BUILDINGS if label.startswith(b + "_")), key=len)

    def test_labels_are_unique(self):
        labels = [d[0] for d in self.details]
        self.assertEqual(len(labels), len(set(labels)))

    def test_no_detail_buried_in_another_volume(self):
        solids = [n for n in self.nodes if n.get("scale") and "mesh" in n and n["name"] != "ground"]
        for label, centre, _size, _mat in self.details:
            for node in solids:
                if node["name"] != self.owner(label):
                    self.assertFalse(inside(centre, node), f"{label} enfoui dans {node['name']}")

    def test_details_stay_within_3_cm_of_their_facade(self):
        for label, centre, size, _mat in self.details:
            b = self.by_name[self.owner(label)]
            out = max(abs(centre[i] - b["translation"][i]) + size[i] / 2 - b["scale"][i] / 2 for i in (0, 2))
            self.assertTrue(0 < out <= 0.03, f"{label} : saillie {out:.3f} m")

    def test_square_and_alley_facing_sides_of_masks_are_dressed(self):
        labels = {d[0] for d in self.details}
        for mask in ("alley_corner_mask", "alley_corner_south_mask"):
            for side in ("X-1", "X+1"):
                self.assertTrue(any(l.startswith(f"{mask}_{side}_") for l in labels), f"{mask} {side}")

    def test_details_never_hide_a_required_subject(self):
        spec = json.loads((ROOT / "Game/Blockout/ouverture-centre-ville.manifest.json").read_text(encoding="utf-8"))
        validation = spec["validation"]
        pairs = [(c, s) for shot in validation["shots"] for c in shot["camera_nodes"]
                 for s in shot["required_subject_nodes"]]
        fallback = validation["fallback_shot"]
        pairs += [(fallback["camera_node"], s) for s in fallback["required_subject_nodes"]]
        boxes = [{"translation": c, "scale": s} for _l, c, s, _m in self.details]
        for camera, subject in pairs:
            eye, target = self.by_name[camera]["translation"], self.by_name[subject]["translation"]
            self.assertFalse(any(intersects_box(eye, target, b) for b in boxes), f"{camera} -> {subject}")

    def test_detail_budget(self):
        # Cubes Engine sans collision ni ombre, deux matériaux : budget du blockout.
        self.assertLessEqual(len(self.details), 1500)


class PrivateReferencesTest(unittest.TestCase):
    def test_no_private_reference_is_tracked(self):
        tracked = subprocess.run(["git", "ls-files"], cwd=ROOT, capture_output=True, text=True)
        if tracked.returncode:
            self.skipTest("dépôt Git indisponible")
        leaks = [p for p in tracked.stdout.splitlines()
                 if "PrivateVisualReferences" in p or Path(p).name.startswith("T_REF_")]
        self.assertEqual(leaks, [])


if __name__ == "__main__":
    unittest.main()
