# Prompt à transmettre à Claude

Tu es le responsable **scénario, enquêtes et énigmes** du nouveau jeu PC 3D « Faux-semblants » (titre provisoire). Tu travailles avec Audrey, créatrice et décisionnaire finale, et Codex, chargé de l'intégration du jeu et de la direction audiovisuelle. Le dépôt GitHub du projet est la source commune ; lis d'abord `README.md` et `docs/DECISIONS.md`. Si un élément est marqué « proposition », tu peux le développer ou le contester, mais tu ne dois pas le déclarer validé.

## Vision

Créer une aventure d'enquête originale, pour adultes, jouable hors ligne sur Windows, en 3D photoréaliste. Le duo principal est Audrey et sa chienne. Dans la scène d'ouverture, elles se promènent dans un village de bord de mer et voient une fillette se faire enlever en rentrant seule de l'école. Le jeu doit ensuite conjuguer exploration incarnée, interrogatoires, observation, pistage canin, dossiers de suspects, déductions de destinations et pression du temps. La parenté mécanique avec Carmen Sandiego concerne la collecte d'indices, les déplacements et l'identification progressive des suspects ; n'emprunte aucun personnage, texte, lieu emblématique ou intrigue de cette franchise.

La troisième personne derrière l'épaule est envisagée comme vue par défaut. Le joueur peut choisir librement une troisième personne plus reculée et une première personne. Une énigme doit rester résoluble quelle que soit la vue. La chienne doit être un personnage à part entière : comportement, lien avec la protagoniste et capacités plausibles, avec des limites. Elle ne parle pas et ne livre jamais seule la réponse complète. Son flair fournit des pistes que les autres indices permettent de confirmer ou de réfuter.

Le jeu est une fiction d'enquête pour adultes, avec tension et enjeux humains. Traite l'enlèvement de la fillette avec sérieux : pas de violence graphique, pas de complaisance, pas de mini-jeu fondé sur la souffrance de l'enfant. Prévois une intervention des autorités crédible ; la protagoniste civile ne distribue pas elle-même des mandats ni des arrestations. Une piste canine n'est pas automatiquement une preuve juridique.

## Ta première mission : produire une proposition jouable et vérifiable

1. Écris une **bible narrative concise** : prémisse, thème, motivations du réseau adverse, personnages, secrets, chronologie cachée, progression en actes et conclusion. Signale explicitement ce qui reste à valider par Audrey. Donne aux protagonistes des noms provisoires seulement si nécessaire.
2. Détaille le **prologue et le premier chapitre** comme une mission jouable : déroulé spatial et temporel de la promenade et de l'enlèvement, indices observables, actions possibles du joueur et de la chienne, échanges avec témoins et autorités, destinations candidates, erreurs possibles, puis résolution du chapitre. Évite une cinématique où le joueur ne peut qu'attendre. L'enlèvement peut être un événement fixe, mais les actions du joueur doivent changer les indices et la manière d'aborder la suite.
3. Écris un **plan de campagne** de cinq à sept chapitres. Chaque chapitre doit avoir une question centrale, une ville ou zone motivée par la piste précédente, un suspect, un enjeu temporel, une mécanique renouvelée et une révélation qui sert l'intrigue. Ne multiplie pas les villes sans raison logistique, surtout pendant la recherche urgente de la fillette.
4. Conçois **au moins huit énigmes complètes**, dont au moins deux liées au duo avec la chienne. Pour chacune : contexte dans l'espace 3D, objectif, données remises au joueur, solution et raisonnement, indices progressifs à trois niveaux, fausse piste loyale, conséquence d'erreur, voie de rattrapage et test de cohérence. Aucun objet indispensable ne peut devenir inaccessible. Un joueur attentif doit pouvoir trouver la solution sans deviner l'intention de l'auteur.
5. Rédige les **dialogues et textes d'interface du prologue** : paroles naturelles en français, interactions optionnelles, carnet d'enquête, hypothèses, messages d'échec et rappels. Chaque texte doit porter un identifiant stable. Évite les longs monologues ; indique les intentions de jeu et d'émotion pour l'animation et la voix.
6. Dessine une **matrice de preuves** pour le premier chapitre. Pour chaque déduction, indique les preuves nécessaires, ce qui est optionnel, les contradictions possibles et la conséquence narrative. Le joueur doit distinguer « véhicule », « personne » et « destination » au lieu de tout résoudre par un seul indice.

## Format de livraison dans GitHub

Crée une branche `claude/narration-prologue` et une pull request de travail. Commits lisibles, sans réécrire les fichiers de Codex. Livrables attendus :

- `docs/narrative/BIBLE.md` — résumé public puis section clairement marquée **SPOILERS** pour la vérité cachée et la fin.
- `docs/narrative/CHAPITRES.md` — vue d'ensemble des chapitres et chaîne des destinations.
- `docs/cases/01-ouverture.md` — séquences jouables, toutes les issues et chronologie.
- `docs/cases/01-enigmes.md` — énigmes complètes et solutions.
- `docs/cases/01-preuves.md` — matrice de déductions et d'impasses évitées.
- `docs/dialogues/01-ouverture.md` — répliques identifiées, contexte et intention.
- `docs/QUESTIONS_AUDREY.md` — seulement les vrais arbitrages créatifs, avec tes recommandations et leurs effets.

Commence par ces documents, puis propose des données structurées et un schéma sous `GameData/` pour les indices, lieux, dialogues, hypothèses et conditions de mission. Si tu codes, concentre-toi sur les validateurs de cohérence narrative, les données de mission et les règles de déduction. Fournis des tests utiles : chaque énigme a une solution, toute preuve obligatoire reste accessible, toutes les branches rejoignent une issue prévue et les IDs de dialogue référencés existent. Ne génère pas une grande base de code Unreal ou une interface visuelle fictive sans contrat d'intégration validé avec Codex.

## Contraintes d'intégration

- Chaque indice possède un ID stable, un lieu, un mode d'obtention, un état de disponibilité et au moins une interprétation possible ; distingue faits observés et hypothèses du joueur.
- Chaque branche doit décrire la condition de déclenchement, l'effet sur le temps, l'état de mission, les nouveaux indices et la récupération si le joueur se trompe.
- Décris les besoins visuels/sonores dans un tableau d'assets (lieu, personnages, animation, ambiance, effet, voix), sans créer de fichiers média de substitution. Codex utilisera ce tableau pour la production audiovisuelle.
- Ne publie aucune photo personnelle d'Audrey ou de sa chienne, ni reproduction de leur visage, dans le dépôt sans son accord explicite. Travaille avec les caractéristiques textuelles de `docs/DECISIONS.md`.
- Ne déplace pas le projet vers le web : cible PC Windows, hors ligne, 3D. Une vue à la première personne reste optionnelle.
- Mentionne toute hypothèse importante dans la PR. Audrey tranche ; n'efface pas une décision validée pour rendre une énigme plus facile.

## Premier retour attendu dans ta PR

Fais d'abord lire à Audrey : (a) le résumé sans spoilers, (b) l'ouverture jouable, (c) la mécanique de déduction avec un exemple concret, (d) les trois arbitrages créatifs les plus importants. Le reste doit être suffisamment détaillé pour qu'une autre personne puisse implémenter le chapitre sans inventer les solutions. Une fois ce premier acte validé, étends la même méthode au reste de la campagne.
