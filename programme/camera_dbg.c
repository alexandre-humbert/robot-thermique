#include "camera.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
// vérifier que DEBUG 0 dans la bibliothèque caméra avant de compiler

int main(int argc, char* argv[])
{
	int mode, opt;
	int delay=0;
	int min, max;
	int m=0,M=0;
	
	mode = 0;
        while ((opt = getopt(argc, argv, "hwm:M:t:")) != -1) {
            switch (opt) {
            case 'h':
                printf("Affiche les données de la camera\n");
                printf("Options : \n");
                printf("-w Affiche un texte formaté pour le web\n");
                printf("-m Température minimale\n");
                printf("-M Température maximale\n");
                printf("-t Aoute un delai en secondes\n");
                exit(EXIT_SUCCESS);
                break;
            case 'w':
                mode = 1;
                break;
            case 'm':
                min = atoi(optarg);
                m =1;
                break;
            case 'M':
                max = atoi(optarg);
                M =1;
                break;
            case 't':
                delay = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s [-w] [-m val_max] [-M val_min] [-t delai]\n",
                        argv[0]);
                exit(EXIT_FAILURE);
            }
        }
        
	
	sleep(delay);
	Camera camera;
	camera_init(&camera,1);
	if (m){camera.min =min;}
	if (M){camera.max=max;}
	if (mode==0){
		printf("Température ambiante : %0.2f \n", camera.temp_amb);
		printf("Valeur min : %0.2f \n", camera.min);
		printf("Valeur max : %0.2f \n", camera.max);
		printf("Position  : %i \n", camera.indice);
		camera_afficher_image(&camera);
        }
	else{
		printf("<p>Température ambiante : %0.2f </p>\n", camera.temp_amb);
		printf("<p>Valeur min : %0.2f </>\n", camera.min);
		printf("<p>Valeur max : %0.2f </p>\n", camera.max);
		printf("<p>Position  : %i </p>\n", camera.indice);
		camera_gen_image(&camera);
	}
	
        exit(EXIT_SUCCESS);
}