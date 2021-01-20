#include "ultrason.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{

	int mode, opt;
	int gain, range;
	gain = 31;
	range = 255;
	mode = 0;
	int delay=0;
        while ((opt = getopt(argc, argv, "hwg:r:t:")) != -1) {
            switch (opt) {
            case 'h':
                printf("Affiche les données du capteur ultrason\n");
                printf("Options : \n");
                printf("-w Affiche un texte formaté pour le web\n");
                printf("-g Modifie le gain\n");
                printf("-r Modifie la portée \n");
                printf("-t Aoute un delai en secondes\n");
                exit(EXIT_SUCCESS);
                break;
            case 'w':
                mode = 1;
                break;
            case 'g':
                gain = atoi(optarg);
                break;
            case 'r':
                range = atoi(optarg);
                break;
            case 't':
                delay = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s [-w] [-g gain] [-r range] \n",
                        argv[0]);
                exit(EXIT_FAILURE);
            }
        }
	
	sleep(delay);
	Ultrason ultrason;
	ultrason_init(&ultrason,(char)range,(char)gain);
	
	if (mode==0){
		printf("Distance : %i cm \n", ultrason.distance);
		printf("Luminosité : %i \n", ultrason.luminosite);
        }
	else{
		printf("<p>Distance : %i cm </p>\n", ultrason.distance);
		printf("<p>Luminosité : %i </p>\n", ultrason.luminosite);
	}
	
        exit(EXIT_SUCCESS);
}