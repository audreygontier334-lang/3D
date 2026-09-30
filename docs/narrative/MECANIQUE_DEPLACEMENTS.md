# Mécanique de déplacements et portrait-robot — « Carmen Sandiego, version moderne et adulte »

> **Décisions d'Audrey (29/09)** ✅ : la référence de jeu est **Carmen Sandiego des années 2000, en version modernisée et pour adultes** ; le **déplacement** en est le cœur : à chaque étape, la joueuse **choisit sa destination**, et ce choix influe sur les résultats, les conséquences et le temps. C'est **la joueuse qui établit le portrait-robot**, composé **comme dans un logiciel de portrait-robot de gendarmerie**. L'**escale de Luxembourg** est gardée. **Trop de fausses routes peuvent changer la fin** (étape de fuite `ETP_7`, fin B).
> Tout le reste de ce document est une **proposition de Claude** (🟡). Données : `GameData/campaign/itineraire.json` ; vérification : `python3 Tools/validate_itineraire.py` (et tests).

## 1. La boucle de jeu

| Carmen Sandiego (années 2000) | « Faux-semblants » (moderne ++, adulte) |
|---|---|
| Arriver dans une ville, interroger trois témoins | Arriver sur un lieu en 3D, enquêter : témoins, traces, Ariane, documents, réquisitions ; **chaque action coûte du temps de campagne** |
| Indices sur la prochaine ville | **Indices de destination**, jamais un seul : au moins deux preuves indépendantes désignent la bonne ; les leurres sont loyaux et réfutables |
| Indices sur le suspect → mandat | **Portrait-robot composé par la joueuse**, trait par trait, dans son carnet ; les témoins peuvent se tromper |
| Choisir la ville suivante sur la carte ; l'avion coûte des heures | **Carte de l'enquête** : trois ou quatre destinations, avec distance, mode (voiture, train, avion) et durée réelle ; l'horloge avance pendant le trajet |
| Mauvaise ville : « jamais vu personne comme ça » | **Fausse route jouée** : court déplacement réel avec une scène sur place, un résultat négatif loyal, et une **piste de retour** vers la bonne destination |
| Mandat faux ou absent : le suspect s'échappe | Portrait incomplet : **refus du magistrat** ; portrait faux : **mauvaise interpellation** (temps perdu, confiance perdue, réseau alerté) ; la vraie cible reste toujours rattrapable |
| Échéance : arrêter le voleur avant la fin de la semaine | **Échéances de l'histoire** : la nuit, le départ du navire jeudi 6 h, la pluie, le déchargement à Anvers, la fuite de Darrigade |

**Pour adultes** : autorités crédibles (les gendarmes et magistrats agissent sur la base de ce que la joueuse apporte), conséquences morales (accuser un innocent a un coût), un réseau qui réagit à l'enquête, aucune violence envers Lila (décision Q3).

## 2. Trois jauges qui rendent les choix conséquents

