# Mission 01 — Prologue « Sortie d'école » et Chapitre 1 « Le fourgon blanc »

> **Statut : proposition non canonique**, à valider par Audrey.
> Documents liés : énigmes `01-enigmes.md` · preuves `01-preuves.md` · répliques `../dialogues/01-ouverture.md` · données `../../GameData/missions/01/`.
> Les identifiants (`LOC_`, `CLU_`, `EVT_`, `ACT_`, `BR_`, `H_`, `DED_`, `PZ_`, `DLG_`) sont stables et partagés avec les données.
>
> **Décisions d'Audrey appliquées (29/09)** : centre-ville côtier inspiré du centre d'Arcachon, lumière chaude de fin d'après-midi ; **aucun port ni front de mer visible dans P0–P3** ; la chienne s'appelle **Ariane**, elle est libre dès le départ (ni laisse ni collier), foulard noir à motifs blancs ; après le départ du fourgon, Ariane sent un objet et mène à un indice. Tout le reste de ce document est une **proposition** (voir `docs/VALIDATION_AUDREY_ACTE1.md` et `docs/QUESTIONS_AUDREY.md`).

---

## 0. En une page (à lire en premier)

1. **16 h 33.** Le joueur prend la main dans la rue piétonne du centre-ville avec Ariane, qui marche librement à ses côtés. Il apprend à marcher, changer de vue et donner des ordres pendant une courte promenade où l'on croise Marcel Dufau sur son banc et où Lila, sortant de l'école, fait coucou à Ariane.
2. **16 h 36–16 h 38.** Une femme au badge d'animatrice aborde Lila au coin de la rue. Rien d'alarmant… jusqu'à ce que Lila hésite devant un fourgon blanc et appelle la chienne. Le joueur dispose d'une **fenêtre d'une dizaine de secondes** : photographier, courir, crier le prénom de Lila, envoyer Ariane (« Ariane, va ! »). Il peut en combiner deux. **Le fourgon part toujours** — mais ce que le joueur a fait détermine ce qu'il sait.
3. **16 h 39 → 19 h 30.** Chapitre 1. Appel au 17, arrivée des gendarmes, témoins qui se contredisent, arrivée de la mère qui cache un message. Chaque action consomme du temps de jeu. Le joueur doit établir trois conclusions distinctes sur un **tableau d'hypothèses** : le **véhicule**, les **personnes**, la **destination**.
4. **Résolution.** Quand le tableau est présenté à l'adjudante-cheffe Mendiondo, les gendarmes déclenchent le dispositif. Une bonne déduction rapide donne un avantage au chapitre 2 ; une erreur coûte du temps et produit un résultat négatif qui relance l'enquête. **Aucun échec définitif.**

---

## 1. Espace de jeu

### 1.1 Plan schématique (aligné sur la maquette de la PR #3)

Coordonnées de la maquette glTF de Codex : mètres, X vers l'est, Z vers le nord, portail de l'école à l'origine. Les distances sont des ordres de grandeur ; c'est la maquette qui fait foi pour les lignes de vue.

```
                               N
                               ▲
      Z_TEMOINS (-25,-18)        Z_PLACE (-8,-8)        Z_ECOLE (0,0)
      boulangerie Lartigue       place de l'Église      grille de l'école des Pins
      banc de Dufau (square)     étude Casteran,        abribus + plan touristique
                                 pharmacie à auvent,
                                 platane, fontaine
                                          Z_CROISEMENT (26,-6)
                  Z_PROMENADE (8,-19)     angle rue des Écoles / rue des Tamaris
                  départ du duo           (abordage, porte-clés de Lila)
                  rue piétonne                     │
                                          Z_RUE_FUITE (29,-11)
                                          stationnement + haie, rue des Tamaris
                                          ──► courbe ──► rond-point est (≈ 300 m, hors maquette)
                                                         sorties : Corniche (N), Étangs (E), D652 (S)
```

