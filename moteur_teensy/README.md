# Programme de pilotage des moteurs par la cartes Teensy
Ce programme a été développé pour résoudre des problèmes de gestions de vitesse des moteurs.  
Contrairement au précédent, celui-ci n'utilise pas d'asservissement.  
Des coefficients ont été ajoutés pour supprimer la différence de vitesse entre les deux roues.  

## Liste des commandes diponibles :
  * **"av"** pour avancer
    * "av n" permet d'avancer à la vitesse n (n compris entre 0 et 255, de préférence entre 120 et 255).
    * "av n m" permet d'avancer à la vitesse n pour la roue droite et m pour la roue gauche.
    * "av 1", "av 2", "av 3" et "av 4" permettent d'avancer à des vitesses prédéfinies.
  * **"st"** pour s'arrêter
  * **"av"** pour reculer
    * "av n" permet de reculer à la vitesse n.
    * "av n m" permet de reculer à la vitesse n pour la roue droite et m pour la roue gauche.
    * "av 1", "av 2", "av 3" et "av 4" permettent de reculer à des vitesses prédéfinies.
   * **"dr"** pour avancer vers la droite
   * **"ga"** pour avancer vers la gauche
   * **"ad"** pour reculer vers la droite
   * **"ag"** pour reculer vers la gauche
   * **"td"** pour tourner à droite
   * **"tg"** pour tourner à gauche   
   * **"pd"** pour pivoter vers la droite
   * **"pg"** pour pivter vers la gauche
