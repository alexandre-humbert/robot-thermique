# Documentation (lire le fichier PDF pour une doc complète)
### Préparation de la machine virtuelle
#### Installation et configuration du partage NFS
#### Installation et configurationde Buildroot
### Installation et configuration du serveur Web
### Installation du programme de gestion des moteurs sur la carte Teensy
Téléchargez et installez  le programm eArduinoIDE depuis ce site : https://www.arduino.cc/en/software  
Téléchargez et installez le programme TeensyDuino depuis ce site : https://www.pjrc.com/teensy/td_download.html  
Choissisez le programme que vous souhaitez installer sur la carte :  
* moteur_teensy si vous ne souhaitez pas avoir d'asservissement
* teensy_v3 si vous souhaitez avoir un asservissement

Ouvrez ce programme dans ArduinoIDE  
Branchez la Teensy, puis dans choisissez Teensy 3.2/3.1 dans Outils -> Type de carte et choissisez votre port dans Outils -> Port  
Ensuite cliquez sur la flèche pour téléverser.

### Création d'une carte SD
##### Sur Linux à partir de Targetfs
Trouvez quel périphérique correspond à votre carte SD avec :  
``` ls /dev ```  
Sur la VM, ce périphérique sera surement */dev/sdd*  
Les instructions sivantes sont données pour un périphérique **sdd**. Adaptez les commandes si le votre est différent.
Ensuite **en root:**  
Effacez la table de partition :
``` dd if=/dev/zero of=/dev/sdd bs=1M count=1 ```  
Créez une nouvelle table : ``` sfdisk -H 255 -S 63 /dev/sdd << EOF ```  
``` 0,1,c,* ```  
``` ,2 ```   
``` EOF ```  
Ensuite créez le partion de boot :  
``` mkfs.vfat /dev/sdd1 -n boot ```
Montez cette partition :  
Créez le point de montage :  
``` mkdir /media/boot ``` puis 
``` mount /dev/sdd1/boot /media/boot ```  
Copiez les fichier sur cette partion :  
``` cd /sec/buildroot/output/images ```
``` cp MLO zImage u-boot.img am335x-boneblack.dtb /media/boot ```
Créez la seconde partiton :  
``` mkfs.ext2 /dev/sdd2 -n ```
Montez cette partition puis placez-vous dans le répértoire copiez votre système de fichier :  
``` cp -r /targetfs/* ./ ```
  
  
##### Sur Windows à partir du fichier image
Il est possible de simplement installer l'image disponible sur ce git avec un logiciel comme [Win32Disk](https://sourceforge.net/projects/win32diskimager/)
Télécharger sdcard.img puis chosissez ce fichier dazns le logiciel, séléctionnez votre carte SD et cliquez sur "Write".
***
### Liens
 * Grove Cape for BeagleBone : https://wiki.seeedstudio.com/Grove_Cape_for_BeagleBone_Series/
 * Gird Eye Panasonic : https://industry.panasonic.eu/components/sensors/grid-eye
 * BeagleBone Black : https://beagleboard.org/black
   * Schéma des GPIO : [ici](https://www.mathworks.com/help/supportpkg/beagleboneio/ug/the-beaglebone-black-gpio-pins.html)
 * Capteurs à ultrasons SRF08 : https://www.robot-electronics.co.uk/htm/srf08tech.html
  
