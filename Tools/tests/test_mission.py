"""Tests de cohérence narrative — lancer avec : python3 -m unittest discover -s Tools/tests -v"""
from __future__ import annotations

import copy
import json
import shutil
import sys
import tempfile
import unittest
from pathlib import Path

TOOLS = Path(__file__).resolve().parent.parent
ROOT = TOOLS.parent
sys.path.insert(0, str(TOOLS))

import render_dialogues  # noqa: E402
import validate_mission as vm  # noqa: E402

MISSION = ROOT / "GameData" / "missions" / "01"


class MutableMission:
    """Copie temporaire de la mission, pour vérifier que le validateur détecte les erreurs."""

    def __init__(self):
        self.tmp = Path(tempfile.mkdtemp())
        shutil.copytree(ROOT / "GameData", self.tmp / "GameData")
        self.dir = self.tmp / "GameData" / "missions" / "01"

    def edit(self, name: str, fn) -> None:
        p = self.dir / name if not name.startswith("../") else (self.dir / name).resolve()
        data = json.loads(p.read_text(encoding="utf-8"))
        fn(data)
        p.write_text(json.dumps(data, ensure_ascii=False), encoding="utf-8")

    def errors(self) -> list[str]:
        return vm.validate(self.dir).errors

    def cleanup(self):
        shutil.rmtree(self.tmp)


