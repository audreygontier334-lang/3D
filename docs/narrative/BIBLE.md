# Bible narrative — « Faux-semblants » (titre provisoire)

> **Statut : proposition de Claude.** Les éléments validés par Audrey sont signalés ✅ (dont ses réponses du 29/09 à Q1, Q2 et Q3, voir `docs/VALIDATION_AUDREY_ACTE1.md`). Tout le reste est marqué 🟡 *proposition* et reste modifiable.
>
> Noms : la chienne s'appelle **Ariane** (décision d'Audrey), écrit en clair. Seule l'héroïne n'a pas encore de nom de jeu : les textes utilisent le jeton `{HEROINE}`, remplacé par le moteur. Les autres noms sont provisoires.

---

## 1. Résumé public (sans spoilers)

Un mardi de fin septembre, à Lescoure-Plage 🟡 — une petite ville côtière fictive entre océan, dunes et forêt de pins —, une femme se promène avec sa chienne Ariane dans le centre-ville ✅ (centre inspiré de celui d'Arcachon, lumière chaude de fin d'après-midi ✅). À la sortie de l'école, une fillette du quartier part seule vers la ruelle voisine ✅. Quelques instants plus tard, dans cette ruelle plus isolée ✅, la femme la voit monter dans un fourgon avec une inconnue. Quelques secondes après, le fourgon a disparu ✅.

Elle n'est ni policière ni détective. Mais elle a vu la scène, sa chienne connaît l'odeur de l'enfant, et les premières heures comptent. Aux côtés des gendarmes, elle interroge les témoins, confronte des versions qui ne concordent pas, suit les pistes que flaire sa chienne et apprend à distinguer ce qu'elle a vu de ce qu'elle croit avoir vu.

L'enlèvement n'avait rien d'un hasard. Le père de la fillette cache quelque chose, la mère ne comprend rien à ce qui arrive, et chaque piste semble avoir un temps de retard. Des pinèdes de l'étang aux quais de Bayonne, puis jusqu'à Anvers, l'enquête la ramènera là où tout a commencé — dans une ville où les gens les plus rassurants ne sont pas forcément ceux qu'ils paraissent.

**Thème** 🟡 : *les apparences qui rassurent*. Un badge officiel, un fourgon d'artisan, une plaque d'immatriculation, un témoin respectable, un père qui se tait, un homme qui console : chaque chapitre met le joueur face à une façade crédible qu'il faut vérifier plutôt que croire. La chienne, elle, ne juge pas sur les apparences — mais son flair ne suffit jamais seul.

**Ton** 🟡 : thriller d'enquête réaliste et tendu, pour adultes. Aucune violence graphique envers l'enfant, aucune scène complaisante. La tension vient du temps qui passe, des témoignages contradictoires et du danger ressenti, pas du spectacle de la souffrance.

---

## 2. Le duo ✅ (caractéristiques validées) / 🟡 (caractérisation proposée)

