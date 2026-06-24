# petitBotLPDna

<h1>Wikidébrouillard petitbot nouvelle version :</h1> <br>

//photo : petitbot en boite elec <br>

<h2>Description : </h2><br>

Petitbot = robot DIY à base de récup’
Ce petit robot est une création 100 % petits debrouillards, alliant ingéniosité et économie de moyens. Il est équipé :

- D’un microcontrôleur ESP8266 pour le piloter à distance (via Wi-Fi) et/ou le programmer selon vos besoins.

- De moteurs FS90R (servomoteurs pour une rotation continue), lui offrant une mobilité simple et aisé à contrôlé.

- D’alimentation par batteries 18650 (récupérées ou recyclées), assurant une autonomie optimale pour des projets nomades.

Conçu avec des matériaux de récupération (boîtier, roues, fils electrique), c’est une porte d’entrée idéale vers l’univers du DIY(DO IT YOURSELF = faire sois-même), de la robotique et de la programmation. il prouve qu’innovation, revalorisation et durabilité  peuvent aller de concert. Idéal pour simplement s’amuser ou commencé à apprendre en le personnalisant !


Discipline Scientifique : Arduino, electricité, informatique 

Difficulté : moyen + 

Durée : 1 journée 

Mots clés : Programmation , robot , récup , défis, plastique, Arduino , esp8266 électronique 

<h2>Liste de matériel </h2><br>
<h3>🔧 Outils et matériel </h3>

- Tournevis (cruciforme et plat)
- Colle chaude ou ruban adhésif double face
- Cutter ou ciseaux
- Pince à dénuder (optionnel si vous savez utiliser très habillement le cutter )
- Fer à souder (optionnel, selon la version)
- pince a sertir (optionnel, selon la version )

<h3>📦 Matériel de base </h3>

Électronique :

- 1 ESP8266 D1 mini
- 2 moteurs FS90R 
- 1 module TP4056 (pour charger la batterie)
- 1 Batteries 18650 + support (ou pack de piles)
- 11 Fils électriques 5cm-10cm (dupond mâle-mâle ou simple à souder)
- 1 Interrupteur deux bornes 
- 2 wago 5 fils 222-415 (optionnel, selon la version)
- 1 platine de prototypage (optionnel, selon la version)
- des pièces de lego pour pouvoir adapter le bot à différents mode de jeu 

<h3>🏗️ 🛞 Structure et mobilité :</h3>

- Châssis (ouvert ou fermé selon la version) il peux être en bois, plastique ou imprimé 3D,etc... 
Pour ce tutotiel on vas utiliser une petit boite dérivation mais vous pouvez adapté n'importe quel contenant tant qu'il fait au moins 8.5*8.5cm, pour laisser assez d'espace au différents composants.
- 2 roues motrice (pieces spécifique ou bouchons de bouteilles, CDs, impréssion 3D, etc.)
- 1 Bille Roulettes Pivotantes ( pour la direction du petitbot)
- 1 bouchon en liège pour ajuster la hauteur de la roulette de direction 

-----

<h2>Introduction</h2>

Ce robot DIY est conçu pour être accessible à tous, quel que soit votre niveau ou vos ressources. Il existe plusieurs versions pour s’adapter à vos contraintes et à vos envies :

- Version 1 "démonstation" : pour faire des démonstration et création de ce robot 

- Version 2 "100 % récup’" : pour les écolos et les bricoleurs malins.

- Version 3 "sans soudure" : pour ceux qui veulent éviter l’électronique complexe.

- Version 4 "pièces neuves" : pour un montage plus fiable et durable.

- Version 5 "indestructible" : pour les robots qui doivent survivre à toutes les aventures !

⚠️ Pour ce tutoriel on vas vous guide dans la réalisation d'une version de démonstration sans soudure, la plus simple à réaliser possible⚠️ 

<h3>chapitre 1 Orgraniser les différents composants :</h3> 

