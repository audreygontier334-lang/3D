# Mission 01 — Matrice des preuves et des déductions

> **Statut : proposition non canonique.** Source de vérité : `GameData/missions/01/deductions.json` et `hypotheses.json`. Ce document les explique ; en cas d'écart, les données font foi et le validateur signale les incohérences.

## Le principe en un exemple

Le joueur ne « résout » pas l'enlèvement d'un coup. Il répond à **trois questions distinctes**, chacune avec ses propres preuves :

| Question | Ce qu'il faut établir | Exemple de raisonnement |
|---|---|---|
| **Véhicule** | Quel fourgon, vraiment ? | La photo montre l'ombre d'un lettrage « BL·NCH·SS·RIE OC·A·· » ; le prospectus de la boulangerie dit « Blanchisserie Océane » ; Dufau jure que leurs camions portent le nom et ne passent que le mardi matin. → Ce n'est pas un fourgon actuel, c'est un **ancien** fourgon de la blanchisserie. Et la plaque renvoie à un plombier qui était à Dax au même moment → **plaque clonée**. |
| **Personnes** | Qui sont-ils ? | Le badge « Sandrine V. » porte l'ancien logo de la mairie ; la directrice n'a aucune Sandrine ; la boulangère l'a entendue demander « l'école de la petite Mercadier ». → **Une fausse animatrice qui savait qui elle venait chercher.** |
| **Destination** | Où sont-ils partis ? | La notaire affirme « vers la Corniche » mais ne pouvait pas voir le rond-point ; la vidéo d'Inès montre le fourgon sur la route des Étangs, le véhicule blanc de la Corniche étant un camping-car ; le fourgon « vu au péage » à 16 h 58 n'aurait pas eu le temps d'y arriver. → **Route des Étangs.** |

Aucun indice ne répond à deux questions à la fois de façon suffisante. La vidéo d'Inès, l'indice le plus riche, donne la direction, une fin de plaque et un bout de lettrage ; il faut encore le prospectus et le fichier des plaques pour conclure sur le véhicule, et le badge ou les témoins pour les personnes.

---

## 1. Déductions

Notation : **ET** dans une ligne, **OU** entre les lignes d'une même déduction.