La mer et le port existent (plage à l'ouest, port au sud) mais **ne sont ni visibles ni audibles** dans P0–P3 : pas de ressac dans l'ambiance de l'ouverture.

### 1.2 Lieux

| ID | Lieu | Zone (PR #3) | Fonction | Accessible |
|---|---|---|---|---|
| `LOC_PROMENADE` | Rue piétonne du centre-ville | `Z_PROMENADE` | Départ, tutoriel, vue sur le croisement et le fourgon (≈ 20–25 m) | Toujours |
| `LOC_BANC_DUFAU` | Banc de Dufau, square des Tamaris | `Z_TEMOINS` | Témoin Dufau ; vue de biais sur le fourgon (≈ 55 m) | Toujours |
| `LOC_COIN_ECOLES` | Angle rue des Écoles / rue des Tamaris | `Z_CROISEMENT` | Lieu de l'abordage ; porte-clés de Lila (objet senti par Ariane) | Toujours (après l'événement, zone balisée mais examinable) |
| `LOC_ACCOTEMENT` | Stationnement sablonneux et haie, rue des Tamaris | `Z_RUE_FUITE` | Stationnement du fourgon ; traces, mégots, barrette, badge | Toujours |
| `LOC_ABRIBUS` | Abribus et banc, rue des Écoles | `Z_ECOLE` | Bouteille d'eau de « Sandrine » ; plan touristique | Toujours |
| `LOC_BOULANGERIE` | Boulangerie Lartigue | `Z_TEMOINS` | Témoin ; tableau de liège avec le prospectus de la blanchisserie | Toujours (ouverte jusqu'à 19 h 30) |
| `LOC_ECOLE` | Grille de l'école des Pins | `Z_ECOLE` | Affiche de la commune (nouveau logo) ; directrice | Toujours |
| `LOC_ETUDE_CASTERAN` | Perron de l'étude notariale | `Z_PLACE` | Témoin Casteran ; test de ligne de vue | Toujours |
| `LOC_SKATEPARK` | Butte du skatepark | `Z_RUE_FUITE` (prolongement) | Témoin Inès ; vue plongeante sur le rond-point | Toujours |
| `LOC_ROND_POINT` | Rond-point est | `Z_RUE_FUITE` (prolongement) | Sorties : Corniche (N), Étangs (E), D652 (S) | Toujours |
| `LOC_POSTE` | Véhicule de commandement des gendarmes, place de l'Église | `Z_PLACE` | Remise des pistes, tableau d'hypothèses, résultats | À partir de 16 h 52 |

**Limites du prologue avant l'événement** : toute la maquette P0–P3 est accessible, sans palissade ni mur invisible. Si le joueur s'approche trop tôt du couple, la règle d'accélération (§2) s'applique.

### 1.3 Distances et temps (utilisés par l'énigme de chronométrie)

| Trajet | Distance | Temps en fourgon (circulation fluide, fin d'après-midi) |
|---|---|---|
| Accotement → rond-point | 300 m | 1 min |
| Rond-point → Relais du Lac (route des Étangs) | 9 km | 9 min |
| Rond-point → péage A63 (via D652) | 32 km | 28 min |
| Rond-point → Lescoure-Nord (Corniche) | 11 km | 12 min |
| Rond-point → port de Capbreton (via D652) | 24 km | 22 min |
| Relais du Lac → rive est de l'étang de Sorbe | 5 km | 7 min (piste) |

Ces valeurs figurent sur le **plan d'information touristique** près de l'abribus (`CLU_PLAN_DISTANCES`) et sur la carte du poste de commandement.

---

## 2. Chronologie visible (ce que le joueur peut constater)

La chronologie cachée complète est dans `BIBLE.md` §5. Ici, uniquement ce qui se produit dans l'espace de jeu.

| Heure | Événement | ID |
|---|---|---|
| 16 h 33 | Prise en main dans la rue piétonne, Ariane libre à côté de {HEROINE} | `EVT_START` |
| 16 h 34 | Sonnerie ; enfants à la grille (audible, visible au loin) | `EVT_SONNERIE` |
| 16 h 35 | Lila descend la rue des Écoles, fait coucou à Ariane depuis le trottoir d'en face | `EVT_LILA_COUCOU` |
| 16 h 36 | Une femme au cordon bleu aborde Lila au coin, lui parle, lui prend la main | `EVT_ABORDAGE` |
| 16 h 37 | Elles marchent jusqu'au fourgon garé sur l'accotement | `EVT_MARCHE_FOURGON` |
| 16 h 37 + 50 s | **Alerte** : Lila s'arrête devant la portière, se retourne, appelle « Ariane ! » ; Ariane grogne | `EVT_ALERTE` |
| + 8 à 12 s | La femme fait monter Lila, portière claquée, départ | `EVT_DEPART` |
| + 6 s | Le fourgon disparaît dans la courbe vers le rond-point | `EVT_HORS_VUE` |
| 16 h 39 | Fin du prologue, début du chapitre 1 (commande « Appeler le 17 » mise en avant) | `EVT_CH1_START` |

**Règle d'accélération** : si {HEROINE} ou Ariane s'approche à moins de 12 m du couple avant `EVT_ALERTE`, la femme presse Lila ; `EVT_ALERTE` et `EVT_DEPART` sont avancés et le départ se produit quand le duo est à ≈ 8 m. Ariane, libre, reste dans un rayon de 3 m autour de {HEROINE} tant qu'elle n'a pas reçu d'ordre d'envoi. L'issue est fixe, l'angle d'observation varie.

---

## 3. Prologue jouable

### 3.1 Séquence P0 — Balade (16 h 33 – 16 h 36)

Objectifs doux (facultatifs, affichés comme suggestions) :
- marcher, trotter, changer de vue (`CAM_SHOULDER`, `CAM_WIDE`, `CAM_FIRST`) ;
- donner à Ariane les ordres « Au pied », « Reste », « Cherche » (balle lancée dans le square), « Montre », « Va ! » ; Ariane est libre, sans laisse ni collier, et revient au rappel ;
- saluer Dufau sur son banc du square (`DLG_P_DUFAU_*`) : il grommelle à propos du fourgon « qui fait un boucan de casserole » et qui est garé là « depuis une demi-heure » — **première graine**, sans insistance ;
- voir Lila faire coucou (`EVT_LILA_COUCOU`) ; Ariane remue la queue. Si le joueur répond (geste), Lila sourit (`DLG_P_LILA_01`).

Temps : réel, compressé. Si le joueur reste immobile, l'horloge avance quand même jusqu'à `EVT_ABORDAGE` (au plus 3 min réelles).

### 3.2 Séquence P1 — L'abordage (16 h 36 – alerte)

Rien d'explicitement suspect : une adulte avec un badge raccompagne une enfant. Détails observables pour un joueur attentif :
- Lila ralentit, regarde autour d'elle ; la femme se penche, parle, montre son badge (`OBS` non collecté automatiquement — il sera confirmé par la piste de Lila, énigme PZ_01) ;
- Ariane fixe la rue, oreille droite dressée : **signal d'intérêt**, aucun texte ne l'explique.

### 3.3 Séquence P2 — La fenêtre d'action (alerte → hors de vue)

Au moment `EVT_ALERTE`, une **invite contextuelle discrète** apparaît (pas de ralenti imposé, pas d'écran figé). Actions possibles :

| ID action | Commande | Durée | Effet sur les indices | Compatible avec |
|---|---|---|---|---|
| `ACT_PHOTO` | Lever le téléphone et photographier | 2 s | Immobile : `CLU_PHOTO_FOURGON` (plaque partielle « GF-4·7 », lettrage fantôme, fragment de numéro). En course : `CLU_PHOTO_FLOUE` (lettrage seul) | `ACT_CRIER`, `ACT_ENVOYER` ; en course = floue |
| `ACT_COURIR` | Sprinter vers le fourgon | continu | Arrivée à ≈ 25 m au départ : `CLU_OBS_CONDUCTEUR`, `CLU_OBS_ECHAPPEMENT`, `CLU_OBS_FEU_FENDU` | `ACT_CRIER`, `ACT_PHOTO` (floue) |
| `ACT_CRIER` | Appeler « Lila ! » | 1 s | La femme se retourne : `CLU_OBS_PASSAGERE` (visage, tenue). Elle se précipite, le cordon s'accroche : `CLU_OBS_BADGE_CHUTE` (le joueur voit où il tombe) | tout |
| `ACT_ENVOYER` | Envoyer Ariane : « Ariane, va ! » (elle est déjà libre) | 1 s | Ariane fonce, aboie, **s'arrête net au bord de la chaussée** (éducation), flaire l'endroit où se tenait la femme : `CLU_CHIENNE_IMPREGNEE` ; elle regarde ensuite la haie (aide pour PZ_02) | `ACT_PHOTO`, `ACT_CRIER` |
| — | Ne rien faire / rester figé | — | Aucun indice de fenêtre ; tous les axes restent résolubles par les indices permanents | — |

Deux actions maximum sont réalistes dans la fenêtre. Le jeu n'impose aucune limite artificielle : c'est le temps qui limite.

**Photo exclusive** : `ACT_PHOTO` donne **soit** la photo nette (`CLU_PHOTO_FOURGON`, joueur immobile), **soit** la photo floue (`CLU_PHOTO_FLOUE`, si `ACT_COURIR` est combiné) — jamais les deux. Le validateur le vérifie.

**Accessibilité** : une option « Temps d'action étendu » (menu Accessibilité, désactivée par défaut) porte la fenêtre de 8–12 s à 20–25 s, ou met l'action en pause jusqu'au choix. Le nombre d'actions (deux au maximum) et leurs conséquences restent identiques.

**Sécurité d'Ariane** : aucune issue où Ariane est blessée. Libre, elle s'arrête toujours au bord de la chaussée, même sans ordre. Le rappel (« Au pied ! ») est instantané. Aucune branche ne suppose de laisse ni de collier.

**Toutes vues** : le fourgon, la plaque arrière et le lettrage sont orientés vers `Z_PROMENADE`, visibles en `CAM_SHOULDER`, `CAM_WIDE` et `CAM_FIRST`. Le visage de la femme n'est lisible qu'avec `ACT_CRIER` (elle se retourne) — c'est voulu et indépendant de la caméra.

### 3.4 Séquence P3 — Hors de vue (16 h 38 – 16 h 39)

Le fourgon disparaît dans la courbe. {HEROINE} reste sur place, souffle court (`DLG_P_HEROINE_CHOC_*`, variante selon les actions). Transition vers le chapitre 1 sans cinématique : l'interface affiche **« Appeler le 17 »** en action principale.

---

## 4. Chapitre 1 — structure

### 4.1 Système de temps

- L'horloge démarre à **16 h 39**. Elle n'avance **que** par les interactions (coûts ci-dessous) et les déplacements entre zones (1 min par zone traversée à pied ; 0 en restant dans une zone).
- L'horloge est visible dans le carnet. Aucun compte à rebours stressant à l'écran.
- **19 h 30** (coucher du soleil ≈ 19 h 45) : si le tableau n'a pas été présenté, Mendiondo convoque {HEROINE} et le chapitre se conclut avec les hypothèses les mieux étayées (voir §6, état C).

### 4.2 Phase A — Premiers gestes (16 h 39 – 16 h 52)

| Interaction | Coût | Obligatoire | Effet |
|---|---|---|---|
| Appel au 17 (`DLG_C1_OPERATRICE_*`) | 3 min | **Oui** (première action mise en avant ; possible de l'appeler plus tard, pénalité : gendarmes arrivent 5 min après l'appel) | Les options de description proposées dépendent des indices de fenêtre. Plus la description est précise, plus tôt la gendarmerie envoie des patrouilles sur les axes (effet narratif, pas mécanique) |
| Protéger la scène (dire aux passants de ne pas toucher le porte-clés) | 1 min | Non | Bonus de confiance avec Mendiondo (`FLAG_SCENE_PROTEGEE`) : elle partage un résultat de plus spontanément |
| Examiner le coin et l'accotement | 2 min | Non | `CLU_PORTE_CLES_LILA`, `CLU_TRACES_PNEUS`, `CLU_MEGOTS`, `CLU_CAISSE_POISSON` ; si `CLU_OBS_BADGE_CHUTE`, `CLU_BADGE` directement dans la haie |

Arrivée des gendarmes : **16 h 52** (ou appel + 13 min). Mendiondo recueille la déposition (`DLG_C1_MENDIONDO_ARRIVEE_*`), installe le poste de commandement place de l'Église et **autorise {HEROINE} à rester comme témoin** : l'équipe cynophile de la gendarmerie est à plus de deux heures.

### 4.3 Phase B — Enquête ouverte (16 h 52 – résolution)

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
| Pistage : piste de Lila | `LOC_COIN_ECOLES` | 5 min | `CLU_PORTE_CLES_LILA` | `CLU_PISTE_LILA`, `CLU_BARRETTE_LILA` (PZ_01) |
| Pistage : objet de la femme | `LOC_ABRIBUS` → haie | 5 min | `CLU_BOUTEILLE` | `CLU_BADGE` (PZ_02) |
| Transmettre la plaque | `LOC_POSTE` | 5 min (plaque complète) / 15 min (partielle) | `CLU_PHOTO_FOURGON` ou `CLU_VIDEO_INES` | `CLU_SIV_CLONE` (PZ_05) |
| Faire appeler la blanchisserie | `LOC_POSTE` | 10 min | `DED_LETTRAGE` établi (PZ_06) | `CLU_APPEL_BLANCHISSERIE` |
| Suggérer la vidéo du Relais du Lac | `LOC_POSTE` | 10 min | `CLU_VIDEO_INES` | `CLU_CCTV_RELAIS` |
| Arrivée de Nadia | `LOC_POSTE` | — | 17 h 35 (automatique) | `EVT_NADIA_ARRIVE` |
| Observer Nadia | à ≤ 15 m d'elle entre 17 h 40 et 17 h 55 | 0 | — | `CLU_NADIA_REACTION` |
| Parler à Nadia | `LOC_POSTE` | 5 min | après 17 h 40 | `CLU_MESSAGE_CHANTAGE`, `CLU_PHOTO_VIE` si réussi (PZ_08) |
| Présenter le tableau | `LOC_POSTE` | 5 min | au moins une hypothèse par axe | Résolution ou branche d'erreur (PZ_09) |

### 4.4 Événements automatiques

| Heure | Événement | Effet |
|---|---|---|
| 16 h 45 | Casteran s'approche de la scène, parle aux passants | Devient interrogeable |
| 17 h 15 | Radio : un agent du péage de l'A63 signale un fourgon blanc à 16 h 58 | `CLU_SIGNALEMENT_PEAGE` (fausse piste loyale, PZ_07) |
| 17 h 35 | Nadia arrive de Bayonne, effondrée | `EVT_NADIA_ARRIVE` ; Casteran l'entoure |
| 17 h 40 | Nadia reçoit un message, s'isole près de la fontaine | Observable (`CLU_NADIA_REACTION`) |
| 18 h 05 | Casteran quitte Nadia pour « préparer du thé à l'étude » (plus tôt si {HEROINE} demande à Mendiondo de prendre sa déposition) | `EVT_CASTERAN_S_ELOIGNE` |
| 18 h 30 | Si le badge n'a pas été trouvé, les gendarmes fouillent la haie et le trouvent | `CLU_BADGE` (rattrapage, coût nul pour le joueur mais pas de bonus) |
| 18 h 45 | Si Nadia n'a pas parlé, elle craque et montre le message aux gendarmes | `CLU_MESSAGE_CHANTAGE`, `CLU_PHOTO_VIE` (rattrapage) |
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
| `BR_NADIA_BRUSQUEE` | Le joueur accuse Nadia ou révèle devant Casteran qu'elle cache quelque chose | +5 min | `FLAG_NADIA_FERMEE` ; elle se ferme | — | Rattrapage automatique à 18 h 45 |
| `BR_NADIA_CONFIANCE` | PZ_08 réussi | — | `FLAG_NADIA_ALLIEE` | `CLU_MESSAGE_CHANTAGE`, `CLU_PHOTO_VIE` | — |
| `BR_ERR_NORD` | Tableau présenté avec destination « Corniche nord » | +25 min | option exclue | `CLU_NEG_NORD` (patrouille : rien ; aucun témoin sur la corniche) | Ré-présenter le tableau |
| `BR_ERR_A63` | Destination « A63 / Espagne » | +30 min | option exclue | `CLU_NEG_A63` (aucun fourgon de ce modèle au péage entre 16 h 55 et 17 h 30) | Ré-présenter |
| `BR_ERR_PORT` | Destination « port de Capbreton » | +20 min | option exclue | `CLU_NEG_PORT` (capitainerie : aucun fourgon ; caisses livrées par le poissonnier) | Ré-présenter |
| `BR_ERR_VEHICULE` | Véhicule « fourgon du plombier » ou « fourgon actuel de la blanchisserie » | +15 min | option exclue | `CLU_NEG_VEHICULE` (vérification géolocalisation / planning) | Ré-présenter |
| `BR_ERR_PERSONNE` | Passagère « véritable animatrice » | +10 min | option exclue | `CLU_NEG_ANIMATRICE` (mairie : aucune Sandrine) | Ré-présenter |
| `BR_RESOLU` | Les trois axes corrects | — | fin de chapitre | — | — |

Une option exclue ne peut plus être choisie. Si plusieurs axes sont faux lors d'une même présentation, les vérifications se font en parallèle : seule la pénalité la plus longue s'applique. Les pénalités incluent le temps de la nouvelle présentation.

**Garantie vérifiée par le validateur** : un joueur qui suit le parcours de référence (`GameData/missions/01/mission.json`, sans aucun indice du prologue) conclut vers 17 h 30 ; même s'il essaie ensuite, une par une, toutes les mauvaises options, il termine avant 19 h 30. Au-delà, la clôture de 19 h 30 applique l'état C.

---

## 6. Résolution et états de départ du chapitre 2

**Vocabulaire** : une **déduction** `DED_` est établie dans le carnet quand le joueur relie les bons indices ; une **hypothèse** `H_` est ce qu'il **présente** sur le tableau. Les états de fin et les bonus dépendent de ce qui est présenté (`H_`) ; une hypothèse facultative ne peut être présentée que si sa déduction est établie. Le validateur refuse la confusion des deux.

À la présentation correcte, Mendiondo appelle le parquet ; le **dispositif Alerte Enlèvement** est demandé avec la description du fourgon, et un peloton est envoyé vers l'étang de Sorbe. {HEROINE} et Ariane partent avec les gendarmes pour la première approche (`DLG_C1_FIN_*`).

| État | Condition | Chapitre 2 commence avec |
|---|---|---|
| **A — Avance** | Résolu avant 18 h 15 **et** hypothèse facultative `H_DEST_RIVE_EST` présentée sur le tableau (possible seulement si la déduction `DED_RIVE_EST` est établie) | Crépuscule, traces de pneus fraîches sur la piste, recherche limitée à 2 airiaux sur 3 |
| **B — Standard** | Résolu avant 19 h 00, ou sans `H_DEST_RIVE_EST` présentée | Nuit tombante, 3 airiaux à vérifier |
| **C — Retard** | Résolu après 19 h 00 ou clôture automatique à 19 h 30 | Nuit noire, pluie fine à partir de 23 h (piste canine dégradée), toute la rive à couvrir |

Bonus indépendants : `CLU_CHIENNE_IMPREGNEE` (la chienne réagit à l'odeur de « Sandrine » au chapitre 2) ; `H_K1_LOUBERE` présentée (exige `DED_K1_LOUBERE` ; les gendarmes savent qui est sur place, négociation possible) ; `FLAG_NADIA_ALLIEE` (Nadia aide aux chapitres 2 et 3).

Clôture automatique à 19 h 30 : si un axe n'est pas établi, Mendiondo retient l'hypothèse la mieux étayée par les preuves effectivement réunies (ordre de priorité défini dans `GameData/missions/01/hypotheses.json`) ; les preuves obligatoires étant garanties par les rattrapages, la destination correcte est toujours retenue.

---

## 7. Indépendance vis-à-vis des caméras

| Moment | Vérification |
|---|---|
| Fourgon dans le prologue | Plaque et lettrage tournés vers `Z_PROMENADE`, lisibles à ≈ 25 m en `CAM_FIRST` avec zoom téléphone, et en `CAM_SHOULDER` / `CAM_WIDE` via la photo ; vérifiable dans la maquette par `Tools/check_opening_sightlines.py` (PR #3) |
| Ligne de vue Casteran (PZ_03) | Le test se fait en se plaçant sur le perron : une silhouette-repère (panneau du rond-point) est masquée par l'auvent et le platane quelle que soit la vue ; en `CAM_WIDE` la caméra se rapproche automatiquement de la hauteur des yeux pendant le test |
| Vidéo d'Inès, photo de vie | Consultées en `CAM_INSPECT` (plein écran), indépendantes de la vue |
| Pistage | Langage corporel d'Ariane lisible dans les trois vues ; en `CAM_FIRST`, Ariane est gardée dans le champ par un léger recentrage |

---

## 8. Tableau des besoins audiovisuels (pour Codex)

Aucun fichier média n'est créé ici. IDs de décor et personnages alignés sur `DIRECTION_VISUELLE.md` quand ils existent.

| Type | ID | Description | Mission / moment | Priorité |
|---|---|---|---|---|
| Lieu | `ENV_PROMENADE` | Rue piétonne du centre-ville, square des Tamaris avec banc de Dufau ; aucune vue sur la mer ou le port | P0–P3 | Haute |
| Lieu | `ENV_ECOLE` | Rue des Écoles, grille, affiche de la commune (nouveau logo), abribus, plan touristique, boulangerie avec tableau de liège | P1, ch.1 | Haute |
| Lieu | `ENV_RUE_FUITE` | Rue des Tamaris (ex-`ENV_RUE_PORT`), stationnement sablonneux, haie de pittosporum, courbe vers le rond-point | P2, ch.1 | Haute |
| Lieu | `ENV_PLACE_EGLISE` *(nouveau)* | Place de l'Église, étude notariale avec perron, pharmacie à auvent, platane, fontaine, véhicule de commandement | ch.1 | Haute |
| Lieu | `ENV_ROND_POINT` *(nouveau)* | Rond-point à trois sorties panneautées + butte du skatepark | ch.1 | Moyenne |
| Personnage | `CHAR_FILLETTE` | Lila, 9 ans : cartable à porte-clés renard, barrette à fleur | P1–P2 | Haute |
| Personnage | `CHAR_RAVISSEURS` | K1 : ≈ 45 ans, casquette grise, barbe courte grisonnante, veste de travail grise. K2 : ≈ 40 ans, carré blond, lunettes, gilet bleu marine, cordon bleu avec badge | P2 | Haute |
| Personnage | `CHAR_DUFAU` *(nouveau)* | Retraité, ancien pêcheur, bob, journal plié, voix râpeuse | P0, ch.1 | Moyenne |
| Personnage | `CHAR_INES` *(nouveau)* | Ado, skate, téléphone | ch.1 | Moyenne |
| Personnage | `CHAR_LARTIGUE` *(nouveau)* | Boulangère, tablier | ch.1 | Moyenne |
| Personnage | `CHAR_CASTERAN` *(nouveau)* | Notaire, la soixantaine, élégante, foulard, ton posé | ch.1 | Haute |
| Personnage | `CHAR_NADIA` *(nouveau)* | Mère, trentaine, tenue de travail (sans uniforme visible), téléphone | ch.1 | Haute |
| Personnage | `CHAR_MENDIONDO` *(nouveau)* | Adjudante-cheffe, uniforme de gendarmerie, quarantaine | ch.1 | Haute |
| Véhicule | `VEH_FOURGON` | Fourgon blanc sans marque réelle, ombre de lettrage « BLANCHISSERIE OCÉANE » sur le flanc et les portes arrière, plaque GF-437-TR (la terre masque le 3 sur la photo), feu arrière droit fendu, pot qui cogne | P2, vidéo, CCTV | Haute |
| Accessoire | `PROP_BADGE` | Badge plastifié « Accueil périscolaire — Commune de Lescoure — Sandrine V. », **ancien** logo | ch.1 | Haute |
| Accessoire | `PROP_FLYER` | Prospectus de la blanchisserie avec téléphone et photo de l'équipe de livraison (K1 identifiable) | ch.1 | Haute |
| Accessoire | `PROP_PORTE_CLES`, `PROP_BARRETTE`, `PROP_BOUTEILLE`, `PROP_MEGOTS`, `PROP_CAISSE` | Objets au sol, examinables | ch.1 | Haute |
| Image | `IMG_PHOTO_FOURGON` / `IMG_PHOTO_FLOUE` | Rendu dans le moteur au moment de la photo | P2 | Haute |
| Vidéo | `VID_INES_ROND_POINT` | Vidéo verticale 12 s, horodatée 16:39:10, camping-car blanc (toit haut, porte-vélos) sur la Corniche à 16:39:16, skate au premier plan, fourgon au rond-point sortie « Étangs » | ch.1 | Haute |
| Vidéo | `VID_CCTV_RELAIS` | Caméra de station, noir et blanc, 16:48, fourgon avec clignotant droit inactif | ch.1 | Moyenne |
| Image | `IMG_PHOTO_VIE` | Lila assise, calme mais inquiète, couverture sur les épaules ; derrière elle, fenêtre sur l'étang, soleil bas dans l'axe, ponton, pins dont un porte un pot à résine. **Aucune marque de violence** | ch.1 | Haute |
| Animation | `ANIM_CHIENNE_*` | Ariane libre, sans laisse : envoi et rappel à la voix, flair au sol, tête haute, cercles/hésitation, marquage assis + regard, arrêt au bord de la route, grognement retenu | P2, pistages | Haute |
| Animation | `ANIM_K2_*` | Se pencher vers l'enfant, main tendue, presser, se retourner surprise | P1–P2 | Haute |
| Animation | `ANIM_NADIA_*` | Arrivée en courant, s'isoler pour lire, cacher le téléphone, s'effondrer | ch.1 | Haute |
| Ambiance | `AMB_CENTRE_VILLE`, `AMB_SORTIE_ECOLE`, `AMB_PLACE` | Vent dans les pins, oiseaux urbains, cris d'enfants, cloches, circulation légère (pas de ressac en P0–P3) ; ambiance qui se vide après l'enlèvement | tout | Haute |
| Effet | `SFX_POT_ECHAPPEMENT` | Cognement métallique caractéristique (entendu en P0, P2, dans la vidéo d'Inès) — **doublé visuellement** (pot qui vibre) pour rester accessible sans le son | P0–P2 | Haute |
| Effet | `SFX_PORTIERE`, `SFX_SONNERIE_ECOLE`, `SFX_NOTIF_TELEPHONE` | — | P1–ch.1 | Moyenne |
| Voix | `VO_*` | Toutes les répliques de `../dialogues/01-ouverture.md` | — | Haute |
| Musique | `MUS_TENSION_01` | Montée progressive après `EVT_DEPART`, en retrait pendant les dialogues | ch.1 | Moyenne |