### {HEROINE}
- ✅ Femme brune d'une trentaine d'années, jean slim taille basse, tee-shirt, veste en jean, bandeau noir à motifs paisley blancs, créoles, baskets noires et blanches style Nike Air Max (sans logo ni nom de marque dans les assets) ; attitude naturelle et expression adaptée à la scène (référence : `docs/DECISIONS.md`, PR #3).
- 🟡 Habite Lescoure depuis plusieurs années ; connaît les commerçants, les habitudes du port, les véhicules qui passent. Elle croise souvent Lila, la fillette, qui caresse Ariane à la sortie de l'école.
- 🟡 Caractère : directe, obstinée, peu patiente avec les discours creux ; elle se méfie de ses propres impressions et apprend à les vérifier. Sa colère est un moteur, pas un défaut à punir.
- 🟡 Statut : **civile et témoin**. Elle ne procède à aucune arrestation, ne fouille pas illégalement, ne délivre aucun mandat. Son rôle : observer, relier, convaincre les autorités d'agir au bon endroit.
- ❓ Métier et passé : à trancher (voir `QUESTIONS_AUDREY.md`, Q5).

### Ariane
- ✅ Croisée malinois / bull terrier, environ 35 kg, beige fauve, masque et oreilles noirs, poitrail et bouts de pattes blancs, musclée, une oreille droite et l'autre tombante.
- ✅ Libre dès le départ : **ni laisse ni collier** ; elle porte seulement un foulard noir à motifs paisley blancs assorti au bandeau de {HEROINE}. Comportement naturel.
- 🟡 Chienne de compagnie **non dressée pour la police**, mais très éduquée : rappel, « au pied », « reste », « cherche », « montre ». Grande sensibilité aux émotions de sa maîtresse.
- 🟡 **Capacités plausibles** : suivre la piste fraîche d'une personne à pied à partir d'un objet de référence ; retrouver un objet porteur d'une odeur ; signaler une personne cachée à courte distance ; réagir à un individu qu'elle a déjà senti.
- 🟡 **Limites affichées au joueur** : elle ne suit pas un véhicule au-delà de quelques mètres ; la piste se dégrade avec le temps, la pluie, le bitume chaud et les passages nombreux ; elle peut se tromper de piste si l'objet de référence est contaminé ; elle ne « désigne » jamais un coupable. Une piste canine est une **orientation**, jamais une preuve juridique — les gendarmes le rappellent.
- 🟡 Langage corporel lisible (utilisé par les énigmes) : truffe basse et allure régulière = piste suivie ; tête haute = odeur portée par l'air ; cercles et hésitation = piste perdue ou croisement ; arrêt assis ou couché, regard vers {HEROINE} = découverte ; oreilles plaquées, poil hérissé = personne inconnue stressante.

---

## 3. Personnages secondaires

| ID | Nom provisoire | Rôle apparent | Ce qu'on découvre |
|---|---|---|---|
| `NPC_LILA` | Lila Mercadier, 9 ans, CM1 | La fillette enlevée ; rentre seule depuis la rentrée, fière de cette autonomie | ✅ Séquestrée sans violence, **retrouvée au chapitre 4** dans une cabane sombre et froide. Son témoignage prudent (recueilli par des professionnels) aide la suite |
| `NPC_NADIA` | Nadia Mercadier | ✅ Mère de Lila, séparée du père ; **arrive après l'enlèvement et ignore tout du trafic** | 🟡 Aide-soignante. Vit depuis un an avec Xavier Darrigade. Sa sincérité est totale : elle est manipulée sans le savoir |
| `NPC_JULIEN` | Julien Mercadier | ✅ Père de Lila, séparé de Nadia ; **mêlé au trafic, veut en sortir** | 🟡 Chauffeur pour Darrigade Logistique. Il a compris que les cartons contiennent des médicaments falsifiés et a voulu tout arrêter. Il reçoit le message de chantage et se tait, par peur |
| `NPC_DARRIGADE` | Xavier Darrigade | Compagnon de Nadia depuis un an ; patron de *Darrigade Logistique* (transport et entrepôts au port de Bayonne). Calme, serviable, il console Nadia et organise une battue | ✅ *Voir SPOILERS* |
| `NPC_MENDIONDO` | Adjudante-cheffe Carole Mendiondo | Commandante de la brigade locale de gendarmerie | Compétente, sceptique envers les civils mais pragmatique ; devient une alliée exigeante |
| `NPC_ARBELOT` | Capitaine Thomas Arbelot | Section de recherches, arrive au chapitre 2 | Dirige l'enquête judiciaire sur le réseau ; associe {HEROINE} comme témoin-clé |
| `NPC_DUFAU` | Marcel Dufau | Retraité bougon, ancien pêcheur ; chaque après-midi sur son banc du square avec ses mots croisés | Connaît tous les véhicules du coin ; témoin fiable mais qui exagère |
| `NPC_INES` | Inès Barrère, 15 ans | Ado au skatepark, filme ses figures | Sa vidéo contient le fourgon en arrière-plan, horodaté |
| `NPC_LARTIGUE` | Josiane Lartigue | Boulangère | A servi l'inconnue vingt minutes avant l'enlèvement |
| `NPC_CASTERAN` | Maître Hélène Casteran | Notaire respectée, sûre d'elle | 🟡 **Innocente.** Elle s'est trompée sur la direction du fourgon (elle a supposé). Son étude a rédigé, sans rien soupçonner, les statuts de sociétés créées pour Darrigade par des prête-noms : ses archives serviront de preuve à la fin |
| `NPC_K1` | Franck Loubère | « Le conducteur » | Ancien livreur licencié de la Blanchisserie Océane ; exécutant endetté, nerveux |
| `NPC_K2` | « Sandrine » (alias) | « La passagère », se fait passer pour une animatrice périscolaire | Passeuse professionnelle du réseau ; garde Lila ; s'échappe au chapitre 3 |

---

## ⚠️ SPOILERS — vérité cachée et fin

> Section réservée à Audrey, Codex et aux personnes qui implémentent. Ne pas afficher en jeu.

### 4. Le réseau adverse

**Ce qu'il fait** 🟡. Un réseau que ses membres appellent *l'Amarre* importe des **médicaments falsifiés** par les ports de l'Atlantique, sous couvert de **faux documents** (certificats d'origine, déclarations de conformité) et de sociétés de façade tenues par des prête-noms.

