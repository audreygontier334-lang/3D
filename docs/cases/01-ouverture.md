# Mission 01 — Prologue « Sortie d'école » et Chapitre 1 « Le fourgon »

> **Statut** : les décisions d'Audrey sont marquées ✅ ; tout le reste est une **proposition** de Claude.
> Documents liés : **découpage scène par scène** `../narrative/DECOUPAGE_SCENES.md` (source pour les visuels) · énigmes `01-enigmes.md` · preuves `01-preuves.md` · répliques `../dialogues/01-ouverture.md` · données `../../GameData/missions/01/`.
> Les identifiants (`LOC_`, `CLU_`, `EVT_`, `ACT_`, `BR_`, `H_`, `DED_`, `PZ_`, `DLG_`, `SC_`) sont stables et partagés avec les données.
>
> **Décisions d'Audrey appliquées (29/09)** ✅
> - Centre-ville inspiré de celui d'Arcachon, fin d'après-midi ; le port reste hors de l'ouverture.
> - **La place de l'école et la ruelle adjacente, orientée vers le front de mer, sont deux lieux et deux plans distincts. L'enlèvement a lieu dans la ruelle, plus isolée.**
> - Ambiance architecturale de la ruelle validée : maisons variées, d'époques différentes, couleurs, clôtures et haies différentes.
> - **Non validés** : le déplacement précis de Lila, sa tenue et ses cheveux (voir §3.5, continuité de Lila).
> - Le niveau d'animation de la place de l'école et **l'apparence du fourgon** seront choisis pendant la construction 3D : **aucun indice indispensable n'en dépend** (vérifié par le validateur).
> - Ariane est libre, sans laisse ni collier, foulard noir à motifs paisley blancs ; l'héroïne porte un bandeau assorti et des baskets noires et blanches style Nike Air Max (sans logo ni nom de marque).
> - **Au bout de la ruelle, on voit la mer au loin** (choix V3, 29/09) ; le port reste hors champ.
> - **L'enlèvement a lieu vers 16 h 30** (29/09). Les minutes exactes de la chronologie ci-dessous sont des propositions.
> - Q1 : pression sur le père, Julien ; la mère, Nadia, arrive après et ignore tout. Q2 : le compagnon de Nadia dirige le réseau (nom « Xavier Darrigade » provisoire) ; Casteran est de bonne foi. Q3 : Lila est retrouvée au chapitre 4.

---

## 0. En une page (à lire en premier)

1. **16 h 25 — la place.** La joueuse prend la main dans la rue piétonne avec Ariane, libre à ses côtés, et arrive sur la place de l'école. Tutoriel doux (marcher, changer de vue, ordres à Ariane). Sonnerie ; Lila sort, fait coucou à Ariane, puis part seule vers la ruelle des Tamaris, son chemin habituel.
2. **16 h 28 — la ruelle.** Le duo suit sa promenade habituelle vers l'océan, qui passe par la même ruelle. En arrivant à l'entrée, la joueuse voit, une vingtaine de mètres plus loin, une femme au badge marcher avec Lila vers un fourgon garé sur le bas-côté. Devant la portière, Lila refuse de monter tant qu'elle n'a pas « appelé maman » ; la femme fait semblant de téléphoner. Lila se retourne et appelle Ariane. Fenêtre d'action de 8 à 12 s : photographier, courir, crier le prénom de Lila, envoyer Ariane (deux actions au plus). **Le fourgon part toujours**, descend la ruelle et tourne au bout, hors de vue.
3. **16 h 31 → 19 h 30 — le chapitre 1.** Appel au 17, gendarmes, témoins qui se contredisent ; arrivée du père qui cache un message, de la mère qui ne sait rien et de son compagnon qui console tout le monde. Chaque action coûte du temps de jeu. La joueuse établit trois conclusions distinctes : **véhicule** (par la plaque), **personnes**, **destination**.
4. **Résolution.** Le tableau présenté à l'adjudante-cheffe Mendiondo déclenche le dispositif vers l'étang de Sorbe. Une bonne déduction rapide donne de meilleures traces au chapitre 2 (Lila, elle, a déjà été déplacée). **Aucun échec définitif.**

---

## 1. Espace de jeu

### 1.1 Deux lieux, deux plans

