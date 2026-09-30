# Dialogues et textes d'interface — Mission 01 (prologue et chapitre 1)

> **Statut : proposition non canonique.** Fichier **généré** depuis `GameData/dialogues/01-ouverture.json` par `Tools/render_dialogues.py` : modifier le JSON, puis relancer le script. Ne pas éditer ce fichier à la main.
>
> `{HEROINE}` est remplacé par le moteur (nom de la protagoniste à décider). Colonnes : **intention** = ce que la ligne doit accomplir dans le jeu ; **émotion** = indication pour la voix et l'animation.

## Locuteurs

| ID | Personnage |
|---|---|
| `HEROINE` | {HEROINE} |
| `CHIENNE` | Ariane (sons et comportement, jamais de parole) |
| `LILA` | Lila Mercadier |
| `K2` | La femme au badge |
| `DUFAU` | Marcel Dufau |
| `OPERATRICE` | Opératrice du 17 |
| `MENDIONDO` | Adjudante-cheffe Mendiondo |
| `GENDARME` | Gendarme (radio) |
| `LARTIGUE` | Josiane Lartigue |
| `DIRECTRICE` | Mme Pujol, directrice |
| `CASTERAN` | Maître Casteran |
| `INES` | Inès |
| `NADIA` | Nadia Mercadier, mère de Lila |
| `PARENT_INES` | Père d'Inès |
| `NARRATION` | Carnet (texte à l'écran, non voisé) |
| `JULIEN` | Julien Mercadier, père de Lila |
| `DARRIGADE` | Xavier Darrigade, compagnon de Nadia |

## `DLG_P_TUTO`

*Prologue, premières secondes. Textes d'aide non voisés.* — lieu : `LOC_PROMENADE`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_P_TUTO_01` | NARRATION | Mardi, fin septembre. 16 h 25. Lescoure-Plage. | Poser le lieu et l'heure | neutre |  |
| `DLG_P_TUTO_02` | NARRATION | Changer de vue : épaule, large, subjective. L'enquête ne dépend jamais de la vue choisie. | Tutoriel caméra | neutre |  |
| `DLG_P_TUTO_03` | NARRATION | Ordres à Ariane : Au pied · Reste · Cherche · Montre. | Tutoriel ordres | neutre |  |
| `DLG_P_TUTO_04` | HEROINE | Allez, Ariane, cherche ! … Bon, tu ne la rapportes pas, mais tu la trouves. C'est déjà ça. | Après le premier « Cherche » avec la balle ; établir la complicité | amusée, tendre |  |

## `DLG_P_DUFAU`

*Facultatif. {HEROINE} passe devant le banc de Dufau, dans le square des Tamaris.* — lieu : `LOC_BANC_DUFAU`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_P_DUFAU_01` | DUFAU | Tiens, les deux inséparables. Elle a encore pris du muscle, celle-là. | Accueil bourru ; Dufau connaît le duo | bougon, affectueux |  |
| `DLG_P_DUFAU_02` | HEROINE | Alors, ces mots croisés ? | Relance banale (Dufau fait ses mots croisés sur le banc) | détendue |  |
| `DLG_P_DUFAU_03` | DUFAU | Avec le boucan de casserole qu'il a fait en arrivant, l'autre, là ? Un fourgon, entré dans la ruelle il y a une bonne demi-heure. Je suis passé devant en venant : il fume dans sa cabine, vitre baissée. Pas moyen de me concentrer. | Première graine : le fourgon et son pot, sans insister. Dufau l'a vu en passant, pas depuis son banc (la ruelle est invisible depuis le square) | râleur |  |
| `DLG_P_DUFAU_04` | HEROINE | Vous exagérez. | Minimiser, comme le ferait n'importe qui | souriante |  |
| `DLG_P_DUFAU_05` | DUFAU | Moi ? Jamais. | Clore avec humour | pince-sans-rire |  |

## `DLG_P_LILA`

*EVT_LILA_COUCOU. Lila sort du portail de l'école et voit Ariane sur la place (plan A).* — lieu : `LOC_ECOLE`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_P_LILA_01` | LILA | Coucou Ariane ! Demain je t'apporte un biscuit ! | Rendre Lila attachante et établir qu'elle connaît Ariane (odeur familière) | joyeuse, fière de rentrer seule |  |
| `DLG_P_LILA_02` | HEROINE | Elle va te le rappeler, t'inquiète pas ! | Réponse si le joueur fait un geste | chaleureuse | FLAG_COUCOU_RENDU |
| `DLG_P_LILA_03` | CHIENNE | [remue la queue, petit jappement] | Lien chienne-enfant | enjouée |  |

## `DLG_P_ABORDAGE`

*EVT_ABORDAGE, dans la ruelle des Tamaris, à une dizaine de mètres de son entrée, hors de vue du portail. Répliques audibles seulement si {HEROINE} est à moins de 30 m ; sinon, gestes seuls. Les répliques 04 à 06 se disent devant la portière et se répètent en variantes (Lila négocie, la femme insiste) jusqu'à l'alerte : elles justifient que le fourgon ne parte pas tout de suite.* — lieu : `LOC_COIN_ECOLES`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_P_ABORDAGE_01` | K2 | Lila ? Bonjour ma grande. Ta maman a eu un souci au travail, elle m'a demandé de te ramener. | La ruse : prénom de l'enfant, référence à la mère | douce, trop douce |  |
| `DLG_P_ABORDAGE_02` | LILA | Mais… elle m'a dit de rentrer toute seule. | Hésitation de l'enfant, qui applique la consigne | hésitante |  |
| `DLG_P_ABORDAGE_03` | K2 | Je sais. C'est exceptionnel. Regarde, je suis du périscolaire. | Faux-semblant : le badge | rassurante, pressée |  |
| `DLG_P_ABORDAGE_04` | LILA | Je veux appeler maman d'abord. | Lila refuse de monter : elle applique la consigne de sa mère ; retient le fourgon jusqu'à l'alerte | têtue, inquiète |  |
| `DLG_P_ABORDAGE_05` | K2 | D'accord, regarde, je l'appelle… Nadia ? Oui, je l'ai. On arrive. … Elle est en réunion, ma puce, elle te rappelle. | Faux appel : elle connaît le prénom de la mère (indice pour DED_RUSE) ; le téléphone reste contre son oreille, écran invisible | douce, pressée |  |
| `DLG_P_ABORDAGE_06` | LILA | Je peux lui parler ? | Lila résiste encore ; variante répétable tant que l'alerte n'a pas eu lieu | méfiante |  |

## `DLG_P_ALERTE`

*EVT_ALERTE puis fenêtre d'action. Variantes selon ACT_CRIER / ACT_ENVOYER.* — lieu : `LOC_ACCOTEMENT`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_P_ALERTE_01` | LILA | Je veux attendre maman… Ariane ! | Déclencheur de l'alerte, visible et audible dans toutes les vues | peur montante |  |
| `DLG_P_ALERTE_02` | CHIENNE | [grognement sourd, oreille droite dressée, corps tendu] | La chienne perçoit le danger avant {HEROINE} | alerte |  |
| `DLG_P_ALERTE_03` | K2 | Monte, on va être en retard. | Pression, sans violence explicite | sèche |  |
| `DLG_P_ALERTE_04` | HEROINE | Lila ! | ACT_CRIER | cri, alarme | ACT_CRIER |
| `DLG_P_ALERTE_05` | K2 | Vas-y, vas-y ! | Réaction à ACT_CRIER : elle se retourne, visage visible, cordon arraché | paniquée | ACT_CRIER |
| `DLG_P_ALERTE_06` | HEROINE | Ariane, va ! | ACT_ENVOYER | ordre, urgence | ACT_ENVOYER |
| `DLG_P_ALERTE_07` | CHIENNE | [aboiements, sprint, arrêt net au bord de la chaussée, flaire le trottoir] | ACT_ENVOYER : Ariane, libre, s'arrête toujours au bord de la chaussée | furieuse puis concentrée | ACT_ENVOYER |

## `DLG_P_HEROINE_CHOC`

*EVT_HORS_VUE. Une ligne selon ce que le joueur a fait.* — lieu : `LOC_ACCOTEMENT`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_P_HEROINE_CHOC_01` | HEROINE | Non, non, non… Elle ne voulait pas monter. | Aucune action : sidération | choc, souffle court | aucun de : ACT_PHOTO, ACT_COURIR, ACT_CRIER, ACT_ENVOYER |
| `DLG_P_HEROINE_CHOC_02` | HEROINE | Je l'ai. Je l'ai, la plaque… en partie. | ACT_PHOTO immobile | tremblante, se raccroche au concret | CLU_PHOTO_FOURGON |
| `DLG_P_HEROINE_CHOC_03` | HEROINE | Floue… Elle est floue. Mais on voit quelque chose sur le côté. | ACT_PHOTO en course | haletante, rageuse | CLU_PHOTO_FLOUE |
| `DLG_P_HEROINE_CHOC_04` | HEROINE | Casquette grise. Barbe. Et ce bruit… c'est lui, le fourgon de tout à l'heure. | ACT_COURIR : mémoriser à voix haute | essoufflée, colère | ACT_COURIR |
| `DLG_P_HEROINE_CHOC_05` | HEROINE | Au pied. C'est bien, c'est bien… Tu l'as sentie, hein ? | ACT_ENVOYER : rappel d'Ariane à la voix | voix qui tremble, main posée sur Ariane | ACT_ENVOYER |
| `DLG_P_HEROINE_CHOC_06` | NARRATION | Appeler le 17. | Action principale mise en avant | neutre |  |

## `DLG_P_ARIANE`

*Réactions spontanées d'Ariane pendant la promenade et l'alerte (comportements décrits par Audrey le 01/10) ; déclenchées par le jeu, jamais indispensables.* — lieu : `—`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_P_ARIANE_GROGNE` | CHIENNE | [grognement sourd, oreilles et queue dressées, regard fixe] | Elle sent un danger, un intrus ou quelque chose d'anormal | à l'affût |  |
| `DLG_P_ARIANE_PROTEGE` | CHIENNE | [se place entre {HEROINE} et la menace, grognement bas, oreilles et queue dressées] | Très protectrice envers {HEROINE} et les enfants : elle s'interpose toujours | protectrice |  |
| `DLG_P_ARIANE_HERISSON_01` | CHIENNE | [grogne vers le massif, poils du dos hérissés] | Fausse alerte : un hérisson dans le square | à l'affût |  |
| `DLG_P_ARIANE_HERISSON_02` | HEROINE | Qu'est-ce que tu as vu ? … Un hérisson. Fausse alerte, ma belle. | La joueuse s'approche du massif après le grognement | amusée |  |
| `DLG_P_ARIANE_PARLE_01` | CHIENNE | [vient s'asseoir face à {HEROINE}, aboie une fois, couine et frétille] | Ariane vient « parler » | insistante |  |
| `DLG_P_ARIANE_PARLE_02` | CHIENNE | [grommelle, fait quelques pas vers la ruelle, s'arrête et se retourne] | Elle montre l'endroit où elle veut emmener {HEROINE} | insistante |  |
| `DLG_P_ARIANE_PARLE_03` | HEROINE | Quoi ? Tu veux aller par là ? | {HEROINE} comprend qu'Ariane veut l'emmener quelque part | intriguée |  |
| `DLG_P_ARIANE_PISTE` | CHIENNE | [part comme une flèche, truffe au sol] | Elle a trouvé une piste et s'élance sans prévenir | concentrée |  |
| `DLG_P_ARIANE_CONTENTE` | CHIENNE | [remue la queue et sautille] | Contente (friandise, balle, félicitations, coucou de Lila) | joyeuse |  |
| `DLG_P_ARIANE_TRISTE` | CHIENNE | [oreilles couchées en arrière, queue basse] | Triste ou grondée | penaude |  |
| `DLG_P_ARIANE_NON` | HEROINE | Non, Ariane ! | Ordre « Non » : elle s'arrête et revient, penaude | ferme |  |
| `DLG_P_ARIANE_BRAVO` | HEROINE | C'est bien, ma belle ! | Félicitations | tendre |  |
| `DLG_P_ARIANE_MONTRE_RIEN` | CHIENNE | [penche la tête, oreille droite dressée, et attend] | Ordre « Montre » sans rien à montrer | attentive |  |
| `DLG_P_ARIANE_SAUT` | CHIENNE | [prend son élan et saute par-dessus sans effort] | Elle franchit un obstacle haut (plus de 2 m) | à l'aise |  |
| `DLG_P_ARIANE_PORTE` | CHIENNE | [se dresse sur la poignée et ouvre la porte] | Elle sait ouvrir les portes | fière |  |

## `DLG_C1_OPERATRICE`

*INT_APPEL_17. Les options de description dépendent des indices déjà obtenus.* — lieu : `LOC_ACCOTEMENT`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_OPERATRICE_01` | OPERATRICE | Gendarmerie, j'écoute. | Ouverture | calme professionnel |  |
| `DLG_C1_OPERATRICE_02` | HEROINE | Une petite fille vient d'être emmenée dans un fourgon {FOURGON_COULEUR}. Lescoure-Plage, la ruelle des Tamaris, à côté de l'école. À l'instant. | Signalement | urgente, essaie de rester claire |  |
| `DLG_C1_OPERATRICE_03` | OPERATRICE | Vous êtes en sécurité ? Vous pouvez me décrire le véhicule et les personnes ? | Questions standard | posée |  |
| `DLG_C1_OPERATRICE_04` | HEROINE | Qu'est-ce que je dis ? | Menu de description | — |  |
| `DLG_C1_OPERATRICE_04A` | ↳ choix | « Un fourgon {FOURGON_COULEUR}, plaque qui commence par GF-4. » | → `DLG_C1_OPERATRICE_05` | | si CLU_PHOTO_FOURGON |
| `DLG_C1_OPERATRICE_04B` | ↳ choix | « Un homme au volant, casquette grise, barbe. Le pot cogne. » | → `DLG_C1_OPERATRICE_05` | | si ACT_COURIR |
| `DLG_C1_OPERATRICE_04C` | ↳ choix | « Une femme blonde, lunettes, gilet bleu, un badge. » | → `DLG_C1_OPERATRICE_05` | | si CLU_OBS_PASSAGERE |
| `DLG_C1_OPERATRICE_04D` | ↳ choix | « Un fourgon {FOURGON_COULEUR}, parti vers le bout de la ruelle. C'est tout ce que j'ai vu. » | → `DLG_C1_OPERATRICE_05` | |  |
| `DLG_C1_OPERATRICE_05` | OPERATRICE | C'est noté. Une patrouille arrive. Restez sur place, ne touchez à rien, et gardez votre téléphone allumé. | Clôture ; consigne de préserver la scène | ferme, rassurante |  |

## `DLG_C1_SCENE`

*INT_PROTEGER_SCENE. Des passants s'approchent du porte-clés.* — lieu : `LOC_COIN_ECOLES`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_SCENE_01` | HEROINE | S'il vous plaît, reculez. Ne touchez à rien, les gendarmes arrivent. | Protéger les traces ; bonus de confiance | autoritaire malgré elle |  |
| `DLG_C1_SCENE_02` | NARRATION | Les passants reculent. Le porte-clés est resté où il est tombé. | Retour | neutre |  |

## `DLG_C1_EXAMEN_ACCOTEMENT`

*INT_EXAMINER_ACCOTEMENT. Remarques de {HEROINE}.* — lieu : `LOC_ACCOTEMENT`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_EXAMEN_ACCOTEMENT_01` | HEROINE | Son porte-clés. Le renard. | CLU_PORTE_CLES_LILA | gorge serrée |  |
| `DLG_C1_EXAMEN_ACCOTEMENT_02` | HEROINE | Garé dans le sens de la descente. Et ça, c'est de l'huile fraîche. | CLU_TRACES_PNEUS | concentrée |  |
| `DLG_C1_EXAMEN_ACCOTEMENT_03` | HEROINE | Trois mégots. Il a attendu longtemps. | CLU_MEGOTS | froide |  |
| `DLG_C1_EXAMEN_ACCOTEMENT_04` | HEROINE | Une caisse de la criée… de Capbreton ? | CLU_CAISSE_POISSON (fausse piste) | intriguée |  |

## `DLG_C1_HAIE`

*INT_FOUILLER_HAIE (après ACT_CRIER ou ACT_ENVOYER).* — lieu : `LOC_ACCOTEMENT`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_HAIE_01` | HEROINE | C'est tombé là-dedans… Là. Un badge. | CLU_BADGE | tendue |  |
| `DLG_C1_HAIE_02` | HEROINE | « Accueil périscolaire, commune de Lescoure. Sandrine V. » Périscolaire, mon œil. | Lecture du badge ; soupçon | méprisante |  |

## `DLG_C1_MENDIONDO_ARRIVEE`

*EVT_GENDARMES_ARRIVENT. Première rencontre avec Mendiondo.* — lieu : `LOC_ACCOTEMENT`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_MENDIONDO_ARRIVEE_01` | MENDIONDO | Adjudante-cheffe Mendiondo. C'est vous qui avez appelé ? Racontez-moi, dans l'ordre. | Autorité, méthode | sèche, efficace |  |
| `DLG_C1_MENDIONDO_ARRIVEE_02` | HEROINE | Elle s'appelle Lila. Neuf ans. Une femme l'a fait monter dans un fourgon {FOURGON_COULEUR}. Elle ne voulait pas. | Déposition résumée (le détail est dans le carnet) | contenue |  |
| `DLG_C1_MENDIONDO_ARRIVEE_03` | MENDIONDO | D'accord. Le parquet est prévenu. Mais pour une alerte, il me faut un véhicule, des personnes et une direction. Pas des impressions. | Poser les trois axes du tableau | exigeante |  |
| `DLG_C1_MENDIONDO_ARRIVEE_04` | MENDIONDO | Notre maître-chien est à plus de deux heures. Votre chienne connaît la petite ? | Justifier la présence du duo | pragmatique, à contrecœur |  |
| `DLG_C1_MENDIONDO_ARRIVEE_05` | HEROINE | Elle la caresse tous les jours à la sortie de l'école. | Réponse | ferme |  |
| `DLG_C1_MENDIONDO_ARRIVEE_06` | MENDIONDO | Alors vous restez. Mais vous m'apportez ce que vous trouvez, et vous ne jouez pas les gendarmes. Ce que flaire votre chienne, c'est une piste. Pas une preuve. | Cadre légal et limites de la chienne | ferme, pas hostile |  |

## `DLG_C1_MENDIONDO_REPROCHE`

*BR_APPEL_TARDIF, à l'arrivée des gendarmes.* — lieu : `LOC_ACCOTEMENT`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_MENDIONDO_REPROCHE_01` | MENDIONDO | Vous avez attendu combien de temps avant d'appeler ? Chaque minute, c'est des kilomètres. | Conséquence narrative de l'appel tardif | reproche sec |  |
| `DLG_C1_MENDIONDO_REPROCHE_02` | HEROINE | Je sais. | Accepter sans se justifier | coupable, mâchoire serrée |  |

## `DLG_C1_DUFAU`

*INT_DUFAU. Sur son banc du square. Dufau est secoué mais reste lui-même.* — lieu : `LOC_BANC_DUFAU`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_DUFAU_01` | DUFAU | La petite Mercadier ? Bon Dieu… Je l'ai vu, votre fourgon, je vous l'ai dit. Il est passé devant l'école vers moins dix et il s'est garé dans la ruelle. | Heure d'arrivée ; Dufau fiable | choqué, bourru |  |
| `DLG_C1_DUFAU_02` | HEROINE | Le conducteur, vous l'avez vu ? | Relance | pressante |  |
| `DLG_C1_DUFAU_02A` | ↳ choix | Le conducteur ? | → `DLG_C1_DUFAU_03` | |  |
| `DLG_C1_DUFAU_02B` | ↳ choix | C'était une livraison ? | → `DLG_C1_DUFAU_05` | |  |
| `DLG_C1_DUFAU_02C` | ↳ choix | Et la plaque ? | → `DLG_C1_DUFAU_07` | |  |
| `DLG_C1_DUFAU_02D` | ↳ choix | Cette caisse de poisson, là ? | → `DLG_C1_DUFAU_08` | | si CLU_CAISSE_POISSON |
| `DLG_C1_DUFAU_03` | DUFAU | Quand je suis passé devant, vers quatre heures, il n'était pas sorti. Il fumait, vitre baissée, une cigarette après l'autre. Casquette grise. | Le conducteur n'est pas sorti (utile pour PZ_02) | précis |  |
| `DLG_C1_DUFAU_04` | DUFAU | Il m'a fait penser à Franck, l'ancien de la blanchisserie. Mais je n'ai plus mes yeux de vingt ans, hein. Je ne veux accuser personne. | Piste K1, avec prudence | hésitant, honnête |  |
| `DLG_C1_DUFAU_05` | DUFAU | Une livraison ? Pas la blanchisserie, en tout cas. Eux, c'est le mardi matin. Et leur camion a le nom écrit en gros dessus. Celui-là, rien d'écrit. | Écarter la blanchisserie actuelle ; graine du lettrage retiré | catégorique |  |
| `DLG_C1_DUFAU_06` | DUFAU | Et ce pot… Un cognement pareil, ça s'oublie pas. | Signature sonore | grimace |  |
| `DLG_C1_DUFAU_07` | DUFAU | Une plaque des Landes, le 40, ça je l'ai vu. Le reste… | Détail de plaque partiel | désolé |  |
| `DLG_C1_DUFAU_08` | DUFAU | Ça ? Le poissonnier en empile toujours là, il livre les restaurants. Rien à voir. | Désamorcer la fausse piste du port | haussement d'épaules |  |

## `DLG_C1_LARTIGUE`

*INT_LARTIGUE. Boulangerie. Josiane Lartigue a vu passer les gendarmes.* — lieu : `LOC_BOULANGERIE`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_LARTIGUE_01` | LARTIGUE | C'est vrai, pour la petite ? Oh mon Dieu. Elle passe tous les matins acheter un pain au chocolat. | Émotion ; explique la piste ancienne de Lila vers la boulangerie (PZ_01) | bouleversée |  |
| `DLG_C1_LARTIGUE_02` | HEROINE | Vous avez vu une femme blonde, avec un badge, cet après-midi ? | Question ciblée (disponible même sans CLU_OBS_PASSAGERE : « une femme avec un badge ») | douce mais pressée |  |
| `DLG_C1_LARTIGUE_03` | LARTIGUE | Oui ! Vers quatre heures dix. Une bouteille d'eau, payée en liquide. Elle m'a demandé où était l'école de la petite Mercadier. | Clé : elle connaissait le nom de famille ; elle ne connaissait pas l'école | se souvient en parlant |  |
| `DLG_C1_LARTIGUE_04` | LARTIGUE | Sur le coup, je me suis dit : une animatrice qui ne sait pas où est l'école… Et puis elle est allée s'asseoir à l'abribus. | Relier à la bouteille (PZ_02) | prise de conscience |  |

## `DLG_C1_LIEGE`

*INT_TABLEAU_LIEGE. {HEROINE} examine les annonces.* — lieu : `LOC_BOULANGERIE`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_LIEGE_01` | HEROINE | Cours de surf, chiots à donner… Deux blanchisseries. | Présenter les deux prospectus | concentrée |  |
| `DLG_C1_LIEGE_02` | HEROINE | « Notre équipe de livraison. » Tiens. | Attirer l'attention sur la photo d'équipe | intriguée |  |

## `DLG_C1_ABRIBUS`

*INT_BANC_ABRIBUS.* — lieu : `LOC_ABRIBUS`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_ABRIBUS_01` | HEROINE | Une bouteille à peine entamée. Du rouge à lèvres sur le goulot. | CLU_BOUTEILLE | attentive |  |
| `DLG_C1_ABRIBUS_02` | HEROINE | Ticket de la boulangerie. Seize heures douze. | CLU_TICKET_BOULANGERIE | attentive |  |
| `DLG_C1_ABRIBUS_03` | HEROINE | Dans un sac. Personne d'autre n'y touche. | Geste de préservation de l'odeur | méthodique |  |

## `DLG_C1_PLAN`

*INT_PLAN_TOURISTIQUE.* — lieu : `LOC_ABRIBUS`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_PLAN_01` | NARRATION | Distances depuis le rond-point notées dans le carnet. | CLU_PLAN_DISTANCES | neutre |  |

## `DLG_C1_AFFICHE`

*INT_AFFICHE_ECOLE.* — lieu : `LOC_ECOLE`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_AFFICHE_01` | HEROINE | Le nouveau logo de la mairie. La vague bleue. | CLU_AFFICHE_COMMUNE ; comparaison possible avec le badge | neutre |  |
| `DLG_C1_AFFICHE_02` | HEROINE | Sur le badge, c'est l'ancien. Le pin et le soleil. | Comparaison si CLU_BADGE est déjà en main | le déclic | CLU_BADGE |

## `DLG_C1_DIRECTRICE`

*INT_DIRECTRICE. Mme Pujol, à la grille, livide.* — lieu : `LOC_ECOLE`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_DIRECTRICE_01` | DIRECTRICE | Les gendarmes m'ont prévenue. Je… Lila est partie à l'heure, seule, comme depuis la rentrée. Sa mère avait signé l'autorisation. | Lila rentre seule depuis peu | sous le choc, se justifie |  |
| `DLG_C1_DIRECTRICE_02` | HEROINE | Vous avez une animatrice qui s'appelle Sandrine ? | Vérifier l'identité du badge | directe |  |
| `DLG_C1_DIRECTRICE_03` | DIRECTRICE | Sandrine ? Non. Nous avons trois animatrices, je les connais toutes. Aucune Sandrine. | Clé : la femme n'est pas animatrice | certaine |  |
| `DLG_C1_DIRECTRICE_04` | DIRECTRICE | Et peu de gens savaient qu'elle rentrait seule. Sa mère, moi, la maîtresse… et le voisinage, sans doute. | Graine : qui savait ? | réfléchit à voix haute |  |

## `DLG_C1_CASTERAN`

*INT_CASTERAN. Casteran, notaire respectée, s'est approchée de la scène. Elle est sincère.* — lieu : `LOC_ETUDE_CASTERAN`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_CASTERAN_01` | CASTERAN | Quelle horreur. Cette petite, je la vois passer tous les jours devant l'étude. | Témoin respectable, sincère | émue, maîtrisée |  |
| `DLG_C1_CASTERAN_02` | CASTERAN | J'étais sur mon perron, j'ai tout vu. Il est descendu jusqu'au bout de la ruelle et il a tourné à droite, vers la Corniche. J'en suis certaine. | Témoignage de bonne foi mais faux : elle a supposé la direction (fausse piste loyale) | assurée, posée |  |
| `DLG_C1_CASTERAN_03` | HEROINE | Vers la Corniche ? Vous êtes sûre ? | Doute | prudente |  |
| `DLG_C1_CASTERAN_03A` | ↳ choix | Merci, maître. Je le dis aux gendarmes. | → `DLG_C1_CASTERAN_04` | |  |
| `DLG_C1_CASTERAN_03B` | ↳ choix | D'ici, on voit vraiment de quel côté il a tourné ? | → `DLG_C1_CASTERAN_05` | |  |
| `DLG_C1_CASTERAN_04` | CASTERAN | Faites, faites. Et dites-leur qu'ils peuvent compter sur moi. | Clôture aimable | courtoise |  |
| `DLG_C1_CASTERAN_05` | CASTERAN | Ma chère, j'ai soixante-deux ans, pas quatre-vingt-dix. Je sais ce que j'ai vu. | Se draper dans son autorité ; ouvre le test de ligne de vue | piquée, souriante |  |

## `DLG_C1_LIGNE_DE_VUE`

*INT_LIGNE_DE_VUE. {HEROINE} se place sur le perron.* — lieu : `LOC_ETUDE_CASTERAN`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_LIGNE_DE_VUE_01` | HEROINE | On voit la ruelle jusqu'au bout… et après, plus rien. Le boulevard est caché par les maisons. Le rond-point, n'en parlons pas. | Constat spatial | lente, surprise |  |
| `DLG_C1_LIGNE_DE_VUE_02` | HEROINE | Elle ne pouvait pas savoir quelle sortie il a prise. Elle a supposé. Ou elle s'est trompée. | Conclusion prudente (pas d'accusation) | pensive |  |

## `DLG_C1_INES`

*INT_INES / PZ_04. Inès est assise sur sa planche, téléphone en main.* — lieu : `LOC_SKATEPARK`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_INES_01` | INES | Quoi ? J'ai rien fait, moi. | Méfiance adolescente | sur la défensive |  |
| `DLG_C1_INES_02` | HEROINE | Tu filmes le rond-point depuis tout à l'heure ? | Question | calme |  |
| `DLG_C1_INES_03` | INES | Je filme mes figures. Et si mes parents voient que j'étais pas à l'étude, je suis morte. | Obstacle | inquiète |  |
| `DLG_C1_INES_03A` | ↳ choix | « Personne ne te reprochera d'avoir aidé. Je te le promets. » | → `DLG_C1_INES_04` | | effet : succès |
| `DLG_C1_INES_03B` | ↳ choix | « Une petite fille a été enlevée. Tu as peut-être la seule image du fourgon. » | → `DLG_C1_INES_05` | | effet : succès |
| `DLG_C1_INES_03C` | ↳ choix | « Si tu ne me montres pas, les gendarmes vont prendre ton téléphone. » | → `DLG_C1_INES_06` | | effet : BR_INES_BRAQUEE |
| `DLG_C1_INES_04` | INES | … OK. Mais c'est vous qui leur dites. | Accord | soulagée |  |
| `DLG_C1_INES_05` | INES | Enlevée ? Attendez… Il y avait un fourgon, oui. Regardez. | Accord, prise de conscience | choquée |  |
| `DLG_C1_INES_06` | INES | Ben qu'ils viennent, alors. Moi je me casse. | Échec : Inès part (rattrapage à 19 h) | vexée, effrayée |  |
| `DLG_C1_INES_07` | HEROINE | Tu peux me l'envoyer ? Et la garder. Surtout, ne l'efface pas. | Obtention de CLU_VIDEO_INES | reconnaissante | résultat = succes |

## `DLG_C1_INES_PARENTS`

*EVT_INES_PARENTS, 19 h 00, au poste.* — lieu : `LOC_POSTE`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_INES_PARENTS_01` | PARENT_INES | Ma fille m'a dit qu'elle avait filmé quelque chose. On vient vous le montrer. | Rattrapage de la vidéo | grave |  |

## `DLG_C1_PISTE_LILA`

*INT_PISTE_LILA / PZ_01. {HEROINE} fait sentir le porte-clés de Lila à Ariane (objet senti après le départ du fourgon).* — lieu : `LOC_COIN_ECOLES`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_PISTE_LILA_01` | HEROINE | Sens. C'est Lila. Cherche Lila. | Lancer la piste | douce, grave |  |
| `DLG_C1_PISTE_LILA_02` | HEROINE | Elle tourne… Elles se sont arrêtées ici. Elles ont parlé. | Point A (si bonne interprétation) | à voix basse |  |
| `DLG_C1_PISTE_LILA_03` | HEROINE | Non, pas la boulangerie. Ça, c'est ce matin. | Point A bis : écarter la piste ancienne | concentrée |  |
| `DLG_C1_PISTE_LILA_04` | HEROINE | Tout droit. Pas d'écart. Elle marchait normalement… Elle l'a suivie. | Point B : pas de lutte | douleur contenue |  |
| `DLG_C1_PISTE_LILA_05` | HEROINE | Qu'est-ce que tu as trouvé ? … Son bracelet. | Point C : CLU_BRACELET_LILA | émue |  |
| `DLG_C1_PISTE_LILA_06` | HEROINE | C'est fini, hein ? Elle est montée là. Tu ne peux pas suivre un moteur. C'est bien, ma belle, c'est bien. | Point D : limite de la chienne, récompense | tendre, frustrée |  |

## `DLG_C1_PISTE_BOUTEILLE`

*INT_PISTE_BOUTEILLE / PZ_02. Variantes selon l'objet choisi.* — lieu : `LOC_ABRIBUS`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_PISTE_BOUTEILLE_01` | HEROINE | Sens bien. Cherche. | Lancer la piste avec l'objet choisi | déterminée |  |
| `DLG_C1_PISTE_BOUTEILLE_02` | HEROINE | Elle ne va nulle part… Il n'est pas sorti du fourgon, c'est ça ? | Échec mégots : piste de 2 m | frustrée | objet choisi = CLU_MEGOTS |
| `DLG_C1_PISTE_BOUTEILLE_03` | HEROINE | Tu repars vers le coin… C'est Lila, ça. Ce n'est pas ce que je cherche. | Échec porte-clés : relance de la piste de Lila | patiente | objet choisi = CLU_PORTE_CLES_LILA |
| `DLG_C1_PISTE_BOUTEILLE_04` | HEROINE | L'abribus, le coin, le fourgon… la haie. Qu'est-ce qu'il y a là-dedans ? | Réussite : piste de la femme jusqu'à la haie | tendue | objet choisi = CLU_BOUTEILLE |
| `DLG_C1_PISTE_BOUTEILLE_05` | HEROINE | Un badge. « Sandrine V. » Tu l'as trouvée, Ariane. | CLU_BADGE | victoire sombre | objet choisi = CLU_BOUTEILLE |

## `DLG_C1_FOUILLE_HAIE`

*EVT_FOUILLE_HAIE, 18 h 30, rattrapage.* — lieu : `LOC_ACCOTEMENT`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_FOUILLE_HAIE_01` | MENDIONDO | On a trouvé un badge dans la haie. Une certaine « Sandrine ». Venez voir. | Rattrapage du badge | factuelle |  |

## `DLG_C1_PLAQUE`

*INT_PLAQUE_* / PZ_05. Au poste, formulaire de consultation.* — lieu : `LOC_POSTE`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_PLAQUE_01` | MENDIONDO | Donnez-moi ce que vous avez. Des points d'interrogation là où vous ne savez pas. | Saisie de la plaque | méthodique |  |
| `DLG_C1_PLAQUE_02` | MENDIONDO | GF-437-TR. Fourgon du même modèle… à un plombier de Dax. | Résultat | neutre |  |
| `DLG_C1_PLAQUE_03` | MENDIONDO | Son traceur le met chez un client, à Dax, à seize heures trente-huit. À cinquante kilomètres. | Innocenter le plombier | sourcils froncés |  |
| `DLG_C1_PLAQUE_04` | HEROINE | Alors ce n'est pas sa plaque. C'est une copie. | DED_PLAQUE_CLONEE | le déclic |  |
| `DLG_C1_PLAQUE_05` | MENDIONDO | Une plaque clonée. Ce ne sont pas des amateurs. | Monter les enjeux | grave |  |

## `DLG_C1_BLANCHISSERIE`

*INT_APPEL_BLANCHISSERIE. Mendiondo appelle, puis rapporte.* — lieu : `LOC_POSTE`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_BLANCHISSERIE_01` | MENDIONDO | La Blanchisserie Océane a vendu un fourgon réformé aux enchères, il y a huit mois. Acheteur : Sud Loc Services. Une SARL toute neuve. | Origine du fourgon ; graine de la SARL | prend des notes |  |
| `DLG_C1_BLANCHISSERIE_02` | MENDIONDO | Et celui qui est venu le chercher, ils s'en souviennent : Franck Loubère. Un ancien livreur. Licencié l'an dernier. | Identité probable de K1 | tendue |  |
| `DLG_C1_BLANCHISSERIE_03` | HEROINE | Casquette grise ? Il est sur la photo du prospectus. | Relier au flyer (DED_K1_LOUBERE) | vive | CLU_FLYER_BLANCHISSERIE |
| `DLG_C1_BLANCHISSERIE_04` | MENDIONDO | Leurs fourgons actuels étaient tous au dépôt cet après-midi. Ils livrent le mardi matin. | Contredire H_VEH_BLANCHISSERIE_ACTIVE | neutre |  |

## `DLG_C1_RELAIS`

*INT_CCTV_RELAIS. Après la vidéo d'Inès.* — lieu : `LOC_POSTE`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_RELAIS_01` | HEROINE | La route des Étangs passe devant le Relais du Lac. Ils ont une caméra à la pompe. | Suggestion du joueur | pressante |  |
| `DLG_C1_RELAIS_02` | MENDIONDO | Seize heures quarante-huit. Un fourgon qui finit en 37-TR, clignotant droit mort. Il prend la fourche de l'étang de Sorbe. | CLU_CCTV_RELAIS | grave, énergique |  |

## `DLG_C1_RADIO_PEAGE`

*EVT_RADIO_PEAGE, 17 h 15. Audible près du poste, reporté dans le carnet sinon.* — lieu : `LOC_POSTE`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_RADIO_PEAGE_01` | GENDARME | Péage de l'A63 : un agent signale un fourgon correspondant au signalement à seize heures cinquante-huit, direction Espagne. Pas de plaque relevée. | Fausse piste loyale (PZ_07) | radio, grésillements |  |
| `DLG_C1_RADIO_PEAGE_02` | MENDIONDO | L'Espagne… Si c'est ça, on a déjà perdu une heure. | Rendre la fausse piste crédible | inquiète |  |

## `DLG_C1_CARTE`

*INT_CARTE_ETANG. Carte murale du poste.* — lieu : `LOC_POSTE`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_CARTE_01` | HEROINE | Rive ouest, la base nautique. Rive est, les vieilles forêts de gemmage. | CLU_CARTE_ETANG | pensive |  |
| `DLG_C1_CARTE_02` | HEROINE | Le soleil en face, sur l'eau… la fenêtre regarde l'ouest. Elle est sur la rive est. | PZ_09 résolu | le déclic | DED_RIVE_EST |

## `DLG_C1_PERE_ARRIVEE`

*EVT_PERE_ARRIVE, 17 h 30. Julien arrive de Bayonne, en tenue de travail.* — lieu : `LOC_POSTE`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_PERE_ARRIVEE_01` | JULIEN | Lila… C'est vrai ? Qui a fait ça ? Qui ? | Arrivée du père ; colère et panique | hors de lui |  |
| `DLG_C1_PERE_ARRIVEE_02` | MENDIONDO | Monsieur Mercadier ? On fait tout ce qu'on peut. Restez près du poste et gardez votre téléphone allumé. | Cadre ; le téléphone deviendra central | ferme |  |

## `DLG_C1_NADIA_ARRIVEE`

*EVT_NADIA_ARRIVE, 17 h 35. Nadia arrive avec Xavier Darrigade, son compagnon.* — lieu : `LOC_POSTE`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_NADIA_ARRIVEE_01` | NADIA | Où est ma fille ? Où est Lila ? | Arrivée ; douleur brute sans surjeu ; elle ne sait rien | panique |  |
| `DLG_C1_NADIA_ARRIVEE_02` | MENDIONDO | Madame Mercadier, on la cherche. Tous. Asseyez-vous, j'ai besoin de vous. | Autorité bienveillante | ferme, humaine |  |
| `DLG_C1_NADIA_ARRIVEE_03` | DARRIGADE | Xavier Darrigade, son compagnon. Dites-moi ce qu'il vous faut : des voitures, des bras, j'appelle tout le monde. | Darrigade s'impose comme soutien idéal (graine, sans insistance) | calme, efficace, rassurant |  |
| `DLG_C1_NADIA_ARRIVEE_04` | CHIENNE | [s'arrête à trois mètres de Darrigade, oreilles plaquées, et détourne la tête de la main qu'il lui tend] | Graine ambiguë : Ariane ne désigne personne, elle garde ses distances | méfiante |  |
| `DLG_C1_NADIA_ARRIVEE_05` | DARRIGADE | Elle a peur, la pauvre bête. Il y a trop de monde. | Il donne lui-même une explication plausible | sourire désarmant |  |

## `DLG_C1_NADIA`

*INT_NADIA_TEMOIGNAGE, à partir de 17 h 40. Nadia, sincère, ne sait rien du trafic.* — lieu : `LOC_POSTE`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_NADIA_01` | NADIA | C'est vous, la dame avec Ariane ? Lila n'arrête pas de parler d'elle. | Ouverture ; lien avec la chienne | épuisée |  |
| `DLG_C1_NADIA_02` | HEROINE | Qui savait qu'elle rentrait seule ? | Question clé | douce, précise |  |
| `DLG_C1_NADIA_03` | NADIA | Personne, presque. C'est elle qui a insisté, à la rentrée. L'école, moi… Julien, son père. Et Xavier. C'est tout. | Cercle proche (graine) | réfléchit en parlant |  |
| `DLG_C1_NADIA_04` | HEROINE | Et Julien, vous êtes encore proches ? | Relance | neutre |  |
| `DLG_C1_NADIA_04A` | ↳ choix | « Il avait des ennuis, ces temps-ci ? » | → `DLG_C1_NADIA_05` | |  |
| `DLG_C1_NADIA_04B` | ↳ choix | « Je vous laisse vous reposer. » | → `DLG_C1_NADIA_07` | |  |
| `DLG_C1_NADIA_05` | NADIA | On est séparés depuis trois ans. Il est chauffeur chez Xavier, à l'entrepôt de Bayonne. Ces dernières semaines, il voulait démissionner. Il ne m'a pas dit pourquoi. | Julien lié à Darrigade Logistique ; il voulait partir | perdue |  |
| `DLG_C1_NADIA_06` | NADIA | Vous croyez que ça a un rapport ? Julien ne ferait jamais de mal à Lila. | Sincérité ; elle ne comprend pas | inquiète, défend le père |  |
| `DLG_C1_NADIA_07` | HEROINE | On va la retrouver. | Clôture | promesse à mi-voix |  |

## `DLG_C1_PERE_OBSERVATION`

*INT_OBSERVER_PERE, entre 17 h 40 et 17 h 55, à moins de 15 m.* — lieu : `LOC_POSTE`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_PERE_OBSERVATION_01` | MENDIONDO | Un problème, monsieur ? | Julien vient de lire son téléphone | attentive |  |
| `DLG_C1_PERE_OBSERVATION_02` | JULIEN | Rien. Le boulot. | Mensonge visible | blanc, voix cassée |  |
| `DLG_C1_PERE_OBSERVATION_03` | CHIENNE | [va se coller contre les jambes de Julien, gémit doucement] | Ariane signale la détresse | inquiète |  |
| `DLG_C1_PERE_OBSERVATION_04` | DARRIGADE | Julien. Tiens bon. On va la retrouver. | Main sur l'épaule ; Julien se raidit et ne dit plus rien (graine clé) | chaleureux en apparence |  |

## `DLG_C1_ELOIGNER_DARRIGADE`

*INT_ELOIGNER_DARRIGADE.* — lieu : `LOC_POSTE`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_ELOIGNER_DARRIGADE_01` | HEROINE | Monsieur Darrigade, les gendarmes cherchent des volontaires pour la battue de demain. Vous pourriez voir avec eux ? | Éloigner Darrigade de Julien sans l'affronter | neutre, calculée |  |
| `DLG_C1_ELOIGNER_DARRIGADE_02` | DARRIGADE | Bien sûr. J'y vais. Julien, je reviens. | Il s'éloigne ; la phrase sonne comme un avertissement pour qui écoute bien | aimable |  |

## `DLG_C1_DARRIGADE_APPELS`

*EVT_DARRIGADE_S_ELOIGNE, 18 h 05.* — lieu : `LOC_POSTE`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_DARRIGADE_APPELS_01` | DARRIGADE | Je passe quelques coups de fil pour la battue. Nadia, ne bouge pas. | Il s'éloigne de lui-même, téléphone à l'oreille près du poste (graine : les fuites) | affairé |  |

## `DLG_C1_PERE`

*INT_PERE / PZ_08. Près de la fontaine, Darrigade éloigné.* — lieu : `LOC_POSTE`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_PERE_01` | JULIEN | Vous êtes la dame avec Ariane… Lila n'arrête pas de parler d'elle. | Ouverture ; lien avec la chienne | épuisé |  |
| `DLG_C1_PERE_02` | HEROINE | Qu'est-ce que je lui dis ? | Choix d'approche | — |  |
| `DLG_C1_PERE_02A` | ↳ choix | « Cette femme a demandé ‹ la petite Mercadier ›. C'est votre nom. Ce n'était pas un hasard. Qu'est-ce qu'on vous demande ? » | → `DLG_C1_PERE_03` | | si CLU_TEMOIN_LARTIGUE; effet : succès |
| `DLG_C1_PERE_02B` | ↳ choix | « Elle s'est fait passer pour une animatrice, elle savait comment rassurer Lila. Quelqu'un vous vise. Qu'est-ce qu'on vous demande ? » | → `DLG_C1_PERE_03` | | si l'un de : CLU_BADGE, CLU_TEMOIN_DIRECTRICE, DED_RUSE; effet : succès |
| `DLG_C1_PERE_02C` | ↳ choix | « Vous nous mentez. Vous êtes dans le coup ? » | → `DLG_C1_PERE_09` | | effet : BR_PERE_BRUSQUE |
| `DLG_C1_PERE_03` | JULIEN | Ils ont écrit « pas de police ». Si je parle… | Aveu partiel | terrifié |  |
| `DLG_C1_PERE_04` | HEROINE | La police est déjà là. Ce qui la protège, c'est qu'on sache où chercher. | Argument décisif | douce, ferme |  |
| `DLG_C1_PERE_05` | JULIEN | Ils veulent que je fasse un dernier voyage. Jeudi, six heures. Après, je la revois. … Il y a une photo. | Révélation du chantage ; CLU_MESSAGE_CHANTAGE, CLU_PHOTO_VIE | s'effondre en parlant |  |
| `DLG_C1_PERE_06` | HEROINE | Un voyage de quoi ? Pour qui ? | Relance | pressante |  |
| `DLG_C1_PERE_07` | JULIEN | Je peux pas. Pas ici. Montrez la photo aux gendarmes, c'est tout ce que je peux faire. | Il se tait sur le commanditaire ; son regard file vers Darrigade, au bout de la place (graine) | peur panique, retenue |  |
| `DLG_C1_PERE_08` | HEROINE | Elle a l'air d'aller bien. Regardez-moi : elle a l'air d'aller bien. On va la chercher. | Réconfort ; transmettre la photo à Mendiondo | voix qui tient |  |
| `DLG_C1_PERE_09` | JULIEN | Dégagez ! Vous êtes même pas flic ! | Échec : Julien se ferme (rattrapage 18 h 45) | colère, larmes |  |

## `DLG_C1_PERE_CRAQUE`

*EVT_PERE_CRAQUE, 18 h 45, si Julien n'a pas parlé.* — lieu : `LOC_POSTE`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_PERE_CRAQUE_01` | JULIEN | Adjudante… J'ai reçu un message. Tenez. Je peux rien dire de plus. | Rattrapage : message et photo accessibles au poste ; il reste muet sur le reste | effondré |  |

## `DLG_C1_TABLEAU`

*INT_PRESENTER_TABLEAU / PZ_10. Mendiondo évalue chaque colonne.* — lieu : `LOC_POSTE`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_TABLEAU_01` | MENDIONDO | Allez-y. Véhicule, personnes, direction. Et pour chaque point, sur quoi vous vous appuyez. | Ouverture de la présentation | concentrée |  |
| `DLG_C1_TABLEAU_02` | MENDIONDO | Sur quoi vous vous appuyez ? Je ne peux pas envoyer des hommes sur une intuition. | Hypothèse sans preuve épinglée (pas de pénalité) | agacée, juste |  |
| `DLG_C1_TABLEAU_03` | MENDIONDO | La patrouille de la Corniche n'a rien. Un camping-car, c'est tout. | BR_ERR_NORD | tendue |  |
| `DLG_C1_TABLEAU_04` | MENDIONDO | La vidéo du péage : c'est un autre modèle, avec une échelle sur le toit. On a perdu une demi-heure. | BR_ERR_A63 | dure |  |
| `DLG_C1_TABLEAU_05` | MENDIONDO | La capitainerie n'a vu aucun fourgon. Et le poissonnier dépose toujours ses caisses dans la ruelle. | BR_ERR_PORT | lasse |  |
| `DLG_C1_TABLEAU_06` | MENDIONDO | Le plombier était à Dax, et les fourgons de la blanchisserie au dépôt. Ce n'est pas ça. | BR_ERR_VEHICULE | sèche |  |
| `DLG_C1_TABLEAU_07` | MENDIONDO | La mairie n'a aucune Sandrine. Et ce logo n'existe plus depuis l'an dernier. | BR_ERR_PERSONNE | sèche |  |
| `DLG_C1_TABLEAU_08` | MENDIONDO | Reprenez. On n'a pas le luxe de se tromper deux fois. | Relance après erreur | pression, pas de mépris |  |
| `DLG_C1_TABLEAU_09` | MENDIONDO | Un fourgon sous une plaque clonée. Une fausse animatrice qui savait qui elle venait chercher. La route des Étangs. … C'est solide. | BR_RESOLU | décidée |  |

## `DLG_C1_FIN`

*RES_M01. Fin du chapitre, crépuscule.* — lieu : `LOC_POSTE`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_FIN_01` | MENDIONDO | Parquet ? Mendiondo. Je demande l'Alerte Enlèvement. J'ai le véhicule, deux suspects, et un secteur : l'étang de Sorbe. | Conséquence concrète de l'enquête | nette, rapide |  |
| `DLG_C1_FIN_02` | MENDIONDO | La rive est. Bien vu. | Reconnaissance si BONUS_RIVE_EST | brève, sincère | BONUS_RIVE_EST |
| `DLG_C1_FIN_03` | MENDIONDO | Vous venez. Votre chienne passe devant, vous derrière elle, et mes gars devant vous. C'est clair ? | Cadre du chapitre 2 | autorité, confiance naissante |  |
| `DLG_C1_FIN_04` | HEROINE | Clair. Allez, Ariane. On va chercher Lila. | Clôture émotionnelle | colère froide, détermination |  |
| `DLG_C1_FIN_05` | DARRIGADE | Nadia, ils partent vers l'étang. Viens, je te ramène à la maison, il faut que tu dormes un peu. | Darrigade apprend la destination (graine de la fuite du chapitre 2) | doux, protecteur |  |

## `DLG_C1_CLOTURE`

*EVT_CLOTURE, 19 h 30, sans résolution.* — lieu : `LOC_POSTE`

| ID | Locuteur | Réplique | Intention | Émotion | Condition |
|---|---|---|---|---|---|
| `DLG_C1_CLOTURE_01` | MENDIONDO | Il fait nuit dans un quart d'heure. Je ne peux plus attendre : avec ce qu'on a, on part sur la route des Étangs. Vous venez. | Clôture automatique ; état C | tendue, décidée |  |
| `DLG_C1_CLOTURE_02` | DARRIGADE | L'étang ? Nadia, viens, je te ramène. | Darrigade apprend la destination (graine de la fuite du chapitre 2) | doux, protecteur |  |

## Textes d'interface

| ID | Texte | Contexte |
|---|---|---|
| `UI_AXE_VEHICULE` | Véhicule | Colonne du tableau d'hypothèses |
| `UI_AXE_PERSONNES` | Personnes | Colonne du tableau d'hypothèses |
| `UI_AXE_DESTINATION` | Destination | Colonne du tableau d'hypothèses |
| `UI_H_VEH_PLAQUE_CLONEE` | Un fourgon qui roule sous une plaque clonée | Hypothèse |
| `UI_H_VEH_ORIGINE_BLANCHISSERIE` | Précision : un ancien fourgon de la Blanchisserie Océane | Hypothèse facultative |
| `UI_H_VEH_PLOMBIER` | Le fourgon du plombier de Dax | Hypothèse |
| `UI_H_VEH_BLANCHISSERIE_ACTIVE` | Un fourgon actuel de la Blanchisserie Océane | Hypothèse |
| `UI_H_K2_FAUSSE_ANIMATRICE` | Une inconnue déguisée en animatrice, qui savait qui elle venait chercher | Hypothèse |
| `UI_H_K2_VRAIE_ANIMATRICE` | Une animatrice du périscolaire | Hypothèse |
| `UI_H_K1_LOUBERE` | Le conducteur : Franck Loubère, ancien livreur | Hypothèse facultative |
| `UI_H_DEST_ETANGS` | La route des Étangs, vers l'étang de Sorbe | Hypothèse |
| `UI_H_DEST_NORD` | La Corniche, vers le nord | Hypothèse |
| `UI_H_DEST_A63` | L'A63, vers l'Espagne | Hypothèse |
| `UI_H_DEST_PORT` | Le port de Capbreton | Hypothèse |
| `UI_H_DEST_RIVE_EST` | Précision : la rive est de l'étang | Hypothèse facultative |
| `UI_CARNET_TITRE` | Carnet d'enquête | Titre |
| `UI_CARNET_FAITS` | Ce que j'ai constaté | Onglet : faits observés (CLU_) |
| `UI_CARNET_DEDUCTIONS` | Ce que j'en déduis | Onglet : déductions (DED_) |
| `UI_CARNET_HYPOTHESES` | Tableau des hypothèses | Onglet |
| `UI_CARNET_CHIENNE` | Langage d'Ariane | Fiche d'aide au pistage |
| `UI_CARNET_CHIENNE_1` | Truffe au sol, allure régulière : elle suit une piste fraîche. | Fiche chienne |
| `UI_CARNET_CHIENNE_2` | Truffe intermittente : odeur plus ancienne. | Fiche chienne |
| `UI_CARNET_CHIENNE_3` | Cercles serrés : quelqu'un s'est arrêté ici, ou la piste se croise. | Fiche chienne |
| `UI_CARNET_CHIENNE_4` | Assise, regard vers moi : elle a trouvé quelque chose. | Fiche chienne |
| `UI_CARNET_CHIENNE_5` | Tête haute, retour vers moi : la piste s'arrête. | Fiche chienne |
| `UI_CARNET_CHIENNE_LIMITE` | Ariane ne suit pas un véhicule. Ce qu'elle trouve oriente l'enquête ; ce n'est pas une preuve. | Fiche chienne |
| `UI_HORLOGE` | Il est {heure}. | Horloge du carnet |
| `UI_RAPPEL_APPEL` | Personne n'a encore appelé les secours. | Rappel si INT_APPEL_17 non fait après 2 min |
| `UI_RAPPEL_TROIS_AXES` | Mendiondo attend trois réponses : le véhicule, les personnes, la destination. | Rappel après 30 min sans nouvelle hypothèse |
| `UI_RAPPEL_POSTE` | Les gendarmes ont installé leur poste place de l'Église. | Rappel à l'arrivée des gendarmes |
| `UI_RAPPEL_NUIT` | Le soleil baisse. Il reste peu de temps avant la nuit. | Rappel à 19 h 00 |
| `UI_ECHEC_PREUVE_MANQUANTE` | Cette hypothèse n'est soutenue par aucune preuve épinglée. | Message d'échec doux |
| `UI_ECHEC_OPTION_EXCLUE` | Cette piste a été vérifiée : elle est exclue. | Option grisée |
| `UI_ECHEC_PISTE_VIDE` | Ariane ne trouve rien à suivre avec cet objet. | Pistage sans résultat |
| `UI_CONTRADICTION` | Contradiction : {indice_a} ne s'accorde pas avec {indice_b}. | Signalée dans le carnet quand deux indices s'opposent |
| `UI_FIN_CHAPITRE` | Chapitre 1 terminé — {heure}. Hypothèses retenues : {n}/3. Découvertes facultatives : {bonus}. | Écran de fin |
| `UI_TITRE_JEU` | Faux-semblants | Écran titre (titre provisoire, non validé) |
| `UI_TITRE_SOUS` | Prologue | Écran titre, sous le titre |
| `UI_TITRE_START` | Appuie sur Entrée pour commencer | Écran titre, invitation à commencer |
| `UI_OBJECTIF` | Objectif | Intitulé du cadre d'objectif (HUD) |
| `UI_OBJ_P_BALADE` | Promène-toi avec Ariane sur la place de l'école. | Objectif du prologue, avant la sonnerie (16 h 25–16 h 26) |
| `UI_OBJ_P_SORTIE` | La sonnerie a retenti : les enfants sortent de l'école. | Objectif du prologue, sortie de l'école (16 h 26–16 h 29) |
| `UI_OBJ_P_RUELLE` | Ariane fixe la ruelle, oreille dressée. Va voir. | Objectif du prologue, Lila devant le fourgon, avant l'alerte (proposition : oriente vers la ruelle sans révéler l'abordage) |
| `UI_OBJ_P_AGIR` | Agis, vite ! Deux actions au plus. | Objectif pendant la fenêtre d'action (compte à rebours affiché à côté) |
| `UI_OBJ_P_REJOINDRE` | Rejoins l'entrée de la ruelle ! | Objectif pendant la retenue du départ |
| `UI_OBJ_P_FUITE` | Le fourgon s'enfuit. | Objectif pendant le départ du fourgon |
| `UI_OBJ_P_APPEL` | Appelle le 17. | Objectif après le départ : action principale |
| `UI_TOUCHES_BASE` | ZQSD : marcher · Maj : courir · 1, 2, 3 : vues · E : parler · R : au pied · X : reste · G : cherche · J : carnet · P : pause | Rappel permanent des commandes (clavier) |
| `UI_TOUCHES_ACTION` | F : photographier · Maj : courir · C : crier « Lila ! » · E : « Ariane, va ! » | Commandes de la fenêtre d'action |
| `UI_TOUCHES_APPEL` | T : appeler le 17 | Commande d'appel au 17 |
| `UI_CARNET_NOUVEAU` | Noté dans le carnet (J) | Notification quand un indice est noté |
| `UI_CARNET_VIDE` | Rien de noté pour l'instant. | Carnet ouvert sans indice |
| `UI_FIN_PROLOGUE_TITRE` | Fin du prologue | Écran de fin du prologue |
| `UI_FIN_PROLOGUE_TEXTE` | 16 h 31. {HEROINE} appelle le 17. L'enquête continue au chapitre 1, en préparation. | Écran de fin du prologue, sous le titre |
| `UI_FIN_PROLOGUE_QUITTER` | Échap : quitter | Écran de fin du prologue, commande |
| `UI_INVITE_DUFAU` | E : saluer Marcel Dufau | Invite quand la joueuse est près du banc de Dufau |
| `UI_INVITE_COUCOU` | E : répondre au coucou de Lila | Invite pendant le coucou de Lila (FLAG_COUCOU_RENDU) |
| `UI_ORDRE_RESTE` | Ariane : « Reste. » | Notification de l'ordre Reste |
| `UI_ORDRE_AU_PIED` | Ariane : « Au pied. » | Notification de l'ordre Au pied |
| `UI_ORDRE_CHERCHE` | Ariane : « Cherche ! » | Notification de l'ordre Cherche (balle lancée) |
| `UI_REPERE_RUELLE` | Ruelle | Repère à l'écran vers l'entrée de la ruelle |
| `UI_MENU_PAUSE` | Pause | Titre du menu pause |
| `UI_MENU_REPRENDRE` | Reprendre | Menu pause |
| `UI_MENU_SENSIBILITE` | Sensibilité de la souris | Menu pause, option |
| `UI_MENU_INVERSER` | Inverser l'axe vertical | Menu pause, option |
| `UI_MENU_SOUS_TITRES` | Taille des sous-titres | Menu pause, option |
| `UI_MENU_TEMPS_ACTION` | Temps d'action allongé | Menu pause, option d'accessibilité OPT_TEMPS_ACTION_ETENDU (s'applique à la prochaine alerte) |
| `UI_MENU_RECOMMENCER` | Recommencer le prologue | Menu pause |
| `UI_MENU_QUITTER` | Quitter le jeu | Menu pause |
| `UI_MENU_AIDE` | Haut / Bas : choisir · Gauche / Droite : régler · Entrée : valider · P : reprendre | Menu pause, aide de navigation |
| `UI_MENU_COMMANDES` | Commandes | Menu pause, titre de la liste des commandes |
| `UI_OUI` | Oui | Valeur d'option |
| `UI_NON` | Non | Valeur d'option |

Les aides progressives des énigmes (`UI_HINT_*`) sont dans `GameData/missions/01/puzzles.json` et reprises dans `docs/cases/01-enigmes.md`.
