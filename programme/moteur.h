#ifndef H_MOTEUR
#define H_MOTEUR
#define UART_PERIPH "/dev/ttyO1"
#include <time.h>
#include <stdio.h>

struct etape // strcuture composée d'une commande et d'une durée
	{
		char commande[10];
		double duree;
	};


typedef struct Moteur Moteur;
struct Moteur
	{
		FILE* uartFile;
		char commande[10];
		int vitesse;
		int obstacle;
		double timer;
		struct timespec t1;
		struct timespec t2;
		int num_etape;
		struct etape parcours[10]; // tableau d'étapes

	};
	

void moteur_init(Moteur* moteur);	
void moteur_commande(Moteur* moteur,const char* input);
void ssleep(float t);
void moteur_avancer(Moteur* moteur);
void moteur_reculer(Moteur* moteur);
void moteur_droite(Moteur* moteur);
void moteur_gauche(Moteur* moteur);
void moteur_stop(Moteur* moteur);
void moteur_contourne_droite(Moteur* moteur);
void moteur_settimer(Moteur* moteur, double timer);
void moteur_reset_parcours(Moteur* moteur);
void moteur_update(Moteur* update);

#endif