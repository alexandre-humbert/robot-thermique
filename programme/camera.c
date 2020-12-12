#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "camera.h"
#define DEBUG 0 //commenter pour enlever tous les printf.

void i2c1_init(int* i2cFile) //fonction qui initialise l'i2c1 utilisé pour la caméra thermique. On utilise un int* qui permet à la fonction de modifier directement la valeur de cet int passé par référence.
{
	int flag;
	*i2cFile = open(I2C_PERIPH, O_RDWR);
	if (*i2cFile < 0) { perror("i2cOpen error"); exit(1); } //On vérifie que tout se passe bien
	flag = ioctl(*i2cFile, I2C_SLAVE_FORCE, ADDR);
	if (flag < 0) { perror("i2cSetAddress error:"); exit(1); } //On vérifie que tout se passe bien
	//Et voilà, l'i2c1 est maintenant initialisé. Fonction à utiliser uniquement au début du main(), et ne pas oublier de close le fichier à la fin.
}

void get_image(int i2cFile, char* rd, char* wr) //Fonction qui remplit les tableaux WR et RD. RD est celui qui contient les valeurs de la matrice thermique.
{
	int flag;
	wr[0] = 0x80;
	flag = write(i2cFile, wr, 1);
	if (flag != 1) { perror("Error in Second Send_I2C"); exit(1); }
	flag = read(i2cFile, rd, 128);
	if (flag != 128) { perror("Read Error I2C_Byte"); }
}

void affiche_image(char* rd) //Fonction qui affiche la matrice 8*8. En réalité c'est une matrice 16*8 ou 8*16, d'où le j=j+2 dans le deuxième for.
{

	int i,j;
	for (i=0;i<8;i++)
	{
		for (j=0;j<16;j+=2)
		{
			printf("%.1f " ,(float)rd[16*i+j]*0.25); //16*i car c'est un tableau en une dimension. Or il y a 2*8 valeurs (rappel : la matrice est de 8*16 en réalité) à dépasser pour passer à l'indice suivante. //0.25 car d'après la doc, résolution = 0.25°C
		}
		printf("%s","\n"); //saut de ligne
	}
}

int indice_chaleur(char* rd,float temp_amb) //retourne l'indice de la colonne du tableau la plus chaude (indice entre 0 et 7)
{
	float temp_sum[TAILLE_TEMP_SUM]; //tableau de float qui contient la somme de chaque colonne du tableau.
	int i;
	for (i = 0;i<TAILLE_TEMP_SUM;i++) //boucle qui initialise le tableau à 0 pour pouvoir sommer par la suite. Idée d'optimisation : au lieu de l'initaliser à 0, l'initialiser à sa première valeur ?
	{
	    //ce qui donnerait quelque chose comme temp_sum[i]=(float)rd[2*i]; et en modifiant la boucle en dessous comme : for (i = 1; i<TAILLE_TEMP_SUM;i++)
	    temp_sum[i] = 0;
	}


	for (i = 0; i<TAILLE_TEMP_SUM;i++) // calcul de la somme de chacune des colonnes de l'image
	{
	    temp_sum[0]=(float)rd[16*i+0]+temp_sum[0]; //16*i car on somme les valeurs des colonnes. Or, pour passer d'une ligne à une autre, il y a 16 (8x2) valeurs à dépasser car matrice 16*8 et ici rd est un tableau 1 dimension
	    temp_sum[1]=(float)rd[16*i+2]+temp_sum[1]; //+2 car on prend la colonne n°2 (on saute une colonne sur 2)
	    temp_sum[2]=(float)rd[16*i+4]+temp_sum[2]; //+4 car on prend la colonne n°4
	    temp_sum[3]=(float)rd[16*i+6]+temp_sum[3];
	    temp_sum[4]=(float)rd[16*i+8]+temp_sum[4];
	    temp_sum[5]=(float)rd[16*i+10]+temp_sum[5];
	    temp_sum[6]=(float)rd[16*i+12]+temp_sum[6];
	    temp_sum[7]=(float)rd[16*i+14]+temp_sum[7];
		//On prend une colonne sur 2 car la matrice est de 16*8, et nous voulons une matrice 8*8.
	}

	int max_temp = 0; // int qui retourne l'indice de la colonne la plus chaude !
	float val_max_temp=0.0; //float qui stocke la somme de la colonne la plus chaude.
	
	for (i = 0;i<TAILLE_TEMP_SUM;i++)
	{
	    if (temp_sum[i]>val_max_temp) //On cherche la colonne la plus chaude
	    {
		max_temp = i; //Si on en trouve une plus chaude qu'une autre, on met à jour l'indice
		val_max_temp = temp_sum[i]; //et le float qui stocke la somme
	    }
	}
	

	//Calcul permet d'obtenir la moyenne de la colonne, afin de la comparer avec la température ambiante et de déterminer si il y a un objet chaud devant le robot :
	val_max_temp = val_max_temp / (4*8); // 0.25=1/4 de résolution et 8 car huit valeurs dans une colonne
	#ifdef DEBUG
		printf("Valeur moyenne de la colonne la plus chaude : %f \n", val_max_temp);
	#endif
	
	if (val_max_temp < COEFF_TEMP*temp_amb) //COEFF_temp est une constante qui permet de déterminer la marge qu'on se donne par rapport à la température ambiante. (définie dans le .h)
	{
		//Si on détermine que la valeur est moins chaude que la température ambiante * le coeff, alors on indique par la valeur -1 qu'aucun objet chaud est détecté.
		max_temp = -1;
	}
	return max_temp;
}

