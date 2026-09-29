# Découpage scène par scène — « Faux-semblants »

> **Source pour la production des visuels.** Fichier **généré** depuis `GameData/scenes/decoupage.json` par `Tools/render_decoupage.py` : modifier le JSON puis relancer le script.
>
> **Légende** : ✅ = décision d'Audrey · 🟡 = proposition de Claude (modifiable). Rien dans ce document ne fixe la tenue, les cheveux ni le trajet précis de Lila, ni l'apparence du fourgon, ni l'animation de la place : ce sont des choix visuels ouverts (tableau ci-dessous).
>
> ⚠️ Contient des spoilers (chapitres 1 à 6).

## Choix visuels ouverts (à trancher par Audrey)

| ID | Question | Décidé par | Contrainte narrative à respecter |
|---|---|---|---|
| V1 | Niveau d'animation de la place de l'école (foule, voitures, parents). | Audrey, pendant la construction 3D | Lila repérable 3 s au portail ; aucun indice n'en dépend. |
| V2 | Apparence du fourgon : couleur, modèle, lettrage éventuel, feu fendu. | Audrey, pendant la construction 3D | Plaque arrière lisible et portière latérale coulissante. Si ombre de lettrage : l'énigme facultative PZ_06 existe ; sinon elle est retirée. Dans les dialogues, la couleur est un jeton {FOURGON_COULEUR}. |
| V3 | Ce que l'on voit au débouché de la ruelle : simple lumière, bout de ciel, bande de mer ? | Audrey | Le boulevard et le rond-point ne doivent pas être visibles (énigme du perron). |
| V4 | Tenue et cheveux de Lila. | Audrey, sur proposition de Codex | Identiques de la sortie d'école à la cabane, y compris sur la photo ; cartable avec porte-clés renard, bracelet en perles. |
| V5 | Apparence de Darrigade (et son nom définitif). | Audrey | Rassurant, rien de menaçant à l'image. |
| V6 | Obscurité des scènes de nuit (chapitre 2). | Audrey | Traces et Ariane lisibles à la lampe dans les trois vues. |
| V7 | Intensité de la pluie en forêt (chapitre 4). | Audrey | Ariane et les traces lisibles. |
| V8 | Ce que l'on montre de Lila dans la cabane (durée, cadrage). | Audrey | Aucune violence, pas de complaisance ; décision d'Audrey : cabane sombre, froide, sans confort. |

## Sommaire

- **Prologue — Sortie d'école** : `SC_P0_A` Promenade dans le centre-ville, `SC_P0_B` Sortie d'école, `SC_P1` Vers la ruelle : l'abordage, `SC_P2` La fenêtre d'action, `SC_P3` Hors de vue
- **Chapitre 1 — Le fourgon** : `SC_C1_01` L'appel et la ruelle vide, `SC_C1_02` Les gendarmes, `SC_C1_03` La piste de Lila, `SC_C1_04` Le square : Dufau et la boulangerie, `SC_C1_05` L'école, l'abribus et la piste de la femme, `SC_C1_06` Le perron de la notaire, `SC_C1_07` Le skatepark, `SC_C1_08` La famille au poste, `SC_C1_09` Les résultats du poste, `SC_C1_10` Le tableau et le départ
- **Chapitre 2 — L'airial** : `SC_C2_01` La route des Étangs, `SC_C2_02` Les airiaux de la rive est, `SC_C2_03` L'airial vide, `SC_C2_04` Qui les a prévenus ?
- **Chapitre 3 — Le dernier voyage** : `SC_C3_01` Julien parle, `SC_C3_02` L'entrepôt, `SC_C3_03` Le dernier voyage
- **Chapitre 4 — La cabane** : `SC_C4_01` La carte des parcelles, `SC_C4_02` La battue, `SC_C4_03` La cabane, `SC_C4_04` Les retrouvailles
- **Chapitre 5 — Le pavillon (plan)** : `SC_C5_01` Anvers : le pavillon
- **Chapitre 6 — Faux-semblants (plan)** : `SC_C6_01` Faux-semblants

---

## Prologue — Sortie d'école

### `SC_P0_A` — Promenade dans le centre-ville

**Décisions d'Audrey qui s'appliquent** ✅

- ✅ Centre-ville inspiré de celui d'Arcachon, en fin d'après-midi.
- ✅ Port hors de la scène d'ouverture.
- ✅ Ariane libre dès le départ, sans laisse ni collier, foulard noir à motifs blancs ; bandeau assorti pour l'héroïne.
- ✅ Niveau d'animation de la place : à choisir par Audrey pendant la construction 3D.

- **Lieu** 🟡 : Rue piétonne du centre-ville, puis arrivée sur la place de l'école (plan A). Centre-ville inspiré d'Arcachon. — décors `ENV_PROMENADE`, `ENV_ECOLE` ; zones `Z_PROMENADE`, `Z_TEMOINS`
- **Moment et lumière** 🟡 : mardi, fin septembre, 16:33 → 16:34 ; fin d'après-midi, lumière chaude et basse, ombres longues ; météo : beau temps, léger vent dans les pins
- **Personnages** : ✅ `CHAR_HEROINE` {HEROINE}, joueuse · ✅ `CHAR_CHIENNE` Ariane, libre, sans laisse ni collier, foulard noir à motifs blancs · 🟡 `CHAR_DUFAU` sur son banc du square, mots croisés · 🟡 `FIGURANTS_PLACE` passants, commerçants ; densité au choix d'Audrey

**Action**

1. 🟡 La joueuse prend la main : marcher, trotter, changer de vue. Ariane marche librement à 1–3 m, flaire les pieds des bancs, revient au rappel.
2. 🟡 Tutoriel doux, facultatif : ordres « Au pied », « Reste », « Cherche » (balle lancée dans le square), « Montre », « Va ! ».
3. 🟡 Facultatif : saluer Dufau ; il grommelle à propos d'un fourgon « qui fait un boucan de casserole » garé dans la ruelle depuis une demi-heure (première graine).

**Déplacements**

| | Qui | De | Vers | Distance | Durée |
|---|---|---|---|---|---|
| 🟡 | Duo | rue piétonne (Z_PROMENADE) | place de l'école | 30–40 m | libre (≈ 1 min) |

**Indices observables**

| | Indice | Ce qu'on voit | Point de vue | Distance | Caméras | Indispensable |
|---|---|---|---|---|---|---|
| 🟡 | `CLU_TEMOIN_DUFAU` | Graine seulement : Dufau parle du fourgon garé dans la ruelle (le témoignage complet vient au chapitre 1). | à côté du banc | < 3 m | SHOULDER, WIDE, FIRST | non |

**Caméras**

- `CAM_SHOULDER` : Vue de départ ; Ariane visible à côté de l'héroïne.
- `CAM_WIDE` : Lecture de la place et des commerces.
- `CAM_FIRST` : Facultative ; Ariane gardée dans le champ par léger recentrage.

**Si un indice est manqué**

- La joueuse ne parle pas à Dufau → Aucune conséquence : il redit tout au chapitre 1 (`INT_DUFAU`).

**Éléments visuels à produire**

- ✅ `ENV_PROMENADE` (lieu) : Rue piétonne commerçante et square avec banc ; architecture inspirée du centre d'Arcachon.
- 🟡 `CHAR_DUFAU` (personnage) : Retraité, bob, journal de mots croisés, voix râpeuse.
- 🟡 `ANIM_CHIENNE_LIBRE` (animation) : Marche libre, flair, retour au rappel, rapport de balle.

**Continuité**

- Héroïne : tenue validée (voir docs/DECISIONS.md, PR #3).
- Ariane : foulard noir à motifs blancs, jamais de laisse ni de collier.

**Points ouverts**

- V1 : niveau d'animation de la place (choix d'Audrey).

**Raccord** 🟡 → `SC_P0_B` : Sonnerie de l'école à 16 h 34 : la caméra ne coupe pas, la joueuse est déjà sur la place.

Références données : `EVT_START`, `DLG_P_TUTO`, `DLG_P_DUFAU`

### `SC_P0_B` — Sortie d'école

**Décisions d'Audrey qui s'appliquent** ✅

- ✅ Une fillette rentre seule de l'école.
- ✅ La place de l'école est un lieu et un plan distincts de la ruelle.
- ✅ Tenue, cheveux et trajet précis de Lila : non validés.

- **Lieu** 🟡 : Place de l'école, devant le portail (plan A). — décors `ENV_ECOLE` ; zones `Z_ECOLE`, `Z_PLACE`
- **Moment et lumière** 🟡 : mardi, 16:34 → 16:35:40 ; même lumière chaude ; façades éclairées de biais ; météo : beau temps
- **Personnages** : ✅ `CHAR_HEROINE` {HEROINE}, joueuse · ✅ `CHAR_CHIENNE` Ariane, libre, sans laisse ni collier, foulard noir à motifs blancs · ✅ `CHAR_FILLETTE` Lila, 9 ans, sort seule · 🟡 `FIGURANTS_ECOLE` enfants et parents ; densité au choix d'Audrey

**Action**

1. 🟡 16 h 34 : sonnerie, les enfants sortent.
2. 🟡 16 h 35 : Lila sort seule, repère Ariane de loin, lui fait coucou main levée (le bracelet se voit) : « Coucou Ariane ! Demain je t'apporte un biscuit ! ». Ariane remue la queue.
3. 🟡 Si la joueuse répond d'un geste, Lila sourit.
4. 🟡 Lila part seule sur le trottoir de la rue de l'École, vers l'angle de la ruelle des Tamaris (son chemin habituel vers la maison, côté océan). Elle tourne à l'angle et sort du champ de la place vers 16 h 35 min 40.

**Déplacements**

| | Qui | De | Vers | Distance | Durée |
|---|---|---|---|---|---|
| 🟡 | Lila | portail | entrée de la ruelle | 40–60 m | ≈ 40 s |
| 🟡 | Duo | place | libre ; Ariane commence à trotter vers l'angle de la ruelle (promenade habituelle vers l'océan) | — | — |

**Indices observables**

| | Indice | Ce qu'on voit | Point de vue | Distance | Caméras | Indispensable |
|---|---|---|---|---|---|---|
| 🟡 | `EVT_LILA_COUCOU` | Lila, son cartable (porte-clés renard) et son bracelet en perles. | depuis la place | ≈ 20 m | SHOULDER, WIDE, FIRST | non |

**Caméras**

- `CAM_SHOULDER` : Lila repérable au moins 3 s au portail.
- `CAM_WIDE` : Lila repérable dans la foule ; son trajet jusqu'à l'angle lisible.
- `CAM_FIRST` : Lila repérable ; la caméra ne force pas le regard.

**Si un indice est manqué**

- La joueuse ne voit pas le coucou → Purement narratif : le lien Lila–Ariane est redit par Julien et Nadia au chapitre 1.

**Éléments visuels à produire**

- 🟡 `ENV_ECOLE` (lieu) : Portail de l'école des Pins sur la place, affiche de la commune au nouveau logo, abribus avec banc et plan touristique.
- 🟡 `CHAR_FILLETTE` (personnage) : Lila, 9 ans. Tenue et cheveux à proposer puis valider par Audrey ; cartable avec porte-clés renard ; bracelet en perles.
- 🟡 `ANIM_LILA_COUCOU` (animation) : Coucou main levée, sourire, reprise de la marche.

**Continuité**

- Lila : tenue et cheveux non validés, identiques dans toutes ses apparitions ; cartable avec porte-clés renard et bracelet en perles visibles (nécessaires aux énigmes).

**Points ouverts**

- V1 : animation de la place.
- V4 : tenue et cheveux de Lila.
- Trajet précis de Lila sur la place : non validé, ajustable.

**Raccord** 🟡 → `SC_P1` : Lila disparaît à l'angle ; la joueuse continue à pied (pas de coupe). Ariane trotte devant vers la ruelle, guidage doux.

