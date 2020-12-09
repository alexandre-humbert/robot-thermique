#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "moteur.h"

void commande_robot(const char* nom_commande) {

	FILE* uart;
	uart = fopen(UART_PERIPH, "w");
	fprintf(uart, "%s\n",nom_commande);
	fclose(uart);

}

void commande_robot_arg(const char* nom_commande,const int i) {

	FILE* uart;
	uart = fopen(UART_PERIPH, "w");
	fprintf(uart, "%s %i\n",nom_commande,i);
	fclose(uart);

}