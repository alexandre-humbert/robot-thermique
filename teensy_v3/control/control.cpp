#include <control.h>

#include <time.h>
#include <TimerOne.h>

extern "C"
{
 #include <coder.h>
 #include <motor.h>
}
// motor characteristics
#define IMPULS_BY_LAP (12 * 52) // 12 pulses by lap and 1/52 reduction ratio

// speed control
#define Period_Asserv 50000  // in microsec (sampling rate of 50ms)
#define kPS 10	// loop gain (25 : min 12 RPM without integral)
#define kPI 3  // integral correction
// Global values
int consign_NB_impuls1 ;
int consign_NB_impuls2 ;
volatile unsigned long int timeout=0;   // unused


void setup_control()
{
   // timer pour l'asservissement
  Timer1.initialize(Period_Asserv); // period value
  Timer1.attachInterrupt( ITimer1);  // accroche de la routine d'interruption
}

void consigne_v1_v2(float v1, float v2)
{
  float consigneVitesseM1;  // 
  float consigneVitesseM2;
  
  consigneVitesseM1 = v1 * (float)Period_Asserv / (60e6);
  consigneVitesseM2 = v2 * Period_Asserv / (60e6);
 
	Serial.print("cons1 =");
	Serial.println(consigneVitesseM1);
	Serial.print("cons2 =");
	Serial.println(consigneVitesseM2);
 
  consign_NB_impuls1 =   (int) (consigneVitesseM1 * IMPULS_BY_LAP) ;
  consign_NB_impuls2 =   (int) (consigneVitesseM2 * IMPULS_BY_LAP) ;
   
}

// Control with integral correction and threshold on integral error
// 
int fonctionP1( int v, int consigne)
{
	float erreur ;
	float puissance ;
	static float erreur_cumul ;
	static float old_consign ;
	
	if (consigne == 0) { erreur_cumul = 0 ; old_consign = 0 ; return 0; } // motor stop
	else if (consigne != old_consign) old_consign=consigne ;
	erreur_cumul += consigne - v ;
	if (erreur_cumul > 255./kPI) erreur_cumul = 255./kPI ;
	erreur = consigne - v ;
 //   if (abs(consigne) > abs(v)) erreur = consigne - v;
//	else erreur = 0 ;
	puissance = kPS * erreur + kPI * erreur_cumul ;
    if (puissance * consigne < 0) puissance = 0 ;
	Serial.print("vitesse =");
	Serial.print(v);
	Serial.print("   consigne =");
	Serial.print(consigne);
	Serial.print("   cumul_error =");
	Serial.print(erreur_cumul);
	Serial.print("   Pwd =");
	Serial.println( puissance);

	if ( fabs(puissance) < 10) return 0;  // arret
	else if ( puissance < -255) return -255;
	else if ( puissance > 255) return 255;
	else return (int)puissance;
}

int fonctionP2( int v, int consigne)
{
	float erreur ;
	float puissance ;
	static float erreur_cumul ;
	static float old_consign ;
	
	if (consigne == 0) { erreur_cumul = 0 ; old_consign = 0 ; return 0; } // motor stop
	else if (consigne != old_consign) old_consign=consigne ;
	erreur_cumul += consigne - v ;
	if (erreur_cumul > 255./kPI) erreur_cumul = 255./kPI ;
	erreur = consigne - v ;
 //   if (abs(consigne) > abs(v)) erreur = consigne - v;
//	else erreur = 0 ;
	puissance = kPS * erreur + kPI * erreur_cumul ;
    if (puissance * consigne < 0) puissance = 0 ;
	Serial.print("vitesse =");
	Serial.print(v);
	Serial.print("   consigne =");
	Serial.print(consigne);
	Serial.print("   cumul_error =");
	Serial.print(erreur_cumul);
	Serial.print("   Pwd =");
	Serial.println( puissance);

	if ( fabs(puissance) < 10) return 0;  // arret
	else if ( puissance < -255) return -255;
	else if ( puissance > 255) return 255;
	else return (int)puissance;
}


// interruption du timer : calcul de la vitesse et asservissement
void ITimer1()
{
	 int vM1, vM2 ;
	if (timeout > 0)  timeout--;  //Not used in this version
	// calcul de vitesse
	delta_value(&vM1,&vM2) ;
//	Serial.print("d1 =");
//	Serial.println(vM1);
//	Serial.print("d2 =");
//	Serial.println(vM2);	
	// appel fonctions d'asservissement
  MotorCC1( fonctionP1( vM1, consign_NB_impuls1));  // calcul de puissance et application
  MotorCC2( fonctionP2( vM2, consign_NB_impuls2));

}