Références données : `EVT_SONNERIE`, `EVT_LILA_COUCOU`, `DLG_P_LILA`

### `SC_P1` — Vers la ruelle : l'abordage

**Décisions d'Audrey qui s'appliquent** ✅

- ✅ La place de l'école et la ruelle adjacente, orientée vers le front de mer, sont deux lieux et deux plans distincts.
- ✅ L'enlèvement a lieu dans la ruelle, plus isolée.
- ✅ Ambiance de la ruelle validée : maisons variées, d'époques différentes, avec des couleurs, clôtures et haies différentes.
- ✅ Déplacement précis de Lila, tenue et cheveux : non validés (faux raccord capillaire à éviter).
- ✅ Apparence du fourgon : à choisir par Audrey ; aucun indice indispensable n'en dépend.

- **Lieu** 🟡 : Transition du plan A au plan B : angle de la rue de l'École, puis entrée de la ruelle des Tamaris, plus isolée, orientée vers l'océan. Maisons variées d'époques différentes, couleurs, clôtures et haies différentes. — décors `ENV_ECOLE`, `ENV_RUE_FUITE` ; zones `Z_ECOLE`, `Z_CROISEMENT`, `Z_RUE_FUITE`
- **Moment et lumière** 🟡 : mardi, 16:35:40 → 16:37:50 ; lumière basse qui entre dans la ruelle depuis l'ouest ; façades à contre-jour vers le débouché ; météo : beau temps
- **Personnages** : ✅ `CHAR_HEROINE` {HEROINE}, joueuse · ✅ `CHAR_CHIENNE` Ariane, libre, sans laisse ni collier, foulard noir à motifs blancs · ✅ `CHAR_FILLETTE` Lila · 🟡 `CHAR_K2` « Sandrine », femme au badge et cordon bleu

**Action**

1. 🟡 La femme attendait à ≈ 10 m dans la ruelle, hors de vue du portail. Elle aborde Lila : « Lila ? Ta maman a eu un souci au travail, elle m'a demandé de te ramener. » Lila hésite : « Mais… elle m'a dit de rentrer toute seule. » La femme montre son badge.
2. 🟡 Elle prend Lila par la main ; elles marchent 15–25 m sur le trottoir vers un fourgon garé sur le bas-côté, dans le sens de la descente, arrière vers l'entrée.
3. 🟡 La joueuse arrive à l'entrée de la ruelle et découvre la scène à 20–30 m. Rien d'explicitement violent. Ariane fixe la ruelle, oreille droite dressée.
4. 🟡 Au moment de l'abordage, le porte-clés renard se détache du cartable et tombe près du caniveau (non montré en gros plan).

**Déplacements**

| | Qui | De | Vers | Distance | Durée |
|---|---|---|---|---|---|
| 🟡 | Lila et la femme | point d'abordage (≈ 10 m dans la ruelle) | portière latérale du fourgon | 15–25 m | ≈ 25 s |
| 🟡 | Duo | angle de la rue de l'École | entrée de la ruelle | variable | au rythme de la joueuse |

**Indices observables**

| | Indice | Ce qu'on voit | Point de vue | Distance | Caméras | Indispensable |
|---|---|---|---|---|---|---|
| 🟡 | `OBS_ABORDAGE` | La femme se penche, parle, montre un badge, prend la main de Lila. | entrée de la ruelle | 20–30 m | SHOULDER, WIDE, FIRST | non |
| 🟡 | `CLU_PORTE_CLES_LILA` | Le porte-clés tombe (visible seulement de près ; il sera trouvé au chapitre 1). | dans la ruelle | ≈ 10 m | SHOULDER, WIDE, FIRST | non |

**Caméras**

- `CAM_SHOULDER` : Entrée de la ruelle : Lila, la femme et le fourgon dans le cadre.
- `CAM_WIDE` : Caméra rapprochée par les clôtures ; rien ne doit masquer le trottoir.
- `CAM_FIRST` : Hauteur d'yeux ; la scène est lisible sans zoom.

**Si un indice est manqué**

- La joueuse est ailleurs → Au plus tard à 16 h 38 min 30, Ariane court vers la ruelle en aboyant ; la joueuse entend Lila appeler et arrive pour voir le départ.

**Éléments visuels à produire**

- ✅ `ENV_RUE_FUITE` (lieu) : Ruelle des Tamaris (plan B) : ambiance validée par Audrey (maisons variées, époques, couleurs, clôtures et haies différentes). Proposition : bas-côté sablonneux, haie de pittosporum côté trottoir d'arrivée.
- 🟡 `CHAR_K2` (personnage) : Femme d'environ 40 ans, tenue ordinaire, cordon bleu avec badge ; détails à proposer.
- 🟡 `VEH_FOURGON` (véhicule) : Apparence au choix d'Audrey (couleur, modèle, lettrage) — décision ✅. Exigences proposées : garé dans le sens de la descente, plaque arrière vers l'entrée, portière latérale côté trottoir.
- 🟡 `ANIM_K2_ABORDAGE` (animation) : Se pencher vers l'enfant, montrer le badge, tendre la main, marcher en pressant le pas.

**Continuité**

- Lila : tenue et cheveux non validés, identiques dans toutes ses apparitions ; cartable avec porte-clés renard et bracelet en perles visibles (nécessaires aux énigmes).

**Points ouverts**