**Structure** 🟡.
- **Tête** ✅ : **Xavier Darrigade**, compagnon de Nadia. Sa société *Darrigade Logistique* (Bayonne) stocke et achemine la marchandise ; ses prête-noms possèdent les sociétés utiles (*Sud Loc Services* pour les véhicules, *SCI des Pins de Sorbe* et *SCI Lande-Haute* pour les planques).
- **Navire** : le caboteur *Maren Sofie*, rotation Bayonne → Anvers.
- **Faussaire** : un imprimeur surnommé *le Typographe*, qui travaille pour plusieurs réseaux.
- **Holding** : *Nordhaven Shipping BV*, société écran à Anvers.
- **Exécutants** : « Sandrine », passeuse professionnelle ; Franck Loubère, homme de main endetté.

**Motivations** 🟡. Darrigade : l'argent et le contrôle. Il s'est rapproché de Nadia il y a un an, **d'abord pour surveiller Julien**, son chauffeur, dont il se méfiait ; il a ensuite joué le rôle du compagnon parfait. Loubère agit par dettes et par peur. « Sandrine » est loyale au réseau qui l'a « recréée ».

**Pourquoi Lila** ✅. Julien Mercadier, chauffeur de Darrigade Logistique, a compris ce qu'il transportait et a annoncé qu'il arrêtait. On ne quitte pas l'Amarre : Darrigade fait enlever sa fille pour le forcer à faire **un dernier voyage** (conduire le camion jusqu'au *Maren Sofie*, jeudi 6 h) et à se taire. Darrigade savait tout de Lila : son trajet, l'heure de sortie, le prénom de sa mère. **Lila est un moyen de pression, jamais une cible de violence.** Mais le réseau n'a aucune intention de la rendre vite : elle est aussi la garantie que Julien ne parlera pas après le départ du navire.

**Pourquoi Julien se tait** 🟡. Il ne peut pas dénoncer Darrigade sans s'accuser lui-même ; Darrigade vit avec Nadia et sera au cœur de la cellule de crise ; et le message dit « pas de police ». Chaque fois que Darrigade s'approche, Julien se ferme — c'est l'indice le plus loyal du chapitre 1.

**Pourquoi l'enquête a toujours un temps de retard** 🟡. Nadia, sincère, raconte tout à Darrigade ; Darrigade reste près du poste de commandement et propose son aide. Il prévient le réseau au chapitre 2 (Lila est déplacée une heure avant l'arrivée des gendarmes) et au chapitre 3 (« Sandrine » échappe au piège du port).

