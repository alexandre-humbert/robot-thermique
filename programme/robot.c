#include "moteur.h"
#include "ultrason.h"
#include "camera.h"
#include <stdio.h> //éviter warning printf
#include <sys/types.h> //Pour le chrono 
#include <sys/stat.h> //Pour le chrono
#define DEBUG 0 //commenter pour enlever tous les printf.


main()
{	/* Déclaration d' "objets" pour la caméra, l'ultrason et le moteur */
	Camera camera; //Voir camera.h 
	Ultrason ultrason; //Voir ultrason.h
	Moteur moteur; //Voir moteur.h
	
	/*INITIALISATION CAMERA THERMIQUE*/	
	camera_init(&camera,3);
	
	/* initialisation ultrason */
	ultrason_init(&ultrason,0x15,0x18);
	ultrason.timer=0.1; // Adapter le timer selon le range
	/* INITIALISTION MOTEUR */
	moteur_init(&moteur);
	
	
	while(1)
	{
		/* MISE A JOUR DES INFORMATIONS à chaque itération */
		
		/* ultrason (pour la distance devant le robot) */ 
		ultrason_update(&ultrason); 
		
		/* camera (pour la matrice thermique) */ 
		camera_update(&camera);
		
		/* moteur (pour gérer le déplacement du robot) */
		moteur_update(&moteur);
		
		/* On adapte la vitesse selon la distance entre le robot et l'obstacle ou la personne */
		if (moteur.obstacle ==0){
		 if (ultrason.distance >=50 || ultrason.distance==0){moteur_changer_vitesse(&moteur,4); }
		 if (ultrason.distance <50 && ultrason.distance >=40){ moteur_changer_vitesse(&moteur,3); }
		 if (ultrason.distance <40 && ultrason.distance >=30){ moteur_changer_vitesse(&moteur,2); }
		 if (ultrason.distance <30 && ultrason.distance >=15){ moteur_changer_vitesse(&moteur,1); }
		}
		
		/* Si on trouve un objet à moins de 15 cm du robot : */
		if (ultrason.distance <15 && ultrason.distance >0)
		{	
			#if DEBUG
			printf("sum pix : %f ;; seuil : %f \n", camera.sum_pix, 5*8*27.85 + 3*8*camera.temp_amb); //25.25
			#endif
			 //On coupe les moteurs
			
			/* On regarde si la somme de toute la matrice est très élevé. Ceci nous permet de savoir si l'obstacle est un vrai obstacle OU la source de chaleur */
			if(camera.sum_pix > (6*8*30 + 2*8*camera.temp_amb)) //On regarde si la somme des pixels est supérieur à 6 colonnes de 8 valeurs = 30 + 2 colonnes de 8 valeurs de temp_amb pour savoir si la source est proche ou pas 
			{ //C'est la source de chaleur devant :
				moteur.num_etape=-1; // Donc on annule le parcours en cours et on attend que la source avance :
				moteur_reset_parcours(&moteur); //Et on reset le parcours précédent
				moteur.obstacle = 0;
				while(ultrason.distance <15 && ultrason.distance >0){ultrason_update(&ultrason); ssleep(1);}
				ssleep(1); //permet d'éviter que le robot pense qu'on est un obstacle quand on commence à s'éloigner..		
			}
			else //sinon c'est un obstacle et on doit le contourner
			{
				//Contournement obstacle : 
				if(moteur.obstacle ==0) //Si on a pas déjà rencontré d'obstacle :
				{	moteur_changer_vitesse(&moteur,3);
					moteur_commande(&moteur,"st");
					moteur.obstacle += 1; //compte le nombre d'obstacle qu'on a rencontré
					moteur_contourne_droite(&moteur); //Le parcours 'Contourne_droite' sera parametré et activé dans cette fonction
				}
				
				else if (moteur.num_etape <= 2) //Les deux premières commandes sont st et ar. C'est pour éviter de relancer contourne droite alors qu'on a paas bougé
				{}
				
				else //on rencontre un nouvel obstacle après les deux premières commandes.
				{
					moteur_commande(&moteur,"st");
					moteur.obstacle += 1; //compte le nombre d'obstacle qu'on a rencontré
					moteur_contourne_droite(&moteur); //Le parcours 'Contourne_droite' sera parametré et activé dans cette fonction
					
				}

			}
		}
		/* Si on a rien à moins de 15 cm devant : */
		else
		{
			if(moteur.num_etape==-1 && moteur.obstacle == 0) //On vérifie qu'il n'y ait pas de parcours déjà en cours.
			{
				if(camera.cible == -1) //Si on ne trouve aucune cible
				{
					//moteur_commande(&moteur,"st"); //on s'arrete.
					moteur_rotation(&moteur); // ou on tourne pour trouver quelqu'un
				}
				
				/*Sinon, on a trouvé une cible. L'indice correspond à la colonne ou l'on a détecté la source de chaleur. Donc on se déplace en fonction de cet indice : */
				else if(camera.cible == 0) //Si la source est tout à gauche :
				{
					moteur_commande(&moteur,"tg"); //tourne fort à gauche.
				}
				else if (camera.cible < 3) //Si la source est un peu à gauche :
				{
					moteur_commande(&moteur,"ga"); //tourne à gauche.
				}
				else if (camera.cible < 5) //si la cible est au milieu
				{
					moteur_commande(&moteur,"av"); // tout droit.
				}
				else if (camera.cible == 7) //si la cible est tout à droite
				{
					moteur_commande(&moteur,"td"); // tourne fort à droite
				}
				else if (camera.cible < 8) //si la cible est un peu à droite :
				{
					moteur_commande(&moteur,"dr"); // tourne à droite
				}
			}
		}
	}

/* //En sortie de boucle (jamais puisque boucle infini), il faudrait :  */
	moteur_commande(&moteur,"st");  //couper les moteurs
	/*Fermeture des I2C : */
	close(camera.i2cFile); //fermeture de l'i2c1
	close(ultrason.i2cFile); //fermeture de l'i2c2	

/* Fermeture UART */
	close(moteur.uartFile);
} 