- V2 : apparence du fourgon.
- V3 : ce qu'on voit au débouché de la ruelle.
- Trajet précis de Lila : non validé, contraintes seulement (abordage hors de vue du portail, 15–25 m jusqu'à la portière).

**Raccord** 🟡 → `SC_P2` : L'alerte se déclenche dès que la joueuse ou Ariane arrive à moins de 5 m de l'entrée de la ruelle, et au plus tard à 16 h 38 min 30 (Ariane s'élance alors en aboyant et la joueuse la suit).

Références données : `EVT_ABORDAGE`, `EVT_MARCHE_FOURGON`, `DLG_P_ABORDAGE`

### `SC_P2` — La fenêtre d'action

**Décisions d'Audrey qui s'appliquent** ✅

- ✅ L'enlèvement a lieu dans la ruelle.
- ✅ Ariane libre, sans laisse ni collier.
- ✅ Apparence du fourgon ouverte : aucun indice indispensable n'en dépend.

- **Lieu** 🟡 : Ruelle des Tamaris (plan B), de l'entrée jusqu'au fourgon. — décors `ENV_RUE_FUITE` ; zones `Z_CROISEMENT`, `Z_RUE_FUITE`
- **Moment et lumière** 🟡 : mardi, 16:37:50 → 16:38:17 ; lumière basse ; contre-jour vers le débouché ; météo : beau temps
- **Personnages** : ✅ `CHAR_HEROINE` {HEROINE}, joueuse · ✅ `CHAR_CHIENNE` Ariane, libre, sans laisse ni collier, foulard noir à motifs blancs · ✅ `CHAR_FILLETTE` Lila · 🟡 `CHAR_K2` « Sandrine » · 🟡 `CHAR_K1` le conducteur, dans la cabine

**Action**

1. 🟡 Devant la portière, Lila se retourne, voit Ariane : « Je veux attendre maman… Ariane ! ». Ariane grogne, corps tendu.
2. 🟡 Invite discrète, sans ralenti imposé : photographier, courir, crier « Lila ! », envoyer Ariane (« Ariane, va ! »). Deux actions au plus en 8–12 s (20–25 s avec l'option d'accessibilité).
3. ✅ La femme fait monter Lila (pas de violence montrée), claque la portière ; le fourgon déboîte et descend la ruelle.
4. 🟡 Envoyée, Ariane fonce en aboyant et s'arrête net au bord de la chaussée quand le fourgon démarre ; elle flaire l'endroit où se tenait la femme, puis regarde la haie.

**Déplacements**

| | Qui | De | Vers | Distance | Durée |
|---|---|---|---|---|---|
| 🟡 | Fourgon | bas-côté | débouché de la ruelle | 70–100 m | ≈ 12 s |
| 🟡 | Joueuse (si elle court) | entrée | mi-ruelle | ≈ 15–20 m | pendant la fenêtre |

**Indices observables**

| | Indice | Ce qu'on voit | Point de vue | Distance | Caméras | Indispensable |
|---|---|---|---|---|---|---|
| 🟡 | `CLU_PHOTO_FOURGON` | Plaque arrière « GF-4·7-·· » (terre sur un chiffre). | entrée de la ruelle, joueuse immobile | 20–30 m | SHOULDER, WIDE, FIRST | non |
| 🟡 | `CLU_PHOTO_FLOUE` | Photo prise en courant : plaque illisible. | dans la ruelle | 15–25 m | SHOULDER, WIDE, FIRST | non |
| 🟡 | `CLU_OBS_CONDUCTEUR` | Profil du conducteur 1–2 s par la vitre quand le fourgon déboîte. | mi-ruelle, en courant | ≈ 8–10 m | SHOULDER, WIDE, FIRST | non |
| 🟡 | `CLU_OBS_ECHAPPEMENT` | Pot qui cogne et tremble (doublé visuellement pour les joueuses sans son). | mi-ruelle | ≈ 10 m | SHOULDER, WIDE, FIRST | non |
| 🟡 | `CLU_OBS_FEU_FENDU` | Feu arrière droit fendu (seulement si le fourgon retenu en a un : visuel ouvert). | mi-ruelle, en courant | ≈ 10 m | SHOULDER, WIDE, FIRST | non |
| 🟡 | `CLU_OBS_PASSAGERE` | La femme se retourne vers l'entrée : silhouette, tenue, lunettes. | entrée | 20–30 m | SHOULDER, WIDE, FIRST | non |
| 🟡 | `CLU_OBS_BADGE_CHUTE` | Le cordon s'accroche à la portière ; quelque chose tombe dans la haie. | entrée ou mi-ruelle | 15–30 m | SHOULDER, WIDE, FIRST | non |
| 🟡 | `CLU_CHIENNE_IMPREGNEE` | Ariane flaire longuement le trottoir puis regarde la haie. | où que soit la joueuse | — | SHOULDER, WIDE, FIRST | non |

**Caméras**

- `CAM_SHOULDER` : Arrière du fourgon et Lila dans le cadre ; possibilité de changer d'épaule.
- `CAM_WIDE` : Ne jamais passer derrière une clôture ; la plaque reste lisible.
- `CAM_FIRST` : Hauteur d'yeux, téléphone levé pour la photo.
- `CAM_INSPECT` : Photo consultable ensuite en plein écran.

**Si un indice est manqué**

- Aucune action pendant la fenêtre → Tous les axes restent résolubles avec les indices permanents (vidéo d'Inès, témoins, badge, plaque).
- Photo manquée → La vidéo d'Inès donne la fin de la plaque (consultation plus longue, 15 min).
- Badge non vu en train de tomber → Piste d'Ariane avec la bouteille (PZ_02), ou fouille des gendarmes à 18 h 30.

**Éléments visuels à produire**

- 🟡 `VEH_FOURGON` (véhicule) : Apparence au choix d'Audrey — décision ✅. Portière coulissante, plaque arrière, pot qui tremble ; aspect au choix d'Audrey.
- 🟡 `ANIM_CHIENNE_ENVOI` (animation) : Sprint, aboiement, arrêt net au bord de la chaussée, flair au sol, regard vers la haie.
- 🟡 `ANIM_K2_DEPART` (animation) : Se retourner, faire monter l'enfant sans violence visible, claquer la portière, cordon qui s'accroche.
- 🟡 `SFX_POT_ECHAPPEMENT` (son) : Cognement métallique, doublé d'une vibration visible du pot.

**Continuité**

- Lila : tenue et cheveux non validés, identiques dans toutes ses apparitions ; cartable avec porte-clés renard et bracelet en perles visibles (nécessaires aux énigmes).
- Aucune scène ne montre de violence envers Lila (décision d'Audrey).

**Points ouverts**

- V2 : apparence du fourgon (la plaque doit rester lisible quel que soit le choix).

**Raccord** 🟡 → `SC_P3` : Le fourgon tourne au bout de la ruelle ; plan continu, pas de cinématique.

Références données : `EVT_ALERTE`, `EVT_DEPART`, `ACT_PHOTO`, `ACT_COURIR`, `ACT_CRIER`, `ACT_ENVOYER`, `DLG_P_ALERTE`

### `SC_P3` — Hors de vue

**Décisions d'Audrey qui s'appliquent** ✅

- ✅ Port hors de la scène d'ouverture.

- **Lieu** 🟡 : Ruelle des Tamaris (plan B), mi-ruelle. — décors `ENV_RUE_FUITE` ; zones `Z_RUE_FUITE`
- **Moment et lumière** 🟡 : mardi, 16:38:17 → 16:39 ; lumière basse ; météo : beau temps
- **Personnages** : ✅ `CHAR_HEROINE` {HEROINE}, joueuse · ✅ `CHAR_CHIENNE` Ariane, libre, sans laisse ni collier, foulard noir à motifs blancs

**Action**

1. 🟡 Le fourgon tourne au bout de la ruelle : personne ne voit de quel côté.
2. 🟡 L'héroïne reste immobile, souffle court ; une réplique selon ce qu'elle a fait (« Je l'ai, la plaque… en partie », « Casquette grise… », « Tu l'as sentie, hein ? »).
3. 🟡 L'interface met en avant « Appeler le 17 ».

**Caméras**

- `CAM_SHOULDER` : Héroïne et Ariane au premier plan, débouché vide au fond.
- `CAM_WIDE` : Idem.
- `CAM_FIRST` : Le débouché de la ruelle.

**Si un indice est manqué**

- — → Aucun indice dans cette scène.

**Éléments visuels à produire**

- 🟡 `ENV_RUE_FUITE_DEBOUCHE` (lieu) : Débouché de la ruelle sur le boulevard (hors champ) ; ce qu'on voit au-delà est un choix d'Audrey (V3).

**Points ouverts**

- V3 : débouché de la ruelle.

**Raccord** 🟡 → `SC_C1_01` : Pas de coupe : le chapitre 1 commence dans la même ruelle, à 16 h 39.

Références données : `EVT_HORS_VUE`, `EVT_CH1_START`, `DLG_P_HEROINE_CHOC`

---

## Chapitre 1 — Le fourgon

### `SC_C1_01` — L'appel et la ruelle vide

- **Lieu** 🟡 : Ruelle des Tamaris (plan B), de l'entrée au bas-côté. — décors `ENV_RUE_FUITE` ; zones `Z_CROISEMENT`, `Z_RUE_FUITE`
- **Moment et lumière** 🟡 : mardi, 16:39 → 16:52 ; fin d'après-midi, lumière chaude ; météo : beau temps
- **Personnages** : ✅ `CHAR_HEROINE` {HEROINE}, joueuse · ✅ `CHAR_CHIENNE` Ariane, libre, sans laisse ni collier, foulard noir à motifs blancs · 🟡 `PASSANTS` quelques riverains qui sortent des maisons

**Action**

1. 🟡 Appel au 17 (3 min) : les options de description dépendent de ce que la joueuse a vu.
2. 🟡 Facultatif : protéger la scène (« Reculez, ne touchez à rien »).
3. 🟡 Examiner la ruelle : porte-clés renard près du caniveau, traces de pneus et tache d'huile sur le bas-côté, trois mégots, une caisse de la criée ; si le cordon a été vu tomber, fouiller la haie (badge).

**Déplacements**

| | Qui | De | Vers | Distance | Durée |
|---|---|---|---|---|---|
| 🟡 | Héroïne | mi-ruelle | entrée / bas-côté | ≤ 25 m | libre |

**Indices observables**

| | Indice | Ce qu'on voit | Point de vue | Distance | Caméras | Indispensable |
|---|---|---|---|---|---|---|
| 🟡 | `CLU_PORTE_CLES_LILA` | Porte-clés renard au sol. | dans la ruelle | ≤ 2 m | SHOULDER, WIDE, FIRST | non |
| 🟡 | `CLU_TRACES_PNEUS` | Empreintes dans le sable, tache d'huile. | bas-côté | ≤ 2 m | SHOULDER, WIDE, FIRST | non |
| 🟡 | `CLU_MEGOTS` | Trois mégots côté conducteur. | bas-côté | ≤ 2 m | SHOULDER, WIDE, FIRST | non |
| 🟡 | `CLU_CAISSE_POISSON` | Caisse « Criée de Capbreton » (fausse piste loyale). | bas-côté | ≤ 3 m | SHOULDER, WIDE, FIRST | non |
| 🟡 | `CLU_BADGE` | Badge « Sandrine V. », ancien logo de la commune, cordon cassé. | dans la haie | ≤ 1 m | SHOULDER, WIDE, FIRST | non |

**Caméras**

- `CAM_SHOULDER` : Surbrillance d'inspection proche.
- `CAM_WIDE` : Caméra rapprochée près des haies.
- `CAM_FIRST` : Idéale pour examiner le sol.
- `CAM_INSPECT` : Objets examinés en plein écran.

**Si un indice est manqué**

- Appel au 17 retardé → Gendarmes à l'appel + 13 min ; reproche de Mendiondo, aucune perte d'indice.
- Badge non trouvé → Piste de la bouteille (SC_C1_05) ou fouille des gendarmes à 18 h 30.

**Éléments visuels à produire**

- 🟡 `PROP_PORTE_CLES` (accessoire) : Porte-clés renard.
- 🟡 `PROP_MEGOTS` (accessoire) : Trois mégots.
- 🟡 `PROP_CAISSE` (accessoire) : Caisse en polystyrène « Criée de Capbreton ».
- 🟡 `PROP_BADGE` (accessoire) : Badge plastifié « Accueil périscolaire — Commune de Lescoure — Sandrine V. », ancien logo, cordon bleu cassé.
- 🟡 `DECAL_TRACES` (décor) : Empreintes de pneus et tache d'huile dans le sable du bas-côté.

**Raccord** 🟡 → `SC_C1_02` : Arrivée des gendarmes par la place ; l'héroïne remonte vers l'entrée de la ruelle pour les accueillir.

Références données : `INT_APPEL_17`, `INT_PROTEGER_SCENE`, `INT_EXAMINER_ACCOTEMENT`, `INT_FOUILLER_HAIE`, `DLG_C1_OPERATRICE`, `DLG_C1_EXAMEN_ACCOTEMENT`

### `SC_C1_02` — Les gendarmes

- **Lieu** 🟡 : Entrée de la ruelle, puis place de l'Église où s'installe le poste de commandement (plan A). — décors `ENV_RUE_FUITE`, `ENV_PLACE_EGLISE` ; zones `Z_CROISEMENT`, `Z_PLACE`
- **Moment et lumière** 🟡 : mardi, 16:52 → 17:00 ; fin d'après-midi ; la place se vide ; météo : beau temps
- **Personnages** : ✅ `CHAR_HEROINE` {HEROINE}, joueuse · ✅ `CHAR_CHIENNE` Ariane, libre, sans laisse ni collier, foulard noir à motifs blancs · 🟡 `CHAR_MENDIONDO` adjudante-cheffe · 🟡 `GENDARMES` deux ou trois gendarmes

**Action**

1. 🟡 Mendiondo recueille la déposition : « Pour une alerte, il me faut un véhicule, des personnes et une direction. Pas des impressions. »
2. 🟡 Elle autorise l'héroïne à rester : l'équipe cynophile est à plus de deux heures ; « ce que flaire votre chienne, c'est une piste, pas une preuve ».
3. 🟡 Le véhicule de commandement s'installe sur la place ; le tableau d'hypothèses devient accessible.

**Déplacements**

| | Qui | De | Vers | Distance | Durée |
|---|---|---|---|---|---|
| 🟡 | Mendiondo et l'héroïne | entrée de la ruelle | place de l'Église | ≈ 40–60 m | 1 min de jeu |

**Caméras**

- `CAM_SHOULDER` : Dialogue en champ-contrechamp léger.
- `CAM_WIDE` : Place lisible, poste repérable.
- `CAM_FIRST` : Visage de Mendiondo à hauteur d'yeux.

**Si un indice est manqué**

- — → Aucun indice dans cette scène.

**Éléments visuels à produire**

- 🟡 `CHAR_MENDIONDO` (personnage) : Adjudante-cheffe, uniforme de gendarmerie, quarantaine.
- 🟡 `PROP_VEHICULE_COMMANDEMENT` (véhicule) : Véhicule de commandement de gendarmerie sur la place.

**Raccord** 🟡 → `SC_C1_03` : Enquête libre : la joueuse choisit l'ordre des scènes SC_C1_03 à SC_C1_09.

Références données : `EVT_GENDARMES_ARRIVENT`, `DLG_C1_MENDIONDO_ARRIVEE`, `DLG_C1_MENDIONDO_REPROCHE`

### `SC_C1_03` — La piste de Lila

- **Lieu** 🟡 : Ruelle des Tamaris (plan B), du point d'abordage au pied de l'emplacement du fourgon. — décors `ENV_RUE_FUITE` ; zones `Z_CROISEMENT`, `Z_RUE_FUITE`
- **Moment et lumière** 🟡 : mardi, libre (après 16:39) → +5 min ; lumière chaude qui baisse ; météo : beau temps
- **Personnages** : ✅ `CHAR_HEROINE` {HEROINE}, joueuse · ✅ `CHAR_CHIENNE` Ariane, libre, sans laisse ni collier, foulard noir à motifs blancs

**Action**

1. 🟡 L'héroïne fait sentir le porte-clés à Ariane : « Sens. C'est Lila. Cherche Lila. »
2. 🟡 A : cercles serrés au point d'abordage, puis traction brève vers la place, truffe intermittente (piste du matin).
3. 🟡 B : allure régulière sur le trottoir, 15–25 m.
4. 🟡 C : Ariane s'assied au pied de l'emplacement du fourgon et regarde l'héroïne : le bracelet en perles, cordon cassé.
5. 🟡 D : tête haute, cercles, retour : la piste s'arrête, Ariane ne suit pas un véhicule.

**Déplacements**

| | Qui | De | Vers | Distance | Durée |
|---|---|---|---|---|---|
| 🟡 | Ariane puis l'héroïne | point d'abordage | emplacement de la portière | 15–25 m | ≈ 1 min réelle |

**Indices observables**

| | Indice | Ce qu'on voit | Point de vue | Distance | Caméras | Indispensable |
|---|---|---|---|---|---|---|
| 🟡 | `CLU_PISTE_LILA` | Langage corporel d'Ariane en quatre points. | suivre Ariane | 2–5 m | SHOULDER, WIDE, FIRST | non |
| 🟡 | `CLU_BRACELET_LILA` | Bracelet en perles dans le sable. | au sol | ≤ 1 m | SHOULDER, WIDE, FIRST | non |

**Caméras**

- `CAM_SHOULDER` : Ariane et son allure toujours dans le cadre.
- `CAM_WIDE` : Trajet complet lisible.
- `CAM_FIRST` : Léger recentrage pour garder Ariane dans le champ.
- `CAM_INSPECT` : Bracelet en plein écran.

**Si un indice est manqué**

- Piste mal interprétée → Témoignages de Lartigue et de la directrice mènent au même constat (Lila a été trompée).

**Éléments visuels à produire**

- ✅ `ANIM_CHIENNE_PISTE` (animation) : Cercles truffe basse, traction brève, allure régulière, assise + regard, tête haute et retour.
- 🟡 `PROP_BRACELET` (accessoire) : Bracelet en perles, cordon cassé.

**Continuité**

- Lila : tenue et cheveux non validés, identiques dans toutes ses apparitions ; cartable avec porte-clés renard et bracelet en perles visibles (nécessaires aux énigmes).

**Raccord** 🟡 → `SC_C1_04` : Retour libre vers la place.

Références données : `INT_PISTE_LILA`, `PZ_01`, `DLG_C1_PISTE_LILA`

### `SC_C1_04` — Le square : Dufau et la boulangerie

- **Lieu** 🟡 : Square avec le banc de Dufau et boulangerie Lartigue avec son tableau de liège (plan A). — décors `ENV_PROMENADE`, `ENV_ECOLE` ; zones `Z_TEMOINS`
- **Moment et lumière** 🟡 : mardi, libre → boulangerie ouverte jusqu'à 19:30 ; fin d'après-midi ; météo : beau temps
- **Personnages** : ✅ `CHAR_HEROINE` {HEROINE}, joueuse · ✅ `CHAR_CHIENNE` Ariane, libre, sans laisse ni collier, foulard noir à motifs blancs · 🟡 `CHAR_DUFAU` témoin bougon · 🟡 `CHAR_LARTIGUE` boulangère

**Action**

1. 🟡 Dufau : fourgon passé devant l'école vers 15 h 50, garé dans la ruelle ; conducteur jamais sorti, fumait ; plaque des Landes ; « pas la blanchisserie, eux c'est le mardi matin » ; lui rappelle « Franck, l'ancien de la blanchisserie » ; la caisse de poisson, c'est le poissonnier.
2. 🟡 Lartigue : vers 16 h 10, une femme au badge a acheté une bouteille d'eau et demandé « l'école de la petite Mercadier » ; elle s'est assise à l'abribus.
3. 🟡 Tableau de liège : deux prospectus de blanchisserie (facultatif, dépend de l'aspect du fourgon).

**Déplacements**

| | Qui | De | Vers | Distance | Durée |
|---|---|---|---|---|---|
| 🟡 | Duo | place | square | ≈ 20–30 m | 1 min de jeu |

**Indices observables**

| | Indice | Ce qu'on voit | Point de vue | Distance | Caméras | Indispensable |
|---|---|---|---|---|---|---|
| 🟡 | `CLU_TEMOIN_DUFAU` | Témoignage. | au banc | < 3 m | SHOULDER, WIDE, FIRST | non |
| 🟡 | `CLU_TEMOIN_LARTIGUE` | Témoignage : « la petite Mercadier ». | comptoir | < 3 m | SHOULDER, WIDE, FIRST | non |
| 🟡 | `CLU_FLYER_BLANCHISSERIE` | Prospectus Blanchisserie Océane (facultatif). | tableau de liège | < 1 m | SHOULDER, WIDE, FIRST | non |
| 🟡 | `CLU_FLYER_PRESSING` | Prospectus Blanchisserie du Courant (facultatif). | tableau de liège | < 1 m | SHOULDER, WIDE, FIRST | non |

**Caméras**

- `CAM_SHOULDER` : Dialogues.
- `CAM_WIDE` : Square et vitrine lisibles.
- `CAM_FIRST` : Lecture du tableau de liège.
- `CAM_INSPECT` : Prospectus en plein écran.

**Si un indice est manqué**

- Lartigue non interrogée → Le message de chantage (18 h 45 au plus tard) montre aussi que l'enlèvement visait la famille.

**Éléments visuels à produire**

- 🟡 `CHAR_LARTIGUE` (personnage) : Boulangère, tablier.
- 🟡 `ENV_BOULANGERIE` (lieu) : Boulangerie de quartier avec tableau de liège (annonces, prospectus).

**Points ouverts**

- V2 : les prospectus n'ont de sens que si le fourgon retenu porte une ombre de lettrage.

**Raccord** 🟡 → `SC_C1_05` : L'abribus et l'école sont à quelques pas.

Références données : `INT_DUFAU`, `INT_LARTIGUE`, `INT_TABLEAU_LIEGE`, `DLG_C1_DUFAU`, `DLG_C1_LARTIGUE`, `DLG_C1_LIEGE`

### `SC_C1_05` — L'école, l'abribus et la piste de la femme

- **Lieu** 🟡 : Portail de l'école et abribus (plan A), puis entrée de la ruelle et haie (plan B). — décors `ENV_ECOLE`, `ENV_RUE_FUITE` ; zones `Z_ECOLE`, `Z_CROISEMENT`, `Z_RUE_FUITE`
- **Moment et lumière** 🟡 : mardi, libre → +10 min ; fin d'après-midi ; météo : beau temps
- **Personnages** : ✅ `CHAR_HEROINE` {HEROINE}, joueuse · ✅ `CHAR_CHIENNE` Ariane, libre, sans laisse ni collier, foulard noir à motifs blancs · 🟡 `CHAR_DIRECTRICE` Mme Pujol, directrice, au portail

**Action**

1. 🟡 Directrice : aucune « Sandrine » parmi les animatrices ; Lila rentre seule depuis la rentrée avec l'accord de sa mère ; peu de gens le savaient.
2. 🟡 Affiche de la commune au nouveau logo (à comparer avec le badge).
3. 🟡 Abribus : bouteille d'eau entamée avec trace de rouge à lèvres, ticket de 16 h 12 ; plan touristique avec les distances.
4. 🟡 Piste de la femme avec la bouteille (dans un sac à crottes) : abribus → entrée de la ruelle → emplacement du fourgon → haie, où Ariane s'assied : le badge.

**Déplacements**

| | Qui | De | Vers | Distance | Durée |
|---|---|---|---|---|---|
| 🟡 | Ariane puis l'héroïne | abribus (plan A) | haie de la ruelle (plan B) | ≈ 60–90 m | 5 min de jeu |

**Indices observables**

| | Indice | Ce qu'on voit | Point de vue | Distance | Caméras | Indispensable |
|---|---|---|---|---|---|---|
| 🟡 | `CLU_TEMOIN_DIRECTRICE` | Témoignage. | portail | < 3 m | SHOULDER, WIDE, FIRST | non |
| 🟡 | `CLU_AFFICHE_COMMUNE` | Nouveau logo. | portail | < 1 m | SHOULDER, WIDE, FIRST | non |
| 🟡 | `CLU_BOUTEILLE` | Bouteille, rouge à lèvres. | banc de l'abribus | < 1 m | SHOULDER, WIDE, FIRST | non |
| 🟡 | `CLU_TICKET_BOULANGERIE` | Ticket 16 h 12. | sous le banc | < 1 m | SHOULDER, WIDE, FIRST | non |
| 🟡 | `CLU_PLAN_DISTANCES` | Distances depuis le rond-point. | panneau | < 1 m | SHOULDER, WIDE, FIRST | non |
| 🟡 | `CLU_BADGE` | Badge dans la haie. | haie | ≤ 1 m | SHOULDER, WIDE, FIRST | non |

**Caméras**

- `CAM_SHOULDER` : Ariane dans le cadre pendant la piste.
- `CAM_WIDE` : Trajet complet lisible d'un lieu à l'autre.
- `CAM_FIRST` : Lecture des objets.
- `CAM_INSPECT` : Affiche, ticket, plan, badge en plein écran.

**Si un indice est manqué**

- Mauvais objet de référence (mégots, porte-clés) → Piste courte ou piste de Lila, +5 min ; relancer avec la bouteille.
- Badge jamais trouvé → Fouille de la haie par les gendarmes à 18 h 30 ; la directrice suffit pour conclure à une fausse animatrice.

**Éléments visuels à produire**

- 🟡 `CHAR_DIRECTRICE` (personnage) : Directrice d'école, livide.
- 🟡 `PROP_BOUTEILLE` (accessoire) : Bouteille d'eau entamée, trace de rouge à lèvres.
- 🟡 `PROP_AFFICHE` (accessoire) : Affiche de rentrée au nouveau logo (vague bleue) ; l'ancien logo (pin et soleil) figure sur le badge.

**Raccord** 🟡 → `SC_C1_06` : De la haie, la joueuse voit le perron de l'étude au bout de la place (proposition).

Références données : `INT_DIRECTRICE`, `INT_AFFICHE_ECOLE`, `INT_BANC_ABRIBUS`, `INT_PLAN_TOURISTIQUE`, `INT_PISTE_BOUTEILLE`, `PZ_02`, `DLG_C1_PISTE_BOUTEILLE`

### `SC_C1_06` — Le perron de la notaire

**Décisions d'Audrey qui s'appliquent** ✅

- ✅ Q2 : Casteran est un témoin de bonne foi qui s'est trompé.

- **Lieu** 🟡 : Perron de l'étude Casteran, place de l'Église (plan A), avec vue en enfilade sur la ruelle. — décors `ENV_PLACE_EGLISE` ; zones `Z_PLACE`
- **Moment et lumière** 🟡 : mardi, à partir de 16:45 → libre ; fin d'après-midi ; la ruelle au loin, à contre-jour ; météo : beau temps
- **Personnages** : ✅ `CHAR_HEROINE` {HEROINE}, joueuse · ✅ `CHAR_CHIENNE` Ariane, libre, sans laisse ni collier, foulard noir à motifs blancs · ✅ `CHAR_CASTERAN` notaire, témoin de bonne foi

**Action**

1. 🟡 Casteran, sûre d'elle : « J'étais sur mon perron, j'ai tout vu. Il est descendu jusqu'au bout de la ruelle et il a tourné à droite, vers la Corniche. »
2. 🟡 « Se mettre à sa place » : l'héroïne monte sur le perron : on voit la ruelle jusqu'au débouché, pas le boulevard. « Elle ne pouvait pas savoir de quel côté il a tourné. Elle a supposé. »

**Déplacements**

| | Qui | De | Vers | Distance | Durée |
|---|---|---|---|---|---|
| 🟡 | Héroïne | place | perron | quelques mètres | 2 min de jeu |

**Indices observables**

| | Indice | Ce qu'on voit | Point de vue | Distance | Caméras | Indispensable |
|---|---|---|---|---|---|---|
| 🟡 | `CLU_TEMOIN_CASTERAN` | Témoignage (fausse piste loyale). | perron | < 3 m | SHOULDER, WIDE, FIRST | non |
| 🟡 | `CLU_LIGNE_DE_VUE` | Vue depuis le perron à hauteur d'yeux. | perron, 1,65 m | vue jusqu'au débouché (≈ 100–150 m) | SHOULDER, WIDE, FIRST | non |

**Caméras**

- `CAM_SHOULDER` : Transition vers la hauteur d'yeux pendant le test.
- `CAM_WIDE` : La caméra reculée descend à hauteur d'yeux pendant le test.
- `CAM_FIRST` : Vue naturelle du test.

**Si un indice est manqué**

- Test non fait → La vidéo d'Inès contredit Casteran ; présenter « Corniche » coûte 25 min et donne un résultat négatif.

**Éléments visuels à produire**

- 🟡 `CHAR_CASTERAN` (personnage) : Notaire, la soixantaine, élégante, foulard, ton posé.
- 🟡 `ENV_PLACE_EGLISE` (lieu) : Place de l'Église : étude notariale avec perron, fontaine, platane ; vue en enfilade sur la ruelle.

**Raccord** 🟡 → `SC_C1_07` : Pour savoir où il a vraiment tourné, il faut une autre source : quelqu'un filmait près du rond-point.

Références données : `INT_CASTERAN`, `INT_LIGNE_DE_VUE`, `PZ_03`, `DLG_C1_CASTERAN`, `DLG_C1_LIGNE_DE_VUE`

### `SC_C1_07` — Le skatepark

- **Lieu** 🟡 : Butte du skatepark près du rond-point du Lac, au-delà du débouché de la ruelle et du boulevard (extension du chapitre 1). — décors `ENV_ROND_POINT` ; zones `Z_RUE_FUITE`
- **Moment et lumière** 🟡 : mardi, libre → +4 min ; fin d'après-midi ; météo : beau temps
- **Personnages** : ✅ `CHAR_HEROINE` {HEROINE}, joueuse · ✅ `CHAR_CHIENNE` Ariane, libre, sans laisse ni collier, foulard noir à motifs blancs · 🟡 `CHAR_INES` 15 ans, filme ses figures

**Action**

1. 🟡 Inès se méfie ; deux approches la convainquent (rassurer ou l'impliquer), la menace la fait partir.
2. 🟡 Vidéo de 12 s à consulter image par image : le fourgon arrive du boulevard, prend la route des Étangs, plaque arrière « 37-TR » ; un camping-car part vers la Corniche.

**Déplacements**

| | Qui | De | Vers | Distance | Durée |
|---|---|---|---|---|---|
| 🟡 | Duo | place | skatepark | ≈ 300–400 m par le boulevard | 1–2 min de jeu |

**Indices observables**

| | Indice | Ce qu'on voit | Point de vue | Distance | Caméras | Indispensable |
|---|---|---|---|---|---|---|
| 🟡 | `CLU_VIDEO_INES` | Vidéo du rond-point. | téléphone d'Inès | plein écran | SHOULDER, WIDE, FIRST, INSPECT | **oui** |

**Caméras**

- `CAM_SHOULDER` : Dialogue.
- `CAM_WIDE` : Butte et rond-point lisibles.
- `CAM_FIRST` : Dialogue.
- `CAM_INSPECT` : Vidéo en plein écran, image par image.

**Si un indice est manqué**

- Inès braquée → Ses parents apportent la vidéo au poste à 19 h 00.
- Vidéo mal lue → La caméra du Relais du Lac (demandable après la vidéo) confirme la route des Étangs.

**Éléments visuels à produire**

- 🟡 `ENV_ROND_POINT` (lieu) : Rond-point du Lac à trois sorties panneautées (Corniche, Étangs, D652), boulevard, butte du skatepark.
- 🟡 `CHAR_INES` (personnage) : Adolescente, skate, téléphone.
- 🟡 `VID_INES_ROND_POINT` (vidéo) : Vidéo verticale 12 s, horodatée 16:39:10, rendue dans le moteur.

**Points ouverts**

- V2 : dans la vidéo, le fourgon doit ressembler au fourgon de la ruelle, quel que soit l'aspect choisi.

**Raccord** 🟡 → `SC_C1_08` : Retour au poste pour transmettre la vidéo.

Références données : `INT_INES`, `PZ_04`, `EVT_INES_PARENTS`, `DLG_C1_INES`, `DLG_C1_INES_PARENTS`

### `SC_C1_08` — La famille au poste

**Décisions d'Audrey qui s'appliquent** ✅

- ✅ Q1 : l'enlèvement sert à faire pression sur Julien, le père, impliqué dans le réseau et désireux d'en sortir ; parents séparés.
- ✅ Q1 : Nadia arrive après l'enlèvement et ignore tout du trafic.
- ✅ Q2 : le compagnon de Nadia dirige le réseau (nom « Xavier Darrigade » provisoire).

- **Lieu** 🟡 : Place de l'Église, autour du poste de commandement et de la fontaine (plan A). — décors `ENV_PLACE_EGLISE` ; zones `Z_PLACE`
- **Moment et lumière** 🟡 : mardi, 17:30 → 18:45 ; le soleil descend ; la place passe dans l'ombre, les toits restent dorés ; météo : beau temps
- **Personnages** : ✅ `CHAR_HEROINE` {HEROINE}, joueuse · ✅ `CHAR_CHIENNE` Ariane, libre, sans laisse ni collier, foulard noir à motifs blancs · ✅ `CHAR_JULIEN` père de Lila, mêlé au trafic · ✅ `CHAR_NADIA` mère de Lila, ne sait rien · ✅ `CHAR_DARRIGADE` compagnon de Nadia, tête du réseau (secret) · 🟡 `CHAR_MENDIONDO` adjudante-cheffe

**Action**

1. 🟡 17 h 30 : Julien arrive de Bayonne en tenue de travail, hors de lui.
2. 🟡 17 h 35 : Nadia arrive avec Darrigade ; Darrigade se présente et propose son aide. Ariane s'arrête à trois mètres de lui, oreilles plaquées, et détourne la tête de sa main ; il l'explique : « Trop de monde. »
3. 🟡 17 h 40 : Julien lit un message, blêmit, ment (« le boulot ») ; Ariane se colle à ses jambes ; Darrigade lui pose la main sur l'épaule : Julien se raidit et se tait.
4. 🟡 Facultatif : écouter Nadia (qui savait que Lila rentrait seule : elle, l'école, Julien, Darrigade ; Julien voulait quitter son travail chez Darrigade).
5. 🟡 Éloigner Darrigade (proposer la battue) ou attendre 18 h 05 ; parler à Julien à l'écart avec compassion et une preuve que l'enlèvement le visait : il montre le message et la photo, refuse de dire pour qui il travaille, son regard file vers Darrigade.

**Déplacements**

| | Qui | De | Vers | Distance | Durée |
|---|---|---|---|---|---|
| 🟡 | Darrigade | près de Julien et Nadia | bout de la place, téléphone à l'oreille | ≈ 20–30 m | à 18 h 05 ou sur demande |

**Indices observables**

| | Indice | Ce qu'on voit | Point de vue | Distance | Caméras | Indispensable |
|---|---|---|---|---|---|---|
| 🟡 | `CLU_ARIANE_DARRIGADE` | Ariane garde ses distances avec Darrigade, sans grogner. | place | 3–10 m | SHOULDER, WIDE, FIRST | non |
| 🟡 | `CLU_PERE_REACTION` | Julien cache son téléphone, se raidit sous la main de Darrigade. | près de la fontaine | ≤ 15 m | SHOULDER, WIDE, FIRST | non |
| 🟡 | `CLU_TEMOIN_NADIA` | Témoignage de Nadia. | poste | < 3 m | SHOULDER, WIDE, FIRST | non |
| 🟡 | `CLU_MESSAGE_CHANTAGE` | « Tu voulais partir. Un dernier voyage, jeudi 6 h, et tu la revois. Pas de police. Elle va bien. » | téléphone de Julien | plein écran | SHOULDER, WIDE, FIRST, INSPECT | non |
| 🟡 | `CLU_PHOTO_VIE` | Photo de Lila (voir SC_C1_09). | téléphone de Julien | plein écran | SHOULDER, WIDE, FIRST, INSPECT | non |

**Caméras**

- `CAM_SHOULDER` : Dialogues ; Julien et Darrigade lisibles ensemble dans le cadre.
- `CAM_WIDE` : Place entière : on voit qui s'approche de qui.
- `CAM_FIRST` : Visages à hauteur d'yeux.
- `CAM_INSPECT` : Message et photo en plein écran.

**Si un indice est manqué**

- Julien braqué ou interrogé devant Darrigade → Il se ferme (+5 min) ; à 18 h 45 il montre lui-même le message aux gendarmes.
- Réaction de Julien non observée → Graine perdue pour ce chapitre, reprise au chapitre 3.

**Éléments visuels à produire**

- 🟡 `CHAR_JULIEN` (personnage) : Trentaine, chauffeur : tenue de travail d'entrepôt sans logo réel.
- 🟡 `CHAR_NADIA` (personnage) : Trentaine, aide-soignante, tenue de ville.
- 🟡 `CHAR_DARRIGADE` (personnage) : Quarantaine soignée, veste sobre, gestes calmes et rassurants ; rien de menaçant à l'image.
- 🟡 `ANIM_JULIEN_RAIDI` (animation) : Lire et cacher le téléphone, se raidir sous une main sur l'épaule, regard fuyant.
- 🟡 `ANIM_CHIENNE_DISTANCE` (animation) : S'arrêter à distance, oreilles plaquées, détourner la tête d'une main tendue, sans grogner.

**Continuité**

- Nadia ne montre jamais qu'elle connaîtrait le réseau (décision d'Audrey).

**Points ouverts**

- V5 : apparence de Darrigade (nom provisoire).

**Raccord** 🟡 → `SC_C1_09` : La photo de Lila part au poste ; Mendiondo la fait agrandir sur l'écran du véhicule.

Références données : `EVT_PERE_ARRIVE`, `EVT_NADIA_ARRIVE`, `EVT_DARRIGADE_S_ELOIGNE`, `EVT_PERE_CRAQUE`, `INT_OBSERVER_PERE`, `INT_NADIA_TEMOIGNAGE`, `INT_ELOIGNER_DARRIGADE`, `INT_PERE`, `PZ_08`, `DLG_C1_PERE`, `DLG_C1_NADIA`, `DLG_C1_NADIA_ARRIVEE`

### `SC_C1_09` — Les résultats du poste

- **Lieu** 🟡 : Intérieur et abords du véhicule de commandement, place de l'Église (plan A). — décors `ENV_PLACE_EGLISE` ; zones `Z_PLACE`
- **Moment et lumière** 🟡 : mardi, libre (17:15 : radio) → 19:30 ; ombre sur la place ; écrans du véhicule allumés ; météo : beau temps
- **Personnages** : ✅ `CHAR_HEROINE` {HEROINE}, joueuse · ✅ `CHAR_CHIENNE` Ariane, libre, sans laisse ni collier, foulard noir à motifs blancs · 🟡 `CHAR_MENDIONDO` adjudante-cheffe

**Action**

1. 🟡 Consultation de la plaque : GF-437-TR appartient au fourgon d'un plombier de Dax qui était à Dax : plaque clonée.
2. 🟡 17 h 15, radio : un fourgon « correspondant au signalement » au péage de l'A63 à 16 h 58 (fausse piste loyale).
3. 🟡 Après la vidéo d'Inès : caméra du Relais du Lac, 16 h 48, plaque « …37-TR ».
4. 🟡 Carte de l'étang de Sorbe ; photo de Lila agrandie (soleil bas face à la fenêtre, ponton, pin avec pot à résine).

**Indices observables**

| | Indice | Ce qu'on voit | Point de vue | Distance | Caméras | Indispensable |
|---|---|---|---|---|---|---|
| 🟡 | `CLU_SIV_CLONE` | Résultat de la plaque. | écran du poste | plein écran | SHOULDER, WIDE, FIRST, INSPECT | **oui** |
| 🟡 | `CLU_SIGNALEMENT_PEAGE` | Radio. | poste | audible + carnet | SHOULDER, WIDE, FIRST | non |
| 🟡 | `CLU_CCTV_RELAIS` | Vidéo de station. | écran | plein écran | SHOULDER, WIDE, FIRST, INSPECT | non |
| 🟡 | `CLU_CARTE_ETANG` | Carte murale. | poste | plein écran | SHOULDER, WIDE, FIRST, INSPECT | non |
| 🟡 | `CLU_APPEL_BLANCHISSERIE` | Mendiondo rapporte l'appel : fourgon réformé vendu à Sud Loc Services, récupéré par Franck Loubère (facultatif, seulement si le fourgon porte une ombre de lettrage). | poste | dialogue | SHOULDER, WIDE, FIRST | non |

**Caméras**

- `CAM_SHOULDER` : Écrans et carte lisibles.
- `CAM_WIDE` : Intérieur du véhicule : caméra rapprochée.
- `CAM_FIRST` : Idéale pour les écrans.
- `CAM_INSPECT` : Documents en plein écran.

**Si un indice est manqué**

- Photo non prise en SC_P2 → La vidéo seule suffit : consultation de 15 min au lieu de 5.
- Signalement du péage cru → +30 min et résultat négatif ; option exclue.

**Éléments visuels à produire**

- 🟡 `IMG_PHOTO_VIE` (image) : Lila assise, calme mais inquiète, couverture sur les épaules ; fenêtre sur l'étang, soleil bas dans l'axe (azimut ≈ 242°), ponton, pin avec pot à résine. Aucune marque de violence. Tenue et cheveux identiques à SC_P0_B.
- 🟡 `VID_CCTV_RELAIS` (vidéo) : Caméra de station, 16:48, plaque « 37-TR ».
- 🟡 `PROP_CARTE_ETANG` (accessoire) : Carte de l'étang de Sorbe : base nautique à l'ouest, forêt d'ancien gemmage à l'est.

**Continuité**

- Lila : tenue et cheveux non validés, identiques dans toutes ses apparitions ; cartable avec porte-clés renard et bracelet en perles visibles (nécessaires aux énigmes).

**Points ouverts**

- V4 : la photo de Lila fixe définitivement sa tenue et ses cheveux pour la suite.

**Raccord** 🟡 → `SC_C1_10` : Quand la joueuse est prête, elle présente le tableau à Mendiondo.

Références données : `INT_PLAQUE_COMPLETE`, `INT_PLAQUE_PARTIELLE`, `INT_CCTV_RELAIS`, `INT_CARTE_ETANG`, `EVT_RADIO_PEAGE`, `PZ_05`, `PZ_07`, `PZ_09`, `DLG_C1_PLAQUE`, `DLG_C1_RADIO_PEAGE`, `DLG_C1_RELAIS`, `DLG_C1_CARTE`, `INT_APPEL_BLANCHISSERIE`, `DLG_C1_BLANCHISSERIE`

### `SC_C1_10` — Le tableau et le départ

**Décisions d'Audrey qui s'appliquent** ✅

- ✅ Q2 : le compagnon de Nadia dirige le réseau.

- **Lieu** 🟡 : Poste de commandement, place de l'Église (plan A). — décors `ENV_PLACE_EGLISE` ; zones `Z_PLACE`
- **Moment et lumière** 🟡 : mardi, au choix (≤ 19:30) → 19:30 au plus tard ; crépuscule qui approche (coucher ≈ 19 h 45) ; météo : beau temps
- **Personnages** : ✅ `CHAR_HEROINE` {HEROINE}, joueuse · ✅ `CHAR_CHIENNE` Ariane, libre, sans laisse ni collier, foulard noir à motifs blancs · 🟡 `CHAR_MENDIONDO` adjudante-cheffe · ✅ `CHAR_NADIA` mère · ✅ `CHAR_DARRIGADE` compagnon

**Action**

1. 🟡 La joueuse présente trois hypothèses : véhicule (plaque clonée), personnes (fausse animatrice qui savait qui elle venait chercher), destination (route des Étangs). Erreur : temps perdu, résultat négatif, option grisée.
2. 🟡 Mendiondo demande l'Alerte Enlèvement et envoie un peloton vers l'étang de Sorbe.
3. 🟡 Darrigade entend la destination et raccompagne Nadia : « Viens, je te ramène à la maison » (graine de la fuite du chapitre 2).
4. 🟡 « Allez, Ariane. On va chercher Lila. »

**Déplacements**

| | Qui | De | Vers | Distance | Durée |
|---|---|---|---|---|---|
| 🟡 | Duo et gendarmes | place | véhicules | ≈ 20 m | — |

**Caméras**

- `CAM_SHOULDER` : Tableau lisible.
- `CAM_WIDE` : Place au crépuscule, départ des véhicules.
- `CAM_FIRST` : Tableau à hauteur d'yeux.
- `CAM_INSPECT` : Tableau d'hypothèses en plein écran.

**Si un indice est manqué**

- Pas de résolution à 19 h 30 → Clôture automatique : les gendarmes partent sur la route des Étangs (état C).

**Éléments visuels à produire**

- 🟡 `UI_TABLEAU` (interface) : Tableau en trois colonnes (véhicule, personnes, destination).

**Raccord** 🟡 → `SC_C2_01` : Fondu sur la route des Étangs au crépuscule ; l'état de départ (A, B ou C) dépend de l'heure de résolution.

Références données : `INT_PRESENTER_TABLEAU`, `PZ_10`, `BR_RESOLU`, `BR_CLOTURE`, `DLG_C1_TABLEAU`, `DLG_C1_FIN`, `DLG_C1_CLOTURE`

---

## Chapitre 2 — L'airial

### `SC_C2_01` — La route des Étangs

- **Lieu** 🟡 : Route des Étangs, Relais du Lac (station-service), fourche vers l'étang de Sorbe. — décors `ENV_ROUTE_ETANGS`
- **Moment et lumière** 🟡 : mardi, ≈ 18:30–19:45 selon l'état → +15 min ; crépuscule (A), nuit tombante (B), nuit noire (C) ; météo : sec ; pluie fine après 23 h en état C
- **Personnages** : ✅ `CHAR_HEROINE` {HEROINE}, joueuse · ✅ `CHAR_CHIENNE` Ariane, libre, sans laisse ni collier, foulard noir à motifs blancs · 🟡 `CHAR_MENDIONDO` adjudante-cheffe · 🟡 `GENDARMES` peloton

**Action**

1. 🟡 Convoi ; arrêt bref au Relais du Lac : le gérant confirme l'heure de passage.
2. 🟡 Briefing sur le capot : trois airiaux sur la rive est (deux en état A).

**Déplacements**

| | Qui | De | Vers | Distance | Durée |
|---|---|---|---|---|---|
| 🟡 | Convoi | Lescoure | fourche de l'étang | 14 km | ≈ 15 min |

**Caméras**

- `CAM_SHOULDER` : Héroïne et Ariane parmi les gendarmes.
- `CAM_WIDE` : Convoi lisible.
- `CAM_FIRST` : Facultative.

**Si un indice est manqué**

- — → Aucun indice indispensable.

**Éléments visuels à produire**

- 🟡 `ENV_ROUTE_ETANGS` (lieu) : Route bordée de pins, station-service isolée.
- 🟡 `ENV_RELAIS_DU_LAC` (lieu) : Petite station-service avec caméra à la pompe.

**Points ouverts**

- V6 : degré d'obscurité acceptable pour les scènes de nuit.

**Raccord** 🟡 → `SC_C2_02` : Arrivée à pied sur les pistes forestières, lampes éteintes.

### `SC_C2_02` — Les airiaux de la rive est

- **Lieu** 🟡 : Pistes forestières numérotées, sable et aiguilles de pin, trois airiaux (clairières habitées) sur la rive est de l'étang de Sorbe. — décors `ENV_FORET_RIVE_EST`
- **Moment et lumière** 🟡 : mardi, nuit → +1 h ; lune et lampes frontales ; contre-jour de l'étang ; météo : sec ou pluie fine (état C)
- **Personnages** : ✅ `CHAR_HEROINE` {HEROINE}, joueuse · ✅ `CHAR_CHIENNE` Ariane, libre, sans laisse ni collier, foulard noir à motifs blancs · 🟡 `GENDARMES` peloton, radio

**Action**

1. 🟡 La joueuse lit les traces (pneus, pas, portières) et oriente le peloton ; Ariane piste sur les passages frais.
2. 🟡 Les gendarmes investissent chaque airial ; la joueuse reste en retrait comme le lui ordonne Mendiondo.

**Déplacements**

| | Qui | De | Vers | Distance | Durée |
|---|---|---|---|---|---|
| 🟡 | Duo et peloton | fourche | airial de Hount-Bielha (en dernier ou non selon les choix) | 1–3 km à pied | selon l'ordre de fouille |

**Indices observables**

| | Indice | Ce qu'on voit | Point de vue | Distance | Caméras | Indispensable |
|---|---|---|---|---|---|---|
| 🟡 | `TRACES_TRANSFERT` | Traces fraîches d'une voiture (pas le fourgon) qui repart vers l'intérieur des terres. | piste | au sol | SHOULDER, WIDE, FIRST | **oui** |

**Caméras**

- `CAM_SHOULDER` : Traces lisibles à la lampe.
- `CAM_WIDE` : Clairière entière.
- `CAM_FIRST` : Idéale pour lire le sol.

**Si un indice est manqué**

- Mauvais airial → Temps perdu ; le suivant est indiqué par les traces et la carte (pas d'impasse).

**Éléments visuels à produire**

- 🟡 `ENV_FORET_RIVE_EST` (lieu) : Forêt landaise d'ancien gemmage, pins à pots de résine, airiaux, sable clair.
- 🟡 `ANIM_CHIENNE_NUIT` (animation) : Pistage de nuit, arrêts et hésitations.

**Points ouverts**

- Détail des énigmes du chapitre 2 à écrire après validation du chapitre 1.

**Raccord** 🟡 → `SC_C2_03` : Le bon airial : une maison basse, fenêtres sur l'étang.

### `SC_C2_03` — L'airial vide

**Décisions d'Audrey qui s'appliquent** ✅

- ✅ Q3 : Lila n'est retrouvée qu'au chapitre 4.

- **Lieu** 🟡 : Airial de Hount-Bielha : maison basse, pièce principale dont la fenêtre donne sur l'étang (celle de la photo), grange. — décors `ENV_AIRIAL_HOUNT_BIELHA`
- **Moment et lumière** 🟡 : mardi, nuit → +20 min ; nuit, lampes ; météo : sec ou pluie fine
- **Personnages** : ✅ `CHAR_HEROINE` {HEROINE}, joueuse · ✅ `CHAR_CHIENNE` Ariane, libre, sans laisse ni collier, foulard noir à motifs blancs · 🟡 `GENDARMES` peloton · 🟡 `CHAR_K1` Loubère, caché dans la grange

**Action**

1. ✅ La pièce est vide : couverture pliée, chaise, verre d'eau. Lila en est partie environ une heure plus tôt (décision : elle n'est retrouvée qu'au chapitre 4).
2. 🟡 Ariane trouve dans l'herbe la gomme parfumée à la fraise de la trousse de Lila, puis la piste s'arrête sur des traces de pneus fraîches.
3. 🟡 Ariane marque la grange : les gendarmes interpellent Loubère, qui tentait de brûler des papiers (état A : un document à moitié sauvé). Aucune violence montrée.

**Indices observables**

| | Indice | Ce qu'on voit | Point de vue | Distance | Caméras | Indispensable |
|---|---|---|---|---|---|---|
| 🟡 | `PIECE_PHOTO` | La pièce correspond à la photo : même fenêtre, même vue sur l'étang. | intérieur | — | SHOULDER, WIDE, FIRST | **oui** |
| 🟡 | `GOMME_LILA` | Gomme parfumée de Lila. | herbe | ≤ 1 m | SHOULDER, WIDE, FIRST | non |
| 🟡 | `DOC_BRULE` | Document à moitié brûlé (état A seulement). | grange | ≤ 1 m | SHOULDER, WIDE, FIRST | non |

**Caméras**

- `CAM_SHOULDER` : Intérieur lisible malgré la nuit.
- `CAM_WIDE` : Airial entier.
- `CAM_FIRST` : Intérieur à hauteur d'yeux, comparaison avec la photo.
- `CAM_INSPECT` : Photo et document en plein écran.

**Si un indice est manqué**

- Pièce non comparée à la photo → Mendiondo fait la comparaison elle-même.

**Éléments visuels à produire**

- 🟡 `ENV_AIRIAL_HOUNT_BIELHA` (lieu) : Airial landais, maison basse, pièce avec fenêtre sur l'étang (raccord exact avec IMG_PHOTO_VIE).
- 🟡 `PROP_GOMME` (accessoire) : Gomme parfumée à la fraise.

**Continuité**

- La pièce doit être strictement celle de IMG_PHOTO_VIE (fenêtre, ponton, pin au pot à résine).

**Raccord** 🟡 → `SC_C2_04` : Retour au poste de la place dans la nuit.

### `SC_C2_04` — Qui les a prévenus ?

**Décisions d'Audrey qui s'appliquent** ✅

- ✅ Q2 : le compagnon de Nadia dirige le réseau.

- **Lieu** 🟡 : Place de l'Église, poste de commandement, de nuit. — décors `ENV_PLACE_EGLISE` ; zones `Z_PLACE`
- **Moment et lumière** 🟡 : mercredi, ≈ 01:00 → +10 min ; nuit, éclairage public ; météo : sec
- **Personnages** : ✅ `CHAR_HEROINE` {HEROINE}, joueuse · ✅ `CHAR_CHIENNE` Ariane, libre, sans laisse ni collier, foulard noir à motifs blancs · 🟡 `CHAR_MENDIONDO` adjudante-cheffe · 🟡 `CHAR_ARBELOT` capitaine, section de recherches · ✅ `CHAR_DARRIGADE` vient apporter du café aux gendarmes

**Action**

1. 🟡 Arbelot prend l'enquête. Question : Lila a été déplacée une heure avant ; qui savait ?
2. 🟡 Darrigade apporte du café « de la part de Nadia » et demande des nouvelles (graine).
3. 🟡 Julien accepte de revenir le lendemain matin, sans rien promettre.

**Indices observables**

| | Indice | Ce qu'on voit | Point de vue | Distance | Caméras | Indispensable |
|---|---|---|---|---|---|---|
| 🟡 | `DARRIGADE_CAFE` | Darrigade présent au poste en pleine nuit, curieux de l'opération. | place | ≤ 10 m | SHOULDER, WIDE, FIRST | non |

**Caméras**

- `CAM_SHOULDER` : Dialogues.
- `CAM_WIDE` : Place de nuit.
- `CAM_FIRST` : Visages.

**Si un indice est manqué**

- — → Aucun indice indispensable.

**Éléments visuels à produire**

- 🟡 `CHAR_ARBELOT` (personnage) : Capitaine de la section de recherches, civil ou uniforme.

**Raccord** 🟡 → `SC_C3_01` : Ellipse jusqu'au mercredi matin.

---

## Chapitre 3 — Le dernier voyage

### `SC_C3_01` — Julien parle

**Décisions d'Audrey qui s'appliquent** ✅

- ✅ Q1 : Julien est impliqué dans le réseau et veut en sortir.

- **Lieu** 🟡 : Salle d'audition de la brigade, puis cour de la gendarmerie. — décors `ENV_GENDARMERIE`
- **Moment et lumière** 🟡 : mercredi, 09:00 → 10:00 ; lumière grise du matin ; météo : couvert
- **Personnages** : ✅ `CHAR_HEROINE` {HEROINE}, joueuse · ✅ `CHAR_CHIENNE` Ariane, libre, sans laisse ni collier, foulard noir à motifs blancs · ✅ `CHAR_JULIEN` père · 🟡 `CHAR_ARBELOT` capitaine

**Action**

1. 🟡 Avec les preuves du chapitre 1, la joueuse aide Arbelot à convaincre Julien : il raconte les cartons de médicaments aux notices fausses, sa démission refusée, le « dernier voyage » (conduire le camion jusqu'au Maren Sofie, jeudi 6 h).
2. 🟡 Il refuse encore de dire qui dirige : « Si je le dis, elle est morte » (il ne nomme pas Darrigade).
3. 🟡 Arbelot monte une opération surveillée : Julien fera le voyage.

**Indices observables**

| | Indice | Ce qu'on voit | Point de vue | Distance | Caméras | Indispensable |
|---|---|---|---|---|---|---|
| 🟡 | `TEMOIGNAGE_JULIEN` | Récit de Julien. | salle | < 3 m | SHOULDER, WIDE, FIRST | **oui** |

**Caméras**

- `CAM_SHOULDER` : Dialogue.
- `CAM_WIDE` : Salle entière.
- `CAM_FIRST` : Visages.

**Si un indice est manqué**

- Julien peu coopératif (pas allié au ch. 1) → Il accepte après une énigme de dialogue plus longue ; jamais d'impasse.

**Éléments visuels à produire**

- 🟡 `ENV_GENDARMERIE` (lieu) : Brigade de gendarmerie de bord de mer, salle d'audition sobre.

**Raccord** 🟡 → `SC_C3_02` : Route vers Bayonne.

### `SC_C3_02` — L'entrepôt

**Décisions d'Audrey qui s'appliquent** ✅

- ✅ Q2 : le compagnon de Nadia dirige le réseau.

- **Lieu** 🟡 : Entrepôt de Darrigade Logistique, zone portuaire de Bayonne (visite des douanes autorisée). — décors `ENV_ENTREPOT_BAYONNE`
- **Moment et lumière** 🟡 : mercredi, 14:00 → 16:00 ; néons d'entrepôt, lumière du jour par les quais ; météo : couvert
- **Personnages** : ✅ `CHAR_HEROINE` {HEROINE}, joueuse · ✅ `CHAR_CHIENNE` Ariane, libre, sans laisse ni collier, foulard noir à motifs blancs · ✅ `CHAR_DARRIGADE` patron des lieux, courtois · 🟡 `DOUANIERS` agents des douanes · 🟡 `CHAR_JULIEN` au volant, comme d'habitude

**Action**

1. 🟡 Darrigade accueille la visite avec amabilité et propose un café à l'héroïne.
2. 🟡 Comparaison de documents : bons de livraison, poids déclarés et pesés, numéros de scellés.
3. 🟡 Ariane marque un carton qui porte l'odeur de « Sandrine ».
4. 🟡 Darrigade s'intéresse de près à « l'opération de demain » (graine).

**Indices observables**

| | Indice | Ce qu'on voit | Point de vue | Distance | Caméras | Indispensable |
|---|---|---|---|---|---|---|
| 🟡 | `DOCS_FALSIFIES` | Écarts entre documents et marchandise. | bureau | plein écran | SHOULDER, WIDE, FIRST | **oui** |
| 🟡 | `CARTON_SANDRINE` | Carton marqué par Ariane. | rayonnage | ≤ 2 m | SHOULDER, WIDE, FIRST | non |

**Caméras**

- `CAM_SHOULDER` : Ariane dans le cadre.
- `CAM_WIDE` : Allées lisibles.
- `CAM_FIRST` : Lecture des documents.
- `CAM_INSPECT` : Documents en plein écran.

**Si un indice est manqué**

- Documents mal comparés → Les douaniers font une contre-vérification (+30 min).

**Éléments visuels à produire**

- 🟡 `ENV_ENTREPOT_BAYONNE` (lieu) : Entrepôt logistique portuaire, rayonnages, quai de chargement ; aucune marque réelle.

**Raccord** 🟡 → `SC_C3_03` : Nuit à Bayonne ; Nadia appelle l'héroïne, puis Darrigade (hors champ).

### `SC_C3_03` — Le dernier voyage

**Décisions d'Audrey qui s'appliquent** ✅

- ✅ Port : hors de l'ouverture seulement ; il peut apparaître ensuite.

- **Lieu** 🟡 : Quais du port de Bayonne, abords du caboteur Maren Sofie. — décors `ENV_QUAIS_BAYONNE`
- **Moment et lumière** 🟡 : jeudi, 05:15 → 06:30 ; nuit finissante, projecteurs du port, aube grise ; météo : bruine
- **Personnages** : ✅ `CHAR_HEROINE` {HEROINE}, joueuse · ✅ `CHAR_CHIENNE` Ariane, libre, sans laisse ni collier, foulard noir à motifs blancs · 🟡 `CHAR_JULIEN` au volant du camion · 🟡 `GENDARMES` planques · 🟡 `COMPLICE` homme du réseau

**Action**

1. 🟡 Julien livre le camion ; « Sandrine » était attendue : c'est un complice qui vient à sa place.
2. 🟡 Interpellation du complice par les gendarmes ; « Sandrine » a été prévenue.
3. 🟡 Ariane retrouve, dans la voiture du complice, un téléphone abandonné par « Sandrine » : ses derniers déplacements dessinent une zone dans la forêt de Lande-Haute.

**Indices observables**

| | Indice | Ce qu'on voit | Point de vue | Distance | Caméras | Indispensable |
|---|---|---|---|---|---|---|
| 🟡 | `TELEPHONE_SANDRINE` | Téléphone et historique de déplacements. | voiture du complice | plein écran | SHOULDER, WIDE, FIRST | **oui** |

**Caméras**

- `CAM_SHOULDER` : Suivi discret depuis une planque.
- `CAM_WIDE` : Quai lisible.
- `CAM_FIRST` : Tension à hauteur d'yeux.
- `CAM_INSPECT` : Carte des déplacements en plein écran.

**Si un indice est manqué**

- Téléphone non trouvé → Les gendarmes le trouvent à la fouille du véhicule (+20 min).

**Éléments visuels à produire**

- 🟡 `ENV_QUAIS_BAYONNE` (lieu) : Quais de port de commerce de nuit, grues, projecteurs.
- 🟡 `VEH_MAREN_SOFIE` (véhicule) : Petit cargo caboteur fictif.

**Points ouverts**

- Le port apparaît ici pour la première fois (hors de l'ouverture, conformément à la décision d'Audrey).

**Raccord** 🟡 → `SC_C4_01` : Départ immédiat vers la forêt ; la pluie commence.

---

## Chapitre 4 — La cabane

### `SC_C4_01` — La carte des parcelles

**Décisions d'Audrey qui s'appliquent** ✅

- ✅ Q1 : Nadia ignore tout du trafic.
- ✅ Q2 : le compagnon de Nadia dirige le réseau.

- **Lieu** 🟡 : Poste de crise en lisière de la forêt de Lande-Haute (camionnette, tables sous auvent). — décors `ENV_PC_FORET`
- **Moment et lumière** 🟡 : jeudi, 08:00 → 09:00 ; jour gris, pluie froide ; météo : pluie
- **Personnages** : ✅ `CHAR_HEROINE` {HEROINE}, joueuse · ✅ `CHAR_CHIENNE` Ariane, libre, sans laisse ni collier, foulard noir à motifs blancs · 🟡 `CHAR_ARBELOT` capitaine · ✅ `CHAR_NADIA` mère, venue apporter un vêtement de Lila · ✅ `CHAR_DARRIGADE` l'accompagne

**Action**

1. 🟡 Superposer la zone du téléphone, le cadastre et les anciennes cabanes de résinier : trois cabanes candidates, dont une sur une parcelle de la SCI Lande-Haute.
2. 🟡 Nadia remet un vêtement de Lila pour Ariane. Darrigade l'accompagne, attentif ; Ariane reste à distance de lui.

**Indices observables**

| | Indice | Ce qu'on voit | Point de vue | Distance | Caméras | Indispensable |
|---|---|---|---|---|---|---|
| 🟡 | `CADASTRE` | Trois cabanes candidates ; une parcelle appartient à une SCI. | table | plein écran | SHOULDER, WIDE, FIRST | **oui** |
| 🟡 | `VETEMENT_LILA` | Objet de référence pour Ariane. | mains de Nadia | < 1 m | SHOULDER, WIDE, FIRST | **oui** |

**Caméras**

- `CAM_SHOULDER` : Table lisible.
- `CAM_WIDE` : Poste de crise.
- `CAM_FIRST` : Carte.
- `CAM_INSPECT` : Cadastre en plein écran.

**Si un indice est manqué**

- Mauvais ordre de fouille → Plus de pluie, piste plus faible, mais aucune impasse : la bonne cabane reste atteignable.

**Éléments visuels à produire**

- 🟡 `ENV_PC_FORET` (lieu) : Poste de crise en lisière, véhicules, auvent, cartes.

**Raccord** 🟡 → `SC_C4_02` : Départ sous la pluie.

### `SC_C4_02` — La battue

- **Lieu** 🟡 : Forêt de Lande-Haute : pistes numérotées, parcelles de pins, fougères, fossés. — décors `ENV_FORET_LANDE_HAUTE`
- **Moment et lumière** 🟡 : jeudi, 09:00 → 17:30 ; pluie froide, lumière qui baisse l'après-midi ; météo : pluie
- **Personnages** : ✅ `CHAR_HEROINE` {HEROINE}, joueuse · ✅ `CHAR_CHIENNE` Ariane, libre, sans laisse ni collier, foulard noir à motifs blancs · 🟡 `GENDARMES` équipes de battue

**Action**

1. 🟡 La joueuse choisit l'ordre des cabanes ; lecture de traces sous la pluie ; Ariane piste avec le vêtement de Lila (piste qui s'affaiblit avec l'heure).
2. 🟡 Deux cabanes vides (une avec traces anciennes d'un chasseur).

**Indices observables**

| | Indice | Ce qu'on voit | Point de vue | Distance | Caméras | Indispensable |
|---|---|---|---|---|---|---|
| 🟡 | `TRACES_PLUIE` | Pas et traces de pneus partiellement effacés. | sol | ≤ 3 m | SHOULDER, WIDE, FIRST | **oui** |

**Caméras**

- `CAM_SHOULDER` : Ariane dans le cadre.
- `CAM_WIDE` : Parcelles lisibles malgré la pluie.
- `CAM_FIRST` : Lecture du sol.

**Si un indice est manqué**

- Piste perdue → Ariane revient au dernier point sûr ; la carte indique la cabane suivante.

**Éléments visuels à produire**

- 🟡 `ENV_FORET_LANDE_HAUTE` (lieu) : Forêt landaise sous la pluie, pistes sableuses, fossés, pins alignés.
- 🟡 `ANIM_CHIENNE_PLUIE` (animation) : Pistage sous la pluie, s'ébrouer, hésiter.

**Points ouverts**

- V7 : intensité de la pluie et de l'obscurité.

**Raccord** 🟡 → `SC_C4_03` : Fin d'après-midi : la dernière cabane, au fond d'une parcelle.

### `SC_C4_03` — La cabane

**Décisions d'Audrey qui s'appliquent** ✅

- ✅ Q3 : Lila est retrouvée au chapitre 4.
- ✅ Q3 : elle n'a subi aucune violence mais a été retenue dans une cabane sombre, froide et sans confort.

- **Lieu** ✅ : Ancienne cabane de résinier en planches, au fond d'une parcelle : sombre, froide, sans aucun confort. — décors `ENV_CABANE`
- **Moment et lumière** 🟡 : jeudi, ≈ 17:45 → ≈ 18:15 ; fin de jour sous la pluie ; intérieur très sombre, lumière des lampes ; météo : pluie
- **Personnages** : ✅ `CHAR_HEROINE` {HEROINE}, joueuse · ✅ `CHAR_CHIENNE` Ariane, libre, sans laisse ni collier, foulard noir à motifs blancs · ✅ `CHAR_FILLETTE` Lila, retenue seule · 🟡 `GENDARMES` équipe · 🟡 `CHAR_K2` « Sandrine », qui revient

**Action**

1. 🟡 Ariane accélère, truffe au sol, et s'assied devant une porte cadenassée : aboiement bref, regard vers l'héroïne.
2. ✅ Les gendarmes ouvrent. Lila est là, recroquevillée dans un coin, frigorifiée et terrifiée, **sans aucune trace de violence**.
3. 🟡 Elle reconnaît Ariane avant de reconnaître les adultes ; Ariane s'approche doucement. L'héroïne s'agenouille, lui met sa veste sur les épaules.
4. 🟡 « Sandrine », qui revenait, est arrêtée sur la piste (ou s'enfuit : choix à faire plus tard).

**Indices observables**

| | Indice | Ce qu'on voit | Point de vue | Distance | Caméras | Indispensable |
|---|---|---|---|---|---|---|
| 🟡 | `CABANE` | Porte cadenassée ; intérieur : planches, sol de terre, un seau, une bouteille d'eau ; rien de plus. | extérieur puis intérieur | ≤ 5 m | SHOULDER, WIDE, FIRST | **oui** |

**Caméras**

- `CAM_SHOULDER` : Plan large sur Ariane devant la porte, puis intérieur.
- `CAM_WIDE` : Caméra rapprochée ; pas de plan qui s'attarde sur la détresse de Lila.
- `CAM_FIRST` : Hauteur d'yeux d'adulte qui s'agenouille.

**Si un indice est manqué**

- La joueuse arrive tard → La scène a lieu de toute façon ; seules la pluie et la nuit changent.

**Éléments visuels à produire**

- ✅ `ENV_CABANE` (lieu) : Cabane de résinier en planches, sombre, froide, sans aucun confort ; aucun signe de violence.
- 🟡 `ANIM_LILA_RETROUVEE` (animation) : Lila recroquevillée, lève les yeux vers Ariane, se laisse approcher.

**Continuité**

- Lila : tenue et cheveux non validés, identiques dans toutes ses apparitions ; cartable avec porte-clés renard et bracelet en perles visibles (nécessaires aux énigmes).
- Même tenue qu'au chapitre 1, salie et humide ; aucune blessure.

**Points ouverts**

- V8 : ce qu'on montre de Lila dans la cabane (durée, cadrage) : à valider par Audrey.

**Raccord** 🟡 → `SC_C4_04` : Lila est portée jusqu'aux véhicules.

### `SC_C4_04` — Les retrouvailles

**Décisions d'Audrey qui s'appliquent** ✅

- ✅ Q3 : Lila est retrouvée au chapitre 4, sans violence subie.
- ✅ Q1 : Nadia ignore tout du trafic.

- **Lieu** 🟡 : Piste forestière près des véhicules de secours. — décors `ENV_FORET_LANDE_HAUTE`
- **Moment et lumière** 🟡 : jeudi, ≈ 18:30 → ≈ 18:45 ; crépuscule pluvieux, gyrophares ; météo : pluie
- **Personnages** : ✅ `CHAR_HEROINE` {HEROINE}, joueuse · ✅ `CHAR_CHIENNE` Ariane, libre, sans laisse ni collier, foulard noir à motifs blancs · ✅ `CHAR_FILLETTE` Lila · ✅ `CHAR_NADIA` mère · ✅ `CHAR_DARRIGADE` compagnon · 🟡 `SECOURS` pompiers, médecin

**Action**

1. 🟡 Lila est enveloppée dans une couverture de survie ; Nadia la serre contre elle.
2. 🟡 Darrigade arrive derrière Nadia et pose la main sur son épaule ; Lila se fige et détourne les yeux (graine forte, sans explication).
3. 🟡 Ariane se place entre Lila et Darrigade, sans grogner. Fin de l'acte I.

**Indices observables**

| | Indice | Ce qu'on voit | Point de vue | Distance | Caméras | Indispensable |
|---|---|---|---|---|---|---|
| 🟡 | `LILA_SE_FIGE` | Lila se fige quand Darrigade approche. | piste | ≤ 5 m | SHOULDER, WIDE, FIRST | non |

**Caméras**

- `CAM_SHOULDER` : Groupe lisible.
- `CAM_WIDE` : Gyrophares et pluie.
- `CAM_FIRST` : Visages.

**Si un indice est manqué**

- Graine manquée → Reprise au chapitre 6 par le témoignage de Lila recueilli par des professionnels.

**Éléments visuels à produire**

- 🟡 `ENV_SECOURS_FORET` (lieu) : Véhicules de secours sur une piste forestière sous la pluie.

**Continuité**

- Lila : tenue et cheveux non validés, identiques dans toutes ses apparitions ; cartable avec porte-clés renard et bracelet en perles visibles (nécessaires aux énigmes).

**Raccord** 🟡 → `SC_C5_01` : Ellipse : quelques jours plus tard, départ pour Anvers.

---

## Chapitre 5 — Le pavillon (plan)

### `SC_C5_01` — Anvers : le pavillon

- **Lieu** 🟡 : Anvers : docks, archives du registre des sociétés, bureau de Nordhaven Shipping BV. — décors `ENV_ANVERS`
- **Moment et lumière** 🟡 : jours suivants, — → — ; lumière du Nord, grise ; météo : variable
- **Personnages** : ✅ `CHAR_HEROINE` {HEROINE}, joueuse · ✅ `CHAR_CHIENNE` Ariane, libre, sans laisse ni collier, foulard noir à motifs blancs · 🟡 `POLICE_BELGE` police fédérale

**Action**

1. 🟡 Reconstituer l'organigramme des sociétés écrans jusqu'aux contrats de fret de Darrigade Logistique.

**Indices observables**

| | Indice | Ce qu'on voit | Point de vue | Distance | Caméras | Indispensable |
|---|---|---|---|---|---|---|
| 🟡 | `ORGANIGRAMME` | Chaîne de prête-noms jusqu'à Darrigade Logistique. | archives | plein écran | SHOULDER, WIDE, FIRST | **oui** |

**Caméras**

- `CAM_SHOULDER` : —
- `CAM_WIDE` : —
- `CAM_FIRST` : —
- `CAM_INSPECT` : Documents en plein écran.

**Si un indice est manqué**

- Organigramme incomplet → La police belge fournit une pièce manquante contre du temps.

**Éléments visuels à produire**

- 🟡 `ENV_ANVERS` (lieu) : Docks et bureaux d'Anvers (plan à détailler).

**Points ouverts**

- Chapitre à détailler après validation de l'acte I.

**Raccord** 🟡 → `SC_C6_01` : Retour à Lescoure.

---

## Chapitre 6 — Faux-semblants (plan)

### `SC_C6_01` — Faux-semblants

**Décisions d'Audrey qui s'appliquent** ✅

- ✅ Q2 : le compagnon de Nadia dirige le réseau ; Casteran est de bonne foi.

- **Lieu** 🟡 : Lescoure : étude Casteran (archives), maison de Nadia, entrepôt de Bayonne. — décors `ENV_PLACE_EGLISE`, `ENV_ENTREPOT_BAYONNE`
- **Moment et lumière** 🟡 : jours suivants, — → — ; fin d'après-midi dorée, comme l'ouverture (proposition) ; météo : beau temps
- **Personnages** : ✅ `CHAR_HEROINE` {HEROINE}, joueuse · ✅ `CHAR_CHIENNE` Ariane, libre, sans laisse ni collier, foulard noir à motifs blancs · ✅ `CHAR_DARRIGADE` tête du réseau · ✅ `CHAR_CASTERAN` témoin de bonne foi dont les archives servent de preuve · 🟡 `CHAR_ARBELOT` capitaine

**Action**

1. 🟡 Démonstration devant Arbelot et le juge : silence de Julien, trajet de Lila connu du cercle proche, deux fuites, archives de l'étude Casteran.
2. 🟡 Perquisition légale : Ariane marque le placard où Darrigade a gardé la veste portée à la cabane.
3. 🟡 Dernière scène : Lila, Nadia et Ariane à la sortie de l'école.

**Indices observables**

| | Indice | Ce qu'on voit | Point de vue | Distance | Caméras | Indispensable |
|---|---|---|---|---|---|---|
| 🟡 | `VESTE_CABANE` | Veste portant l'odeur de la cabane et de Lila. | placard | ≤ 2 m | SHOULDER, WIDE, FIRST | **oui** |

**Caméras**

- `CAM_SHOULDER` : —
- `CAM_WIDE` : —
- `CAM_FIRST` : —

**Si un indice est manqué**

- Démonstration incomplète → Le juge demande une pièce de plus ; jamais d'échec définitif.

**Éléments visuels à produire**

- 🟡 `ENV_ETUDE_CASTERAN` (lieu) : Archives de l'étude notariale.

**Points ouverts**

- Chapitre à détailler après validation de l'acte I.

**Raccord** 🟡 → `FIN` : Générique.
