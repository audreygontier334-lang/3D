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
import validate_spatial as vs  # noqa: E402
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
            d["clues"].append({"id": "CLU_ORPHELIN", "name": "x", "location": "LOC_BANC_DUFAU", "kind": "objet",
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

    # --- corrections demandées par Codex (revue PR #4)

    def test_photo_nette_et_floue_ne_se_cumulent_pas(self):
        def cumul(d):
            for a in d["prologue"]["actions"]:
                if a["id"] == "ACT_PHOTO":
                    a["grants_instead_if_with"]["ACT_COURIR"] = ["CLU_PHOTO_FLOUE", "CLU_PHOTO_FOURGON"]
        self.mm.edit("mission.json", cumul)
        self.assertTrue(any("[fenêtre]" in e and "CLU_PHOTO_FOURGON" in e for e in self.mm.errors()))

    def test_accessibilite_ne_change_pas_le_nombre_d_actions(self):
        self.mm.edit("mission.json", lambda d: d["prologue"]["window_accessibility"].__setitem__("max_actions_unchanged", False))
        self.assertTrue(any("accessibilité" in e for e in self.mm.errors()))

    def test_etat_de_fin_sur_une_deduction_refuse(self):
        def bad(d):
            d["resolution"]["chapter2_states"][0]["condition"]["requires"] = ["DED_RIVE_EST"]
        self.mm.edit("mission.json", bad)
        self.assertTrue(any("[états]" in e and "DED_RIVE_EST" in e for e in self.mm.errors()))

    def test_bonus_sur_une_deduction_refuse(self):
        def bad(d):
            for b in d["resolution"]["bonuses"]:
                if b["id"] == "BONUS_K1_IDENTIFIE":
                    b["from"] = "DED_K1_LOUBERE"
        self.mm.edit("mission.json", bad)
        self.assertTrue(any("[états]" in e and "DED_K1_LOUBERE" in e for e in self.mm.errors()))

    def test_hypothese_bonus_sans_deduction_refusee(self):
        def bad(d):
            for h in d["hypotheses"]:
                if h["id"] == "H_DEST_RIVE_EST":
                    h["requires_any_of"] = [["CLU_PHOTO_VIE"]]
        self.mm.edit("hypotheses.json", bad)
        self.assertTrue(any("[états]" in e and "H_DEST_RIVE_EST" in e for e in self.mm.errors()))

    def test_condition_de_dialogue_en_prose_refusee(self):
        def bad(d):
            d["scenes"][0]["lines"][0]["condition"] = "si le joueur a été gentil"
        self.mm.edit("../../dialogues/01-ouverture.json", bad)
        self.assertTrue(any("condition en prose" in e for e in self.mm.errors()))

    def test_condition_de_dialogue_id_inconnu(self):
        def bad(d):
            d["scenes"][0]["lines"][0]["requires"] = {"all": ["CLU_INVENTE"]}
        self.mm.edit("../../dialogues/01-ouverture.json", bad)
        self.assertTrue(any("CLU_INVENTE" in e for e in self.mm.errors()))

    def test_condition_de_dialogue_cle_inconnue(self):
        def bad(d):
            d["scenes"][0]["lines"][0]["requires"] = {"quand": "demain"}
        self.mm.edit("../../dialogues/01-ouverture.json", bad)
        self.assertTrue(any("requires mal formé" in e for e in self.mm.errors()))


class TestDecisionsAudrey(unittest.TestCase):
    """Les décisions validées par Audrey le 29/09 ne doivent pas être contredites par les textes et les données."""

    SOURCES = ["docs/cases", "docs/dialogues", "docs/narrative", "GameData/missions", "GameData/dialogues"]
    INTERDITS = {
        r"\b(la|en|sa|une|de) laisse\b": "laisse (Ariane est libre)",
        r"\bcollier\b": "collier (Ariane n'en porte pas)",
        r"\bl[âa]ch(er|ée|é)\b": "lâcher la chienne (Ariane est déjà libre)",
        r"front de mer": "front de mer (hors champ en P0–P3)",
        r"\{CHIENNE\}": "jeton {CHIENNE} (la chienne s'appelle Ariane)",
        r"ENV_RUE_PORT|Z_RUE_PORT|Z_FRONT\b|LOC_FRONT_MER|LOC_CALE\b|ACT_LACHER": "ancien identifiant",
        r"chienne n'ont pas encore de nom": "le nom d'Ariane est décidé",
        r"ça mord|poissons ont fui|canne à pêche": "pêche de Dufau (il est dans le square, pas au bord de l'eau)",
        # Réponses d'Audrey du 29/09 à Q1–Q3
        r"douan|TMAU|retenue sur|conteneur bloqué": "Nadia douanière / conteneur bloqué (Q1 : c'est le père qui est visé)",
        r"CLU_NADIA_REACTION|INT_OBSERVER_NADIA|\bINT_NADIA\b|BR_NADIA_|FLAG_NADIA_(ALLIEE|FERMEE)|EVT_NADIA_CRAQUE|CASTERAN_ELOIGNEE|EVT_CASTERAN_S_ELOIGNE":
            "ancien identifiant (Nadia porteuse du message, Casteran collée à Nadia)",
        r"Casteran (est|serait) la tête|tête et architecte|a menti exprès": "Casteran est de bonne foi (Q2 : le compagnon de la mère dirige le réseau)",
        r"retrouvée saine et sauve au chapitre 2|retrouvée au chapitre 2": "Lila est retrouvée au chapitre 4 (Q3)",
    }
    # phrases qui énoncent justement la décision
    AUTORISES = ("ni laisse ni collier", "sans laisse ni collier", "ne suppose de laisse ni de collier",
                 "aucun port ni front de mer", "ni front de mer", "sans laisse", "ne la lâche pas",
                 "(ex-`ENV_RUE_PORT`)")

    def test_aucune_contradiction_avec_les_decisions(self):
        import re
        fautes = []
        for base in self.SOURCES:
            for f in sorted((ROOT / base).rglob("*")):
                if f.suffix not in (".md", ".json") or not f.is_file():
                    continue
                for n, ligne in enumerate(f.read_text(encoding="utf-8").splitlines(), 1):
                    propre = ligne
                    for ok in self.AUTORISES:
                        propre = propre.replace(ok, "")
                    for motif, raison in self.INTERDITS.items():
                        if re.search(motif, propre, re.IGNORECASE):
                            fautes.append(f"{f.relative_to(ROOT)}:{n} — {raison}")
        self.assertEqual(fautes, [], "\n".join(fautes))

    def test_chantage_vise_le_pere(self):
        clues = {c["id"]: c for c in json.loads((MISSION / "clues.json").read_text(encoding="utf-8"))["clues"]}
        self.assertIn("Julien", clues["CLU_MESSAGE_CHANTAGE"]["fact"])
        interactions = {i["id"]: i for i in json.loads((MISSION / "interactions.json").read_text(encoding="utf-8"))["interactions"]}
        self.assertIn("CLU_MESSAGE_CHANTAGE", interactions["INT_PERE"]["grants"])
        self.assertEqual(interactions["INT_PERE"]["requires"]["all"], ["FLAG_DARRIGADE_ELOIGNE"])

    def test_zones_de_la_maquette_codex(self):
        zones = {z["id"] for z in json.loads((MISSION / "locations.json").read_text(encoding="utf-8"))["zones"]}
        self.assertEqual(zones, {"Z_PROMENADE", "Z_ECOLE", "Z_CROISEMENT", "Z_RUE_FUITE", "Z_PLACE", "Z_TEMOINS"})

    def test_etats_de_fin_documentes_avec_les_hypotheses(self):
        texte = (ROOT / "docs/cases/01-ouverture.md").read_text(encoding="utf-8")
        ligne_a = next(l for l in texte.splitlines() if l.startswith("| **A — Avance**"))
        self.assertIn("H_DEST_RIVE_EST", ligne_a)


class TestExigencesSpatiales(unittest.TestCase):
    """spatial_requirements.json : lien entre la mission et la maquette 3D de la PR #3."""

    def setUp(self):
        self.mm = MutableMission()

    def tearDown(self):
        self.mm.cleanup()

    def errors(self):
        return vs.validate(self.mm.dir).errors

    def edit_el(self, eid, fn):
        def f(d):
            for el in d["elements"]:
                if el["id"] == eid:
                    fn(el)
        self.mm.edit("spatial_requirements.json", f)

    def test_fichier_valide(self):
        r = vs.validate(MISSION)
        self.assertEqual(r.errors, [], "\n".join(r.errors))

    def test_zone_inconnue_refusee(self):
        self.edit_el("INT_DUFAU", lambda el: el.__setitem__("zone_id", "Z_PORT"))
        self.assertTrue(any("zone inconnue Z_PORT" in e for e in self.errors()))

    def test_repere_inconnu_refuse(self):
        self.edit_el("CLU_PORTE_CLES_LILA", lambda el: el.__setitem__("marker_id", "EVT_LAISSE"))
        self.assertTrue(any("repère inconnu EVT_LAISSE" in e for e in self.errors()))

    def test_obligatoire_sans_repli_refuse(self):
        self.edit_el("INT_INES", lambda el: el.__setitem__("fallback", None))
        self.assertTrue(any("INT_INES" in e and "aucune solution de repli" in e for e in self.errors()))

    def test_obligatoire_dependant_d_une_seule_camera_refuse(self):
        self.edit_el("INT_DUFAU", lambda el: el.__setitem__("cameras", ["CAM_FIRST"]))
        self.assertTrue(any("INT_DUFAU" in e and "toutes les vues" in e for e in self.errors()))

    def test_rattrapage_qui_n_en_est_pas_un(self):
        self.edit_el("INT_INES", lambda el: el["fallback"].__setitem__("ids", ["EVT_RADIO_PEAGE"]))
        self.assertTrue(any("n'est pas un événement de rattrapage" in e for e in self.errors()))

    def test_zone_divergente_des_donnees(self):
        self.edit_el("INT_LARTIGUE", lambda el: el.__setitem__("zone_id", "Z_ECOLE"))
        self.assertTrue(any("INT_LARTIGUE" in e and "zone du lieu" in e for e in self.errors()))

    def test_interaction_non_couverte(self):
        self.mm.edit("spatial_requirements.json", lambda d: d.__setitem__(
            "elements", [e for e in d["elements"] if e["id"] != "INT_APPEL_17"]))
        self.assertTrue(any("INT_APPEL_17" in e and "[couverture]" in e for e in self.errors()))


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


if __name__ == "__main__":
    unittest.main()
