#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "moteur.h"
#include <string.h>

#define DEBUG 0 //0 desactive et 1 active

void moteur_init(Moteur* moteur)
{
	moteur->uartFile = fopen(UART_PERIPH, "w");
	strncpy(moteur->commande,"",10);
	moteur_reset_parcours(moteur);
	moteur->obstacle=0;
}

void moteur_commande(Moteur* moteur,const char* input) {
	if(strcmp(input,moteur->commande)==0)
	{
	#if DEBUG  // debug avec la console
		printf("commande déjà en cours : %s \n", moteur->commande);
	#endif
	}
	else
	 {
		#if DEBUG  // debug avec la console
		printf("commande : %s, ancienne commande : %s \n", input, moteur->commande);
		#endif
		strncpy(moteur->commande,input,10);
		fprintf(moteur->uartFile,"%s\n",input);
	 }
}



void moteur_contourne_droite(Moteur* moteur)
{	
	strncpy((moteur->parcours[0]).commande,"av",10);
	(moteur->parcours[0]).duree = 2;
	strncpy((moteur->parcours[1]).commande,"st",10);
	(moteur->parcours[1]).duree = 2;
	strncpy((moteur->parcours[2]).commande,"ar",10);
	(moteur->parcours[2]).duree = 2;
	strncpy((moteur->parcours[3]).commande,"st",10);
	(moteur->parcours[3]).duree = 2;
	strncpy((moteur->parcours[4]).commande,"dr",10);
	(moteur->parcours[4]).duree = 2;
	strncpy((moteur->parcours[5]).commande,"st",10);
	(moteur->parcours[5]).duree = 2;
	strncpy((moteur->parcours[6]).commande,"st",10);
	(moteur->parcours[6]).duree = 2;
	


	
	moteur->num_etape=0; // active le parcours

}

void ssleep(float t)
{
	usleep((int)(t*1000000));
}

void moteur_reset_parcours(Moteur* moteur)
{	
	int i;
	moteur->num_etape=-1;
	for (i=0;i<10 ;i++){
		strncpy((moteur->parcours[i]).commande,"",10);
		(moteur->parcours[i]).duree =0;
	}
}

void moteur_update(Moteur* moteur)
{
	clock_gettime(CLOCK_MONOTONIC,&(moteur->t2));
	if ((((double)((moteur->t2).tv_sec)+(double)((moteur->t2).tv_nsec)*1.0e-9) - ((double)((moteur->t1).tv_sec)+(double)((moteur->t1).tv_nsec)*1.0e-9))>moteur->timer)
	{
		if (moteur->num_etape!=-1){
			moteur_commande(moteur,(moteur->parcours[moteur->num_etape]).commande);
			(moteur->num_etape)++;
			moteur->timer = (moteur->parcours[moteur->num_etape]).duree;
			
		}
		if (moteur->num_etape >10 || ((moteur->parcours[moteur->num_etape]).duree==0 && moteur->num_etape !=-1))
		{
			moteur_reset_parcours(moteur);
		}
		clock_gettime(CLOCK_MONOTONIC,&(moteur->t1));
		#if DEBUG
			printf("Etape numéro : %i \n",moteur->num_etape);
		#endif
	}	

}
void moteur_avancer(Moteur* moteur){moteur_commande(moteur,"av");}
void moteur_reculer(Moteur* moteur){moteur_commande(moteur,"ar");}
void moteur_droite(Moteur* moteur){moteur_commande(moteur,"dr");}
void moteur_gauche(Moteur* moteur){moteur_commande(moteur,"ga");}
void moteur_stop(Moteur* moteur){moteur_commande(moteur,"st");}
void moteur_settimer(Moteur* moteur, double timer){moteur->timer = timer;}



