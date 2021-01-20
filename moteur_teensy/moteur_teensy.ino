#include <Encoder.h>
#define SERIAL Serial // Mettre Serial pour communiquer en USB avec un PC et Serial1 pour communiquer en UART avec la BeagleBone
IntervalTimer monTimer;
Encoder CodeurDroit(5, 6); 
Encoder CodeurGauche(8, 7);
int vitesse=255, rayon=50;
int c1=0, c2=0; // Consignes
int p1=0, p2=0; // Puissances
float v1, v2; // Vitesses

void setup() {
  monTimer.begin(asservissement, 100000); // On appelle la boucle d'asservisement toutes les 100ms
  SERIAL.begin(9600);
  // Initialisation entrées/sorties
  pinMode(14,OUTPUT);
  pinMode(15,OUTPUT);
  pinMode(16,OUTPUT);
  pinMode(17,OUTPUT);
  pinMode(22,OUTPUT);
  pinMode(23,OUTPUT);


}

// Boucle d'asserviement : renvoie seulement la vitesse des moteurs v1 et v2 
void asservissement(){
  v1 = (float)(CodeurGauche.read())*1/100; // Valeur approsximative de la vitesse à partir du nombre d'impulsion
  v2 = (float)(CodeurDroit.read())*1/100;
  SERIAL.print(v1); // On envoie la vitesse sur le lien série
  SERIAL.print(" ");
  SERIAL.println(v2);
  CodeurGauche.write(0); // On reset les compteurs d'impulsion
  CodeurDroit.write(0);
  // Ajouter une fonction d'asservissement ici
  // Les consignes sont les variables c1 et c2
  // Les puissances des moteurs sont p1 et p2
  Moteur(); // On appelle la fonction moteur pour mettre à jour les puissance (inutile puisque pas d'asserviseement)
}

// Pilote les moteur avec une puissance +-p1 pour le moteur et +- p2 pour le moteur gauche
void Moteur(){
  if (abs(p1) <50){p1=0;} // Si la puissance demandée est trop faible, on arrête les moteurs
  if (abs(p2) <50){p2=0;}
  if (p2>0){ // moteur 2 en avant
    analogWrite(22,p2);
    digitalWrite(14,HIGH);
    digitalWrite(15,LOW);
  }
  else{ // moteur 2 en arrière
    analogWrite(22,-p2);
    digitalWrite(14,LOW);
    digitalWrite(15,HIGH);
  }
  
  if (p1>0){ // moteur 1 en avant
    analogWrite(23,p1);
    digitalWrite(16,HIGH);
    digitalWrite(17,LOW);
  }
  else{ // moteur 1 en arrière
    analogWrite(23,-p1);
    digitalWrite(16,LOW);
    digitalWrite(17,HIGH);
  }
}

void loop(){ 
  if ( SERIAL.available() > 0){ // Si communication sur port série
    String cmd;
    int arg1=0,arg2=0,nb_arg=0;
    cmd = SERIAL.readStringUntil(0x0a); // On récupère la chaine
    SERIAL.setTimeout(50);
    if (cmd.indexOf(" ")>=0){ // On récupère 0, 1 ou 2 arguments maximum
      if(cmd.indexOf(" ")==cmd.lastIndexOf(" ")){
        arg1 = cmd.substring(cmd.indexOf(" ")+1).toInt();
        nb_arg=1;
      }
      else{
      arg1 = cmd.substring(cmd.indexOf(" ")+1,cmd.lastIndexOf(" ")).toInt();
      arg2 = cmd.substring(cmd.lastIndexOf(" ")+1).toInt();
      nb_arg = 2;
      }
    }
    // On modifie les consignes selon la commande reçue
    if (cmd.indexOf("av")>=0){ // Avancer
      if(nb_arg==1){
        if (arg1==1){c1=120; c2=120;} // Vitesse 1
        else if (arg1==2){c1=150; c2=150;} // Vitesse 2
        else if (arg1==3){c1=200; c2=200;} // Vitesse 3
        else if (arg1==4){c1=255; c2=255;} // Vitesse 4
        else{
          c1=arg1; c2=arg1; // Vitesse arg1
        }
      }
      else if(nb_arg==2){ // Vitesse arg1 arg2
        c1=arg1; c2=arg2; 
      }
      else{
        c1=vitesse;c2=vitesse; // Avancer
      }
    }
    if (cmd.indexOf("ar")>=0){ // Reculer ( identique fonction avancer)
      if(nb_arg==1){
        if (arg1==1){c1=-120; c2=-120;}
        else if (arg1==2){c1=-150; c2=-150;}
        else if (arg1==3){c1=-200; c2=-200;}
        else if (arg1==4){c1=-255; c2=-255;}
        else{
          c1=-arg1; c2=-arg1;
        }
      }
      else if(arg2==2){
        c1=-arg1;c2=-arg2;
      }
      else{
        c1=-vitesse;c2=-vitesse;
      }
    }
    if (cmd.indexOf("ga")>=0){ // Avancer gauche
      c2 = vitesse; c1 = int(vitesse*rayon/100);
    }
    if (cmd.indexOf("dr")>=0){ // Avancer droite
      c2 = int(vitesse*rayon/100); c1 = vitesse;
    }
    if (cmd.indexOf("ad")>=0){ // Reculer droite
      c2 = -vitesse; c1 = int(-vitesse*rayon/100);
    }
    if (cmd.indexOf("ag")>=0){ // Reculer gauche
      c2 = int(-vitesse*rayon/100); c1= -vitesse;
    }
    if (cmd.indexOf("st")>=0){ // Arrêter
      c1 = 0; c2 = 0;
    }
    if (cmd.indexOf("td")>=0){ // Tourner à droite
      c2 = 0; c1= vitesse;
    }
    if (cmd.indexOf("tg")>=0){ // Tourner à gauche
      c2 = vitesse; c1= 0;
    }
    if (cmd.indexOf("pd")>=0){ // Pivoter sur la droite
      c2 = -vitesse; c1= vitesse;
    }
    if (cmd.indexOf("pg")>=0){ // Pivoter sur la gauche
      c2 = vitesse; c1= -vitesse;
    }
    if (cmd.indexOf("sp")>=0){ // Changer la vitesse
      if(nb_arg==1){
        if (arg1==1){vitesse=120;} // Vitesse 1
        else if (arg1==2){vitesse=120;} // Vitesse 2
        else if (arg1==3){vitesse=150;} // Vitesse 3
        else if (arg1==4){vitesse=255;} // Vitesse 4
        else{
          vitesse=arg1;
        }
      }
    }
    if (cmd.indexOf("rd")>=0){ // Changer la rayon de courbure (0-100%)
      if(nb_arg==1){rayon=arg1;}
    }
    
    p1 = c1; p2 =(int)(c2*0.55+10); // Pas d'asserviseement donc les puissances sont égales aux consignes avec seulement une correction sur un moteur
    Moteur(); // On appelle la fonction moteur pour mettre à jour les puissances
  }
}
