# Choix visuels à trancher : Lila (V4) et le fourgon (V2)

> **Pour Audrey.** Tout ce document est une **proposition** de Claude (🟡). Pour répondre, une ligne suffit, par exemple « V4 : B, V2 : A », ou « autre » avec ta description. Codex produira ensuite les modèles à partir de ton choix.
>
> Rappels ✅ : tenue, cheveux et trajet précis de Lila ne sont pas encore validés (faux raccord capillaire à éviter) ; l'apparence du fourgon se choisit pendant la construction 3D ; **aucun indice indispensable ne dépend de ces choix** (vérifié par `Tools/validate_mission.py`).

## V4 — Lila (9 ans, proposition), tenue et cheveux

Contraintes narratives, quelle que soit l'option :
- repérable à 20–40 m sur la place, au milieu d'autres enfants ;
- **cartable** avec un porte-clés renard (il tombe à l'abordage) ; **bracelet en perles** au poignet (il casse en montant dans le fourgon) ;
- **même tenue et même coiffure** du prologue jusqu'à la cabane du chapitre 4, y compris sur la photo de vie ; au chapitre 4, tenue salie et humide ;
- une fin septembre douce : pas de manteau d'hiver ;
- aucune ressemblance avec une enfant réelle ; aucune marque.

| | A — « Jaune moutarde » *(recommandée)* | B — « Marinière » | C — « Salopette » |
|---|---|---|---|
| Haut | Sweat jaune moutarde uni | Marinière blanche à rayures bleu marine | Tee-shirt blanc |
| Bas | Jean droit bleu moyen | Jupe en jean et collants bleu marine | Salopette en velours vert sapin |
| Chaussures | Baskets blanches à scratch | Baskets bleu marine | Bottines marron |
| Cartable | Rouge brique | Jaune | Bleu ciel |
| Cheveux | Châtain, carré court au niveau du menton, sans accessoire | Brun foncé, **une** tresse dans le dos | Blond foncé, mi-longs lâchés avec une frange |
| Avantages | Silhouette la plus lisible de loin ; coiffure courte, donc peu de risque de faux raccord en animation | Très « bord de mer » ; rayures lisibles | Silhouette originale |
| Risques | Aucun notable | Rayures : moirage possible à distance en 3D | Cheveux lâchés et frange : plus de risques de faux raccords et plus coûteux en simulation |

## V2 — le fourgon

Contraintes narratives, quelle que soit l'option :
- **plaque arrière lisible** et **portière latérale coulissante** (côté trottoir) ;
- **pot d'échappement qui cogne et tremble** (son et vibration visible) ;
- ancien véhicule d'entreprise réformé, sans marque réelle : aucun logo de constructeur, silhouette générique de fourgon moyen ;
- l'**ombre de lettrage** « Blanchisserie Océane » est facultative : si tu la gardes, l'énigme facultative PZ_06 existe ; sinon, elle est retirée ;
- le feu arrière fendu est facultatif.

| | A — « Blanc usé » *(recommandée)* | B — « Gris anthracite » | C — « Bleu délavé » |
|---|---|---|---|
| Couleur | Blanc cassé, rayures et traces de rouille | Gris foncé mat | Bleu pâle délavé par le soleil |
| Lettrage | Ombre de lettrage visible sur le flanc et les portes arrière (PZ_06 possible) | Aucun (PZ_06 retirée) | Ombre plus claire du lettrage (PZ_06 possible) |
| Détails | Feu arrière droit fendu, pare-chocs rayé | Vitres arrière teintées | Galerie de toit vide |
| Effet | Le plus banal : on ne le remarque pas, puis on s'en souvient | Plus inquiétant, mais « trop méchant » pour un véhicule qui doit passer inaperçu | Original, facile à reconnaître sur les vidéos |
| Risques | Couleur la plus courante : les fausses pistes (péage) restent crédibles | Contraste faible au crépuscule dans les vidéos | Moins crédible pour un ancien fourgon de blanchisserie |

## Ce qui en dépend

- Codex : modèles `CHAR_FILLETTE` et `VEH_FOURGON`, textures, animations du coucou et de l'abordage.
- Claude : jeton `{FOURGON_COULEUR}` dans les dialogues, maintien ou retrait de PZ_06, descriptions de la continuité de Lila dans `GameData/scenes/decoupage.json`.
