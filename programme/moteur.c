#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "moteur.h"
#include <string.h>

#define DEBUG 0 //0 desactive et 1 active

void moteur_init(Moteur* moteur)
{
	/* Méthode initialisation moteur */
	moteur->uartFile = fopen(UART_PERIPH, "w"); //Ouverture UART
	strncpy(moteur->commande,"",10); //Initialisation tableau de commande à des commandes vides.
	moteur_reset_parcours(moteur); //Initialisation parcours à des commandes vides ET à une durée nulle.
	moteur->obstacle=0; //pas d'obstacles.
}

void moteur_commande(Moteur* moteur,const char* input) 
{
	/* Vérifie que la commande n'est pas déjà en cours */
	if(strcmp(input,moteur->commande)==0) //input est l'ancienne commande
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
		strncpy(moteur->commande,input,10); //L'ancienne commande prend la valeur de la nouvelle.
		fprintf(moteur->uartFile,"%s\n",input); //On écrit cette commande dans l'UART pour démarrage moteur.
	 }
}



void moteur_contourne_droite(Moteur* moteur)
{	
	/* Reset du parcours précédent */
	moteur_reset_parcours(moteur);
	
	/* Parcours pour contourner l'obstacle : */
	/* stop pendant 1 secondes */
	strncpy((moteur->parcours[0]).commande,"st",10); 
	(moteur->parcours[0]).duree = 1.0;
	
	/* recule pendant 1.1 secondes */
	strncpy((moteur->parcours[1]).commande,"ar",10);
	(moteur->parcours[1]).duree = 1.1;
	
	/* pivote à droite pendant 1 secondes */
	strncpy((moteur->parcours[2]).commande,"td",10);
	(moteur->parcours[2]).duree = 1.0;
	
	/* avance pendant 1.1 secondes */
	strncpy((moteur->parcours[3]).commande,"av",10);
	(moteur->parcours[3]).duree = 1.1;
	
	/* pivote à gauche pendant 1 secondes */
	strncpy((moteur->parcours[4]).commande,"tg",10);
	(moteur->parcours[4]).duree = 1.0;
	
	/* avance pendant 0.75 secondes */
	strncpy((moteur->parcours[5]).commande,"av",10);
	(moteur->parcours[5]).duree = 0.75;
	
	/* stop pendant 0.5 secondes */
	strncpy((moteur->parcours[6]).commande,"st",10);
	(moteur->parcours[6]).duree = 0.5;
	
	/* On passe num_etape à 0 pour dire qu'un parcours est actif */
	moteur->num_etape=0;
	moteur->timer = (moteur->parcours[moteur->num_etape]).duree; //MAJ timer

}

void ssleep(float t)
{
	/* Fonction sleep() maison */
	usleep((int)(t*1000000));
}

void moteur_reset_parcours(Moteur* moteur)
{	
	/* Remet tout comme lors de l'initialisation à la fin d'un parcours */
	int i;
	moteur->num_etape=-1; //indique qu'aucun parcours est en cours
	
	/* Remise le tableau de parcours à vide ainsi que la durée à 0 de chaque étape */
	for (i=0;i<10 ;i++)
	{
		strncpy((moteur->parcours[i]).commande,"",10);
		(moteur->parcours[i]).duree =0;
	}

}

void moteur_update(Moteur* moteur)
{
	clock_gettime(CLOCK_MONOTONIC,&(moteur->t2)); //MAJ temps 2
	
	/* condition de chronomètre. Permet d'attendre que la commande soit fini avant de passer à la suivante : on regarde que t2-t1 > timer */
	if ((((double)((moteur->t2).tv_sec)+(double)((moteur->t2).tv_nsec)*1.0e-9) - ((double)((moteur->t1).tv_sec)+(double)((moteur->t1).tv_nsec)*1.0e-9))>moteur->timer)
	{
		if (moteur->num_etape!=-1) //Si un parcours est en cours :
		{
			moteur_commande(moteur,(moteur->parcours[moteur->num_etape]).commande); // On envoie la commande à l'anti doublon (évite que la commande soit envoyée à l'UART alors qu'elle est déjà en cours)                  
			(moteur->num_etape)++; //On passe à l'étape d'après
			moteur->timer = (moteur->parcours[moteur->num_etape]).duree; // MAJ du timer avec la durée de l'étape suivante
			
		}
		
		/* Si on a atteint le nombre maximum d'étapes (10) OU que la durée soit égal ) 0 ET que le num_etape soit différent de -1, alors on reset le parcours. */
		if (moteur->num_etape >10 || ((moteur->parcours[moteur->num_etape]).duree==0 && moteur->num_etape !=-1))
		{
			moteur_reset_parcours(moteur);
			moteur->obstacle = 0;
		}
		clock_gettime(CLOCK_MONOTONIC,&(moteur->t1)); //MAJ temps 1
		#if DEBUG
			printf("Etape numéro : %i \n",moteur->num_etape);
		#endif
	}	

}

/* Quelques commandes pour du débuage */
void moteur_avancer(Moteur* moteur){moteur_commande(moteur,"av");}
void moteur_reculer(Moteur* moteur){moteur_commande(moteur,"ar");}
void moteur_droite(Moteur* moteur){moteur_commande(moteur,"dr");}
void moteur_gauche(Moteur* moteur){moteur_commande(moteur,"ga");}
void moteur_stop(Moteur* moteur){moteur_commande(moteur,"st");}

void moteur_settimer(Moteur* moteur, double timer){moteur->timer = timer;} //Set le timer !



