# Mission 01 — Énigmes du prologue et du chapitre 1

> **Statut : proposition non canonique.** 10 énigmes, dont 3 liées au duo avec la chienne (PZ_01, PZ_02, PZ_08). Chaque énigme renvoie aux indices (`CLU_`) décrits dans `01-ouverture.md` et `GameData/missions/01/clues.json`.
>
> **Règle générale des aides** : les indices progressifs sont proposés dans le carnet, à la demande du joueur, niveau par niveau. Le niveau 3 donne la démarche, jamais la réponse à cliquer. Demander une aide ne coûte pas de temps de jeu (option d'accessibilité), mais est comptabilisé pour les statistiques de fin de chapitre.

## Vue d'ensemble

| ID | Titre | Axe | Duo chienne | Obligatoire | Déduction produite |
|---|---|---|---|---|---|
| PZ_01 | La piste de Lila | Personne | ✅ | Non (renforce) | `DED_RUSE` |
| PZ_02 | L'objet de référence | Personne | ✅ | Non (rattrapage 18 h 30) | fournit `CLU_BADGE` |
| PZ_03 | Ce que voyait la notaire | Destination | — | Non | `DED_CASTERAN_NON_FIABLE` |
| PZ_04 | Douze secondes de vidéo | Destination | — | **Oui** | `DED_SORTIE_ETANGS` |
| PZ_05 | La plaque | Véhicule | — | **Oui** | `DED_PLAQUE_CLONEE` |
| PZ_06 | Les lettres fantômes | Véhicule | — | **Oui** | `DED_LETTRAGE` |
| PZ_07 | Le fourgon du péage | Destination | — | Non | `DED_PEAGE_EXCLU` |
| PZ_08 | Ce que Nadia ne dit pas | Personne | ✅ (réaction de la chienne) | Non (rattrapage 18 h 45) | `DED_CHANTAGE` |
| PZ_09 | Soleil sur l'étang | Destination | — | Non (avantage ch. 2) | `DED_RIVE_EST` |
| PZ_10 | Le tableau des hypothèses | Tous | — | **Oui** | Résolution |

« Obligatoire » = nécessaire pour conclure correctement. Chaque énigme obligatoire a au moins deux voies d'accès aux données (voir `01-preuves.md`).

---

## PZ_01 — La piste de Lila ✅ duo

**Contexte 3D.** Angle rue des Écoles / rue des Tamaris (`LOC_COIN_ECOLES`). Le porte-clés renard de Lila (`CLU_PORTE_CLES_LILA`) est au sol, près du caniveau. La chienne connaît Lila, qui la caresse souvent.

**Objectif.** Comprendre comment Lila a été emmenée : de force ou par ruse ?

**Données remises.**
- `CLU_PORTE_CLES_LILA` (objet de référence évident, l'odeur de Lila).
- Le comportement de la chienne pendant la piste, en quatre points (animations `ANIM_CHIENNE_*`) :
  - **A — l'angle** : cercles serrés sur 2 m², truffe basse ; puis une traction brève vers la boulangerie, truffe intermittente ;
  - **B — le trottoir de la rue des Tamaris** : allure régulière, droite, truffe basse, sur 25 m ;
  - **C — l'accotement** : s'assied, regarde {HEROINE} (découverte : barrette à fleur, `CLU_BARRETTE_LILA`) ;
  - **D — le bord de la chaussée** : tête haute, cercles, retour vers {HEROINE}.
- Le carnet propose, pour chaque point, trois interprétations à associer.

**Solution et raisonnement.**
- A : Lila s'est **arrêtée** et a piétiné sur place (conversation). La traction vers la boulangerie, truffe intermittente, correspond à une **piste plus ancienne** (son trajet du matin) : à ignorer.
- B : elle a **marché normalement** jusqu'au fourgon — aucun écart, aucune trace de lutte.
- C : **objet trouvé** : la barrette, tombée au moment de monter.
- D : **fin de piste** : Lila est partie en véhicule. La chienne ne peut pas suivre plus loin (limite affichée).
- Conclusion `DED_RUSE` : *Lila a suivi l'inconnue sans résister, jusqu'au fourgon. Elle a été trompée, pas saisie.* Ce qui suppose que l'inconnue avait de quoi la rassurer.

**Indices progressifs.**
1. « Observe la façon dont Ariane se déplace, pas seulement où elle va. »
2. « Une truffe qui reste au sol en ligne droite, ce n'est pas la même chose que des cercles ou une tête levée. Relis la fiche "Langage de Ariane" du carnet. »
3. « À l'angle, deux odeurs de Lila se croisent : celle de ce matin et celle de tout à l'heure. Laquelle est la plus fraîche ? Et au bout, pourquoi Ariane lève-t-elle la tête ? »

**Fausse piste loyale.** La traction vers la boulangerie : elle est réelle (Lila y passe chaque matin), mais la truffe intermittente indique une odeur ancienne. La fiche du carnet explique ce signal dès le tutoriel.

**Conséquence d'erreur.** Si le joueur associe « lutte » à B ou suit la boulangerie : la déduction `DED_RUSE` n'est pas créée ; {HEROINE} note une hypothèse incertaine (`H_FORCE`). Coût : 5 min de piste inutile vers la boulangerie.

**Rattrapage.** Le témoignage de Lartigue (l'inconnue connaissait le nom de famille de Lila) et celui de la directrice (aucune « Sandrine » parmi les animatrices) mènent au même constat par une autre voie. La piste peut être relancée tant que la scène n'est pas piétinée (jusqu'à 19 h 30).

**Test de cohérence.**
- La barrette est placée exactement sur la position de la portière latérale de `VEH_FOURGON` dans le prologue.
- La piste B longe le trottoir que la femme et Lila ont emprunté dans l'animation `EVT_MARCHE_FOURGON`.
- La piste s'arrête au bord de la chaussée même si le joueur la relance.

---

## PZ_02 — L'objet de référence ✅ duo

**Contexte 3D.** Abribus de la rue des Écoles (`LOC_ABRIBUS`), puis la rue jusqu'à la haie de pittosporum de l'accotement.

**Objectif.** Faire suivre à la chienne la piste de l'inconnue, pour retrouver ce qu'elle a pu laisser.

**Données remises.**
- Trois objets candidats, tous ramassables dans un sac à crottes (geste réaliste qui évite la contamination) :
  - `CLU_BOUTEILLE` — bouteille d'eau entamée, sur le banc de l'abribus, trace de rouge à lèvres sur le goulot ;
  - `CLU_MEGOTS` — trois mégots sur l'accotement ;
  - `CLU_PORTE_CLES_LILA`.
- `CLU_TICKET_BOULANGERIE` — ticket froissé sous le banc : 1 bouteille d'eau, 16 h 12, espèces.
- `CLU_TEMOIN_LARTIGUE` — la femme au badge a acheté une bouteille d'eau et s'est assise à l'abribus.
- `CLU_TEMOIN_DUFAU` — le conducteur « n'est pas sorti de sa cabine, il fumait vitre baissée ».
- Si `CLU_OBS_PASSAGERE` : la femme portait du rouge à lèvres.

**Solution et raisonnement.** La bouteille (heure du ticket, témoignage de Lartigue, rouge à lèvres) porte l'odeur de la femme. Les mégots sont ceux du conducteur, qui n'est pas sorti. Le porte-clés est l'odeur de Lila. Avec la bouteille, la chienne suit une piste abribus → angle (se confond un instant avec celle de Lila) → accotement → **haie**, où elle s'assied : `CLU_BADGE`, arraché par la portière.

**Indices progressifs.**
1. « Pour suivre quelqu'un, Ariane a besoin d'un objet que cette personne a touché, et seulement elle. »
2. « Qui a acheté quelque chose à la boulangerie à 16 h 12 ? Où s'est-elle assise ensuite ? »
3. « Le conducteur est-il sorti du fourgon ? Si non, les mégots ne mènent nulle part. Le porte-clés, c'est Lila. Il reste un objet. »

**Fausse piste loyale.** Les mégots, bien visibles et « suspects ». Dufau précise que le conducteur n'est pas sorti ; la piste des mégots s'arrête à l'accotement, là où ils ont été jetés.

**Conséquence d'erreur.** Mégots : piste de 2 m, fin immédiate, +5 min (`BR_PISTE_MAUVAIS_OBJET`). Porte-clés : relance PZ_01 (utile, mais pas de badge). Aucun objet n'est détruit ni contaminé.

**Rattrapage.**
- Si le joueur a crié (`CLU_OBS_BADGE_CHUTE`), il sait où chercher : fouille directe de la haie, sans pistage.
- Si Ariane a été envoyée pendant la fenêtre (`ACT_ENVOYER` → `CLU_CHIENNE_IMPREGNEE`), elle part spontanément vers la haie lors de l'examen de l'accotement.
- À 18 h 30, les gendarmes fouillent la haie et trouvent le badge (`EVT_FOUILLE_HAIE`).

**Test de cohérence.**
- Le badge est placé dans la haie, à la hauteur de la portière latérale, côté trottoir.
- Le ticket indique 16 h 12, compatible avec `CLU_TEMOIN_LARTIGUE` et avec l'abordage à 16 h 36.
- Le badge reste accessible jusqu'à la fin du chapitre (aucun passant ne le déplace).

---

## PZ_03 — Ce que voyait la notaire

**Contexte 3D.** Perron de l'étude Casteran, place de l'Église (`LOC_ETUDE_CASTERAN`). Entre le perron et le rond-point : l'auvent vert de la pharmacie et un grand platane.

**Objectif.** Évaluer le témoignage de Maître Casteran, selon qui le fourgon est parti « vers le nord, par la Corniche ».

**Données remises.**
- `CLU_TEMOIN_CASTERAN` : « J'étais sur mon perron, j'ai tout vu. Il a tourné à gauche au rond-point, vers la Corniche. »
- Le perron lui-même, où {HEROINE} peut se placer (action « Se mettre à sa place »).
- Le panneau du rond-point, point de repère visible depuis la rue des Tamaris.

**Solution et raisonnement.** Debout sur le perron, à hauteur d'yeux, on voit la rue des Tamaris et le début de la courbe, **mais pas le rond-point** : l'auvent masque la chaussée, le platane cache le panneau et les sorties. Casteran a pu voir le fourgon partir vers l'est, pas la sortie qu'il a prise. Son affirmation « j'ai tout vu » n'est pas fiable : `DED_CASTERAN_NON_FIABLE`.
{HEROINE} ne conclut **pas** qu'elle ment : le carnet note « s'est trompée ou a supposé ». (Graine payée au chapitre 6.)

**Indices progressifs.**
1. « Un témoin sincère peut se tromper. Où se tenait-elle exactement ? »
2. « Place-toi sur le perron. Que vois-tu vraiment d'ici ? »
3. « Cherche le panneau du rond-point depuis le perron. Si tu ne le vois pas, elle non plus. »

**Fausse piste loyale.** L'assurance et la respectabilité de la notaire. Et, dans la vidéo d'Inès, un camping-car blanc sur la Corniche à 16:39:16 (voir PZ_04).

**Conséquence d'erreur.** Croire Casteran mène à présenter « Corniche nord » (`BR_ERR_NORD`, +25 min, `CLU_NEG_NORD`).

**Rattrapage.** La vidéo d'Inès contredit directement ce témoignage ; `CLU_NEG_NORD` également. Le test de ligne de vue reste faisable à tout moment.

**Test de cohérence.**
- Dans la scène, un rayon de visibilité depuis la hauteur d'yeux (1,65 m) au perron ne doit atteindre aucune des trois sorties du rond-point. À vérifier par Codex dans les trois caméras.
- Depuis le perron, la rue des Tamaris est visible jusqu'à la courbe (Casteran a bien vu le fourgon partir).

---

## PZ_04 — Douze secondes de vidéo

**Contexte 3D.** Butte du skatepark (`LOC_SKATEPARK`), vue plongeante sur le rond-point. Inès, 15 ans, filme ses figures.

**Objectif.** Obtenir la vidéo, puis établir par quelle sortie le fourgon a quitté le rond-point.

**Données remises.**
- Dialogue de persuasion (`DLG_C1_INES_*`) : Inès a peur que ses parents apprennent qu'elle devait être à l'étude. Deux approches réussissent : rassurer (« personne ne te reprochera d'aider ») ou l'impliquer (« tu as peut-être filmé la seule image du fourgon »). La menace (« les gendarmes vont saisir ton téléphone ») la braque ; elle part (rattrapage à 19 h).
- `CLU_VIDEO_INES` : 12 s, horodatée 16:39:10, lecture image par image en `CAM_INSPECT`. Le rond-point tourne dans le sens inverse des aiguilles d'une montre. Depuis la rue des Tamaris (entrée ouest), la première sortie est la D652 (sud), la deuxième la route des Étangs (est), la troisième la Corniche (nord).
  - 16:39:12 : le fourgon entre, passe devant la sortie D652 **sans** la prendre ;
  - 16:39:13–14 : masqué par le skateur ;
  - 16:39:15 : arrière du fourgon sur la route des Étangs, derrière le panneau « Étang de Sorbe 14 » ; plaque arrière partiellement lisible « ··-·37-TR » ; le cognement du pot est audible ;
  - 16:39:16 : un **camping-car blanc** (toit haut, porte-vélos) s'éloigne sur la Corniche.
- `CLU_PLAN_DISTANCES` pour le nom des sorties.

**Solution et raisonnement.** Le fourgon ne prend pas la D652 (visible à 16:39:12), réapparaît sur la route des Étangs à 16:39:15. Le véhicule blanc sur la Corniche est un camping-car : toit plus haut, porte-vélos, silhouette différente, pas de cognement. `DED_SORTIE_ETANGS`.

**Indices progressifs.**
1. « Repasse la vidéo image par image. Où est le fourgon juste avant et juste après le passage du skateur ? »
2. « Il y a deux véhicules blancs dans cette vidéo. Compare leur hauteur, leur arrière et le bruit. »
3. « Le fourgon cogne. Le véhicule sur la Corniche a un porte-vélos. Sur quelle route voit-on la plaque "37-TR" ? »

**Fausse piste loyale.** Le camping-car sur la Corniche, qui semble confirmer Casteran.

**Conséquence d'erreur.** Conclure « Corniche » (`BR_ERR_NORD`).

**Rattrapage.** Si Inès est braquée, ses parents l'amènent au poste à 19 h (`EVT_INES_PARENTS`). La vidéo du Relais du Lac (`CLU_CCTV_RELAIS`, demandable seulement après la vidéo d'Inès) confirme ensuite la route.

**Test de cohérence.**
- Le signal sonore du pot est doublé par un indice visuel (échappement qui tremble, fumée) pour les joueurs sans son.
- Les horodatages sont compatibles : départ 16:38, 300 m, rond-point 16:39:12, Relais du Lac (9 km) 16:48.

---

## PZ_05 — La plaque

**Contexte 3D.** Poste de commandement (`LOC_POSTE`). Un formulaire de consultation que l'adjudante remplit avec {HEROINE} (le joueur saisit les caractères, `?` = inconnu).

**Objectif.** Identifier le véhicule par sa plaque.

**Données remises.**
- `CLU_PHOTO_FOURGON` (si photo prise immobile) : « GF-4·7-·· » — la terre cache le chiffre du milieu et la fin.
- `CLU_VIDEO_INES` : « ··-·37-TR ».
- `CLU_TEMOIN_DUFAU` : « une plaque des Landes, 40 ».
- Résultat `CLU_SIV_CLONE` : la plaque GF-437-TR est celle d'un fourgon du même modèle appartenant à un plombier de Dax ; le véhicule, équipé d'un traceur de flotte, se trouvait chez un client à Dax à 16 h 38.

**Solution et raisonnement.** Combinaison des fragments : **GF-437-TR** (les positions se recoupent : 4-?-7 et ?-3-7 → 437). Le propriétaire légitime était à 50 km : la plaque est **clonée**. Le véhicule n'est pas celui du plombier. `DED_PLAQUE_CLONEE`.

**Indices progressifs.**
1. « Une plaque française : deux lettres, trois chiffres, deux lettres. Aligne ce que tu as. »
2. « La photo donne le début, la vidéo la fin. Le chiffre du milieu apparaît sur l'une des deux. »
3. « Si le véhicule enregistré était à Dax au même moment, que peux-tu en conclure sur la plaque ? »

**Fausse piste loyale.** Le nom du plombier fourni par le fichier : tentant comme suspect. Sa géolocalisation l'innocente.

**Conséquence d'erreur.** Présenter « fourgon du plombier » : `BR_ERR_VEHICULE`, +15 min, `CLU_NEG_VEHICULE`.

**Rattrapage.** Une seule source suffit : avec la photo seule ou la vidéo seule, la consultation avec `?` renvoie 12 véhicules dont un seul fourgon blanc de ce modèle ; coût 15 min au lieu de 5.

**Test de cohérence.** Les deux fragments sont compatibles entre eux et avec le modèle `VEH_FOURGON`. Le numéro n'appartient à aucune personne réelle (format fictif à vérifier par Codex avant publication).

---

## PZ_06 — Les lettres fantômes

**Contexte 3D.** Boulangerie, tableau de liège (`LOC_BOULANGERIE`) ; carnet.

**Objectif.** Découvrir à qui a appartenu le fourgon.

**Données remises.**
- Selon les actions du prologue, au moins une source de lettrage : `CLU_PHOTO_FOURGON`, `CLU_PHOTO_FLOUE`, `CLU_VIDEO_INES` (image 16:39:15 : « ··ANCHISS·· »).
  - Lettrage complet reconstituable : « BL·NCH·SS·RIE OC·A·· » et « 05 58 ·7 ·0 12 » (photo nette).
- `CLU_FLYER_BLANCHISSERIE` : « Blanchisserie Océane — livraison de linge aux professionnels — 05 58 47 30 12 », photo de « notre équipe de livraison ».
- `CLU_FLYER_PRESSING` : « Blanchisserie du Courant — pressing, dépôt en boutique — 05 58 41 22 12 ».
- `CLU_TEMOIN_DUFAU` : « C'était pas la blanchisserie : eux, c'est le mardi matin, et leur camion a le nom dessus. »

**Solution et raisonnement.** Le lettrage « OC·A·· » et les chiffres « ·7 ·0 » correspondent à la Blanchisserie **Océane** (47 30), pas au pressing du Courant (41 22), qui ne livre pas. Le fourgon est un **ancien** véhicule de la Blanchisserie Océane dont le nom a été retiré (d'où l'ombre), et non un de ses fourgons actuels (livraisons le mardi matin, lettrage intact). `DED_LETTRAGE`.

**Indices progressifs.**
1. « L'ombre d'un autocollant retiré garde la forme des lettres. Qu'est-ce qui était écrit ? »
2. « Deux blanchisseries sont affichées. Compare les lettres et les chiffres qui restent. »
3. « Un fourgon actuel porte son nom. Celui-ci l'a perdu. Qu'est-ce que ça dit de son histoire ? »

**Fausse piste loyale.** Le pressing du Courant, dont le numéro se termine aussi par 12.

**Conséquence d'erreur.** Présenter « fourgon actuel de la blanchisserie » : `BR_ERR_VEHICULE`, +15 min ; le planning de la blanchisserie (`CLU_NEG_VEHICULE`) confirme que ses fourgons étaient au dépôt.

**Rattrapage.** Le lettrage partiel de la vidéo d'Inès (toujours obtenable) suffit à écarter le pressing (« ANCHISS » + « OC » visible sur la porte arrière à 16:39:15 en zoom). Une fois `DED_LETTRAGE` établie, l'appel à la blanchisserie (`CLU_APPEL_BLANCHISSERIE`) apprend que le fourgon réformé a été vendu aux enchères il y a huit mois à Sud Loc Services, et récupéré par **Franck Loubère**, ancien livreur licencié. La photo d'équipe du prospectus montre un homme à casquette grise que Dufau reconnaît : `DED_K1_LOUBERE` (optionnelle).

**Test de cohérence.** Le lettrage visible dans chaque source est un sous-ensemble du lettrage réel ; aucune source ne montre une lettre contradictoire.

---

## PZ_07 — Le fourgon du péage

**Contexte 3D.** Poste de commandement ; radio des gendarmes ; carte murale et plan touristique.

**Objectif.** Évaluer le signalement d'un fourgon blanc au péage de l'A63 à 16 h 58 (`CLU_SIGNALEMENT_PEAGE`, reçu à 17 h 15).

**Données remises.**
- Signalement : fourgon blanc, péage de l'A63, 16 h 58, direction Espagne.
- `CLU_VIDEO_INES` : le fourgon est au rond-point à 16:39:12.
- `CLU_PLAN_DISTANCES` : rond-point → péage A63 : 32 km, environ 28 min.
- Optionnel : `CLU_CCTV_RELAIS` (fourgon au Relais du Lac à 16 h 48).

**Solution et raisonnement.** 16 h 39 + 28 min = 17 h 07 au plus tôt. Pour être au péage à 16 h 58, il aurait fallu rouler à près de 100 km/h de moyenne sur une départementale qui traverse deux villages : invraisemblable. Et s'il a pris la route des Étangs (PZ_04), il n'était même pas sur la bonne route. Si la vidéo du Relais est connue, l'incompatibilité est totale. Le fourgon du péage est un autre véhicule. `DED_PEAGE_EXCLU`.

**Indices progressifs.**
1. « À quelle heure le fourgon était-il au rond-point ? Et combien de temps faut-il pour aller au péage ? »
2. « Additionne. Est-il arrivé avant ou après 16 h 58 ? »
3. « Un fourgon qui prend la route des Étangs peut-il être sur la D652 en même temps ? »

**Fausse piste loyale.** Le signalement officiel, par radio, qui semble décisif et que les gendarmes prennent au sérieux.

**Conséquence d'erreur.** Présenter « A63 / Espagne » : `BR_ERR_A63`, +30 min ; `CLU_NEG_A63` : la vidéo du péage montre un fourgon d'un autre modèle avec une échelle sur le toit.

**Rattrapage.** `CLU_NEG_A63`, puis relecture de la vidéo d'Inès.

**Test de cohérence.** Les temps du plan sont cohérents avec ceux du §1.3 de `01-ouverture.md` et avec le passage au Relais du Lac (9 km en 9 min).

---

## PZ_08 — Ce que Nadia ne dit pas ✅ duo (réaction de la chienne)

**Contexte 3D.** Place de l'Église, près de la fontaine et du poste. Nadia arrive à 17 h 35 ; Maître Casteran, sa propriétaire, ne la lâche pas.

**Objectif.** Obtenir que Nadia révèle le message qu'elle a reçu à 17 h 40.

**Données remises.**
- `CLU_NADIA_REACTION` (si observée entre 17 h 40 et 17 h 55) : elle lit son téléphone, blêmit, le range précipitamment, dit aux gendarmes « rien, c'est le travail ». **Ariane va se coller contre ses jambes** et gémit doucement — signal de détresse, visible dans toutes les vues.
- `CLU_TEMOIN_LARTIGUE` : la femme a demandé « l'école de la petite Mercadier ».
- `CLU_BADGE` + `CLU_AFFICHE_COMMUNE` / `CLU_TEMOIN_DIRECTRICE` : fausse animatrice.
- `DED_RUSE` (si établie).
- Présence de Casteran : tant qu'elle est à côté, Nadia se tait. Elle s'éloigne si {HEROINE} demande à Mendiondo de prendre sa déposition, ou d'elle-même à 18 h 05 (« je vais préparer du thé à l'étude »).

**Solution et raisonnement.** Parler à Nadia **à l'écart de Casteran**, avec **compassion**, en présentant **un élément montrant que l'enlèvement la visait** : l'inconnue connaissait son nom (Lartigue), ou s'est fait passer pour une animatrice pour rassurer Lila (badge + directrice / `DED_RUSE`). Nadia comprend qu'on ne la soupçonne pas et montre le message (`CLU_MESSAGE_CHANTAGE`) et la photo (`CLU_PHOTO_VIE`). `DED_CHANTAGE`.

**Indices progressifs.**
1. « Nadia a peur. De quoi, ou de qui ? Regarde comment réagit Ariane. »
2. « Elle ne parlera pas devant n'importe qui. Et elle ne parlera pas si elle se sent accusée. »
3. « Montre-lui que tu sais que ce n'était pas un hasard : quelqu'un connaissait son nom. »

**Fausse piste loyale.** Nadia se comporte comme quelqu'un de coupable (mensonge, téléphone caché). Le joueur peut la soupçonner de complicité ; le dialogue permet ce soupçon mais il mène à la fermeture.

**Conséquence d'erreur.** Accusation, ou discussion devant Casteran : `BR_NADIA_BRUSQUEE`, +5 min, Nadia fermée.

**Rattrapage.** À 18 h 45, Nadia craque et montre le message aux gendarmes (`EVT_NADIA_CRAQUE`). Le joueur peut ensuite consulter la photo au poste.

**Test de cohérence.** Casteran est bien présente à côté de Nadia de 17 h 35 à 18 h 05 (sauf éloignement demandé). La réaction de la chienne ne dépend pas du son.

---

## PZ_09 — Soleil sur l'étang

**Contexte 3D.** Carnet (`CAM_INSPECT` sur `IMG_PHOTO_VIE`) et carte murale du poste de commandement (étang de Sorbe, allongé nord-sud ; rive ouest : base nautique avec pontons et forêt dunaire ; rive est : forêt de pins « ancien gemmage — sentier de la résine » et quelques airiaux).

**Objectif.** Situer la pièce où se trouve Lila.

**Données remises.**
- `CLU_PHOTO_VIE` : Lila assise, une couverture sur les épaules ; derrière elle une fenêtre donnant sur l'étang ; le soleil bas, **droit dans l'axe**, fait scintiller l'eau ; un ponton de bois ; un pin porte un **pot à résine** en terre cuite.
- `CLU_MESSAGE_CHANTAGE` : reçu à 17 h 40 (« Elle va bien »).
- Carte de l'étang (poste) ; légende « ancien gemmage » sur la rive est.

**Solution et raisonnement.** En fin d'après-midi, fin septembre, le soleil est à l'ouest-sud-ouest. Si le soleil est droit dans l'axe de la fenêtre au-dessus de l'eau, la fenêtre regarde vers l'ouest : la maison est donc sur la **rive est**, face à l'étang. Le pot à résine confirme une forêt anciennement gemmée, indiquée sur la rive est. `DED_RIVE_EST`.

**Indices progressifs.**
1. « À quelle heure la photo a-t-elle été prise, à peu près ? Où est le soleil à ce moment-là ? »
2. « Le soleil est en face, au-dessus de l'eau. Vers où regarde la fenêtre ? »
3. « Si la fenêtre regarde l'ouest et donne sur l'étang, de quel côté de l'étang est la maison ? Et que dit la légende de la carte sur ce petit pot accroché au pin ? »

**Fausse piste loyale.** Le ponton, qui évoque la base nautique et ses pontons de la rive ouest.

**Conséquence d'erreur.** Conclure « rive ouest » : aucune pénalité immédiate, mais `DED_RIVE_EST` non établie, donc `H_DEST_RIVE_EST` impossible à présenter → le chapitre 2 démarre au mieux en état B ; au chapitre 2, les traces de pneus à la base nautique sont anciennes et le joueur se réoriente (rattrapage au chapitre 2).

**Rattrapage.** La photo reste consultable dans le carnet à tout moment ; l'énigme peut être résolue jusqu'à la présentation du tableau.

**Test de cohérence.** Soleil vers 17 h 30 fin septembre sous 44° de latitude nord : azimut ≈ 242° (ouest-sud-ouest), hauteur ≈ 23° (calcul vérifié par le validateur). Le rendu de `IMG_PHOTO_VIE` doit respecter cette position (soleil au-dessus de l'eau, pas derrière la maison).

---

## PZ_10 — Le tableau des hypothèses

**Contexte 3D.** Poste de commandement. Le carnet s'ouvre sur un tableau en trois colonnes : **Véhicule**, **Personnes**, **Destination**. Chaque colonne propose des hypothèses débloquées par les indices ; le joueur y épingle les preuves qui les soutiennent.

**Objectif.** Présenter à l'adjudante-cheffe Mendiondo une hypothèse étayée par axe.

**Données remises.** Toutes les déductions précédentes. Hypothèses disponibles :

| Axe | Hypothèses (✔ = correcte) |
|---|---|
| Véhicule | ✔ `H_VEH_ANCIEN_BLANCHISSERIE` (ancien fourgon de la blanchisserie, plaque clonée) · `H_VEH_PLOMBIER` · `H_VEH_BLANCHISSERIE_ACTIVE` |
| Personnes | ✔ `H_K2_FAUSSE_ANIMATRICE` (inconnue déguisée en animatrice, qui connaissait la famille) · `H_K2_VRAIE_ANIMATRICE` ; optionnel : ✔ `H_K1_LOUBERE` |
| Destination | ✔ `H_DEST_ETANGS` (route des Étangs, secteur étang de Sorbe) · `H_DEST_NORD` · `H_DEST_A63` · `H_DEST_PORT` ; optionnel : ✔ `H_DEST_RIVE_EST` |

**Solution et raisonnement.** Voir la matrice `01-preuves.md`. Chaque hypothèse correcte exige des preuves précises ; Mendiondo refuse une hypothèse sans preuve épinglée (« Sur quoi vous vous appuyez ? ») sans pénalité de temps.

**Indices progressifs.**
1. « Trois questions différentes : le véhicule, les personnes, la destination. Un seul indice ne répond jamais aux trois. »
2. « Pour chaque colonne, quelle preuve contredit les autres hypothèses ? »
3. « Relis les déductions de ton carnet : chacune correspond à une colonne. »

**Fausse piste loyale.** `H_DEST_PORT` : la caisse de la criée de Capbreton (`CLU_CAISSE_POISSON`) près de l'accotement. Dufau (« le poissonnier en empile toujours là ») ou `CLU_NEG_PORT` l'expliquent.

**Conséquence d'erreur.** Branches `BR_ERR_*` (voir `01-ouverture.md` §5) : temps perdu, résultat négatif, option exclue.

**Rattrapage.** Ré-présentation autant de fois que nécessaire ; clôture automatique à 19 h 30.

**Test de cohérence.** Le validateur `Tools/validate_mission.py` vérifie que chaque hypothèse correcte a au moins un jeu de preuves suffisant accessible dans tous les scénarios de prologue, avant 19 h 30.
