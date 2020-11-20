#include <coder.h>
// fonction bas niveau de commande des moteurs

volatile long int posM1 ;   // volatile pour modif dans l'interruption  multi processus
volatile long int posM2 ;   // position sur 32 bits
volatile long int posM1Anc = 0;   // volatile pour modif dans l'interruption  multi processus
volatile long int posM2Anc = 0;   // position pr�c�dente pour le calcul de la vitesse
volatile long int posM1old = 0;   // volatile pour modif dans l'interruption  multi processus
volatile long int posM2old = 0;   // position pr�c�dente pour le calcul de la vitesse

void setup_coder_pin()
{
  // interruption
  
    posM1 = 0 ; posM2 = 0 ;
	pinMode( M1In1, INPUT); // inter
	pinMode( M1In0, INPUT); // quadrature
	attachInterrupt( M1In1, ISRM1, FALLING);  // init interrupt function vector for M1In1
	// moteur 2
	pinMode( M2In1, INPUT); // inter
	pinMode( M2In0, INPUT); // quadrature
	attachInterrupt( M2In1, ISRM2, FALLING);  // init interrupt function vector for M1In2
}

// interruption fourche optique du moteur 1
void ISRM1()
{   
   noInterrupts();  // zone ex
 	// tester M1In0
    if (digitalRead( M1In0) == HIGH) posM1++;  // forward rotation
	else posM1--;  // backward rotation
	
	interrupts();  // zone ex
  
}
// interruption fourche optique du moteur 2
void ISRM2()
{
	noInterrupts();  // zone ex
  // tester M1In0
	if (digitalRead( M2In0) == HIGH) posM2++;  // forward rotation
	else posM2--;
	
	interrupts();  // zone ex
  
}

void depl_value(int *d1, int *d2)
{
  *d1 = posM1;
  *d2 = posM2;
}


void delta_value(int *delta1, int *delta2)
{
	*delta1 = posM1 - posM1Anc ;
	*delta2 = posM2 - posM2Anc ;
  posM1old = posM1Anc;
  posM2old = posM2Anc;
  posM1Anc = posM1;
  posM2Anc = posM2;
}

