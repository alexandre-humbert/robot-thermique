#include "moteur.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{	
	Moteur moteur;
	moteur_init(&moteur);
	int opt;
	
        while ((opt = getopt(argc, argv, "h")) != -1) {
            switch (opt) {
            case 'h':
                printf("Envoyer des commandes aux moteurs\n\n");
                printf("Exemple : \"moteur av\" pour faire avancer le robot\n\n");
                printf("Commandes : \n\n");
                printf("av Pour avancer\n");
                printf("ar Pour reculer\n");
                printf("dr Pour tourner à droite \n");
                printf("ga pour tourner à gauche \n\n");
                printf("Voir la documentation pour les autres commandes \n");
                exit(EXIT_SUCCESS);
                break;
            default:
                fprintf(stderr, "Usage: %s [-h] \n", argv[0]);
                exit(EXIT_FAILURE);
            }
        }	
	
	
	
	
	
	char cmd[10];
	if (argc>1){
		strncpy(cmd,argv[1],10);
	
		if (argc>2){
			strcat(cmd," ");
			strcat(cmd,argv[2]);
			if (argc>3){
				strcat(cmd," ");
				strcat(cmd,argv[3]);
			}
		}
		moteur_commande(&moteur,cmd);
	}
	
}