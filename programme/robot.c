#include "moteur.h"
#include "ultrason.h"
#include "camera.h"
#define DEBUG 0 //commenter pour enlever tous les printf.
main()
{
	unsigned char ancienne_commande[2]="st"; //stock la commande en cours pour ne pas relancer la même commande en boucle .. de base, st
	int i2c1file;
	i2c1_init(&i2c1file); //On passe le paramètre par référence.
	int i2c2file;
	i2c2_init(&i2c2file); //on passe par référénce


	unsigned char I2C1_WR_Buf[MAX_BUFF_SIZE_WR]; // Buffer donnees envoyees
	unsigned char I2C1_RD_Buf[MAX_BUFF_SIZE_RD]; // Buffer donnees recues
	float temp_amb; // Température ambiante
	float sum_tab; //Valeur qui stocke la somme de toutes les valeurs du tableau renvoyé par la caméra thermique

	/* Initialisation obstacle */
	int obstacle = 0; //Au début, aucun obstacle devant

	/* initialisation ultrason */
	unsigned char I2C2_WR_Buf[MAX_BUFF_SIZE];
	unsigned char I2C2_RD_Buf[MAX_BUFF_SIZE];

	while(1)
	{
		/* initialisation CAMERA */
		temp_amb = get_temp_amb(i2c1file, I2C1_RD_Buf, I2C1_WR_Buf); // Acquisition de la tempréature ambiante
		get_image(i2c1file, I2C1_RD_Buf, I2C1_WR_Buf); //Les tableaux sont passés par référence pour qu'ils soient modifiés dans la fonction 
		int cible = indice_chaleur(I2C1_RD_Buf,temp_amb, &sum_tab); 	/* retourne l'indice de la colonne la plus chaud */
		//On passe sum tab par référence car c'est la fonction qui va écraser la valeur de cette variable. Elle permet de définir si on doit arrêter le robot ou contourner un obstacle.
		#ifdef DEBUG
			printf("cible : %i , température ambiante : %f \n", cible, temp_amb);
		#endif

		/* SEQUENCE CONTOURNEMENT OBJET */
		if(obstacle==1) //si la caméra détecte un objet chaud loin alors la somme est proche de celle d'une somme de température ambiante
		{
			if(sum_tab<6999)//Si la camera voit un objet mais loin (6999 a été determiné via des tests.
			{
				commande_robot("st", ancienne_commande); // on recul un peu
				usleep(1000000); ///1sec
				//On va essayer de le contourner par la droite 
				commande_robot("av", ancienne_commande); // on recul un peu
				usleep(500000); ///0.5sec
				commande_robot("td", ancienne_commande); // pivote à droite
				usleep(2000000); //2sec
				commande_robot("ar", ancienne_commande); //on avance mais plus de détection d'obstacle, on prie pour que y'ait rien à sa droite
				usleep(1000000); //1sec
				commande_robot("tg", ancienne_commande); //pivote à gauche
				usleep(2500000); //2sec
				obstacle = 0;
			}
			else //si la camera détecte un objet chaud très près alors la somme est élevée
			{
				commande_robot("st", ancienne_commande); // on recul un peu
				obstacle = 0;
				usleep(2500000);
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
		{
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
