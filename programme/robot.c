#include "moteur.h"
#include "ultrason.h"
#include "camera.h"
#include<stdio.h> //éviter warning printf
#define DEBUG 0 //commenter pour enlever tous les printf.

void contourne_droite(unsigned char* ancienne_commande)
{
	commande_robot("st", ancienne_commande); // on recul un peu
	usleep(1000000); //0.5sec
	//On va essayer de le contourner par la droite 
	commande_robot("av", ancienne_commande); // on recul un peu
	usleep(1000000); //0.5sec
	commande_robot("st", ancienne_commande); // on recul un peu
	usleep(1000000); //0.5sec


	commande_robot("td", ancienne_commande); // pivote à droite
	usleep(2500000); //2sec
	commande_robot("st", ancienne_commande); // on recul un peu
	usleep(1000000); //0.5sec

	commande_robot("ar", ancienne_commande); //on avance mais plus de détection d'obstacle, on prie pour que y'ait rien à sa droite
	usleep(750000); //1sec
	commande_robot("st", ancienne_commande); // on recul un peu
	usleep(1000000); //0.5sec

	commande_robot("tg", ancienne_commande); //pivote à gauche : soucis avec cette commande, la roue va à la vitesse max alors que j'ai /2 sur la teensy. Sur la commande pivote droite il n'y a pas ce soucis ...
	usleep(1350000); //2sec
	commande_robot("st", ancienne_commande); // on recul un peu
	usleep(1000000); //0.5sec

}


main()
{
	unsigned char ancienne_commande[2]="st"; //stock la commande en cours pour ne pas relancer la même commande en boucle .. de base, st
	
	/*INITIALISATION CAMERA THERMIQUE*/	
	int i2c1file;
	i2c1_init(&i2c1file); //On passe le paramètre par référence.
	unsigned char I2C1_WR_Buf[MAX_BUFF_SIZE_WR]; // Buffer donnees envoyees
	unsigned char I2C1_RD_Buf[MAX_BUFF_SIZE_RD]; // Buffer donnees recues
	float temp_amb; // Température ambiante
	float sum_tab; //Valeur qui stocke la somme de toutes les valeurs du tableau renvoyé par la caméra thermique
	
	/* initialisation ultrason */
	unsigned char I2C2_WR_Buf[MAX_BUFF_SIZE];
	unsigned char I2C2_RD_Buf[MAX_BUFF_SIZE];
	int i2c2file;
	i2c2_init(&i2c2file); //on passe par référénce
	
	/*INITIALISATION recherche source chaude*/
	int cible; //indice de la cible = indice de la colonne du tableau la plus chaude
	int cibles[9]; //tableau qui permet de moyenner : tab[x] correspond au nombre de fois qu'a été renvoyé l'indice x par la fonction qui chercher l'indice de la colonne la plus chaude
	int max_cible; //permet de stocker temporairement le maximum du tableau ci-dessus
	int i = 0;

	/* Initialisation obstacle */
	int obstacle = 0; //Au début, aucun obstacle devant

	while(1)
	{

		
		if(obstacle==1) //si la caméra détecte un objet chaud loin alors la somme est proche de celle d'une somme de température ambiante
		{
			temp_amb = get_temp_amb(i2c1file, I2C1_RD_Buf, I2C1_WR_Buf); // Acquisition de la tempréature ambiante
			get_image(i2c1file, I2C1_RD_Buf, I2C1_WR_Buf);
			cible = indice_chaleur(I2C1_RD_Buf,temp_amb, &sum_tab);
			#ifdef DEBUG
				printf("somme tableau : %f \n", sum_tab);
			#endif
			if(sum_tab<6249)//Si la camera voit un objet mais loin (6249 a été determiné via des tests.
			{
				while(mesure_distance(i2c2file, I2C2_WR_Buf, I2C2_RD_Buf)<=15)
				{
					/* SEQUENCE CONTOURNEMENT OBJET */
					contourne_droite(ancienne_commande);
					/*FIN SEQUENCE*/
					//On re mesure, si < 15 alors on a pas assez décalé vers la droite donc on recommence
				}
				obstacle = 0;
			}
			
			else //si la camera détecte un objet chaud très près alors la somme est élevée
			{
				commande_robot("st", ancienne_commande); // on stop les moteurs
				while(mesure_distance(i2c2file, I2C2_WR_Buf, I2C2_RD_Buf)<=15)
				{
					//Sans la boucle on prend le risque de partir dans la séquence pour contourner un objet
					usleep(1500000); //1,5 seconde entre chaque tentative
				}
				obstacle = 0; //plus d'obstacle devant
			}
		}

		/* CODE MOTEUR */
		else if(mesure_distance(i2c2file, I2C2_WR_Buf, I2C2_RD_Buf)<=15) //On approche pas à + de 15 cm de la source de chaleur
		{
			commande_robot("st", ancienne_commande);
			obstacle = 1; //Un obstacle est présent
		}
		/*moteur : commande_robot(const char* nom_commande) avec nom de commande : "av", etc */

		else
		{	max_cible =0;
			for(i=0;i<9;i=i+1)
			{
				cibles[i] =  0;

			}
			for(i=0;i<5;i=i+1)
			{
				usleep(30000);
				temp_amb = get_temp_amb(i2c1file, I2C1_RD_Buf, I2C1_WR_Buf); // Acquisition de la tempréature ambiante
				get_image(i2c1file, I2C1_RD_Buf, I2C1_WR_Buf);
				cible = indice_chaleur(I2C1_RD_Buf,temp_amb, &sum_tab);
				cibles[cible+1] =  cibles[cible+1] + 1;

			}
			for(i=0;i<9;i=i+1)
			{
				if(cibles[i] >= max_cible)
				{
					max_cible = cibles[i];
					cible = i-1;
				}

			}
			#ifdef DEBUG
			printf("Cible après moyennage : %i \n", cible);
			#endif
			obstacle = 0; //pas d'obstacle
			if(cible >=0) //On vérifie qu'une cible est présente. Via l'indice de la colonne la plus chaude.
			{
				if(cible == 0)
				{
					commande_robot("ad", ancienne_commande); //tourne à gauche.
				}
				else if (cible <3)
				{
					commande_robot("br", ancienne_commande); //tourne à gauche.
				}
				else if (cible <5)
				{
					commande_robot("ar", ancienne_commande); // tout droit.
				}
				else if (cible ==7)
				{
					commande_robot("ag", ancienne_commande); // tourne à droite
				}
				else if (cible <8)
				{
								
					commande_robot("bl", ancienne_commande); // tourne à droite
				}
			}
			else
			{
				commande_robot("st", ancienne_commande);
			}
		}
		

	}	commande_robot("st", ancienne_commande);
		/*Fermeture des I2C : */
		close(i2c1file); //fermeture de l'i2c1
		close(i2c2file); //fermeture de l'i2c2	
} 
