#include "moteur.h"
#include "ultrason.h"
#include "camera.h"
#include <stdio.h> //éviter warning printf
#include <sys/types.h>
#include <sys/stat.h>
#define DEBUG 0 //commenter pour enlever tous les printf.



main()
{	
	Camera camera;
	Ultrason ultrason;
	Moteur moteur;
	
	/*INITIALISATION CAMERA THERMIQUE*/	
	camera_init(&camera,3);
	
	/* initialisation ultrason */
	ultrason_init(&ultrason,0x07,0x15);

	/* INITIALISTION MOTEUR */
	moteur_init(&moteur);
	
	
	while(1)
	{
		ultrason_update(&ultrason);
		camera_update(&camera);
		moteur_update(&moteur);
		
		
		
		if (moteur.obstacle==1){
			moteur_contourne_droite(&moteur);
			moteur.obstacle=1;
		}
		
		if (ultrason.distance <15 && ultrason.distance >0)
		{
			moteur_commande(&moteur,"st");
			moteur.num_etape=-1; // On annule le parcours
		}
		else{
			if(moteur.num_etape==-1) //On vérifie qu'il n'y a pas de parcours en cours.
			{
				if(camera.cible == -1)
				{
					moteur_commande(&moteur,"st"); //on s'arrete.
				}
				else if(camera.cible == 0)
				{
					moteur_commande(&moteur,"tg"); //tourne à gauche.
				}
				else if (camera.cible < 3)
				{
					moteur_commande(&moteur,"ga"); //tourne à gauche.
				}
				else if (camera.cible < 5)
				{
					moteur_commande(&moteur,"av"); // tout droit.
				}
				else if (camera.cible == 7)
				{
					moteur_commande(&moteur,"td"); // tourne à droite
				}
				else if (camera.cible < 8)
				{
							
					moteur_commande(&moteur,"dr"); // tourne à droite
				}
			}
		}


	}
	
	moteur_commande(&moteur,"st");
	/*Fermeture des I2C : */
	close(camera.i2cFile); //fermeture de l'i2c1
	close(ultrason.i2cFile); //fermeture de l'i2c2	
	close(moteur.uartFile);
} 
