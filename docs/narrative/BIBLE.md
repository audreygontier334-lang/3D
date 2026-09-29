# Bible narrative — « Faux-semblants » (titre provisoire)

> **Statut : proposition de Claude, non canonique.** Rien dans ce document ne devient décision avant validation d'Audrey. Les éléments déjà validés sont repris de `docs/DECISIONS.md` et signalés ✅. Tout le reste est marqué 🟡 *proposition*.
>
> Noms : la protagoniste et sa chienne n'ont pas encore de nom de jeu. Les textes utilisent les jetons `{HEROINE}` et `{CHIENNE}`, remplacés par le moteur. Les autres noms sont provisoires.

---

## 1. Résumé public (sans spoilers)

Un mardi de fin septembre, à Lescoure-Plage 🟡 — un village fictif de la côte landaise, entre océan, dunes et forêt de pins —, une femme promène sa chienne sur le front de mer ✅. À la sortie de l'école, elle voit une fillette du quartier monter dans un fourgon blanc avec une inconnue. Quelques secondes plus tard, le fourgon a disparu ✅.

Elle n'est ni policière ni détective. Mais elle a vu la scène, sa chienne connaît l'odeur de l'enfant, et les premières heures comptent. Aux côtés des gendarmes, elle interroge les témoins, confronte des versions qui ne concordent pas, suit les pistes que flaire sa chienne et apprend à distinguer ce qu'elle a vu de ce qu'elle croit avoir vu.

Retrouver la fillette n'est que le début. L'enlèvement n'avait rien d'un hasard : derrière lui se cache un réseau qui fabrique de faux papiers et fait passer des marchandises par les ports de l'Atlantique. De la forêt landaise aux quais de Bayonne, de Porto à Anvers, l'enquête la ramènera là où tout a commencé — dans un village où chacun n'est pas ce qu'il paraît.

**Thème** 🟡 : *les apparences qui rassurent*. Un badge officiel, un fourgon d'artisan, une plaque d'immatriculation, un témoin respectable, une mère qui dit « je ne sais pas » : chaque chapitre met le joueur face à une façade crédible qu'il faut vérifier plutôt que croire. La chienne, elle, ne juge pas sur les apparences — mais son flair ne suffit jamais seul.

**Ton** 🟡 : thriller d'enquête réaliste et tendu, pour adultes. Aucune violence graphique envers l'enfant, aucune scène complaisante. La tension vient du temps qui passe, des témoignages contradictoires et du danger ressenti, pas du spectacle de la souffrance.

---

## 2. Le duo ✅ (caractéristiques validées) / 🟡 (caractérisation proposée)

### {HEROINE}
- ✅ Femme brune d'une trentaine d'années, jean, tee-shirt, veste en jean, bandeau noué, créoles, baskets blanches épaisses ; expression sérieuse ou en colère selon la scène.
- 🟡 Habite Lescoure depuis plusieurs années ; connaît les commerçants, les habitudes du port, les véhicules qui passent. Elle croise souvent Lila, la fillette, qui caresse sa chienne à la sortie de l'école.
- 🟡 Caractère : directe, obstinée, peu patiente avec les discours creux ; elle se méfie de ses propres impressions et apprend à les vérifier. Sa colère est un moteur, pas un défaut à punir.
- 🟡 Statut : **civile et témoin**. Elle ne procède à aucune arrestation, ne fouille pas illégalement, ne délivre aucun mandat. Son rôle : observer, relier, convaincre les autorités d'agir au bon endroit.
- ❓ Métier et passé : à trancher (voir `QUESTIONS_AUDREY.md`, Q5).

### {CHIENNE}
- ✅ Croisée malinois / bull terrier, environ 35 kg, beige fauve, masque et oreilles noirs, poitrail et bouts de pattes blancs, musclée, une oreille droite et l'autre tombante.
- 🟡 Chienne de compagnie **non dressée pour la police**, mais très éduquée : rappel, « au pied », « reste », « cherche », « montre ». Grande sensibilité aux émotions de sa maîtresse.
- 🟡 **Capacités plausibles** : suivre la piste fraîche d'une personne à pied à partir d'un objet de référence ; retrouver un objet porteur d'une odeur ; signaler une personne cachée à courte distance ; réagir à un individu qu'elle a déjà senti.
- 🟡 **Limites affichées au joueur** : elle ne suit pas un véhicule au-delà de quelques mètres ; la piste se dégrade avec le temps, la pluie, le bitume chaud et les passages nombreux ; elle peut se tromper de piste si l'objet de référence est contaminé ; elle ne « désigne » jamais un coupable. Une piste canine est une **orientation**, jamais une preuve juridique — les gendarmes le rappellent.
- 🟡 Langage corporel lisible (utilisé par les énigmes) : truffe basse et allure régulière = piste suivie ; tête haute = odeur portée par l'air ; cercles et hésitation = piste perdue ou croisement ; arrêt assis ou couché, regard vers {HEROINE} = découverte ; oreilles plaquées, poil hérissé = personne inconnue stressante.

---

## 3. Personnages secondaires (tous 🟡)

