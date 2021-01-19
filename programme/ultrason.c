#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "ultrason.h"
#include <time.h>

#define DEBUG 0

void ultrason_init(Ultrason* ultrason,char range, char gain)
{
	int flag;
	ultrason->i2cFile = open(I2C_PERIPH_2, O_RDWR); //Ouverture I2C
	if (ultrason->i2cFile < 0) { perror("I2C - Erreur ouverture du bus I2C"); exit(1); }
	
	flag = ioctl(ultrason->i2cFile, I2C_SLAVE_FORCE, ADDR_SRF02);
	if (flag < 0) { perror("I2C - Erreur adresse"); exit(1); }

	unsigned char I2C_WR_Buf[1];
	unsigned char I2C_RD_Buf[2];
	I2C_WR_Buf[0] = 0x02; // registre du ranging
	I2C_WR_Buf[1] = range; // 07*4.3 = 30.1 cm
	
	/* on essaye d'écrire dans le registre d'adresse 0x02 pour changer le range */
	flag = write(ultrason->i2cFile, I2C_WR_Buf, 2);
	if (flag != 2) { perror("Erreur changement ranging ultrason"); exit(1); }

	I2C_WR_Buf[0] = 0x01; // registre du gain
	I2C_WR_Buf[1] = gain; // valeur du gain (passé en paramètre)

	/* on essaye d'écrire dans le registre d'adresse 0x01 pour changer le gain */
	flag = write(ultrason->i2cFile, I2C_WR_Buf, 2);
	if (flag != 2) { perror("Erreur changement gain ultrason"); exit(1); }
	
	/* On initialise aussi le chronomètre */
	ultrason->timer=0.01;
	clock_gettime(CLOCK_MONOTONIC,&(ultrason->t1)); // On initialise le temps t1
	
	/* Lancement de chaque méthodes pour mettre à jour les attributs lors de l'initialisation de l'ultrason */
	ultrason_requete_mesure(ultrason); 
	usleep(100000);
	ultrason_lecture_distance(ultrason); //Mets à jour la distance
	ultrason_lecture_luminosite(ultrason);//Mets à jour la luminosité
}


void ultrason_requete_mesure(Ultrason* ultrason)
{
	int flag;
	unsigned char I2C_WR_Buf[1];
	unsigned char I2C_RD_Buf[2];
	I2C_WR_Buf[0] = 0x00; 
	I2C_WR_Buf[1] = 0x51;
	flag = write(ultrason->i2cFile, I2C_WR_Buf, 2);
	if (flag != 2) { perror("Erreur envoi requete mesure ultrason"); exit(1); }
}

void ultrason_lecture_distance(Ultrason* ultrason)
{
	/* Mis à jour de la distance */
	int flag;
	unsigned char I2C_WR_Buf[1];
	unsigned char I2C_RD_Buf[2];

	I2C_WR_Buf[0] = 0x02; //On se place dans le regsitre 0x02
	flag = write(ultrason->i2cFile, I2C_WR_Buf, 1); //On écrit la requète
	if (flag != 1) { perror("Erreur requete lecture distance");}
	
	flag = read(ultrason->i2cFile, I2C_RD_Buf, 2); //Et on lit la valeur renvoyée. (2 valeurs)
	if (flag != 2) { perror("Erreur lecture distance"); }

	ultrason->distance = 256*(int)I2C_RD_Buf[0] +  (int)I2C_RD_Buf[1]; //On récupère la donnée
}

void ultrason_lecture_luminosite(Ultrason* ultrason)
{
	/* Mis à jour de la luminosité */
	int flag;
	unsigned char I2C_WR_Buf[2];
	unsigned char I2C_RD_Buf[2];


	I2C_WR_Buf[0] = 0x01; //On se place dans le regsitre 0x01
	/* Requète */
	flag = write(ultrason->i2cFile, I2C_WR_Buf, 1); 
	if (flag != 1) { perror("Erreur requete lecture luminosite"); exit(1); }

	/* Lecture */
	flag = read(ultrason->i2cFile, I2C_RD_Buf, 1);
	if (flag != 1) { perror("Erreur lecture luminosite"); }

	/* Mis à jour attribut */
	ultrason->luminosite = (int)I2C_RD_Buf[0]; 
}


void ultrason_mesure(Ultrason* ultrason) 
{
	/* Méthode qui regroupe toutes les méthodes nécessaires pour mettre à jour la distance et la luminosité. */
	ultrason_requete_mesure(ultrason);
	usleep(5000);
	ultrason_lecture_distance(ultrason);
	ultrason_lecture_luminosite(ultrason);

}

void ultrason_update(Ultrason* ultrason)
{
	clock_gettime(CLOCK_MONOTONIC,&(ultrason->t2)); //MAJ temps t2
	
	/* On regarde que le chronomètre (t2 - t1) a atteint son timer. */
	/* c'est pour ne pas trop solliciter le capteur ultrason. En effet, c'est une onde qui se propage et qui revient, donc il a fallu prendre en compte ceci et ne pas renvoyer d"onde avant que l'ancienne ne soit revenue. */
	
	if ((((double)((ultrason->t2).tv_sec)+(double)((ultrason->t2).tv_nsec)*1.0e-9) - ((double)((ultrason->t1).tv_sec)+(double)((ultrason->t1).tv_nsec)*1.0e-9))>ultrason->timer)
	{
		ultrason_mesure(ultrason); //Méthode qui regroupe toutes les méthodes nécessaires pour mettre à jour la distance et la luminosité.
		clock_gettime(CLOCK_MONOTONIC,&(ultrason->t1)); //MAJ t1
		#if DEBUG
			printf("Distance %i cm\n",ultrason->distance);
		#endif
	}
}

void ultrason_settimer(Ultrason* ultrason, double timer){ultrason->timer=timer;} //Choix du temps entre chaque update



