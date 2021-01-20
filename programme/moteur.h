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


typedef struct Moteur Moteur; //Raccourci pour écrire directement Moteur au lieu de struct Moteur.
struct Moteur
	{
		FILE* uartFile; // ouverture fichier uart
		char commande[10]; // Stock des commandes
		int vitesse; // 
		int obstacle; // 0 : pas d'obstacle, 1 : il y a un obstacle.
		double timer;
		struct timespec t1; // Stock un temps
		struct timespec t2; // Stock un temps
		int num_etape; // Stock le numéro d'étape du parcours.
		struct etape parcours[10]; // Stock un parcours. Notamment pour le contournement d'obstacle.
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
void moteur_rotation(Moteur* moteur);
void moteur_settimer(Moteur* moteur, double timer);
void moteur_reset_parcours(Moteur* moteur);
void moteur_update(Moteur* moteur);
void moteur_changer_vitesse(Moteur* moteur,int vitesse);

#endif
