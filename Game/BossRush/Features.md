# BossRush
## Concept

Nous visons un jeu comportant au moins 2 boss avec chacun des patterns différents (plus si le temps nous le permet),  notre jeu serait un shooter a la 3ème personne. Son but sera de détruire les noyaux de chaque boss et nous rajouterons un timer pour inciter le joueur à le faire le plus vite possible.


###Charater

- camera a la 3eme personne (vu comme dans le jeu returnal)
- Deplacement classique marche/course
- Tir normal/charger
- Dash (avec une immune degat)
- Tir Special qui permettrais de se teleporter (a voir encore si pertinent dans le jeu)
- Animation
- Collision 
- Particule

###Boss1

####Deplacement
- Immobile

####Pattern 1 

- Attaque 1: lance un cube le scale et tourne sur lui meme
- Attaque 2: pose tous ses cubes au sol les scales jusqu'a un mur et tourne sur lui a la maniere d'une helice
- Attaque 3: Place au dessus du joueur plein de cube oriente de maniere differente et les scales 

####Pattern 2
- Attaque 4: invoque 4 cube qui se scale et forme une croix 

  
####
  Ps: les cubes reviennent au noyau ou disparaissent avant de lancer une nouvelle attaque
  et les paterns se cumules 
  
###Boss2
####Deplacement
- Fait des cercles dans l'arene 

####Pattern 1

- Attaque 1: Tir des balles a la position du joueur
- Attaque 2: Tir 2 enorme balle qui forme un 8 (comme Janus dans Smite)
- Attaque 3: Dash sur le joueur tres vite

####Pattern 2
- Attaque 4: invoque plusieurs balles au dessus de lui et les propages sur la map

####Pattern 3

- Attaque 5: Se mets au centre de l'arene et declenche une vague circulaire sur l'arene (peut le faire plusieur fois de suite )


###Boss3
####Deplacement

- Se teleport a un endroit dans l'arene 

####Pattern 1

- Attaque 1: Tir un laser(quelque chose qui y ressemble du moins) sur le joueur
- Attaque 2: rentre dans le sol suis le joueur et sort en AOE pour toucher le joueur 
- Attaque 3: 

####Pattern 2
- Attaque 4: invoque d'autre entite qui se deplace pres du sol en tirant des lasers

####Pattern 3

- Attaque 5: tir un enorme laser sur une grande parti de l'arene
 
###Arene
- Arene vide avec des bords 
###Nice to have

- Des musique/SFX
- Des VFX pour les boss et les attaques 
- de L'UI au moins pour la barre d'hp des boss et du personnage
- fog via shader (progressif au fur-et-à mesure des boss) pour rajouter de la pression au joueur