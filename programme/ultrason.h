#ifndef H_ULTRASON
#define H_ULTRASON

#define I2C_PERIPH_2 "/dev/i2c-2" // Bus du capteur ultrason
#define ADDR_SRF02 0x70 // Adresse du capteur ultrason
#define MAX_BUFF_SIZE 4 
#include <time.h>

typedef struct Ultrason Ultrason;
struct Ultrason
	{
		int i2cFile; // Stock le retour de la fonction open() pour l'i2c
		int distance; // Stock distance entre robot et l'objet devant
		int luminosite; // Stock luminosité de l'environnement ou le robot est
		double timer;
		struct timespec t1; // temps 1
		struct timespec t2; // temps 1
	// Les deux temps permettre de créer un chronomètre
};


void ultrason_init(Ultrason* ultrason,char range, char gain);
void ultrason_requete_mesure(Ultrason* ultrason);
void ultrason_lecture_distance(Ultrason* ultrason);
void ultrason_lecture_luminosite(Ultrason* ultrason);
void ultrason_mesure(Ultrason* ultrason);
void ultrason_update(Ultrason* ultrason);
void ultrason_settimer(Ultrason* ultrason, double timer);
#endif