float get_temp_amb(int i2cFile, char* rd, char* wr) //retourne la température ambiante
{	
	float temp;
	int flag;
	wr[0] = 0x0E;
	flag = write(i2cFile, wr, 1);
	if (flag != 1) { perror("Error in Second Send_I2C"); exit(1); }
	flag = read(i2cFile, rd, 2);
	if (flag != 2) { perror("Read Error I2C_Byte"); }
	temp = ((float)rd[1]*256+(float)rd[0])*0.0625; //0.0625°C = résolution du capteur de température ambiente (=thermistor)
	return temp;
}





//
//	Fonctions utilisées pour l'interface web 
//


void gen_image(char* I2C_RD_Buf,int max, int min) // Affiche une image au format SVG
{	
	float p1,p2,p3;
	int bleu, vert, rouge,x, y, i;
	p1 = min + (max -min)/4;
	p2 = min + (max -min)/2;
	p3 = min + 3*(max -min)/4;
	printf("<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 8 8\">\n");
	for (i=0;i<128;i+=2){
	// Choix couleur
	if ((float)I2C_RD_Buf[i] >= min && (float)I2C_RD_Buf[i] < p1){rouge=0; vert=(int)(((float)I2C_RD_Buf[i]-min)*4/((max-min)*255)); bleu=255;}
	if ((float)I2C_RD_Buf[i] >= p1 && (float)I2C_RD_Buf[i] < p2){rouge=0; vert=255; bleu=255 - (int)(((float)I2C_RD_Buf[i]-p1)*4/((max-min))*255);}
	if ((float)I2C_RD_Buf[i] >= p2 && (float)I2C_RD_Buf[i] < p3){rouge= (int)(((float)I2C_RD_Buf[i]-p2)*4/((max-min))*255); vert=255; bleu=0;}
	if ((float)I2C_RD_Buf[i] >= p3){rouge=255 ; vert=255-(int)(((float)I2C_RD_Buf[i]-p3)*4/(max-min)*255); bleu=0;}

	x =(int)(i%16/2);
	y= (int)(i/16);
	printf("<rect fill=\"rgb(%i,%i,%i)\" x=\"%i\" y=\"%i\" width=\"1\" height=\"1\" />\n",rouge,vert,bleu,x,y);
	}
	printf("</svg>");
}

int max_image(char* I2C_RD_Buf)
{	
	int i;
	float max;
	max = (float)I2C_RD_Buf[0];
	for (i=0;i<128;i+=2){
		if ((float)I2C_RD_Buf[i] > max){
		max = (float)I2C_RD_Buf[i];
		}
	}
}

int min_image(char* I2C_RD_Buf)
{
	int i;
	float min;
	min = (float)I2C_RD_Buf[0];
	for (i=0;i<128;i+=2){
		if ((float)I2C_RD_Buf[i] < min){
		min = (float)I2C_RD_Buf[i];
		}
	}
}




