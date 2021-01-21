# Programme
## Description
Ce programme en langage C permet au robot de suivre une personne avec une matrice thermique.  
Il est composé de 4 fichiers : 
  * le fichier moteur.c
  * le fichier camera.c
  * le fichier ultrason.c
  * le fichier robot.c
  
 ## moteur.c
 Il contient tout le code permettant de communiquer avec la Teensy en UART. Elle permet d'avancer, reculer, s'arrêter.
 
 ##  caméra.c
 Il contient le code pour acquérir une image depuis la matrice thermique en I2C et le code pour exploiter ces données.
 
 ##  ultrason.c
  Il contient le code pour mesurer une distance avec le capteur à ultrason
  
  ## robot.c
  Il contient le programme principale.  
  Il permet de piloter le robot en fonction des données des capteurs.
  
  ***
  
  
  
## Les programmes de débogage
  
### moteur
Ce programme permet d'enoyer des commandes aux moteurs.    
Exemples:  
  * "moteur av" permet d'envoyer la commande avancer.  
  * "moteur av 1" permet d'envoyer la commande avancer à la vitesse 1.  
Se référer à la partie "Programme de pilotage des moteurs de la Teensy" de la documentation pour plus d'information sur les commandes.
  
### camera
Ce programme permet d'afficher une image de la caméra sous forme de matrice ainsi que la température ambiante et d'autres infromations issues de la caméra.    
 Il dispose de plusieurs options :  
   * -h pour afficher l'aide
   * -t pour ajouter un délai
   * -m pour chosir une temprérature minimale
   * -M pour chosir une tempréature maximale
   * -W pour afficher une image au format SVG
    
### ultrason
Ce programme permet d'afficher la distance et la luminosité mesurées par le capteur ultrason.
Il dispose de plusieurs options :  
  * -h pour afficher l'aide
  * -t pour ajouter un délai
  * -g pour changer le gain
  * -r pour changer la portée
  
  
  # Changelog
## 14/01/2021
  * Modification des durées dans les parcours
  * Modification du mode de calcul des seuils de détection
  
## 07/01/2021
  * Ajout du contournement des obstacles
  * Ajout de timer pour mettre à jourles données des capteurs sans bloquer le programmes
  * Améliorations diverses

## Version du 21/12/2020
  * Améliorations

## Version du 19/12/2020
  * Ajout de commande sur la Teensy
  * Ajout d'une boucle d'évitement d'obstacles
  * Ajout d'une fonction "somme image"
  

## Version du 17/12/2020
  * Ajout d'une fonction moyenneur
  * Correction des directions moteurs
  
## Version du 10/12/2020
 * Ajout d'une boucle pour suivre la personne
  
## Version du 03/12/2020
 * Ajout des fonctions permettant de lire la caméra
 * Ajout des fonctions permettant de lire la distance
 
## Version du 26/11/2020
 * Première version publiée
 
 
 
  

