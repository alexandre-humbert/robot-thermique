#include <motor.h>
// fonction bas niveau de commande des moteurs

void setup_motor_pin()
{
	// sortie
  pinMode( M1Enable, OUTPUT);  
  pinMode( M2Enable, OUTPUT);  
  pinMode( M1Out0, OUTPUT);  
  pinMode( M1Out1, OUTPUT);  
  pinMode( M2Out0, OUTPUT);  
  pinMode( M2Out1, OUTPUT);  
}

void MotorCC1_TOR( bool bEnable, bool bSens)
{
  if ( bSens)
  {
    digitalWrite( M1Out0, HIGH);    // rive gauche du pont � Vc
    digitalWrite( M1Out1, LOW);    // rive droite du pont � 0
  }
  else
  {
    digitalWrite( M1Out0, LOW);    // rive gauche du pont � 0
    digitalWrite( M1Out1, HIGH);    // rive droite du pont � Vc
  }

  
  digitalWrite( M1Enable, bEnable?HIGH:LOW);    // autoriser ou non
  
}

void MotorCC2_TOR( bool bEnable, bool bSens)
{
  if ( bSens)
  {
    digitalWrite( M2Out0, HIGH);    // rive gauche du pont � Vc
    digitalWrite( M2Out1, LOW);    // rive droite du pont � 0
  }
  else
  {
    digitalWrite( M2Out0, LOW);    // rive gauche du pont � 0
    digitalWrite( M2Out1, HIGH);    // rive droite du pont � Vc
  }
  
  digitalWrite( M2Enable, bEnable?HIGH:LOW);    // autoriser ou non
  
}

void MotorCC1__( int iPuissance, bool bSens)
{
  if ( bSens)
  {
    digitalWrite( M1Out0, HIGH);    // rive gauche du pont � Vc
    digitalWrite( M1Out1, LOW);    // rive droite du pont � 0
  }
  else
  {
    digitalWrite( M1Out0, LOW);    // rive gauche du pont � 0
    digitalWrite( M1Out1, HIGH);    // rive droite du pont � Vc
  }

  analogWrite( M1Enable, iPuissance);
}

void MotorCC2__( int iPuissance, bool bSens)
{
  if ( bSens)
  {
    digitalWrite( M2Out0, HIGH);    // rive gauche du pont � Vc
    digitalWrite( M2Out1, LOW);    // rive droite du pont � 0
  }
  else
  {
    digitalWrite( M2Out0, LOW);    // rive gauche du pont � 0
    digitalWrite( M2Out1, HIGH);    // rive droite du pont � Vc
  }

  analogWrite( M2Enable, iPuissance);
  
}
void MotorCC1( int iPuissance)
{
  if ( iPuissance>=0)
  {
    digitalWrite( M1Out0, HIGH);    // rive gauche du pont � Vc
    digitalWrite( M1Out1, LOW);    // rive droite du pont � 0
  }
  else
  {   // sens n�gatif
    iPuissance = -iPuissance;
    digitalWrite( M1Out0, LOW);    // rive gauche du pont � 0
    digitalWrite( M1Out1, HIGH);    // rive droite du pont � Vc
  }

//Serial.print("M1:");
//Serial.print(iPuissance);
//Serial.print("P1:");
//Serial.print(posM1-posM1old);
  analogWrite( M1Enable, iPuissance);  // 

}
void MotorCC2( int iPuissance)
{
  if ( iPuissance>=0)
  {
    digitalWrite( M2Out0, HIGH);    // rive gauche du pont � Vc
    digitalWrite( M2Out1, LOW);    // rive droite du pont � 0
  }
  else
  {   // sens n�gatif
    iPuissance = -iPuissance;
    digitalWrite( M2Out0, LOW);    // rive gauche du pont � 0
    digitalWrite( M2Out1, HIGH);    // rive droite du pont � Vc
  }

//Serial.print("M2:");
//Serial.print(iPuissance);
//Serial.print(M2Enable);
//Serial.print("P2:");
//Serial.print(posM2-posM2old);
//Serial.println();
  analogWrite( M2Enable, iPuissance);  

}
// divers
void Stop()
{
  MotorCC1_TOR( 0, true);  
  MotorCC2_TOR( 0, true);  
}