| ID | Nom provisoire | Rôle apparent | Ce qu'on découvre |
|---|---|---|---|
| `NPC_LILA` | Lila Mercadier, 9 ans, CM1 | La fillette enlevée ; rentre seule depuis la rentrée, fière de cette autonomie | Retrouvée saine et sauve au chapitre 2 ; son témoignage prudent (recueilli par des professionnels) aide la suite |
| `NPC_NADIA` | Nadia Mercadier | Mère de Lila, élève seule sa fille | Agente des douanes au port de Bayonne. Elle a bloqué un conteneur aux documents falsifiés. Elle reçoit un message de chantage et le cache d'abord aux gendarmes |
| `NPC_MENDIONDO` | Adjudante-cheffe Carole Mendiondo | Commandante de la brigade locale de gendarmerie | Compétente, sceptique envers les civils mais pragmatique ; devient une alliée exigeante |
| `NPC_ARBELOT` | Capitaine Thomas Arbelot | Section de recherches, arrive au chapitre 2 | Dirige l'enquête judiciaire sur le réseau ; associe {HEROINE} comme témoin-clé |
| `NPC_DUFAU` | Marcel Dufau | Retraité bougon, pêche au bord du courant | Connaît tous les véhicules du coin ; témoin fiable mais qui exagère |
| `NPC_INES` | Inès Barrère, 15 ans | Ado au skatepark, filme ses figures | Sa vidéo contient le fourgon en arrière-plan, horodaté |
| `NPC_LARTIGUE` | Josiane Lartigue | Boulangère | A servi l'inconnue vingt minutes avant l'enlèvement |
| `NPC_CASTERAN` | Maître Hélène Casteran | Notaire du village, propriétaire de l'appartement de Nadia ; aimable, respectée | *Voir SPOILERS* |
| `NPC_K1` | Franck Loubère | « Le conducteur » | Ancien livreur licencié de la Blanchisserie Océane ; exécutant endetté, nerveux |
| `NPC_K2` | « Sandrine » (alias) | « La passagère », se fait passer pour une animatrice périscolaire | Passeuse professionnelle du réseau, s'échappe au chapitre 2 |

---

## ⚠️ SPOILERS — vérité cachée et fin

> Section réservée à Audrey, Codex et aux personnes qui implémentent. Ne pas afficher en jeu.

### 4. Le réseau adverse 🟡

**Ce qu'il fait.** Un réseau que ses membres appellent *l'Amarre* importe des **médicaments falsifiés** et d'autres marchandises de contrebande par des ports de l'Atlantique, en les couvrant de **faux documents** : certificats d'origine, déclarations de conformité, et fausses identités pour ses propres membres. Les identités sont construites à partir de personnes décédées sans héritiers proches, dont les dossiers passent par une étude notariale.

**Structure.**
- **Façade logistique** : *Transmarine Adour* (commissionnaire de transport à Bayonne) organise les expéditions.
- **Navire** : le caboteur *Maren Sofie*, rotation Bayonne → Leixões (Porto) → Anvers.
- **Faussaire** : un imprimeur de Porto surnommé *le Typographe*.
- **Holding** : *Nordhaven Shipping BV*, société écran à Anvers.
- **Tête et architecte des identités** : **Maître Hélène Casteran**, notaire de Lescoure. Elle a bâti le réseau en recyclant des successions et en créant des SCI qui possèdent les planques.

**Motivations.** Argent, d'abord. Pour Casteran, aussi une revanche froide : l'étude de son père a été ruinée par une affaire de succession ; elle méprise les institutions qu'elle sert en façade. Loubère agit par dettes et par peur. « Sandrine » est une professionnelle loyale au réseau qui l'a « recréée ».

**Pourquoi Lila.** Nadia Mercadier a immobilisé au port de Bayonne le conteneur **TMAU 482113-7**, dont les certificats sont faux. Le *Maren Sofie* doit appareiller **jeudi à 6 h**. Le réseau enlève Lila pour contraindre Nadia à lever la retenue sans prévenir personne. Casteran, propriétaire de l'appartement de Nadia, savait que Lila rentrait seule depuis la rentrée et connaissait son itinéraire. **L'enfant est un moyen de pression, pas une cible d'exploitation** ; le réseau compte la relâcher une fois le navire parti, mais Loubère est instable, ce qui rend l'urgence réelle.

### 5. Chronologie cachée 🟡

