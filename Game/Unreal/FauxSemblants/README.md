# Faux-semblants — projet Unreal (squelette, jalon 1)

> **Statut : squelette écrit par Claude sans pouvoir lancer Unreal. Il n'a encore jamais été compilé.** Attends-toi à quelques erreurs à la première compilation : copie-les à Claude, qui les corrigera. Les personnages et décors sont des **volumes provisoires** (boîtes aux bonnes dimensions et couleurs), pas les graphismes finaux.

## Ce que contient ce squelette

- `FauxSemblants.uproject` : le projet (Unreal Engine 5.4 ou plus récent).
- `Source/FauxSemblants/` : le code C++ du jeu :
  - `FSHeroCharacter` : l'héroïne jouable, marche et course, trois vues (épaule, reculée, subjective) ;
  - `FSDogCharacter` : Ariane, libre, qui suit, revient au rappel et s'arrête toujours au bord de la chaussée ;
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

| Action | Clavier | Manette |
|---|---|---|
| Se déplacer | ZQSD (AZERTY) ou WASD | stick gauche |
| Regarder | souris | stick droit |
| Courir | Maj gauche | clic stick gauche |
| Vue épaule / reculée / subjective | 1 / 2 / 3, ou V pour faire défiler | clic stick droit |
| Changer d'épaule | Tab | — |
| Pendant l'alerte : photographier, crier « Lila ! », « Ariane, va ! » | F, C, E (courir compte comme une action) | X, Y, B |
| Rappeler Ariane | R | — |
| Appeler le 17 (fin du prologue) | T | — |
| Sauvegarder | F5 | — |

## Ce que tu devrais voir

Mardi 16 h 25 sur la place de l'école. À 16 h 27, Lila (volume jaune moutarde) sort et part vers la ruelle, cachée derrière l'angle. Si tu la suis jusqu'à l'entrée de la ruelle, tu vois la femme au badge l'aborder, puis Lila refuser de monter dans le fourgon (volume blanc). L'alerte donne quelques secondes pour agir (deux actions au plus), puis le fourgon part vers la mer au loin. Les répliques et les indices obtenus s'affichent en haut de l'écran. Il n'y a pas encore d'interface finale.

## Limites connues (prochaines étapes)

- Pas d'animations ni de modèles photoréalistes : volumes provisoires seulement.
- Les textes s'affichent en messages de débogage, pas encore dans une vraie interface.
- La règle d'accélération (la femme presse Lila si le duo approche à moins de 12 m) n'est pas encore codée.
- Le chapitre 1, la carte des déplacements et le portrait-robot viendront ensuite.
- Les données sont lues en JSON (`Content/Data/M01/`, recopiées par le script) ; le passage aux Data Tables décrit dans `docs/INTEGRATION_UNREAL.md` viendra plus tard.
