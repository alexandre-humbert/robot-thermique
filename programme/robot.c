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
	int x = 0;
	while(1)
	{
		x++;
		/* initialisation CAMERA */
		temp_amb = get_temp_amb(i2c1file, I2C1_RD_Buf, I2C1_WR_Buf); // Acquisition de la tempréature ambiante
		get_image(i2c1file, I2C1_RD_Buf, I2C1_WR_Buf); //Les tableaux sont passés par référence pour qu'ils soient modifiés dans la fonction 
		int cible = indice_chaleur(I2C1_RD_Buf,temp_amb); 	/* retourne l'indice de la colonne la plus chaud */
		#ifdef DEBUG
			printf("cible : %i , température ambiante : %f \n", cible, temp_amb);
		#endif
		/* initialisation ultrason */
		unsigned char I2C2_WR_Buf[MAX_BUFF_SIZE];
		unsigned char I2C2_RD_Buf[MAX_BUFF_SIZE];


		/* CODE MOTEUR */
		if(mesure_distance(i2c2file, I2C2_WR_Buf, I2C2_RD_Buf)<=15) //On approche pas à + de 15 cm de la source de chaleur
		{
			commande_robot("st", ancienne_commande);
		}
		/*moteur : commande_robot(const char* nom_commande) avec nom de commande : "av", etc */

		else
		{
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