class TestMissionValide(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.report = vm.validate(MISSION)
        cls.m = vm.Mission(MISSION)

    def test_aucune_erreur(self):
        self.assertEqual(self.report.errors, [], "\n".join(self.report.errors))

    def test_chaque_enigme_a_une_solution(self):
        for p in self.m.puzzles.values():
            with self.subTest(p["id"]):
                self.assertTrue(p["answer"])
                self.assertEqual(len(p["hints"]), 3)

    def test_au_moins_huit_enigmes_dont_deux_avec_la_chienne(self):
        self.assertGreaterEqual(len(self.m.puzzles), 8)
        self.assertGreaterEqual(sum(1 for p in self.m.puzzles.values() if p.get("duo")), 2)

    def test_preuves_obligatoires_accessibles_dans_tous_les_scenarios(self):
        required = [h for h in self.m.hypotheses.values() if h["correct"] and not h.get("optional")]
        for combo, grants in vm.prologue_scenarios(self.m):
            have = vm.reachable(self.m, grants, adversarial=True)
            for h in required:
                with self.subTest(scenario=combo, hypothese=h["id"]):
                    self.assertTrue(vm.hypothesis_supported(h, have))

    def test_chaque_hypothese_fausse_est_refutable(self):
        best = set().union(*(g for _, g in vm.prologue_scenarios(self.m)))
        have = vm.reachable(self.m, best, adversarial=False)
        for h in self.m.hypotheses.values():
            if not h["correct"]:
                with self.subTest(h["id"]):
                    self.assertTrue(set(h["contradicted_by"]) & have)

    def test_toutes_les_branches_rejoignent_l_issue(self):
        res = self.m.meta["resolution"]["id"]
        for b in self.m.branches.values():
            with self.subTest(b["id"]):
                self.assertEqual(b["rejoins"], res)

    def test_ids_de_dialogue_references_existent(self):
        refs = [it["dialogue"] for it in self.m.interactions.values()]
        refs += [e["dialogue"] for e in self.m.events.values() if e.get("dialogue")]
        refs += [b["dialogue"] for b in self.m.branches.values() if b.get("dialogue")]
        for r in refs:
            with self.subTest(r):
                self.assertIn(r, self.m.scenes)
        for h in self.m.hypotheses.values():
            self.assertIn(h["label"], self.m.ui)

    def test_parcours_de_reference_dans_les_temps(self):
        end, _, problems = vm.simulate_path(self.m, self.m.meta["reference_path"]["steps"])
        self.assertEqual(problems, [])
        self.assertLessEqual(end, vm.hm("18:00"))

    def test_soleil_ouest_en_fin_d_apres_midi(self):
        az, alt = vm.solar_azimuth(44.3, -1.2, 272, "17:35", 2)
        self.assertTrue(225 < az < 270)
        self.assertGreater(alt, 10)
        az_noon, _ = vm.solar_azimuth(44.3, -1.2, 272, "13:55", 2)
        self.assertAlmostEqual(az_noon, 180, delta=5)

    def test_dialogues_markdown_a_jour(self):
        self.assertEqual(render_dialogues.main(["x", "--check"]), 0,
                         "Relancer : python3 Tools/render_dialogues.py")


class TestLeValidateurDetecteLesErreurs(unittest.TestCase):
    def setUp(self):
        self.mm = MutableMission()

    def tearDown(self):
        self.mm.cleanup()

    def test_rattrapage_supprime_rend_une_preuve_inaccessible(self):
        # Sans le rattrapage de 19 h, un joueur qui braque Inès n'a plus la vidéo.
        self.mm.edit("events.json", lambda d: d["events"].__setitem__(
            slice(None), [e for e in d["events"] if e["id"] != "EVT_INES_PARENTS"]))
        self.assertTrue(any("inatteignable" in e for e in self.mm.errors()))

    def test_indice_sans_source(self):
        def add(d):
            d["clues"].append({"id": "CLU_ORPHELIN", "name": "x", "location": "LOC_CALE", "kind": "objet",
                               "availability": "toujours", "fact": "x", "interpretations": ["x"]})
        self.mm.edit("clues.json", add)
        self.assertTrue(any("CLU_ORPHELIN" in e and "aucune source" in e for e in self.mm.errors()))

    def test_dialogue_reference_inexistant(self):
        self.mm.edit("interactions.json", lambda d: d["interactions"][0].__setitem__("dialogue", "DLG_FANTOME"))
        self.assertTrue(any("DLG_FANTOME" in e for e in self.mm.errors()))

    def test_branche_qui_ne_rejoint_pas_l_issue(self):
        self.mm.edit("branches.json", lambda d: d["branches"][0].__setitem__("rejoins", "RES_AILLEURS"))
        self.assertTrue(any("ne rejoint pas" in e for e in self.mm.errors()))

    def test_fausse_piste_sans_contradiction(self):
        def strip(d):
            for h in d["hypotheses"]:
                if h["id"] == "H_DEST_NORD":
                    h["contradicted_by"] = []
        self.mm.edit("hypotheses.json", strip)
        self.assertTrue(any("H_DEST_NORD" in e for e in self.mm.errors()))

    def test_penalites_trop_lourdes(self):
        def heavy(d):
            for b in d["branches"]:
                if b["id"] == "BR_ERR_A63":
                    b["time_cost"] = 120
        self.mm.edit("branches.json", heavy)
        self.assertTrue(any("[temps]" in e for e in self.mm.errors()))

    def test_bonne_reponse_absente_des_options(self):
        def bad(d):
            for p in d["puzzles"]:
                if p["id"] == "PZ_04":
                    p["answer"]["correct"] = "port"
        self.mm.edit("puzzles.json", bad)
        self.assertTrue(any("PZ_04" in e for e in self.mm.errors()))

    def test_choix_de_dialogue_vers_une_ligne_absente(self):
        def bad(d):
            for sc in d["scenes"]:
                for ln in sc["lines"]:
                    for ch in ln.get("choices", []):
                        ch["goto"] = "DLG_NULLE_PART"
                        return
        self.mm.edit("../../dialogues/01-ouverture.json", bad)
        self.assertTrue(any("DLG_NULLE_PART" in e for e in self.mm.errors()))


if __name__ == "__main__":
    unittest.main()


class TestSchemas(unittest.TestCase):
    """Conformité aux schémas JSON (ignoré si le paquet jsonschema n'est pas installé)."""

    def test_donnees_conformes(self):
        try:
            import jsonschema
        except ImportError:
            self.skipTest("jsonschema non installé")
        sch = lambda n: json.loads((ROOT / "GameData" / "schema" / f"{n}.schema.json").read_text(encoding="utf-8"))
        load = lambda n: json.loads((MISSION / n).read_text(encoding="utf-8"))
        pairs = [("clue", load("clues.json")["clues"]), ("location", load("locations.json")["locations"]),
                 ("interaction", load("interactions.json")["interactions"]), ("event", load("events.json")["events"]),
                 ("deduction", load("deductions.json")["deductions"]), ("hypothesis", load("hypotheses.json")["hypotheses"]),
                 ("branch", load("branches.json")["branches"]), ("puzzle", load("puzzles.json")["puzzles"]),
                 ("dialogue", [json.loads((ROOT / "GameData/dialogues/01-ouverture.json").read_text(encoding="utf-8"))])]
        for name, items in pairs:
            for item in items:
                with self.subTest(schema=name, id=item.get("id", name)):
                    jsonschema.validate(item, sch(name))
