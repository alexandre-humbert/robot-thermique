#include "moteur.h"
#include "ultrason.h"
#include "camera.h"
main()
{
	int i2c1file;
	i2c1_init(&i2c1file); //On passe le paramètre par référence.
	int i2c2file;
	i2c2_init(&i2c2file); //on passe par référénce
	int etat = 1; //avancer
	unsigned char I2C1_WR_Buf[MAX_BUFF_SIZE_WR]; // Buffer donnees envoyees
	unsigned char I2C1_RD_Buf[MAX_BUFF_SIZE_RD]; // Buffer donnees recues
	float temp_amb; // Température ambiante
	commande_robot("av");
	int x = 0;
	while(x<300)
	{
		x++;
		/* initialisation CAMERA */
		temp_amb = get_temp_amb(i2c1file, I2C1_RD_Buf, I2C1_WR_Buf); // Acquisition de la tempréature ambiante
		get_image(i2c1file, I2C1_RD_Buf, I2C1_WR_Buf); //Les tableaux sont passés par référence pour qu'ils soient modifiés dans la fonction 
		int cible = indice_chaleur(I2C1_RD_Buf,temp_amb); 	/* retourne l'indice de la colonne la plus chaud */


		/* initialisation ultrason */
		unsigned char I2C2_WR_Buf[MAX_BUFF_SIZE];
		unsigned char I2C2_RD_Buf[MAX_BUFF_SIZE];

		/* CODE MOTEUR */
		if(mesure_distance(i2c2file, I2C2_WR_Buf, I2C2_RD_Buf)<=15) //On approche pas à + de 15 cm de la source de chaleur
		{
			commande_robot("st");
			etat=0; //temporaire : robot arreter 
		}
		/*moteur : commande_robot(const char* nom_commande) avec nom de commande : "av", etc */
		else if (etat==0)
		{
			commande_robot("av");
			 etat = 1;
		}

		if (etat==1 && cible >=0){
			if (cible <3){
			commande_robot("fr");
			}
			else if (cible <5){
			commande_robot("av");
			}
			if (cible <8){
			commande_robot("fl");
			}
		}
		

	}	commande_robot("st");
		/*Fermeture des I2C : */
		close(i2c1file); //fermeture de l'i2c1
		close(i2c2file); //fermeture de l'i2c2	
}     