### 5. Chronologie cachée 🟡

| Moment | Événement caché |
|---|---|
| J-1 an | Darrigade rencontre Nadia, l'ex-compagne de son chauffeur Julien. Ils s'installent ensemble six mois plus tard. |
| J-8 mois | La Blanchisserie Océane vend aux enchères un fourgon réformé. Acheteur : *Sud Loc Services*, SARL d'un prête-nom de Darrigade, statuts rédigés par l'étude Casteran (qui l'ignore). Loubère le récupère. |
| J-3 semaines | Julien ouvre un carton abîmé à l'entrepôt : boîtes de médicaments aux notices fausses. Il annonce à Darrigade qu'il arrête. |
| J-10 jours | À table, Nadia raconte que Lila rentre seule depuis la rentrée. Darrigade transmet l'itinéraire à « Sandrine ». |
| J-2 | Loubère pose de fausses plaques, clonées sur un fourgon identique appartenant à un plombier de Dax. Il retire le lettrage de la blanchisserie ; il en reste une ombre. |
| J 15 h 50 | Le fourgon passe devant l'école et se gare sur le bas-côté sablonneux de la ruelle des Tamaris, dans le sens de la descente. Dufau le remarque depuis son banc (pot d'échappement qui cogne). |
| J 16 h 12 | « Sandrine » achète une bouteille d'eau à la boulangerie et demande « l'école de la petite Mercadier ». Elle laisse la bouteille sur le banc de l'abribus. |
| J 16 h 22 | « Sandrine » quitte l'abribus et va attendre à une dizaine de mètres dans la ruelle, hors de vue du portail. |
| J 16 h 28 | Dans la ruelle, « Sandrine » aborde Lila : « Ta maman a eu un souci au travail, elle m'a demandé de te ramener. » Elle connaît le prénom de Nadia. Lila la suit, hésitante, puis refuse de monter tant qu'elle n'a pas « appelé maman » : « Sandrine » fait semblant de téléphoner à Nadia. |
| J 16 h 30 ✅ | Lila monte dans le fourgon (heure décidée par Audrey : vers 16 h 30). Son bracelet en perles casse et tombe au pied de la portière. Le cordon du badge s'accroche à la portière et tombe dans la haie. Le fourgon descend la ruelle, tourne sur le boulevard, puis prend au rond-point du Lac la route des Étangs. |
| J 16 h 30 | Casteran, sur son perron, voit le fourgon descendre la ruelle mais pas le boulevard ; elle **suppose** qu'il a tourné vers la Corniche et l'affirme de bonne foi. |
| J 16 h 31 | Vidéo d'Inès : le fourgon au rond-point, sortie route des Étangs. |
| J 16 h 40 | Caméra du Relais du Lac : le fourgon (plaque « …37-TR ») prend la fourche de l'étang de Sorbe. |
| J 16 h 50 | Un autre fourgon, d'un autre modèle, passe au péage de l'A63 ; un agent le signale à tort à 17 h 15. |
| J 16 h 57 | Arrivée à l'airial de Hount-Bielha, rive est de l'étang de Sorbe (SCI des Pins de Sorbe). |
| J 17 h 30 | Julien arrive de Bayonne ; Nadia et Darrigade arrivent à 17 h 35. |
| J 17 h 35 | « Sandrine » photographie Lila, fenêtre face à l'étang, soleil couchant dans l'axe. |
| J 17 h 40 | Julien reçoit le message : « Tu voulais partir. Un dernier voyage, jeudi 6 h, et tu la revois. Pas de police. Elle va bien. » avec la photo. |
| J fin du ch. 1 (≤ 19 h 30) | Le dispositif part vers l'étang de Sorbe. Darrigade, qui l'apprend devant le poste, prévient « Sandrine » par une messagerie chiffrée. |
| J, dès la fin du ch. 1 | Environ une heure avant l'arrivée des gendarmes (l'heure dépend de la résolution du chapitre 1), « Sandrine » transfère Lila en voiture vers une **cabane de résinier** abandonnée au fond de la forêt de Lande-Haute (SCI Lande-Haute), à 35 km. Loubère reste à l'airial pour effacer les traces. |
| J nuit | Chapitre 2 : les gendarmes, guidés par le duo, investissent l'airial : vide. Loubère interpellé ; il se tait. Indices du transfert. |
| J+1 | Chapitre 3 : Julien accepte de coopérer ; préparation d'une opération surveillée au port de Bayonne. |
| J+2 6 h | Opération au port : le camion, le *Maren Sofie*, « Sandrine » attendue. Prévenue, elle ne vient pas en personne ; son complice est arrêté ; son téléphone abandonné livre la zone de la cabane. |
| J+2 journée → soir | Chapitre 4 : battue dans la forêt de Lande-Haute, pluie froide. Lila est retrouvée vers 18 h, glacée, affamée et terrifiée, **sans aucune violence subie**. |

