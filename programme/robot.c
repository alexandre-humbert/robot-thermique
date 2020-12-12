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


	unsigned char I2C1_WR_Buf[MAX_BUFF_SIZE_WR]; // Buffer I2C1 donnees envoyees
	unsigned char I2C1_RD_Buf[MAX_BUFF_SIZE_RD]; // Buffer I2C1 donnees recues
	float temp_amb; // Température ambiante
	int x = 0; //Int qui permet de faire des tests sur de plus courtes durées que while(1)
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
		unsigned char I2C2_WR_Buf[MAX_BUFF_SIZE];// Buffer I2C2 donnees envoyees
		unsigned char I2C2_RD_Buf[MAX_BUFF_SIZE];// Buffer I2C2 donnees recues


		/* CODE MOTEUR */
		if(mesure_distance(i2c2file, I2C2_WR_Buf, I2C2_RD_Buf)<=15) //On approche pas à + de 15 cm de tout objet
		{
			commande_robot("st", ancienne_commande);
		}

		else //Si y'a rien à moins de 15 cm :
		{
			if(cible >=0) //On vérifie qu'une cible (objet chaud) est présente. Via l'indice de la colonne la plus chaude. (= -1 si aucun objet chaud est detecté)
			{
				if(cible == 0) //position objet chaud = 0 => si l'objet est tout à gauche
				{
					commande_robot("ga", ancienne_commande); //Voir code teensy pour + de détail : tourne à gauche fort : 1 roue va 2 fois plus vite que l'autre.
				}
				else if (cible <3) // position objet chaud entre 1 et 2
				{
					commande_robot("fr", ancienne_commande); //tourne à gauche moins fort : une roue va 1.5 fois plus vite que l'autre
				}
				else if (cible <5) // position objet chaud entre 3 et 4
				{
					commande_robot("av", ancienne_commande); // tout droit : les deux roues sont à la même vitesse
				}
				else if (cible ==7) // position objet chaud = 7 => si l'objet est tout à droite
				{
					commande_robot("dr", ancienne_commande); // tourne à droite moins fort : une roue va 1.5 fois plus vite que l'autre
				}
				else if (cible <8) // position objet chaud entre 5 et 6
				{	
					commande_robot("fl", ancienne_commande); // tourne à droite fort : 1 roue va 2 fois plus vite que l'autre.
				}
			}
			else // Si aucun objet chaud (cible) est détecté alors on arrête le robot (= arrêt des moteurs).
			{
				commande_robot("st", ancienne_commande);
			}
		}
		

	}	
	//Sortie de boucle while :
	commande_robot("st", ancienne_commande); //on arrête les moteurs
	/*Fermeture des I2C : */
	close(i2c1file); //fermeture de l'i2c1
	close(i2c2file); //fermeture de l'i2c2	
} 
