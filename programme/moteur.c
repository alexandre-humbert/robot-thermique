#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "moteur.h"
#include <string.h>

#define DEBUG 0 //commenter pour cacher printf

void commande_robot(const char* nom_commande, unsigned char* ancienne_commande) 
{
	//Avant de lancer une nouvelle commande, on vérifie que l'ancienne n'est pas la même.
	
	if((nom_commande[0] == ancienne_commande[0]) && nom_commande[1] == ancienne_commande[1]) //Si la commande est déjà en cours :
	{
	#ifdef DEBUG 
		//On affiche la commande qui est déjà en cours, et on ne fait rien d'autre.
		printf("commande déjà en cours : %s \n", nom_commande);
	#endif
	}
	
	else //si la nouvelle commande est différente de l'ancienne alors,
	 {
		#ifdef DEBUG  
			// On affiche la commande à effectuer, ainsi que l'ancienne
			printf("commande : %s, ancienne commande : %c%c \n", nom_commande, ancienne_commande[0], ancienne_commande[1]);
		#endif
		
		//On met à jour le tableau de caractères passé par référence qui contient la commande en cours. (ici, ancienne_commande).
	 	ancienne_commande[0] = nom_commande[0];
		ancienne_commande[1] = nom_commande[1];
		
		//Enfin, on envoie la commande à la Teensy via l'UART1
	 	FILE* uart;
		uart = fopen(UART_PERIPH, "w");
		fprintf(uart, "%s\n",nom_commande);
		fclose(uart);
	 }


}

void commande_robot_arg(const char* nom_commande,const int i) 
{
	//Fonction qui permet d'envoyer une commande à la teensy avec un paramètre. 
	//Pour le moment, elle n'est jamais utilisée.
	FILE* uart;
	uart = fopen(UART_PERIPH, "w");
	fprintf(uart, "%s %i\n",nom_commande,i);
	fclose(uart);

}