Coordonnées indicatives de la maquette glTF de Codex (PR #3) : mètres, X vers l'est, Z vers le nord, portail de l'école à l'origine. **La maquette actuelle (commit `bcb280a`) place encore la ruelle dans le prolongement de la place ; les exigences de séparation sont demandées à Codex** (`GameData/missions/01/spatial_requirements.json`, demandes `REQ_RUELLE_*`). C'est la maquette adaptée qui fera foi pour les distances exactes.

```
PLAN A — LA PLACE DE L'ÉCOLE (animée, ouverte)            PLAN B — LA RUELLE DES TAMARIS (isolée)
                                                           orientée vers la mer ; au débouché,
  Z_TEMOINS          Z_PLACE           Z_ECOLE             la mer au loin ; boulevard hors champ
  square, banc de    place de l'Église portail de l'école
  Dufau,             étude Casteran    abribus, plan              ← vers l'océan
  boulangerie        (perron),         touristique        ┌──────────────────────────────────┐
                     fontaine, poste                      │ débouché   bas-côté + haie    entrée│
                     des gendarmes                        │ (on ne voit   FOURGON    ABORDAGE  ◄──── Z_CROISEMENT
        Z_PROMENADE                                       │  pas où il   (EVT_PISTE) (EVT_INDICE│     angle avec la
        rue piétonne,                                     │  tourne)                 _ODEUR)   │     rue de l'École,
        départ du duo      rue de l'École ───────────────►│◄──── 70–100 m ──────────►◄ 15–25 ►◄10►│     HORS de vue du
                                                           └──────────────────────────────────┘     portail
                                                                  Z_RUE_FUITE
     boulevard (hors champ) → rond-point du Lac (≈ 300 m) : sorties Corniche (nord), route des Étangs (est), D652 (sud)
```

La mer et le port existent mais **ne sont pas des lieux jouables dans P0–P3**. ✅ **Au bout de la ruelle, on voit la mer au loin** (décision d'Audrey, choix V3). Proposition : une bande de mer au-dessus du boulevard, dont la chaussée reste masquée par la pente, un muret ou les maisons d'angle, pour que l'énigme du perron (PZ_03) reste valable. Le port n'est pas visible.

### 1.2 Lieux

| ID | Lieu | Zone (PR #3) | Fonction | Accessible |
|---|---|---|---|---|
| `LOC_PROMENADE` | Rue piétonne du centre-ville | `Z_PROMENADE` | Départ, tutoriel | Toujours |
| `LOC_ECOLE` | Portail de l'école des Pins, sur la place | `Z_ECOLE` | Sortie de Lila, coucou ; affiche de la commune ; directrice | Toujours |
| `LOC_ABRIBUS` | Abribus et banc, rue de l'École | `Z_ECOLE` | Bouteille d'eau de « Sandrine » ; plan touristique | Toujours |
| `LOC_BANC_DUFAU` | Banc de Dufau, square | `Z_TEMOINS` | Témoin Dufau (a vu le fourgon passer devant l'école et entrer dans la ruelle) | Toujours |
| `LOC_BOULANGERIE` | Boulangerie Lartigue | `Z_TEMOINS` | Témoin ; tableau de liège | Jusqu'à 19 h 30 |
| `LOC_ETUDE_CASTERAN` | Perron de l'étude notariale | `Z_PLACE` | Témoin Casteran ; test de ligne de vue sur la ruelle | Toujours |
| `LOC_POSTE` | Véhicule de commandement des gendarmes | `Z_PLACE` | Famille, résultats, tableau d'hypothèses | À partir de 16 h 44 |
| `LOC_COIN_ECOLES` | **Entrée de la ruelle des Tamaris** (angle avec la rue de l'École) | `Z_CROISEMENT` | Point de vue de la joueuse pendant l'alerte ; à 10 m dans la ruelle : abordage, porte-clés de Lila | Toujours |
| `LOC_ACCOTEMENT` | **Ruelle des Tamaris** : bas-côté sablonneux et haie | `Z_RUE_FUITE` | Stationnement du fourgon ; traces, mégots, bracelet, badge dans la haie | Toujours |
| `LOC_SKATEPARK` | Butte du skatepark, près du rond-point | `Z_RUE_FUITE` (prolongement) | Témoin Inès ; vue sur le rond-point | Toujours |
| `LOC_ROND_POINT` | Rond-point du Lac | `Z_RUE_FUITE` (prolongement) | Sorties Corniche, route des Étangs, D652 | Toujours |

### 1.3 Distances et temps

| Trajet | Distance | Temps | Statut |
|---|---|---|---|
| Portail de l'école → entrée de la ruelle | 40–60 m, avec un angle de rue qui masque l'intérieur de la ruelle depuis le portail | ≈ 40 s à pied d'enfant | Proposition (à fixer dans la maquette) |
| Entrée de la ruelle → point d'abordage | ≈ 10 m | — | Proposition |
| Point d'abordage → portière du fourgon | 15–25 m (lecture de la piste d'Ariane) | ≈ 25 s à pied | Proposition, accepté par Codex |
| Portière → débouché de la ruelle | 70–100 m | ≈ 12 s en fourgon | Proposition |
| Débouché → rond-point du Lac par le boulevard | ≈ 300 m | ≈ 1 min | Proposition |
| Rond-point → Relais du Lac (route des Étangs) | 9 km | 9 min | Proposition |
| Rond-point → péage A63 (via D652) | 32 km | 28 min | Proposition |
| Rond-point → Lescoure-Nord (Corniche) | 11 km | 12 min | Proposition |
| Rond-point → port de Capbreton (via D652) | 24 km | 22 min | Proposition |
| Relais du Lac → rive est de l'étang de Sorbe | 5 km | 7 min (piste) | Proposition |

Les distances routières figurent sur le **plan touristique** de l'abribus (`CLU_PLAN_DISTANCES`) et sur la carte du poste.

---

## 2. Chronologie visible

La chronologie cachée complète est dans `BIBLE.md` §5.

| Heure | Événement | Où | ID |
|---|---|---|---|
| 16 h 25 | Prise en main dans la rue piétonne, Ariane libre à côté de {HEROINE} | Plan A | `EVT_START` |
| 16 h 26 | Sonnerie ; enfants au portail | Plan A | `EVT_SONNERIE` |
| 16 h 27 | Lila sort, fait coucou à Ariane, part seule vers la ruelle | Plan A | `EVT_LILA_COUCOU` |
| 16 h 27 min 40 | Lila tourne dans la ruelle et sort du champ de la place | A → B | — |
| 16 h 28 | Dans la ruelle, à 10 m de l'entrée, une femme au cordon bleu aborde Lila | Plan B | `EVT_ABORDAGE` |
| 16 h 28 min 40 | Elles marchent jusqu'au fourgon garé sur le bas-côté (≈ 25 s) | Plan B | `EVT_MARCHE_FOURGON` |
| ≈ 16 h 29 | Devant la portière, Lila refuse de monter : « Je veux appeler maman d'abord. » La femme fait semblant d'appeler Nadia et négocie (`DLG_P_ABORDAGE_04–06`) | Plan B | — |
| 16 h 29 min 50 (ou plus tôt, voir règle) | **Alerte** : Lila, devant la portière, voit Ariane à l'entrée de la ruelle et l'appelle ; Ariane grogne | Plan B | `EVT_ALERTE` |
| + 8 à 12 s (≈ 16 h 30) | La femme fait monter Lila, portière claquée, départ ; le bracelet de Lila casse et tombe au pied de la portière (non montré) | Plan B | `EVT_DEPART` |
| + 12 s | Le fourgon atteint le bout de la ruelle et tourne, hors de vue | Plan B | `EVT_HORS_VUE` |
| 16 h 31 | Début du chapitre 1 (« Appeler le 17 » mis en avant) | Plan B | `EVT_CH1_START` |

**Règle de déclenchement de l'alerte (proposition)** : l'alerte se produit dès que la joueuse (ou Ariane) arrive à moins de 5 m de l'entrée de la ruelle, et **au plus tard à 16 h 30**. Jusqu'à l'alerte, c'est le refus de Lila (faux appel de la femme) qui retient le fourgon : l'attente devant la portière dure moins d'une minute et a une raison visible. Si la joueuse est ailleurs à 16 h 30, Ariane s'élance vers la ruelle en aboyant et Lila crie « Ariane ! ». **Règle d'accélération** : si le duo s'approche à moins de 12 m du couple avant l'alerte, la femme presse Lila et le départ est avancé.

**Règle de départ (proposition, vérifiée par le validateur)** — `mission.json`, `prologue.departure_rule` :
- le départ a lieu 8 à 12 s après l'alerte, **mais il est retenu** (Lila résiste, la femme insiste) tant que la joueuse n'est pas à moins de 5 m de l'entrée de la ruelle, **30 s au plus** ;
- la position la plus éloignée atteignable pendant P0–P1 est à 110 m de l'entrée (demande `REQ_PROLOGUE_DISTANCE_ENTREE`) : en courant (5 m/s), 22 s, donc toujours dans les 38 s disponibles ;
- si la joueuse ne vient pas malgré tout (elle reste immobile), un **plan court non interactif de 4 s** (`CAM_DEPART_COURT`, depuis l'entrée) montre le départ, plaque arrière lisible, puis rend le contrôle ; dans ce cas, aucune action de fenêtre n'est acquise ;
- le fourgon met ≈ 12 s pour atteindre le bout de la ruelle : même dans le pire cas (alerte à 16 h 30, fenêtre de 12 s, 30 s de retenue), il est hors de vue avant 16 h 31, début du chapitre 1. Avec l'option d'accessibilité, l'horloge du chapitre 1 démarre quand même à 16 h 31.

---

## 3. Prologue jouable

### 3.1 Séquence P0 — La place de l'école (16 h 25 – 16 h 27 min 40) — plan A

Objectifs doux (facultatifs) :
- marcher, trotter, changer de vue (`CAM_SHOULDER`, `CAM_WIDE`, `CAM_FIRST`) ;
- ordres à Ariane : « Au pied », « Reste », « Cherche » (balle lancée dans le square), « Montre », « Va ! » ; elle revient au rappel ;
- saluer Dufau sur son banc (`DLG_P_DUFAU_*`) : il grommelle à propos du fourgon « qui fait un boucan de casserole » et qui s'est garé dans la ruelle « depuis une bonne demi-heure » — **première graine**, sans insistance ;
- voir Lila sortir et faire coucou (`EVT_LILA_COUCOU`) ; Ariane remue la queue ; si la joueuse répond, Lila sourit (`DLG_P_LILA_*`).

Le niveau d'animation de la place (nombre de parents, d'enfants, de voitures) est un **choix visuel ouvert d'Audrey** ; aucun indice n'en dépend. Seule contrainte : Lila doit rester repérable pendant au moins 3 s au portail et pendant sa marche vers l'angle de la ruelle, dans les trois vues.

### 3.2 Séquence P1 — Vers la ruelle, l'abordage (16 h 27 min 40 – alerte) — transition A → B

- Lila sort du champ de la place en tournant dans la ruelle ; la joueuse ne voit **pas** l'abordage depuis la place (décision : l'enlèvement a lieu dans la ruelle isolée).
- Le duo poursuit sa promenade habituelle, qui passe par la ruelle pour rejoindre l'océan (proposition). Ariane trotte devant vers l'angle : guidage doux, sans obligation.
- En arrivant à l'entrée de la ruelle, la joueuse découvre la scène à ≈ 20–30 m : une femme au badge tient Lila par la main et marche avec elle vers un fourgon garé sur le bas-côté, ou se tient déjà devant la portière pendant que Lila refuse de monter (faux appel). Rien d'explicitement violent. Ariane fixe la ruelle, oreille droite dressée.
- Répliques de l'abordage (`DLG_P_ABORDAGE_*`) audibles seulement à moins de 30 m ; sinon, gestes seuls.

### 3.3 Séquence P2 — La fenêtre d'action (alerte → hors de vue) — plan B

Au moment `EVT_ALERTE`, une **invite contextuelle discrète** apparaît (pas de ralenti imposé, pas d'écran figé).

| ID action | Commande | Durée | Effet sur les indices | Compatible avec |
|---|---|---|---|---|
| `ACT_PHOTO` | Lever le téléphone et photographier | 2 s | Immobile : `CLU_PHOTO_FOURGON` (**plaque arrière** partielle « GF-4·7 » ; ombre de lettrage seulement si le fourgon retenu en a une). En course : `CLU_PHOTO_FLOUE` | `ACT_CRIER`, `ACT_ENVOYER` ; en course = floue |
| `ACT_COURIR` | Sprinter dans la ruelle | continu | Le fourgon déboîte du bas-côté : profil du conducteur par la vitre (`CLU_OBS_CONDUCTEUR`), pot qui cogne et tremble (`CLU_OBS_ECHAPPEMENT`), feu arrière fendu (`CLU_OBS_FEU_FENDU`) | `ACT_CRIER`, `ACT_PHOTO` (floue) |
| `ACT_CRIER` | Appeler « Lila ! » | 1 s | La femme se retourne vers l'entrée : `CLU_OBS_PASSAGERE`. Elle se précipite, le cordon s'accroche à la portière : `CLU_OBS_BADGE_CHUTE` (la joueuse voit tomber quelque chose dans la haie) | tout |
| `ACT_ENVOYER` | « Ariane, va ! » | 1 s | Ariane fonce, aboie, **s'arrête net au bord de la chaussée** quand le fourgon démarre, flaire l'endroit où se tenait la femme : `CLU_CHIENNE_IMPREGNEE` ; elle regarde ensuite la haie | `ACT_PHOTO`, `ACT_CRIER` |
| — | Ne rien faire | — | Aucun indice de fenêtre ; tous les axes restent résolubles par les indices permanents | — |

- **Photo exclusive** : nette **ou** floue, jamais les deux (vérifié par le validateur).
- **Accessibilité** : option « Temps d'action étendu » (20–25 s ou pause jusqu'au choix), mêmes effets.
- **Sécurité d'Ariane** : aucune issue où elle est blessée ; elle s'arrête toujours au bord de la chaussée ; rappel instantané.
- **Orientation (proposition)** : le fourgon est garé **dans le sens de la descente**, arrière tourné vers l'entrée de la ruelle. La plaque arrière est donc face à la joueuse pendant toute la scène et pendant le départ : c'est ce qui rend la photo possible **quel que soit l'aspect du fourgon**. Portière latérale côté trottoir et haie, du même côté que la joueuse.
- **Toutes vues** : Lila, la femme, l'arrière du fourgon et la portière doivent être lisibles depuis l'entrée de la ruelle dans les trois vues (demande `REQ_RUELLE_VUE_ENTREE`). Le visage de la femme n'est lisible qu'avec `ACT_CRIER` (elle se retourne), indépendamment de la caméra.

### 3.4 Séquence P3 — Hors de vue (16 h 30 – 16 h 31) — plan B

Le fourgon tourne au bout de la ruelle ; **personne sur place ne voit de quel côté** (c'est ce qui rend le témoignage de Casteran faux et la vidéo d'Inès nécessaire). {HEROINE} reste à mi-ruelle, souffle court (`DLG_P_HEROINE_CHOC_*`). L'interface met en avant **« Appeler le 17 »**.

### 3.5 Continuité de Lila

| Élément | Statut | Contrainte |
|---|---|---|
| Âge (9 ans, CM1), prénom, trajet habituel école → maison par la ruelle | Proposition | Utilisés par les dialogues |
| Cartable avec un **porte-clés renard** accroché | Proposition, **nécessaire** à PZ_01 | Visible au portail et dans la ruelle ; le porte-clés se détache à l'abordage |
| **Bracelet en perles** au poignet (choisi parce qu'il ne dépend pas de la coiffure) | Proposition, **nécessaire** à PZ_01 | Visible au coucou (main levée) ; tombe au pied de la portière |
| Tenue | **Non validée** | Tenue d'école ordinaire de fin septembre, identique de P0 au chapitre 4 (même journée, puis captivité). À proposer par Codex et valider par Audrey |
| Cheveux (couleur, longueur, coiffure) | **Non validés** | Aucun indice n'en dépend. Une fois choisis, identiques dans toutes les scènes et sur la photo du chapitre 1 (faux raccord à éviter) |
| Trajet précis dans la place et la ruelle | **Non validé** | Seules contraintes : sortie visible 3 s, marche visible jusqu'à l'angle, abordage hors de vue du portail, 15–25 m de marche jusqu'à la portière |

**Accessoires scène par scène** (proposition ; détail dans `DECOUPAGE_SCENES.md`, bloc « Continuité » de chaque scène) :

| Scène | Porte-clés renard | Bracelet en perles | Cartable |
|---|---|---|---|
| `SC_P0_B` place, coucou | sur le cartable | au poignet (visible, main levée) | sur le dos |
| `SC_P1` abordage | se détache et reste au sol (≈ 10 m dans la ruelle) | au poignet | sur le dos |
| `SC_P2` portière, départ | **absent** (au sol derrière elle) | au poignet jusqu'à la montée ; casse hors champ et tombe au pied de la portière | monte avec elle |
| Chapitre 1, photo de vie (`CLU_PHOTO_VIE`) | absent | **absent** | — |
| `SC_C4_03`–`SC_C4_04` cabane, retrouvailles | absent | absent | absent (proposition : resté chez les ravisseurs) |

---

## 4. Chapitre 1 — structure

### 4.1 Système de temps

- L'horloge démarre à **16 h 31**. Elle n'avance **que** par les interactions (coûts ci-dessous) et les déplacements entre zones (1 min par zone traversée à pied ; 0 en restant dans une zone).
- L'horloge est visible dans le carnet. Aucun compte à rebours stressant à l'écran.
- **19 h 30** (coucher du soleil ≈ 19 h 45) : si le tableau n'a pas été présenté, Mendiondo convoque {HEROINE} et le chapitre se conclut avec les hypothèses les mieux étayées (voir §6, état C).

### 4.2 Phase A — Premiers gestes (16 h 31 – 16 h 44)

| Interaction | Coût | Obligatoire | Effet |
|---|---|---|---|
| Appel au 17 (`DLG_C1_OPERATRICE_*`) | 3 min | **Oui** (première action mise en avant ; possible de l'appeler plus tard, pénalité : gendarmes arrivent 5 min après l'appel) | Les options de description proposées dépendent des indices de fenêtre. Plus la description est précise, plus tôt la gendarmerie envoie des patrouilles sur les axes (effet narratif, pas mécanique) |
| Protéger la scène (dire aux passants de ne pas toucher le porte-clés) | 1 min | Non | Bonus de confiance avec Mendiondo (`FLAG_SCENE_PROTEGEE`) : elle partage un résultat de plus spontanément |
| Examiner la ruelle (entrée et bas-côté) | 2 min | Non | `CLU_PORTE_CLES_LILA`, `CLU_TRACES_PNEUS`, `CLU_MEGOTS`, `CLU_CAISSE_POISSON` ; si `CLU_OBS_BADGE_CHUTE`, `CLU_BADGE` directement dans la haie |

Arrivée des gendarmes : **16 h 44** (ou appel + 13 min). Mendiondo recueille la déposition (`DLG_C1_MENDIONDO_ARRIVEE_*`), installe le poste de commandement place de l'Église et **autorise {HEROINE} à rester comme témoin** : l'équipe cynophile de la gendarmerie est à plus de deux heures.

### 4.3 Phase B — Enquête ouverte (16 h 44 – résolution)

Ordre libre. Chaque ligne est détaillée dans les énigmes ou dans les dialogues.

| Interaction | Lieu | Coût | Condition | Indices obtenus |
|---|---|---|---|---|
| Témoignage Dufau | `LOC_BANC_DUFAU` | 4 min | — | `CLU_TEMOIN_DUFAU` |
| Témoignage Lartigue | `LOC_BOULANGERIE` | 3 min | — | `CLU_TEMOIN_LARTIGUE` |
| Examiner le tableau de liège | `LOC_BOULANGERIE` | 1 min | — | `CLU_FLYER_BLANCHISSERIE`, `CLU_FLYER_PRESSING` |
| Examiner le banc de l'abribus | `LOC_ABRIBUS` | 1 min | — | `CLU_BOUTEILLE`, `CLU_TICKET_BOULANGERIE` (le lien avec la femme vient de Lartigue) |
| Lire le plan touristique | `LOC_ABRIBUS` | 1 min | — | `CLU_PLAN_DISTANCES` |
| Examiner l'affiche de la grille | `LOC_ECOLE` | 1 min | — | `CLU_AFFICHE_COMMUNE` |
| Parler à la directrice | `LOC_ECOLE` | 3 min | — | `CLU_TEMOIN_DIRECTRICE` (aucune « Sandrine » parmi les animatrices ; Lila rentre seule depuis la rentrée) |
| Témoignage Casteran | `LOC_ETUDE_CASTERAN` | 3 min | après 16 h 45 | `CLU_TEMOIN_CASTERAN` |
| Test de ligne de vue | `LOC_ETUDE_CASTERAN` | 2 min | `CLU_TEMOIN_CASTERAN` | `CLU_LIGNE_DE_VUE` (PZ_03) |
| Convaincre Inès | `LOC_SKATEPARK` | 4 min | — | `CLU_VIDEO_INES` (PZ_04 partiel) |
| Pistage : piste de Lila | `LOC_COIN_ECOLES` | 5 min | `CLU_PORTE_CLES_LILA` | `CLU_PISTE_LILA`, `CLU_BRACELET_LILA` (PZ_01) |
| Pistage : objet de la femme | `LOC_ABRIBUS` → haie | 5 min | `CLU_BOUTEILLE` | `CLU_BADGE` (PZ_02) |
| Transmettre la plaque | `LOC_POSTE` | 5 min (plaque complète) / 15 min (partielle) | `CLU_PHOTO_FOURGON` ou `CLU_VIDEO_INES` | `CLU_SIV_CLONE` (PZ_05) |
| Faire appeler la blanchisserie | `LOC_POSTE` | 10 min | `DED_LETTRAGE` établi (PZ_06, **facultatif** : dépend de l'aspect du fourgon) | `CLU_APPEL_BLANCHISSERIE` |
| Suggérer la vidéo du Relais du Lac | `LOC_POSTE` | 10 min | `CLU_VIDEO_INES` | `CLU_CCTV_RELAIS` |
| Arrivée de Julien (père) | `LOC_POSTE` | — | 17 h 30 (automatique) | `EVT_PERE_ARRIVE` |
| Arrivée de Nadia et Darrigade | `LOC_POSTE` | — | 17 h 35 (automatique) | `EVT_NADIA_ARRIVE`, `CLU_ARIANE_DARRIGADE` |
| Écouter Nadia | `LOC_POSTE` | 4 min | après 17 h 40 | `CLU_TEMOIN_NADIA` (facultatif) |
| Observer Julien | à ≤ 15 m de lui entre 17 h 40 et 17 h 55 | 0 | — | `CLU_PERE_REACTION` |
| Éloigner Darrigade (proposer la battue) | `LOC_POSTE` | 1 min | après 17 h 35 | `FLAG_DARRIGADE_ELOIGNE` |
| Parler à Julien | `LOC_POSTE` | 5 min | après 17 h 40, Darrigade éloigné | `CLU_MESSAGE_CHANTAGE`, `CLU_PHOTO_VIE` si réussi (PZ_08) |
| Présenter le tableau | `LOC_POSTE` | 5 min | au moins une hypothèse par axe | Résolution ou branche d'erreur (PZ_10) |

### 4.4 Événements automatiques

| Heure | Événement | Effet |
|---|---|---|
| 16 h 45 | Casteran s'approche de la scène, parle aux passants | Devient interrogeable |
| 17 h 15 | Radio : un agent du péage de l'A63 signale un fourgon « correspondant au signalement » à 16 h 50, sans plaque relevée | `CLU_SIGNALEMENT_PEAGE` (fausse piste loyale, PZ_07) |
| 17 h 30 | Julien, le père, arrive de Bayonne en tenue de travail | `EVT_PERE_ARRIVE` |
| 17 h 35 | Nadia arrive, effondrée, avec son compagnon Xavier Darrigade ; Ariane garde ses distances avec lui | `EVT_NADIA_ARRIVE`, `CLU_ARIANE_DARRIGADE` |
| 17 h 40 | Julien reçoit un message, s'isole près de la fontaine | Observable (`CLU_PERE_REACTION`) |
| 18 h 05 | Darrigade s'éloigne pour « quelques coups de fil pour la battue » (plus tôt si {HEROINE} l'y envoie) | `EVT_DARRIGADE_S_ELOIGNE` |
| 18 h 30 | Si le badge n'a pas été trouvé, les gendarmes fouillent la haie et le trouvent | `CLU_BADGE` (rattrapage, coût nul pour le joueur mais pas de bonus) |
| 18 h 45 | Si Julien n'a pas parlé, il montre le message aux gendarmes | `CLU_MESSAGE_CHANTAGE`, `CLU_PHOTO_VIE` (rattrapage) |
| 19 h 00 | Si la vidéo d'Inès n'a pas été obtenue, ses parents l'amènent au poste | `CLU_VIDEO_INES` (rattrapage) |
| 19 h 30 | Clôture | Voir §6 |

Grâce à ces rattrapages, **toutes les preuves obligatoires deviennent accessibles avant 19 h 30**, quelles que soient les décisions du joueur.

---

## 5. Branches

Chaque branche : déclencheur · effet sur le temps · état de mission · nouveaux indices · récupération.

| ID | Déclencheur | Temps | État | Nouveaux indices | Récupération |
|---|---|---|---|---|---|
| `BR_APPEL_TARDIF` | Le joueur fait autre chose avant d'appeler le 17 | gendarmes = appel + 13 min | `FLAG_APPEL_TARDIF` ; Mendiondo le lui reproche (`DLG_C1_MENDIONDO_REPROCHE`) | — | Aucune perte d'indice |
| `BR_PISTE_MAUVAIS_OBJET` | Pistage lancé avec les mégots ou le porte-clés au lieu de la bouteille (PZ_02) | +5 min | — | Mégots → piste courte vers l'accotement (le conducteur n'est pas sorti) ; porte-clés → `CLU_PISTE_LILA` | L'indice de la chienne (regard vers le banc) et Lartigue orientent vers la bouteille |
| `BR_PERE_BRUSQUE` | La joueuse accuse Julien ou lui parle en présence de Darrigade | +5 min | `FLAG_PERE_FERME` ; il se ferme | — | Rattrapage automatique à 18 h 45 |
| `BR_PERE_CONFIANCE` | PZ_08 réussi | — | `FLAG_PERE_ALLIE` | `CLU_MESSAGE_CHANTAGE`, `CLU_PHOTO_VIE` | — |
| `BR_ERR_NORD` | Tableau présenté avec destination « Corniche nord » | +25 min | option exclue | `CLU_NEG_NORD` (patrouille : rien ; aucun témoin sur la corniche) | Ré-présenter le tableau |
| `BR_ERR_A63` | Destination « A63 / Espagne » | +30 min | option exclue | `CLU_NEG_A63` (aucun fourgon de ce modèle au péage entre 16 h 45 et 17 h 30) | Ré-présenter |
| `BR_ERR_PORT` | Destination « port de Capbreton » | +20 min | option exclue | `CLU_NEG_PORT` (capitainerie : aucun fourgon ; caisses livrées par le poissonnier) | Ré-présenter |
| `BR_ERR_VEHICULE` | Véhicule « fourgon du plombier » ou « fourgon actuel de la blanchisserie » | +15 min | option exclue | `CLU_NEG_VEHICULE` (vérification géolocalisation / planning) | Ré-présenter |
| `BR_ERR_PERSONNE` | Passagère « véritable animatrice » | +10 min | option exclue | `CLU_NEG_ANIMATRICE` (mairie : aucune Sandrine) | Ré-présenter |
| `BR_RESOLU` | Les trois axes corrects | — | fin de chapitre | — | — |

Une option exclue ne peut plus être choisie. Si plusieurs axes sont faux lors d'une même présentation, les vérifications se font en parallèle : seule la pénalité la plus longue s'applique. Les pénalités incluent le temps de la nouvelle présentation.

**Garantie vérifiée par le validateur** : un joueur qui suit le parcours de référence (`GameData/missions/01/mission.json`, sans aucun indice du prologue) conclut vers 17 h 22 ; même s'il essaie ensuite, une par une, toutes les mauvaises options, il termine avant 19 h 30. Au-delà, la clôture de 19 h 30 applique l'état C.

---

## 6. Résolution et états de départ du chapitre 2

**Vocabulaire** : une **déduction** `DED_` est établie dans le carnet quand le joueur relie les bons indices ; une **hypothèse** `H_` est ce qu'il **présente** sur le tableau. Les états de fin et les bonus dépendent de ce qui est présenté (`H_`) ; une hypothèse facultative ne peut être présentée que si sa déduction est établie. Le validateur refuse la confusion des deux.

À la présentation correcte, Mendiondo appelle le parquet ; le **dispositif Alerte Enlèvement** est demandé avec la description du fourgon, et un peloton est envoyé vers l'étang de Sorbe. {HEROINE} et Ariane partent avec les gendarmes pour la première approche (`DLG_C1_FIN_*`).

| État | Condition | Chapitre 2 commence avec |
|---|---|---|
| **A — Avance** | Résolu avant 18 h 15 **et** hypothèse facultative `H_DEST_RIVE_EST` présentée sur le tableau (possible seulement si la déduction `DED_RIVE_EST` est établie) | Crépuscule ; 2 airiaux sur 3 à vérifier ; traces du transfert très fraîches ; Loubère surpris en train de brûler des papiers (un document à moitié sauvé, utile au chapitre 3) |
| **B — Standard** | Résolu avant 19 h 00, ou sans `H_DEST_RIVE_EST` présentée | Nuit tombante ; 3 airiaux à vérifier ; traces du transfert lisibles |
| **C — Retard** | Résolu après 19 h 00 ou clôture automatique à 19 h 30 | Nuit noire, pluie fine à partir de 23 h (piste d'Ariane dégradée) ; toute la rive à couvrir ; Loubère interpellé plus tard, traces pauvres |

**Quel que soit l'état, Lila n'est plus à l'airial** (décision d'Audrey : elle est retrouvée au chapitre 4). Darrigade apprend la destination en même temps que Nadia (`DLG_C1_FIN_05` / `DLG_C1_CLOTURE_02`) et fait déplacer Lila environ une heure avant l'arrivée des gendarmes. Ce que l'état change, c'est la qualité des traces laissées par ce transfert, donc les indices de départ des chapitres 3 et 4.

Bonus indépendants : `CLU_CHIENNE_IMPREGNEE` (la chienne réagit à l'odeur de « Sandrine » au chapitre 2) ; `H_K1_LOUBERE` présentée (exige `DED_K1_LOUBERE` ; les gendarmes savent qui est sur place, négociation possible) ; `FLAG_PERE_ALLIE` (Julien accepte plus vite de coopérer au chapitre 3).

Clôture automatique à 19 h 30 : si un axe n'est pas établi, Mendiondo retient l'hypothèse la mieux étayée par les preuves effectivement réunies (ordre de priorité défini dans `GameData/missions/01/hypotheses.json`) ; les preuves obligatoires étant garanties par les rattrapages, la destination correcte est toujours retenue.

---

## 7. Indépendance vis-à-vis des caméras et des visuels ouverts

| Moment | Vérification |
|---|---|
| Coucou de Lila (plan A) | Lila repérable 3 s au portail depuis la place, dans les trois vues, quelle que soit l'animation de la place (choix ouvert). Facultatif pour l'enquête |
| Alerte et départ (plan B) | Depuis l'entrée de la ruelle : Lila, la femme, la portière et la **plaque arrière** visibles dans les trois vues, sans obstacle ; la plaque reste lisible 4 s pendant le départ. Indépendant de la couleur, du modèle et du lettrage du fourgon |
| Badge qui tombe (`ACT_CRIER`) | Portière et haie du même côté que la joueuse, pour que la chute soit visible à 20–30 m ; si ce n'est pas lisible dans une vue, Ariane regarde la haie et la fouille des gendarmes (18 h 30) sert de repli |
| Ligne de vue Casteran (PZ_03) | Depuis le perron, à hauteur d'yeux : la ruelle est visible jusqu'à son débouché, mais **pas** le boulevard ni le rond-point. En `CAM_WIDE`, la caméra descend à hauteur d'yeux pendant le test |
| Vidéo d'Inès, photo de vie | Consultées en `CAM_INSPECT` (plein écran), indépendantes de la vue |
| Pistages | Langage corporel d'Ariane lisible dans les trois vues ; en `CAM_FIRST`, Ariane est gardée dans le champ par un léger recentrage |
| Visuels ouverts | Aucun indice indispensable ne dépend de l'apparence du fourgon, de l'animation de la place ni de l'apparence de Lila : contrôle automatique `check_open_visuals` du validateur |

### 7.1 Indices à risque avec le nouveau cadrage (réponse à la demande de Codex)

| Indice | Risque | Parade proposée | Indispensable ? |
|---|---|---|---|
| `CLU_PHOTO_FOURGON` (plaque) | Fourgon vu de trois quarts ou masqué par une clôture/haie de la ruelle | Fourgon garé dans le sens de la descente, arrière vers l'entrée ; aucun obstacle haut entre l'entrée et l'arrière du fourgon | Non (la vidéo d'Inès suffit) |
| `CLU_OBS_BADGE_CHUTE` | Portière côté haie invisible depuis l'entrée si la joueuse est sur le trottoir opposé | Portière et haie du côté du trottoir d'arrivée ; repli : regard d'Ariane, fouille à 18 h 30 | Non |
| `CLU_OBS_CONDUCTEUR` | Le fourgon s'éloigne de la joueuse : le conducteur ne se voit que de dos | Le fourgon déboîte du bas-côté (profil visible 1 à 2 s par la vitre) ; repli : témoignage de Dufau | Non |
| `CLU_OBS_PASSAGERE` | Visage à 25 m, peu lisible | On ne demande qu'une silhouette et une tenue, pas un visage détaillé ; repli : Lartigue | Non |
| Lettrage fantôme (`DED_LETTRAGE`) | Dépend de l'aspect du fourgon, encore ouvert | Devenu **facultatif** (bonus : origine du fourgon, puis Loubère) | Non |
| `EVT_LILA_COUCOU` | Place très animée : Lila perdue dans la foule | Contrainte « repérable 3 s » ; purement narratif | Non |
| Perron de Casteran | La ruelle n'est plus dans l'axe de la place | Demande à Codex : depuis le perron, voir l'intérieur de la ruelle jusqu'au débouché (`REQ_RUELLE_PERRON`) ; repli : la vidéo d'Inès contredit Casteran | Non |

## 8. Tableau des besoins audiovisuels (pour Codex)

Aucun fichier média n'est créé ici. IDs de décor et personnages alignés sur `DIRECTION_VISUELLE.md` quand ils existent.

| Type | ID | Description | Mission / moment | Priorité |
|---|---|---|---|---|
| Lieu | `ENV_PROMENADE` | Rue piétonne du centre-ville, square avec banc de Dufau (plan A) | P0–P3 | Haute |
| Lieu | `ENV_ECOLE` | Place de l'école (plan A) : portail, affiche de la commune (nouveau logo), abribus, plan touristique ; boulangerie avec tableau de liège. Niveau d'animation : choix ouvert d'Audrey | P0, ch.1 | Haute |
| Lieu | `ENV_RUE_FUITE` | **Ruelle des Tamaris (plan B, distinct de la place)** ✅ ambiance validée : maisons variées d'époques différentes, couleurs, clôtures et haies différentes ; orientée vers l'océan. Proposition : bas-côté sablonneux, haie de pittosporum côté trottoir d'arrivée, débouché lumineux sur le boulevard (hors champ) | P1–P3, ch.1 | Haute |
| Lieu | `ENV_PLACE_EGLISE` *(nouveau)* | Place de l'Église, étude notariale avec perron, pharmacie à auvent, platane, fontaine, véhicule de commandement | ch.1 | Haute |
| Lieu | `ENV_ROND_POINT` *(nouveau)* | Rond-point à trois sorties panneautées + butte du skatepark | ch.1 | Moyenne |
| Personnage | `CHAR_FILLETTE` | Lila, 9 ans : cartable à porte-clés renard, bracelet en perles (nécessaires aux énigmes). **Tenue et cheveux non validés** : à proposer, puis identiques dans toutes les scènes | P0–P2, photo ch.1, ch.4 | Haute |
| Personnage | `CHAR_RAVISSEURS` | K1 : ≈ 45 ans, casquette grise, barbe courte grisonnante, veste de travail grise. K2 : ≈ 40 ans, carré blond, lunettes, gilet bleu marine, cordon bleu avec badge | P2 | Haute |
| Personnage | `CHAR_DUFAU` *(nouveau)* | Retraité, ancien pêcheur, bob, journal de mots croisés et stylo, voix râpeuse | P0, ch.1 | Moyenne |
| Personnage | `CHAR_INES` *(nouveau)* | Ado, skate, téléphone | ch.1 | Moyenne |
| Personnage | `CHAR_LARTIGUE` *(nouveau)* | Boulangère, tablier | ch.1 | Moyenne |
| Personnage | `CHAR_CASTERAN` *(nouveau)* | Notaire, la soixantaine, élégante, foulard, ton posé | ch.1 | Moyenne |
| Personnage | `CHAR_NADIA` *(nouveau)* | Mère, trentaine, aide-soignante (tenue de ville), téléphone ; ne sait rien | ch.1 | Haute |
| Personnage | `CHAR_JULIEN` *(nouveau)* | Père, trentaine, chauffeur : tenue de travail d'entrepôt sans logo réel, téléphone | ch.1 | Haute |
| Personnage | `CHAR_DARRIGADE` *(nouveau)* | Compagnon de Nadia, la quarantaine, soigné, veste sobre, gestes calmes et rassurants ; rien de menaçant à l'image | ch.1 | Haute |
| Personnage | `CHAR_MENDIONDO` *(nouveau)* | Adjudante-cheffe, uniforme de gendarmerie, quarantaine | ch.1 | Haute |
| Véhicule | `VEH_FOURGON` | **Apparence au choix d'Audrey pendant la construction 3D** (couleur, modèle, lettrage éventuel). Seules exigences : plaque arrière GF-437-TR lisible (terre sur le 3 pour la photo), pot qui cogne et tremble, portière latérale coulissante. Facultatif : ombre de lettrage « BLANCHISSERIE OCÉANE », feu arrière droit fendu | P2, vidéo, CCTV | Haute |
| Accessoire | `PROP_BADGE` | Badge plastifié « Accueil périscolaire — Commune de Lescoure — Sandrine V. », **ancien** logo | ch.1 | Haute |
| Accessoire | `PROP_FLYER` | Prospectus de la blanchisserie avec téléphone et photo de l'équipe de livraison (K1 identifiable) | ch.1 | Haute |
| Accessoire | `PROP_PORTE_CLES`, `PROP_BRACELET`, `PROP_BOUTEILLE`, `PROP_MEGOTS`, `PROP_CAISSE` | Objets au sol, examinables | ch.1 | Haute |
| Image | `IMG_PHOTO_FOURGON` / `IMG_PHOTO_FLOUE` | Rendu dans le moteur au moment de la photo | P2 | Haute |
| Vidéo | `VID_INES_ROND_POINT` | Vidéo verticale 12 s, horodatée 16:31:10, skate au premier plan ; le fourgon arrive du boulevard, prend la sortie « Étangs », plaque arrière lisible « 37-TR » ; un camping-car à porte-vélos part vers la Corniche | ch.1 | Haute |
| Vidéo | `VID_CCTV_RELAIS` | Caméra de station, 16:40, fourgon dont la plaque finit par « 37-TR » | ch.1 | Moyenne |
| Image | `IMG_PHOTO_VIE` | Lila assise, calme mais inquiète, couverture sur les épaules ; derrière elle, fenêtre sur l'étang, soleil bas dans l'axe, ponton, pins dont un porte un pot à résine. **Aucune marque de violence** | ch.1 | Haute |
| Animation | `ANIM_CHIENNE_*` | Ariane libre, sans laisse : envoi et rappel à la voix, flair au sol, tête haute, cercles/hésitation, marquage assis + regard, arrêt au bord de la route, grognement retenu | P2, pistages | Haute |
| Animation | `ANIM_K2_*` | Se pencher vers l'enfant, main tendue, presser, se retourner surprise | P1–P2 | Haute |
| Animation | `ANIM_NADIA_*` | Arrivée en courant, s'effondrer, parler en cherchant ses mots | ch.1 | Haute |
| Animation | `ANIM_JULIEN_*` | Arrivée, lire le téléphone et le cacher, se raidir sous une main posée sur l'épaule, regard fuyant vers Darrigade | ch.1 | Haute |
| Animation | `ANIM_DARRIGADE_*` | Consoler, main sur l'épaule, téléphoner en marchant près du poste, tendre la main à Ariane | ch.1 | Haute |
| Animation | `ANIM_CHIENNE_DISTANCE` | Ariane s'arrête à distance, oreilles plaquées, détourne la tête d'une main tendue (sans grogner) | ch.1 | Haute |
| Ambiance | `AMB_CENTRE_VILLE`, `AMB_SORTIE_ECOLE`, `AMB_PLACE` | Vent dans les pins, oiseaux urbains, cris d'enfants, cloches, circulation légère (mer visible au loin mais ressac non audible en P0–P3 : proposition) ; ambiance qui se vide après l'enlèvement | tout | Haute |
| Effet | `SFX_POT_ECHAPPEMENT` | Cognement métallique caractéristique (entendu en P0, P2, dans la vidéo d'Inès) — **doublé visuellement** (pot qui vibre) pour rester accessible sans le son | P0–P2 | Haute |
| Effet | `SFX_PORTIERE`, `SFX_SONNERIE_ECOLE`, `SFX_NOTIF_TELEPHONE` | — | P1–ch.1 | Moyenne |
| Voix | `VO_*` | Toutes les répliques de `../dialogues/01-ouverture.md` | — | Haute |
| Musique | `MUS_TENSION_01` | Montée progressive après `EVT_DEPART`, en retrait pendant les dialogues | ch.1 | Moyenne |