| Déduction | Ensembles de preuves suffisants | Optionnel / renfort | Contradictions possibles | Conséquence narrative |
|---|---|---|---|---|
| `DED_SORTIE_ETANGS` — le fourgon a pris la route des Étangs | `CLU_VIDEO_INES` | `CLU_CCTV_RELAIS`, `CLU_TRACES_PNEUS` (parti vers l'est) | `CLU_TEMOIN_CASTERAN` (nord) — levée par `CLU_LIGNE_DE_VUE` ; camping-car de la vidéo | Oriente les recherches vers l'étang de Sorbe |
| `DED_PEAGE_EXCLU` — le fourgon du péage n'est pas le bon | `CLU_SIGNALEMENT_PEAGE` + `CLU_VIDEO_INES` + `CLU_PLAN_DISTANCES` · ou `CLU_SIGNALEMENT_PEAGE` + `CLU_CCTV_RELAIS` | — | Le signalement officiel lui-même | Évite 30 min perdues sur l'A63 |
| `DED_CASTERAN_NON_FIABLE` — la notaire ne pouvait pas voir | `CLU_TEMOIN_CASTERAN` + `CLU_LIGNE_DE_VUE` · ou `CLU_TEMOIN_CASTERAN` + `CLU_VIDEO_INES` | — | Respectabilité du témoin | Graine du chapitre 6 ; le carnet note « s'est trompée ou a supposé » |
| `DED_RIVE_EST` — Lila est sur la rive est | `CLU_PHOTO_VIE` + `CLU_CARTE_ETANG` | `CLU_MESSAGE_CHANTAGE` (heure) | Ponton (évoque la base nautique, rive ouest) | État A possible au chapitre 2 |
| `DED_LETTRAGE` — ancien fourgon de la Blanchisserie Océane | (`CLU_PHOTO_FOURGON` ou `CLU_PHOTO_FLOUE` ou `CLU_VIDEO_INES`) + `CLU_FLYER_BLANCHISSERIE` | `CLU_TEMOIN_DUFAU` (pas un fourgon actuel) | `CLU_FLYER_PRESSING` (numéro en …12) | Débloque l'appel à la blanchisserie |
| `DED_PLAQUE_CLONEE` — plaque copiée | `CLU_SIV_CLONE` (obtenu avec `CLU_PHOTO_FOURGON` et/ou `CLU_VIDEO_INES`) | `CLU_TEMOIN_DUFAU` (plaque 40) | Le plombier comme suspect — levé par sa géolocalisation | Montre qu'il s'agit de professionnels |
| `DED_FAUSSE_ANIMATRICE` — elle se faisait passer pour animatrice | `CLU_BADGE` + `CLU_AFFICHE_COMMUNE` · ou `CLU_TEMOIN_DIRECTRICE` · ou `CLU_BADGE` + `CLU_TEMOIN_LARTIGUE` | `CLU_OBS_PASSAGERE`, `DED_RUSE` | Le badge semble authentique au premier regard | Premier « faux-semblant » explicite |
| `DED_CIBLE` — elle savait qui elle venait chercher | `CLU_TEMOIN_LARTIGUE` · ou `CLU_MESSAGE_CHANTAGE` | `CLU_TEMOIN_DIRECTRICE` (peu de gens savaient que Lila rentrait seule) | Hypothèse d'un enlèvement au hasard | Ouvre la piste du chantage ; clé pour convaincre Nadia |
| `DED_RUSE` — Lila a été trompée, pas saisie | `CLU_PISTE_LILA` + `CLU_BARRETTE_LILA` | `DLG_P_ABORDAGE` entendu | Hypothèse `H_FORCE` si le joueur lit mal la chienne | Argument pour Nadia (PZ_08) |
| `DED_CHANTAGE` — pression sur Nadia | `CLU_MESSAGE_CHANTAGE` | `CLU_NADIA_REACTION` | Nadia paraît coupable | Enjeu du chapitre 3 (jeudi 6 h) |
| `DED_K1_LOUBERE` — le conducteur est probablement Loubère | `CLU_APPEL_BLANCHISSERIE` + `CLU_FLYER_BLANCHISSERIE` + (`CLU_TEMOIN_DUFAU` ou `CLU_OBS_CONDUCTEUR`) | — | Dufau lui-même doute | Bonus chapitre 2 (les gendarmes savent qui est sur place) |

## 2. Hypothèses du tableau (PZ_10)

| Axe | Hypothèse | Correcte | Requiert | Réfutée par | Si présentée à tort |
|---|---|---|---|---|---|
| Véhicule | `H_VEH_ANCIEN_BLANCHISSERIE` | ✔ | `DED_LETTRAGE` **et** `DED_PLAQUE_CLONEE` | — | — |
| Véhicule | `H_VEH_PLOMBIER` | ✘ | — | `CLU_SIV_CLONE` (géolocalisation), `CLU_NEG_VEHICULE` | `BR_ERR_VEHICULE` +15 min |
| Véhicule | `H_VEH_BLANCHISSERIE_ACTIVE` | ✘ | — | `CLU_TEMOIN_DUFAU`, `CLU_APPEL_BLANCHISSERIE`, `CLU_NEG_VEHICULE` | `BR_ERR_VEHICULE` +15 min |
| Personnes | `H_K2_FAUSSE_ANIMATRICE` | ✔ | `DED_FAUSSE_ANIMATRICE` **et** `DED_CIBLE` | — | — |
| Personnes | `H_K2_VRAIE_ANIMATRICE` | ✘ | — | `CLU_AFFICHE_COMMUNE`, `CLU_TEMOIN_DIRECTRICE`, `CLU_NEG_ANIMATRICE` | `BR_ERR_PERSONNE` +10 min |
| Personnes | `H_K1_LOUBERE` *(facultative)* | ✔ | `DED_K1_LOUBERE` | — | — |
| Destination | `H_DEST_ETANGS` | ✔ | `DED_SORTIE_ETANGS` | — | — |
| Destination | `H_DEST_NORD` | ✘ | — | `CLU_LIGNE_DE_VUE`, `CLU_VIDEO_INES`, `CLU_NEG_NORD` | `BR_ERR_NORD` +25 min |
| Destination | `H_DEST_A63` | ✘ | — | `CLU_VIDEO_INES`, `CLU_CCTV_RELAIS`, `CLU_NEG_A63` | `BR_ERR_A63` +30 min |
| Destination | `H_DEST_PORT` | ✘ | — | `CLU_TEMOIN_DUFAU`, `CLU_VIDEO_INES`, `CLU_NEG_PORT` | `BR_ERR_PORT` +20 min |
| Destination | `H_DEST_RIVE_EST` *(facultative)* | ✔ | `DED_RIVE_EST` | — | — |

Une hypothèse présentée sans preuve épinglée est refusée par Mendiondo sans pénalité (`DLG_C1_TABLEAU_02`).

## 3. Accessibilité des preuves obligatoires

Pour chaque preuve dont dépend une hypothèse obligatoire : au moins une voie **toujours** disponible et, si cette voie peut échouer, un rattrapage.

| Preuve | Voie principale | Peut échouer ? | Rattrapage |
|---|---|---|---|
| `CLU_VIDEO_INES` | Convaincre Inès (`INT_INES`) | Oui (menace) | `EVT_INES_PARENTS` 19 h 00 |
| `CLU_FLYER_BLANCHISSERIE` | Tableau de liège | Non | — |
| `CLU_SIV_CLONE` | Consultation de plaque (photo et/ou vidéo) | Non (15 min au pire) | — |
| `CLU_TEMOIN_DIRECTRICE` | Directrice | Non | — |
| `CLU_BADGE` (+ affiche) | Pistage avec la bouteille, ou fouille de la haie | Oui (mauvais objet) | `EVT_FOUILLE_HAIE` 18 h 30 |
| `CLU_TEMOIN_LARTIGUE` | Boulangère (ouverte jusqu'à 19 h 30) | Non | `CLU_MESSAGE_CHANTAGE` via `EVT_NADIA_CRAQUE` 18 h 45 couvre `DED_CIBLE` |

Le validateur rejoue les **11 combinaisons d'actions du prologue** (aucune action, une ou deux actions parmi photo, course, cri, chienne lâchée), chacune en mode normal et en mode « le joueur échoue à tout ce qui est facultatif ». Les trois hypothèses obligatoires restent atteignables dans les 22 cas.

## 4. Impasses évitées

| Risque de blocage | Parade |
|---|---|
| Le joueur n'a rien fait pendant l'enlèvement | Tous les axes se résolvent avec les indices permanents (vidéo, prospectus, badge, témoins). |
| Le joueur braque Inès | Ses parents apportent la vidéo à 19 h. |
| Le joueur ne trouve pas le badge | Les gendarmes fouillent la haie à 18 h 30 ; la directrice suffit de toute façon pour `DED_FAUSSE_ANIMATRICE`. |
| Le joueur accuse Nadia | Elle montre le message elle-même à 18 h 45. |
| Le joueur croit la notaire | Pénalité de temps, résultat négatif, option grisée ; la vidéo reste disponible. |
| Le joueur se trompe sur tous les axes, plusieurs fois | Pire cas vérifié : parcours de référence terminé à 17 h 28 ; même avec toutes les mauvaises options essayées une à une, 19 h 23 < 19 h 30. |
| Le joueur n'avance plus du tout | Clôture de 19 h 30 : les gendarmes partent sur la route des Étangs, chapitre 2 en état C. |
| La vue choisie masque un indice | Indices placés et testés pour les trois caméras ; photo et vidéos en `CAM_INSPECT`. |
| Joueur sans son | Le cognement du pot est doublé par un indice visuel ; la réaction de la chienne à Nadia est visuelle. |
| Un objet indispensable disparaît | Aucun objet n'est déplaçable par les PNJ ; la boulangerie ferme à 19 h 30 mais son témoignage a un équivalent (message de chantage). |