- placer l'esp8266 sur la plaquette de prototypage 
 ![esp8266onbreadboard](photo-markd/breadboardEsp8266.jpg) <br>
---
- prenez deux wago et noté un moins (-) sur l'un et un plus (+) l'autre. (ça aidera pour l'organisation, la réalisation de ce tutoriel ainsi que le diagnostic si il y a un  problème) 🔍️vous pouvez ouvrir tous les leviers ~~80° 
 ![wago-vide](./photo-markd/wago-222-415.jpg) <br>
---

- prépare 8 fils avec des connecteurs dupond M-M (si possible choisir 3 fils de couleur rouge/chaude, 3 fils de couleur noir/froide et 2 fils avec des couleurs différentes)
 ![fils-dupond](./photo-markd/fils-dupond.png)
---
- prenez deux moteur FS90R et noté sur l'un "1" et sur l'autre "2"  (ça aidera pour l'organisation, la réalisation de ce tutoriel ainsi que le diagnostic si il y a un  problème)
 ![moteur-petitbot](./photo-markd/motor-fs90r.jpg)


<h3> chapitre 2 montage éléctrique de l'ESP8266: </h3>

1er étape : vérifier que l'interrupteur sur le boitier pile est bien sur "OFF" 

---
2ème étape : suivre dans l'ordre 

- placer le fil noir du boitier de pile dans le wago "-" 
- placer le fil rouge du boitier de pile dans le wago "+"
- prenez un fil de couleur froide placer une extremité dans le wago - et l'autre extremité dans la broche en face du "G" sur l'esp 
- prenez un fil de couleur chaude placer une extremité dans le wago + et l'autre extremité dans la broche en face du "5V" sur l'esp8266 <br>
![montage-elec-step1](photo-markd/montage-step1.png)

3ème étape : mettre l'interrupteur sur "ON" et voir si la led sur l'esp s'allumé (c'est bon signe!)


<h3>chapitre 3 montage des moteurs : </h3>

1er étape : vérifier que l'interrupteur sur le boitier pile est bien sur "OFF" 

---
2ème étape : suivre dans l'ordre 

- faite un groupe de trois fils dupont avec un fil de couleur froide (il représente le "-") , un de couleur chaude (il représente le "+") et un fil de couleur différentes (il représente le signal ) scotché les trois au niveau de la partie noir du connecteur dupont 
![groupe_dupont](./photo-markd/fils_moteurs.png)

- branché le moteur sur l'un des groupe déjà préparer
![connection_of_motor](./photo-markd/dupont-to-motor.png)

- faire la même chose avec l'autre groupe de fils pour le deuxième moteur
--- 

- placer les deux fils de couleur froide des deux groupe de fils que vous venez de faire dans le wago (-) 
- placer les deux fils de couleur chaude des deux groupe de fils que vous venez de faire dans le wago (+)
- placer le fils "signal" du moteur "1" au niveau de la broche en face du "D2" sur l'esp8266
- placer le fils "signal" du moteur "2" au niveau de la broche en face du "D1" sur l'esp8266

![montage-elec-step2-simple](./photo-markd/montage-step2-simple.png)

![montage-elec-step2-complet](./photo-markd/montage-step2complet.png)

<h3>chapitre 4 montage dans le chassis : </h3>

- on as choisie une boite de dérivation electrique avec 8 entrées 

- on commence par retiré un partie du joint en du dessous du boitier 
![mod-chassis](./photo-markd/mod-chassis.png)

- faite chauffer la colle chaude et placé les moteur à l'arrière du boitier 
![emplacement-moteur](./photo-markd/emplacement-moteur.jpg)

- à l'avant du chassis vous pouvez placer la roulette et adapté la hauteur avec un bouchon en liège et de la colle chaude 
![roulette](./photo-markd/roulette.jpg)


- débranché les fils moteur au niveau de leur jonction les fils groupé 
- faite passer les fils des moteurs à traver les deux entrées en dessous de la boite de dérivation 
- reconnecter les groupe de fils et les fils moteurs 

- commencer par mettre les wago dans le fond de la boite-chassis puis la plaque de prototypage attender d'avoir programmer l'esp avant de mettre le boitier de pile et de refermé le boitier 

<h3>chapitre 5 programmation de l'esp : </h3>

vous pouvez soit téléchager le fichier en .ino pour pouvoir l'ouvrir avec IDE Arduino et les fichiers des pages sur visual-studio-code 

<h3>chapitre 6 ajout des options : </h3>

<h3>chapitre 7 Les autres version du petitbot : </h3>

<h3>liste de liens pour l'achat </h3>

https://www.rapidonline.com/rapid-2wd-servo-robot-platform-70-6415

boite de dérivation electrique , il sert de boitier externe. 
=> grande taille(https://www.leroymerlin.fr/produits/boite-de-derivation-etanche-en-saillie-debflex-12-entrees-65104445.html 15€) 
=> petite taille (https://www.leroymerlin.fr/produits/boitier-de-connexion-etanche-en-saillie-debflex-8-65104354.html 3€)

boitier a pile : (https://www.amazon.fr/CABLEPELADO-Support-batterie-interrupteur-Tension/dp/B07Q25XNJ5?dib=eyJ2IjoiMSJ9.BFL6s120iTYlGAsGwcGsyZnZvyMALaf54WHZWPwP_Z0tr7D-zjhRkOd-hwP03EfjjikO-pADcI-ijZN3KHZFY3-xSzThEOqi6aWVcxiZhZNg5tpqwIW61mqkF5vES5y6rZzSNFmaDNjZmVTHzn6hRnaWpKrihjCxGBsBrA9XPaiHu2OP4rbaoUHKLKC0wpAn1whVhVu2VNKzK7NO1G2ShXf_AgnmxD0wKSDrtC_l0k35z6ZJ1QYJsbquS8vzZS4BkX4278YrDQ-v69cBRQtnmFm8b1LDCS4En-nfZsgsSdU.1y_NHmnwbWJhcVZ9ds6cYTyPUjbWeDlOgRiV4VQsgIM&dib_tag=se&keywords=boitier%2Bpile%2Baa%2Bpour%2B4%2Bpiles&qid=1777554385&sr=8-13&th=1  8€)

batterie 18650 composant de charge (https://fr.aliexpress.com/item/1005008762313099.html?spm=a2g0o.productlist.main.3.81d1a4K5a4K5Ko&algo_pvid=8acbe006-3760-4a6a-a753-1b4b3b46e080&algo_exp_id=8acbe006-3760-4a6a-a753-1b4b3b46e080-2&pdp_ext_f=%7B%22order%22%3A%2281%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21EUR%211.38%210.99%21%21%2110.79%217.70%21%402103890917775555335548967e3938%2112000046560334680%21sea%21FR%210%21ABX%211%210%21n_tag%3A-29910%3Bd%3Ac01cf778%3Bm03_new_user%3A-29895%3BpisId%3A5000000203551276&curPageLogUid=iv3x6efRg7TC&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005008762313099%7C_p_origin_prod%3A 1€)


boitier pour la batterie 18650 : (https://fr.aliexpress.com/item/1005008905937032.html?spm=a2g0o.productlist.main.4.63993a37q7DkNt&aem_p4p_detail=202604300632045523531614246580004589694&algo_pvid=6585d5e3-70be-4779-a36c-94cafbeb3c85&algo_exp_id=6585d5e3-70be-4779-a36c-94cafbeb3c85-3&pdp_ext_f=%7B%22order%22%3A%22942%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21EUR%211.65%211.64%21%21%2112.85%2112.85%21%40211b61a417775559243313399e1192%2112000047143660742%21sea%21FR%210%21ABX%211%210%21n_tag%3A-29910%3Bd%3Ac01cf778%3Bm03_new_user%3A-29895&curPageLogUid=l3PqUGMzWZsG&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005008905937032%7C_p_origin_prod%3A&search_p4p_id=202604300632045523531614246580004589694_1 2€)

pile 18650 (10€) 
---
connecteur pour les fils , wago 5fils  (https://fr.aliexpress.com/item/1005010094735670.html?spm=a2g0o.productlist.main.5.7e3425a82xKXVv&algo_pvid=3e63c5d3-ee75-4491-9de5-c8744789cb01&algo_exp_id=3e63c5d3-ee75-4491-9de5-c8744789cb01-4&pdp_ext_f=%7B%22order%22%3A%22434%22%2C%22spu_best_type%22%3A%22price%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21EUR%214.93%214.09%21%21%2138.46%2131.91%21%402103847817775560939894093e1df0%2112000051124972590%21sea%21FR%210%21ABX%211%210%21n_tag%3A-29910%3Bd%3Ac01cf778%3Bm03_new_user%3A-29895&curPageLogUid=WxYYlbO58he4&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005010094735670%7C_p_origin_prod%3A 12€)

Esp8266 d1 mini en usb c (3.2€ WeMos D1 Mini Pro V3.0 NodeMcu 4 mo/16 mo octets Lua WIFI carte de développement Internet des objets basée sur ESP8266 CH340G Nodemcu V2) 

fils (1€ https://fr.aliexpress.com/item/1005003250665155.html?spm=a2g0o.productlist.main.36.29d25HYo5HYozI&aem_p4p_detail=20260430070612608172473510140004622218&algo_pvid=5c793725-4c72-449a-8f96-20955e714c85&algo_exp_id=5c793725-4c72-449a-8f96-20955e714c85-35&pdp_ext_f=%7B%22order%22%3A%221093%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21EUR%213.80%210.99%21%21%214.33%211.13%21%40211b80f717775579722225393eca07%2112000028309478192%21sea%21FR%210%21ABX%211%210%21n_tag%3A-29910%3Bd%3Ac01cf778%3Bm03_new_user%3A-29895%3BpisId%3A5000000203551292&curPageLogUid=tTvLf5BW2eD8&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005003250665155%7C_p_origin_prod%3A&search_p4p_id=20260430070612608172473510140004622218_9)

bouton led (0.6€ https://fr.aliexpress.com/item/1005003178868423.html?spm=a2g0o.productlist.main.4.ec63OrDtOrDtfH&aem_p4p_detail=2026043007092913458394020162240005012336&algo_pvid=3cf6d4ca-d2f7-464b-93db-3feb64f715ce&algo_exp_id=3cf6d4ca-d2f7-464b-93db-3feb64f715ce-3&pdp_ext_f=%7B%22order%22%3A%22138%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21EUR%210.57%210.56%21%21%210.65%210.64%21%40211b804117775581695226572e7c3a%2112000044760847186%21sea%21FR%210%21ABX%211%210%21n_tag%3A-29910%3Bd%3Ac01cf778%3Bm03_new_user%3A-29895&curPageLogUid=mA1cV0CQX7Wv&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005003178868423%7C_p_origin_prod%3A&search_p4p_id=2026043007092913458394020162240005012336_1) 

moteur fs90r (4.5€ https://fr.aliexpress.com/item/1005009332783738.html?spm=a2g0o.productlist.main.3.5b1aJ6CFJ6CFa0&algo_pvid=b78c1de7-662a-4f8f-8916-71b876a0c304&algo_exp_id=b78c1de7-662a-4f8f-8916-71b876a0c304-0&pdp_ext_f=%7B%22order%22%3A%2215%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21EUR%2115.65%2111.89%21%21%21122.05%2192.79%21%4021038e6617775587342935406eacef%2112000048783184269%21sea%21FR%210%21ABX%211%210%21n_tag%3A-29910%3Bd%3Ac01cf778%3Bm03_new_user%3A-29895%3BpisId%3A5000000205215073&curPageLogUid=uL0FQ1VBu1vN&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005009332783738%7C_p_origin_prod%3A) 
