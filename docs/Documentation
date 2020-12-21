> fichier .ino : à ouvrir avec arduino, ainsi que teensyduino.
Mettre tous les fichiers dans le dossier librairies d’Arduino pour qu’il les trouve quand on les appelle avec <>. (motor.h, interface.h coder.h, controle.h). Sinon utiliser des "" pour les .h et les mettre dans le même dossier.

> Voir robot.c ligne 75 : bien penser à régler le seuil : 1) Exécuter le code une fois (Teensy débranchée) avec un printf de sum_tab. 
                                                        2) Se positionner à environ 15cm du capteur ultrason et adapter la valeur en fonction du printf().

> Voir camera.c ligne 93 : régler le seuil : En général 24° est un bon seuil. A voir en fonction de l'environnement.

> Screenshots DTS, DTSI. 

> A partir d'une machine virtuelle avec buildroot : make menu config --> BOA (pour le débug), quoi d'autres ? 

> Make device-tree, kernel, filesystem

> Automatisation IP : screenshot du fichier /etc/network/interfaces (permet d'attribuer les bonnes IPs au démarrage de la VM.

> Tests des commandes : sur la carte bootée en nfs : cd /dev puis echo > ttyO1 nom_commande ou alors utiliser l'interfac WEB mais y'a pas toutes les commandes disponibles sur la teensy

> Pour ne avoir 40 trucs branchés sur le PC : 
1) relier le câble ethernet du PC vers le Robot.
2) Brancher la beaglebone à une batterie externe.
3) Brancher un câble USB du PC vers cette même batterie externe (pour avoir la même masse, sinon GTKterm n'est pas content).
4) Brancher l'USB qui permet de contrôler la carte (serial blabla).
5) Ouvrir GTKterm
6) Reboot la carte
7) BOOT NFS
8) retirer l'USB du PC vers la batterie externe
9) retirer l'USB serial
10) sur un terminal PC : en root : telnet 192.168.101.29
11) Il ne reste plus que le câble ethernet de brancher ! (il faut néanmoins en avoir un plus grand que celui fourni). 

> boot en NFS : détailler les étapes (run loadbootenv, run importbootenv, setenv ipaddr XXXX, setenv serverip XXXX, saveenv. saveeenv permet de ne pas avoir à se retaper toutes ces commandes et faire directement run nfsboot.

> Tableau qui regroupe les commandes ?

> Le branchement entre les 2 moteurs sont inversés parce qu'en marche avant y'avait des problèmes d'asservissement. => tout est inversé. Exemple pour le faire avancer on utilise la commande marche arrière d'un robot normal. 

> Liste de matériel : > Une carte BeagleBone Black : https://components101.com/microcontrollers/beaglebone-black-pinout-datasheet
                      > Un robot motorisé avec sa carte d'interface moteurs (Teensy 3.1) : https://www.pjrc.com/teensy/teensy31.html
                      > Un capteur infrarouge Grid-Eye Panasonic : https://b2b-api.panasonic.eu/file_stream/pids/fileversion/4110
                      https://industrial.panasonic.com/cdbs/www-data/pdf/ADI8000/ADI8000COL13.pdf
                      > Un capteur à ultrasons SRF08 : https://www.robot-electronics.co.uk/htm/srf08tech.html
                      
> Fonctionnement : le robot suit une source de chaleur (humaine) et s'arrête quand il arrive trop près. Il évite aussi les obstacles en les contournant par la droite. 
