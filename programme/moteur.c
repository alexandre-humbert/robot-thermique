#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "moteur.h"
#include <string.h>

#define DEBUG 0 //commenter pour cacher printf

void commande_robot(const char* nom_commande, unsigned char* ancienne_commande) {
	if((nom_commande[0] == ancienne_commande[0]) && nom_commande[1] == ancienne_commande[1])
//strcmp(nom_commande, ancienne_commande)==0)
	{
	#ifdef DEBUG  // debug avec la console
		printf("commande déjà en cours : %s \n", nom_commande);
	#endif
	}
	else
	 {
		#ifdef DEBUG  // debug avec la console
			printf("commande : %s, ancienne commande : %c%c \n", nom_commande, ancienne_commande[0], ancienne_commande[1]);
		#endif
	 	ancienne_commande[0] = nom_commande[0];//Mis à jour de la nouvelle commande dans la variable qui mémorise
		ancienne_commande[1] = nom_commande[1];
	 	FILE* uart;
		uart = fopen(UART_PERIPH, "w");
		fprintf(uart, "%s\n",nom_commande);
		fclose(uart);
	 }


}

void commande_robot_arg(const char* nom_commande,const int i) {

	FILE* uart;
	uart = fopen(UART_PERIPH, "w");
	fprintf(uart, "%s %i\n",nom_commande,i);
	fclose(uart);

}
