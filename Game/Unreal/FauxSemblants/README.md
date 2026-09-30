# Faux-semblants — projet Unreal (squelette, jalon 1)

 > **Statut : jalon de blockout. Audrey a confirmé l'ouverture du projet et une première compilation C++ réussie. Les corrections de reprise doivent encore être compilées et essayées en jeu sur son PC.** Les personnages et décors sont des **volumes provisoires** (boîtes aux bonnes dimensions et couleurs), pas les graphismes finaux.

## Installation vérifiée sur le PC d'Audrey (01/10/2026)

- Copie Git du dépôt : `C:\Dev\3D` (hors OneDrive, chemin sans accents). Git portable : `C:\Dev\outils\MinGit`.
- Moteur : Unreal Engine 5.8.3, Visual Studio 2022 (MSVC 14.44), SDK Windows 10.0.26100.
- `FauxSemblantsEditor` Win64 Development compile avec `Build.bat` (branche `claude/compil-ue58`).
- `Scripts/setup_prologue.py` lancé sans intervention (`UnrealEditor-Cmd.exe <projet> -ExecutePythonScript=...`) : `L_Prologue` construit et enregistré (36 volumes de maquette).
- Pas encore essayé manette ou clavier en jeu : il reste à ouvrir `L_Prologue` et cliquer sur Jouer.

## Ce que contient ce squelette

- `FauxSemblants.uproject` : le projet (Unreal Engine 5.4 ou plus récent).
- `Source/FauxSemblants/` : le code C++ du jeu :
  - `FSHeroCharacter` : l'héroïne jouable, marche et course, trois vues (épaule, reculée, subjective) ;
  - `FSDogCharacter` : Ariane, libre, qui suit et revient au rappel par déplacement direct ; l'envoi vise un point du trottoir, avec navigation et limite de chaussée encore à vérifier ;
  - `FSPrologueDirector` : la scène d'ouverture minutée (sortie de Lila, abordage, refus devant la portière, alerte, fenêtre d'action, départ du fourgon retenu ou plan court) ;
  - `FSMissionSubsystem` : l'horloge, les indices obtenus, les déductions, les textes, la sauvegarde locale ;
  - `FSGameMode`, `FSSaveGame`.
- `Scripts/setup_prologue.py` : construit le niveau `L_Prologue` à partir de la maquette de Codex et copie les données du jeu (`GameData/`).
- `Config/` : réglages et commandes (clavier AZERTY et QWERTY, manette).

## Installer sur ton PC Windows (une seule fois)

1. **GitHub Desktop** (le plus simple) : https://desktop.github.com. Connecte-toi, puis *File › Clone repository* → `audreygontier334-lang/3D`. Choisis la branche **`claude/stoic-gauss-axnd0g`** (menu *Current branch*).
2. **Epic Games Launcher** : https://www.unrealengine.com/download. Onglet *Unreal Engine › Bibliothèque* → installe **Unreal Engine 5.4** (ou plus récent). Compte environ 60 à 100 Go.
3. **Visual Studio 2022 Community** (gratuit) : https://visualstudio.microsoft.com. À l'installation, coche **« Développement de jeux en C++ »** et **« Développement Desktop en C++ »**. Unreal en a besoin pour compiler le code.

### Sur un PC portable avec deux cartes graphiques (par exemple Intel + NVIDIA)

Pour qu'Unreal utilise la carte NVIDIA : *Paramètres › Système › Écran › Graphiques*, ajoute `UnrealEditor.exe` (dans `C:\Program Files\Epic Games\UE_5.4\Engine\Binaries\Win64\`) et choisis **« Hautes performances »**. Branche le chargeur pendant que tu travailles.

## Ouvrir le projet

