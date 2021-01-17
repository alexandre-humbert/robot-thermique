#include <Encoder.h>
#define SERIAL Serial1 // Serial1 pour UART BeagleBone et Serial pour USB
IntervalTimer monTimer;
Encoder CodeurDroit(5, 6);
Encoder CodeurGauche(8, 7);
int vitesse=255, rayon=50;
int c1=0, c2=0;
int p1=0, p2=0;

void setup() {
  monTimer.begin(asservissement, 100000);
  SERIAL.begin(9600);
  pinMode(14,OUTPUT);
  pinMode(15,OUTPUT);
  pinMode(16,OUTPUT);
  pinMode(17,OUTPUT);
  pinMode(22,OUTPUT);
  pinMode(23,OUTPUT);


}
void asservissement(){
  SERIAL.print(CodeurGauche.read());
  SERIAL.print(" ");
  SERIAL.println(CodeurDroit.read());
//  if (abs(c1) >100 && abs(c2)>100 && (c2==c1)){
//    if (CodeurGauche.read()-2 < CodeurDroit.read()){
//      p2-=5;
//  }
//    else if (CodeurGauche.read()+2 > CodeurDroit.read()){
//      p2+=5;
//  }
//  }
  Moteur();
  CodeurGauche.write(0);
  CodeurDroit.write(0);
}

void Moteur(){
  if (p2>0){
    analogWrite(22,p2);
    digitalWrite(14,HIGH);
    digitalWrite(15,LOW);
  }
  else{
    analogWrite(22,-p2);
    digitalWrite(14,LOW);
    digitalWrite(15,HIGH);
  }
  
  if (p1>0){
    analogWrite(23,p1);
    digitalWrite(16,HIGH);
    digitalWrite(17,LOW);
  }
  else{
    analogWrite(23,-p1);
    digitalWrite(16,LOW);
    digitalWrite(17,HIGH);
  }
}

void loop(){ 
  if ( SERIAL.available() > 0){
    String cmd;
    int arg1=0,arg2=0,nb_arg=0;
    cmd = SERIAL.readStringUntil(0x0a);
    SERIAL.setTimeout(50);
    if (cmd.indexOf(" ")>=0){
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
    if (cmd.indexOf("av")>=0){
      if(nb_arg==1){
        if (arg1==1){c1=120; c2=120;}
        else if (arg1==2){c1=150; c2=150;}
        else if (arg1==3){c1=200; c2=200;}
        else if (arg1==4){c1=255; c2=255;}
        else{
          c1=arg1; c2=arg1;
        }
      }
      else if(nb_arg==2){
        c1=arg1; c2=arg2;
      }
      else{
        c1=vitesse;c2=vitesse;
      }
    }
    if (cmd.indexOf("ar")>=0){
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
     if (cmd.indexOf("tg")>=0){
      c2 = vitesse-30; c1 = 0;
    }
    if (cmd.indexOf("ga")>=0){
      c2 = vitesse-30; c1 = int(vitesse*rayon/100)+30;
    }
    if (cmd.indexOf("dr")>=0){
      c2 = int(vitesse*1/2); c1 = vitesse;
    }
    if (cmd.indexOf("ad")>=0){
      c2 = -vitesse; c1 = int(-vitesse*1/2);
    }
    if (cmd.indexOf("ag")>=0){
      c2 = int(-vitesse*1/2); c1= -vitesse;
    }
    if (cmd.indexOf("td")>=0){
      c2 = 0
      
      robot; c1 = vitesse;
    }
    if (cmd.indexOf("st")>=0){
      c1 = 0; c2 = 0;
    }
    if (cmd.indexOf("sp")>=0){
      if(nb_arg==1){vitesse=arg1;}
    }
    if (cmd.indexOf("rd")>=0){
      if(nb_arg==1){rayon=arg1;}
    }
    p1 = c1; p2 =c2;
    Moteur();
  }
}