### 6. Progression en actes 🟡

- **Acte I — Lila** (prologue, chapitres 1 à 4) ✅ : de l'enlèvement à la libération de Lila, en un peu plus de deux jours. **Tout se passe dans la région** (Lescoure, étang de Sorbe, Bayonne, forêt landaise) : on ne part pas à l'étranger pendant qu'une enfant est captive.
- **Acte II — La façade** (chapitres 5 et 6) : Anvers, puis retour à Lescoure. Suivre la marchandise jusqu'à la holding, puis confondre la tête du réseau avec des preuves recevables.

### 7. Conclusion 🟡

À Anvers, les statuts de *Nordhaven Shipping BV* et les contrats de fret mènent à *Darrigade Logistique* par une chaîne de prête-noms. De retour à Lescoure, {HEROINE} rassemble ce qu'une joueuse attentive a pu soupçonner dès le chapitre 1 :
- Julien se taisait chaque fois que Darrigade approchait ;
- la « petite Mercadier » et son trajet n'étaient connus que du cercle familial ;
- les deux fuites (chapitres 2 et 3) ont suivi des informations que seule Nadia connaissait… et qu'elle racontait à Darrigade ;
- les archives de l'étude Casteran relient Sud Loc Services et les deux SCI aux mêmes prête-noms, payés par Darrigade.

La fin n'est pas une bagarre : c'est une **démonstration**, présentée au capitaine Arbelot et au juge d'instruction. Ariane joue un dernier rôle pendant la perquisition légale de l'entrepôt : elle marque un placard où Darrigade a gardé la veste portée à la cabane (odeur de résine et de Lila). Darrigade est mis en examen. Julien, qui a coopéré, est poursuivi mais protégé. Le choc le plus dur est pour Nadia ; la dernière scène réunit Lila, sa mère et Ariane à la sortie de l'école.

Variante à trancher plus tard : le *Typographe* reste introuvable (fin ouverte, suite possible) ou il est arrêté à Anvers.

### 8. Indices plantés dès le chapitre 1 (fair-play)

| Graine | Où | Payée au chapitre |
|---|---|---|
| Julien se ferme dès que Darrigade approche | Énigme PZ_08 | 4 puis 6 |
| Ariane garde ses distances avec Darrigade, oreilles plaquées (sans rien « désigner ») | Chapitre 1, place de l'Église | 6 |
| « Sandrine » connaît le prénom de Nadia et le trajet de Lila : qui savait ? | Témoins Lartigue et directrice ; Nadia | 2 puis 6 |
| Darrigade passe des appels « pour la battue » près du poste | Chapitre 1 | 2 (fuite) puis 6 |
| Le fourgon appartient à une SARL récente (Sud Loc Services) | Appel à la blanchisserie | 5 puis 6 |
| L'airial appartient à une SCI | Chapitre 2 | 5 puis 6 |
| Casteran s'est trompée de bonne foi : un témoin respectable n'est pas une preuve | Énigme PZ_03 | 6 (ses archives, elles, sont fiables) |