| Jauge | Monte / baisse | Effet |
|---|---|---|
| **Réseau alerté** (0–3) | +1 par mauvaise interpellation ou par certaines fausses routes bruyantes (réveiller le plombier de Dax, se montrer à Rotterdam, aller d'abord chez Nadia) | Chaque niveau fait disparaître un indice **facultatif** à l'étape suivante (le réseau nettoie derrière lui) |
| **Confiance des gendarmes** (0–3) | −1 par fausse route coûteuse ou demande refusée ; +1 par déduction juste présentée à temps | À 2 ou plus : réquisitions plus rapides et moyens accélérés (hélicoptère en forêt, vol direct) ; à 0, chaque demande coûte +30 min |
| **Fraîcheur de la piste** (0–3) | −1 par fausse route, avec le temps et la pluie | Efficacité du flair d'Ariane ; à 0, elle donne une direction mais plus d'objet précis |

**Garanties** (vérifiées par le validateur) : les indices **obligatoires** ont toujours une autre source ; aucune fausse route ne bloque l'histoire ; même en visitant toutes les mauvaises destinations, la bonne est atteinte avant l'échéance ; **Lila est retrouvée au chapitre 4** quelle que soit la route (Q3).

## 3. Les six étapes de la campagne

| Étape | Départ | Question | Bonne destination | Leurres (réfutables) |
|---|---|---|---|---|
| `ETP_1` (ch. 1) | Lescoure, mardi ≈ 17 h 30 | Où le fourgon a-t-il emmené Lila ? | Étang de Sorbe, rive est | Corniche (témoin de bonne foi qui s'est trompé), péage de l'A63, port de Capbreton |
| `ETP_2` (ch. 2) | Étang, mercredi 1 h | L'airial est vide : où continuer ? | Port de Bayonne, entrepôt Darrigade | Marina d'Hossegor, pistes vers Mimizan, Dax (plombier innocent) |
| `ETP_3` (ch. 3) | Bayonne, jeudi 6 h 30 | Où fouiller après le téléphone abandonné ? | Lande-Haute nord-est → **Lila retrouvée (ch. 4)** | Lande-Haute sud, cabane de pêcheur à Capbreton |
| `ETP_4` (ch. 5) | Lescoure, samedi | Où mène la chaîne du fret ? | Anvers | Rotterdam (escale), Porto (ancienne route) |
| `ETP_5` (ch. 5) | Anvers, dimanche soir | Qui détient Nordhaven ? | Luxembourg (holding) | Genève (virement annulé), Amsterdam (boîte aux lettres) |
| `ETP_6` (ch. 6) | Retour à Lescoure, mardi soir | Où frapper en premier ? | Étude Casteran (archives, témoin de bonne foi) | Maison de Nadia (alerte Darrigade), entrepôt de Bayonne (déjà vidé) |
| `ETP_7` (ch. 6, **seulement si** réseau alerté = 3 ou ≥ 5 fausses routes) | Lescoure, mercredi 8 h | Darrigade a fui : où le rattraper avant le ferry de 17 h ? | Port de Bilbao, terminal du ferry (mandat d'arrêt européen) | Aéroport de Biarritz (billets intacts), gare d'Hendaye (voiture abandonnée) |

Missions jouables : `ETP_1` → M01, `ETP_2` → M02, `ETP_3` → M03 et M04, `ETP_4` et `ETP_5` → M05 (Anvers), `ETP_6` → M06 (fin A), `ETP_7` → M07 « La fuite » (fin B), dans `GameData/missions/`.

**Deux fins** ✅ (principe) / 🟡 (contenu) : **fin A**, Darrigade interpellé dans les Landes ; **fin B**, après sa fuite, arrêté en Espagne grâce au mandat d'arrêt européen fondé sur le portrait de la joueuse. Il reste toujours rattrapable : seule la fin change.

L'étape 1 existe déjà dans la mission 01 (tableau d'hypothèses, branches `BR_ERR_*`) ; proposition : chaque mauvaise destination y devient un vrai court trajet avec les gendarmes et une scène sur place. L'escale `ETP_5` (Luxembourg) donne à l'acte II une vraie poursuite à travers l'Europe (✅ gardée par Audrey).

## 4. Le portrait-robot, établi par la joueuse ✅

- Dans le carnet, une fiche par suspect (« Sandrine », le conducteur, la tête du réseau) ; pour chaque trait, **la joueuse choisit** une valeur parmi plusieurs (âge, cheveux, signe distinctif, couverture, véhicule, habitudes, liens de sociétés…). **Rien n'est jamais prérempli ni deviné par le jeu.**
- Chaque vrai trait a au moins une source (souvent deux) ; certains témoins se trompent (pièges loyaux : la voiture de « Sandrine » conduite par un homme, Casteran qui a rédigé des statuts de bonne foi).
- Pour agir (interpellation, perquisition, mandat d'arrêt européen), la joueuse présente son portrait : trop peu de traits → refus ; un trait faux → mauvaise personne interpellée et conséquences sur les jauges.
- ✅ Rendu : **comme un logiciel de portrait-robot de gendarmerie**. Proposition : le visage et la silhouette s'assemblent trait par trait (âge, forme du visage, cheveux, yeux, lunettes, signes distinctifs) ; les traits non liés au visage (véhicule, couverture, habitudes, sociétés) s'affichent en fiche à côté. Le portrait reste celui que la joueuse a composé, juste ou faux.

## 5. Réponses d'Audrey (29/09)

1. Escale de Luxembourg : **gardée**.
2. Trop de fausses routes **peuvent changer la fin** : étape `ETP_7` et fin B.
3. Portrait-robot : **comme un logiciel de gendarmerie**.
