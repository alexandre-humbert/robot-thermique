# Le robot à guidage thermique
## Description
Le robot doit suivre une personne grâce à un capteur thermique.
## Liste des choses à faire:
  - [x] Machine virtuelle avec Buildroot et boot NFS
  - [x] Liaison UART entre la carte beagle Bone et la carte Teensy
  - [x] Acquérir une image de la matrice thermique et l'afficher
  - [x] Déterminer la position de l'objet le plus chaud sur une image.
  - [ ] Le robot peut suivre une personne
  - [ ] Le robot ne s'approche pas à plus de 15 cm
  - [ ] Le robot ne percute pas d'obstacle
  - [ ] Le robot uutilise plusieurs capteurs pour un meilleur suivi
  
  ## Matériel
   * Une carte BeagleBone Black
   * Un robot motorisé avec sa carte d'interface moteurs (Teensy 3.1)
   * Un capteur infrarouge Grid-Eye Panasonic
   * Un capteur à ultrasons SRF08
  
  ## Contraintes
  Le développement doit être réalisé en langage C sur une machine virtuelle Linux Debian
