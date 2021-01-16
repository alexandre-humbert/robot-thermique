## Description
Ce programme en langage C permet au robot de suivre une personne avec une matrice thermique.  
Il est composé de 4 classes : 
  * La classe moteur
  * la classe camera
  * La classe ultrason
  * la classe robot
  
 ## La classe moteur
 Elle contient tout le code permettant de communiquer avec la Teensy en UART. Elle permet d'avancer, reculer, s'arrêter ..
 
 ## La classe caméra
 Elle contient le code pour acquérir une image depuis la matrice thermique en I2C et le code pour exploiter ces données.
 
 ## La classe ultrason
  Elle contient le code pour mesurer une distance avec le capteur à ultrason
  
  ## La classe robot
  Elle contient un objet de chacune des classes.  
  Elle permet de piloter le robot en fonction des données des capteurs.
  
  ***
  # Changelog

## Version du 21/12/2020
  * Améliorations

## Version du 19/12/2020
  * Ajout de commande sur la Teensy
  * Ajout d'une boucle d'évitement d'obstacles
  * Ajout d'une fonction "somme image"
  

## Version du 17/12/2020
  * Ajout d'une fonction moyenneur
  * Correction des directions moteurs
  
## Version du 17/12/2020
 * Ajout d'une boucle pour suivre la personne
  