1. Dans l'explorateur Windows, va dans `3D\Game\Unreal\FauxSemblants\`.
2. Clic droit sur **`FauxSemblants.uproject`** → *Generate Visual Studio project files* (sous Windows 11 : *Afficher d'autres options*). Si ta version d'Unreal n'est pas la 5.4, choisis d'abord *Switch Unreal Engine version…*.
3. Double-clic sur `FauxSemblants.uproject`. Unreal propose de **reconstruire les modules** : réponds **Oui**. La première compilation prend plusieurs minutes.
   - **En cas d'échec** : copie le message d'erreur (ou le fichier `Saved\Logs\FauxSemblants.log`) et envoie-le à Claude.
4. Dans l'éditeur : menu **Outils (Tools) › Exécuter un script Python… (Execute Python Script…)** → choisis `Scripts\setup_prologue.py`. Le niveau se construit en quelques secondes.
5. Clique sur **Jouer (Play)**.

## Commandes

| Action | Clavier (modifiable) | Manette |
|---|---|---|
| Se déplacer | ZQSD (AZERTY) ou WASD | stick gauche |
| Regarder | souris (molette : distance de la caméra) | stick droit |
| Courir | Maj gauche | clic stick gauche |
| Vue épaule / reculée / subjective | 1 / 2 / 3, ou V pour faire défiler | clic stick droit |
| Changer d'épaule | Tab | — |
| Parler, répondre, ramasser | E | B |
| Pendant l'alerte : photographier, crier « Lila ! », « Ariane, va ! » | F, C, E (courir compte comme une action) | X, Y, B |
| Ariane : au pied / reste / cherche (balle) | R / X / G | LB / croix bas / RB |
| Appeler le 17 (fin du prologue) | T (ou le téléphone) | — |
| Carnet | J | Select (View) |
| Inventaire (sac en bandoulière, preuves) | I | croix haut |
| Téléphone | O | croix gauche |
| Lampe torche | L | croix droite |
| Menu principal | P ou Échap | Start |
| Sauvegarde rapide / chargement rapide | F5 / F9 | — |
| Menus : choisir, régler, valider, revenir | flèches, Entrée, Échap | croix, A, B |

Dans l'éditeur, Échap arrête la partie de test : utiliser P pour le menu pause.

## Ce que tu devrais voir

Mardi 16 h 25 sur la place de l'école. À 16 h 27, Lila (volume jaune moutarde) sort et part vers la ruelle, cachée derrière l'angle. Si tu la suis jusqu'à l'entrée de la ruelle, tu vois la femme au badge l'aborder, puis Lila refuser de monter dans le fourgon (volume blanc). L'alerte donne quelques secondes pour agir (deux actions au plus), puis le fourgon part vers la mer au loin. Les répliques et les indices obtenus s'affichent en haut de l'écran. Il n'y a pas encore d'interface finale.

## Interface du prologue (branche `claude/prologue-lisible`)

`AFSHUD` dessine l'interface sans asset : écran titre (Entrée), carton d'ouverture, horloge, objectif selon la phase du prologue (compte à rebours pendant la fenêtre d'action), sous-titres avec le nom de la personne qui parle, rappels de commandes, notifications, carnet d'indices (J) et écran de fin après l'appel au 17. Les textes viennent de `GameData/dialogues/01-ouverture.json` (`UI_…`, `DLG_…`) : relancer `Scripts/setup_prologue.py` après une modification pour recopier les données.

## Confort de jeu (branche `claude/confort-jeu`)

- Promenade jouable : répliques du tutoriel, conversation facultative avec Dufau sur son banc (E), réponse au coucou de Lila (E, `FLAG_COUCOU_RENDU`).
- Ordres à Ariane : au pied, reste, cherche (balle lancée ; la première trouvaille déclenche `DLG_P_TUTO_04`). Son allure suit celle de l'héroïne.
- Menu pause : sensibilité de la souris, inversion de l'axe vertical, taille des sous-titres, temps d'action allongé (`OPT_TEMPS_ACTION_ETENDU`), recommencer, quitter. Réglages gardés dans `GameUserSettings.ini` (section `[FauxSemblants]`).
- Interface : invites d'interaction, repère vers l'entrée de la ruelle quand il faut y aller, barre du temps restant, éclair de photo.
- Déplacements plus nets (accélération et freinage), zoom à la molette, rotation au stick indépendante de la cadence d'images.
- `setup_prologue.py` donne à chaque volume de la maquette un tag égal à son nom (ex. `dufau_placeholder`).

## Menu principal, inventaire et téléphone (branche `claude/menu-inventaire`)

- **Écran titre** : nouvelle partie, charger, commandes, son, options, aide, quitter. **Menu principal** (P) : retour au jeu, sauvegarder, charger, modifier les commandes, son, options, aide, recommencer, quitter. Le jeu est suspendu dans ces menus.
- **Modifier les commandes** : chaque commande du clavier se réassigne (Entrée puis la nouvelle touche) ; une touche déjà prise est échangée ; « Rétablir les commandes d'origine ». Les choix vont dans `Saved/Config/Windows/Input.ini` de la joueuse. La manette garde ses boutons.
- **Son** : volume général, musique, effets, voix, ambiance, interface, couper le son, tester. Les catégories sont les classes `/Game/Audio/SC_*` créées par `setup_prologue.py` ; les sons y seront rangés par Codex. Sons d'interface provisoires synthétisés par le script (`SFX_UI_*`, `SFX_Notification`, `SFX_Photo`).
- **Sauvegardes** : trois emplacements et la sauvegarde rapide (F5/F9) ; toute la scène est reprise (heure, phase du prologue, actions, positions, inventaire, téléphone).
- **Aide** : objectifs, commandes, Ariane, carnet, inventaire, téléphone, sauvegarde, accessibilité.
- **Inventaire** (I) : téléphone, bonbons pour Ariane (elle revient au pied), petit Opinel, gants (à enfiler avant de ramasser une preuve), lampe torche (spot fixé à la caméra), et la rubrique Preuves (indices de nature « objet » ou « document »). Un acteur portant les tags `Preuve` et `CLU_…` devient ramassable avec E.
- **Téléphone** (O) : clavier d'appel, répertoire, journal d'appels, messages, mails, notifications, batterie. Contenus dans `GameData/telephone/01-prologue.json` (proposition à valider), délivrés au fil du prologue. Appeler le 17 après le départ du fourgon termine le prologue, comme la touche T.

## Limites connues (prochaines étapes)

- Pas d'animations ni de modèles photoréalistes : volumes provisoires seulement.
- Interface provisoire dessinée en C++ (polices du moteur) : l'habillage graphique reste à faire.
- La règle d'accélération (la femme presse Lila si le duo approche à moins de 12 m) n'est pas encore codée.
- Le chapitre 1, la carte des déplacements et le portrait-robot viendront ensuite.
- Les données sont lues en JSON (`Content/Data/M01/`, recopiées par le script) ; le passage aux Data Tables décrit dans `docs/INTEGRATION_UNREAL.md` viendra plus tard.

## Reprise : contrôles du premier essai

Après récupération des corrections, fermer l’éditeur, reconstruire le projet C++, puis rouvrir Unreal et lancer le script. Depuis le Journal de sortie, sélectionner Python et exécuter :

```python
import os, runpy, unreal; runpy.run_path(os.path.join(unreal.Paths.project_dir(), "Scripts", "setup_prologue.py"), run_name="__main__")
```

Le script reconstruit `L_Prologue` ; enregistrer ailleurs toute modification manuelle du niveau avant de le relancer. Les classes C++ et les repères du glTF sont vérifiés avant la reconstruction.

- Avant la sortie de Lila et après la disparition du fourgon : leurs anciennes positions ne doivent pas bloquer la joueuse.
- Les petits indices ne bloquent pas la marche et reprennent les positions du glTF.
- Envoyer Ariane vers la ruelle en restant sur la place : son arrivée peut déclencher l’alerte, mais ne suffit pas à libérer le départ du fourgon. La retenue attend la joueuse ou expire avec le plan court.
- Tester ensuite les trottoirs, les six vues et le rappel d’Ariane autour du coude. Le déplacement direct d’Ariane n’a pas encore de navigation autour des obstacles ; ce point reste à réaliser.

Vérification hors Unreal : syntaxe Python contrôlée. La compilation C++ et ces essais en jeu nécessitent Unreal ; ils ne sont pas attestés par le contrôle de syntaxe.

## Premier essai sur un PC à 16 Go de RAM

Un profil temporaire dans `DefaultEngine.ini` limite le rendu à 30 images/s, réserve 512 Mo au pool de textures et désactive flou de mouvement, profondeur de champ et reflets écran. Ces réglages visent la charge graphique ; ils ne garantissent pas de résoudre une saturation de RAM pendant la compilation des shaders. Ils pourront être retirés pour la production finale.

1. Enregistrer le travail puis fermer Unreal avant de récupérer la branche `claude/stoic-gauss-axnd0g` dans GitHub Desktop (Fetch puis Pull).
2. Après une éventuelle compilation C++, fermer Visual Studio. Fermer les applications inutiles après avoir enregistré leur travail.
3. Rouvrir le projet, attendre la compilation des shaders, puis exécuter `Scripts/setup_prologue.py` via Outils › Exécuter un script Python. Le script reconstruit le niveau : sauvegarder ailleurs les modifications manuelles avant de le relancer.
4. Lancer Jouer et relever le message exact si une erreur apparaît. Ces réglages n'ont pas encore été mesurés sur le PC d'Audrey.

Ne pas effacer le cache des shaders pour cet essai : il faudrait ensuite les recompiler.
