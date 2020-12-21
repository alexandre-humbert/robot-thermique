#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "camera.h"
#define DEBUG 0 //commenter pour enlever tous les printf.
void i2c1_init(int* i2cFile)
{
	int flag;
	*i2cFile = open(I2C_PERIPH, O_RDWR);
	if (*i2cFile < 0) { perror("i2cOpen error"); exit(1); }
	flag = ioctl(*i2cFile, I2C_SLAVE_FORCE, ADDR);
	if (flag < 0) { perror("i2cSetAddress error:"); exit(1); }
}

void get_image(int i2cFile, char* rd, char* wr )
{
	int flag;
	wr[0] = 0x80;
	flag = write(i2cFile, wr, 1);
	if (flag != 1) { perror("Error in Second Send_I2C"); exit(1); }
	flag = read(i2cFile, rd, 128);
	if (flag != 128) { perror("Read Error I2C_Byte"); }
}

void affiche_image(char* rd) //im correspond à I2C_RD_Buf
{

	int i,j;
	for (i=0;i<8;i++){
		for (j=0;j<16;j+=2){
			printf("%.1f " ,(float)rd[16*i+j]*0.25);
		}
		printf("%s","\n");
	}
}

int indice_chaleur(char* rd,float temp_amb, float* sum_tab) //retourne l'indice du tableau de la zone la plus chaude entre 0 et 7
{			    //im correspond à I2C_RD_Buf

	float temp_sum[TAILLE_TEMP_SUM];
	int i;
	for (i = 0;i<TAILLE_TEMP_SUM;i++) //boucle initialisation tableau à sa 1ère valeur
	{
	    temp_sum[i] = (float)rd[2*i];
	}


	for (i = 1; i<TAILLE_TEMP_SUM;i++) // calcul de la somme de chacune des colonnes de l'image
	{
	    temp_sum[0]=(float)rd[16*i+0]+temp_sum[0];
	    temp_sum[1]=(float)rd[16*i+2]+temp_sum[1];
	    temp_sum[2]=(float)rd[16*i+4]+temp_sum[2];
	    temp_sum[3]=(float)rd[16*i+6]+temp_sum[3];
	    temp_sum[4]=(float)rd[16*i+8]+temp_sum[4];
	    temp_sum[5]=(float)rd[16*i+10]+temp_sum[5];
	    temp_sum[6]=(float)rd[16*i+12]+temp_sum[6];
	    temp_sum[7]=(float)rd[16*i+14]+temp_sum[7];
	}

	int max_temp = 0; // int qui retourne l'indice de la colonne la plus chaude !
	float val_max_temp=0.0;
	*sum_tab = 0; //Valeur qui nous sert à stocker la totalité des valeurs du tableau.
//On en a besoin pour savoir si on est assez proche du monsieur pour considérer que c'est lui l'obstacle ou alors si c'est vraiment un obstacle.
	for (i = 0;i<TAILLE_TEMP_SUM;i++)
	{
	    *sum_tab = temp_sum[i] + *sum_tab;
	    if (temp_sum[i]>val_max_temp)
	    {
		max_temp = i;
		val_max_temp = temp_sum[i]; //valeur qui définit s'il y a une source chaude
	    }
	}

	/*#ifdef DEBUG
		printf("\n SOMME TABLEAU : %f \n", *sum_tab);
	#endif*/
	//val_max_temp = val_max_temp*0.25/TAILLE_TEMP_SUM;

	/*if(val_max_temp>26)
	{
	    printf("Il y a un objet chaud devant moi.");
	}

	printf("L'indice de la colonne la plus chaude est %i, valeur = %f \n", max_temp, val_max_temp);*/
	// Trouver la temperature minimale et la temperature maximale.
	val_max_temp = val_max_temp / (4*8); // 4 car 0.25 précision et 8 car huit valeurs dans une colonne
	

	if (val_max_temp < 23.2)
	{
	max_temp = -1;
	}
	else
	{
	#ifdef DEBUG
		printf("Valeur moyenne colonne : %f \n", val_max_temp);
	#endif
	#ifdef DEBUG
		printf("indice : %i \n", max_temp);
	#endif
	}
	return max_temp;
}

float get_temp_amb(int i2cFile, char* rd, char* wr)
{	
	float temp;
	int flag;
	wr[0] = 0x0E;
	flag = write(i2cFile, wr, 1);
	if (flag != 1) { perror("Error in Second Send_I2C"); exit(1); }
	flag = read(i2cFile, rd, 2);
	if (flag != 2) { perror("Read Error I2C_Byte"); }
	temp = ((float)rd[1]*256+(float)rd[0])*0.0625;
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
