#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "moteur.h"

#define DEBUG 0 //commenter pour cacher printf

void commande_robot(const char* nom_commande, char* ancienne_commande) {
	if(ancienne_commande == nom_commande)
	{
	#ifdef DEBUG  // debug avec la console
		printf("commande déjà en cours");
	#endif

	}
	else
	 {
	 	ancienne_commande = nom_commande; //Mis à jour de la nouvelle commande dans la variable qui mémorise
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