| Moment | Événement caché |
|---|---|
| J-8 mois | La Blanchisserie Océane vend aux enchères un fourgon réformé. Acheteur : *Sud Loc Services*, SARL dont les statuts ont été rédigés par l'étude Casteran. Loubère, licencié de la blanchisserie, le récupère. |
| J-3 semaines | Nadia bloque le conteneur TMAU 482113-7 et signale les certificats. |
| J-10 jours | Casteran entend Nadia dire que Lila rentre seule désormais. Elle transmet l'itinéraire. |
| J-2 | Loubère pose de fausses plaques, clonées sur un fourgon identique appartenant à un plombier de Dax. Il retire le lettrage de la blanchisserie ; il en reste une ombre. |
| J 15 h 50 | Le fourgon arrive par la route de la forêt et se gare sur l'accotement sablonneux de la rue du Port. Dufau le remarque (pot d'échappement qui cogne). |
| J 16 h 12 | « Sandrine » achète une bouteille d'eau à la boulangerie, demande le chemin de l'école alors qu'elle porte un badge d'animatrice périscolaire. Elle laisse la bouteille sur le banc de l'abribus. |
| J 16 h 30 | Sortie de l'école élémentaire des Pins. |
| J 16 h 36 | « Sandrine » aborde Lila au coin de la rue des Écoles : « Ta maman a eu un souci au travail, elle m'a demandé de te ramener. » Elle connaît le prénom de Nadia. Lila la suit, hésitante. |
| J 16 h 38 | Lila monte dans le fourgon. Le cordon du badge de « Sandrine » s'accroche à la portière et tombe dans la haie. Le fourgon part vers le sud, tourne à droite au bout de la rue du Port, prend au rond-point la route des Étangs. |
| J 16 h 38 | Casteran, sur le seuil de son étude, a observé la scène de loin. Elle affirmera ensuite, pour égarer les recherches, que le fourgon est parti vers le nord par la corniche. |
| J 16 h 39 | Vidéo d'Inès : le fourgon au rond-point, sortie route des Étangs. |
| J 16 h 58 | Un autre fourgon blanc (modèle différent, échelle sur le toit) passe au péage de l'A63 ; un agent le signale à tort à 17 h 15. |
| J 16 h 48 | Caméra du Relais du Lac (station-service, 9 km) : fourgon blanc, feu clignotant arrière droit hors service. |
| J 17 h 05 | Arrivée à l'airial de Hount-Bielha, rive est de l'étang de Sorbe (propriété de la SCI des Pins de Sorbe, montée par l'étude Casteran). |
| J 17 h 35 | « Sandrine » photographie Lila, assise dans la pièce principale, fenêtre face à l'étang, soleil couchant dans l'axe. |
| J 17 h 40 | Nadia reçoit le message de chantage avec la photo. |
| J 19 h 30 → 2 h | Chapitre 2 : les recherches se resserrent sur la rive est. Transfert de Lila prévu à 2 h par barque vers la rive ouest. |
| J+1 1 h 10 (cible) | Lila retrouvée saine et sauve par les gendarmes guidés par l'enquête du duo. Loubère interpellé. « Sandrine » fuit par l'étang. |
| J+2 6 h | Appareillage prévu du *Maren Sofie* (enjeu du chapitre 3). |

### 6. Progression en actes 🟡

- **Acte I — Les premières heures** (prologue, chapitres 1 et 2) : de l'enlèvement à la libération de Lila. Unité de lieu : le village, la forêt, l'étang. Aucun voyage lointain tant que l'enfant n'est pas retrouvée.
- **Acte II — Les papiers** (chapitres 3 et 4) : Bayonne puis Porto. Comprendre ce que Nadia a découvert, qui fabrique les faux documents et qui est vraiment « Sandrine ».
- **Acte III — La façade** (chapitres 5 et 6) : Anvers, puis retour à Lescoure. Remonter la holding jusqu'à la personne qui a tout conçu, et la confondre avec des preuves recevables.

### 7. Conclusion 🟡

À Anvers, les statuts de *Nordhaven Shipping BV* renvoient à une succession de Lescoure ; la chaîne des identités recyclées mène à l'étude Casteran. De retour au village, {HEROINE} reconstitue ce que le joueur attentif a pu soupçonner dès le prologue : Casteran ne pouvait pas voir le carrefour depuis son seuil, elle a pourtant décrit une direction ; elle savait que Lila rentrait seule ; ses actes ont créé la SCI de l'airial et la SARL du fourgon.

La fin n'est pas une bagarre : c'est une **démonstration**. Le joueur assemble devant le capitaine Arbelot et le juge d'instruction une chaîne de preuves que la défense ne peut pas démonter ; la chienne joue un dernier rôle en retrouvant, dans la maison de Casteran, la cache des registres d'identités — une découverte qui, cette fois, se fait **avec un mandat** et en présence des enquêteurs. Casteran est mise en examen. Nadia est réintégrée ; Lila revient saluer la chienne à la sortie de l'école.

Variante de ton à trancher : fin amère (le *Typographe* reste introuvable, le réseau repoussera ailleurs) ou fin pleinement résolue. Recommandation : fin résolue pour Casteran, ouverte pour le *Typographe*, ce qui laisse une suite possible.

### 8. Indices plantés dès le prologue (fair-play)

| Graine | Où | Payée au chapitre |
|---|---|---|
| Casteran décrit une direction qu'elle ne pouvait pas voir | Énigme E3, ligne de vue | 6 |
| « Sandrine » connaît le prénom de Nadia et l'itinéraire de Lila | Énigme E1, témoignage Lila (ch. 2) | 2 puis 6 |
| Le fourgon appartient à une SARL récente | Énigme E4 (appel blanchisserie) | 3 puis 6 |
| L'airial appartient à une SCI | Chapitre 2 | 5 puis 6 |
| Casteran « s'occupe » de Nadia avec insistance | Dialogues ch. 1 | 6 |
