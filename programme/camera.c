#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "camera.h"
#include <time.h>
#define DEBUG 1 

void camera_init(Camera* camera,int moyenne)
{
	int flag;
	camera->i2cFile = open(I2C_PERIPH, O_RDWR); //ouverture i2c
	if (camera->i2cFile < 0) { perror("Erreur ouverture du bus I2C"); exit(1); }
	
	flag = ioctl(camera->i2cFile, I2C_SLAVE_FORCE, ADDR);
	if (flag < 0) { perror("Erreur adresse camera I2C"); exit(1); }
	
	camera->debut_tab=0; // On initialise l'indice du tableau tournant à 0
	camera->moyenne=moyenne; //Nombre d'itérations pour le moyennage
	camera->cibles=(int*) malloc(camera->moyenne*sizeof(int));
	int i;
	for (i=0;i<camera->moyenne;i++){camera->cibles[i]=-1;} // On initialise le tableau tournant avec des -1 (pas de mouvement)
	camera->timer=0.1; // Timer à 100 ms soit 10 HZ
	
	camera_get_image(camera); //MAJ tableau qui contient la matrice
	camera_get_temp_amb(camera); // MAJ température ambiente
	camera_min_image(camera); // Min tableau
	camera_max_image(camera); // Max tableau
	usleep(1);
	camera_indice_chaleur(camera); 
	clock_gettime(CLOCK_MONOTONIC,&(camera->t1)); // On initialise t1
}

void camera_get_image(Camera* camera)
{	
	unsigned char img_hex[2* NB_PIXELS]; //La matrice renvoie deux fois plus de valeur que celle qu'on veut (8*8)
	unsigned char wr[1];
	int flag;
	wr[0] = 0x80; //adresse du registre du premier pixel
	flag = write(camera->i2cFile, wr, 1);
	if (flag != 1) { perror("Erreur requete I2C camera"); exit(1); }
	flag = read(camera->i2cFile, img_hex, 128);
	if (flag != 128) { perror("Erreur lecture I2C camera"); }
	
	camera->sum_pix = 0; //Somme tableau = 0
	int i;
	for (i = 0; i < NB_PIXELS; i++) 
	{
		if (img_hex[2 * i + 1] < 0x08) //Si la valeur est inférieure à 8
		{
			camera->img[i] = (256*(float)img_hex[2*i+1]+(float)img_hex[2*i])*0.25;
		}
		else  //Si la valeur est supérieure à 8
		{
			camera->img[i] = (256*(float)img_hex[2*i+1] + (float)img_hex[2 * i] -4096)*0.25;
		}
		camera->sum_pix += camera->img[i];
	}
}

void camera_afficher_image(Camera* camera) 
{
	/* Affiche tableau en deux dimensions : 8*8. Il represente les températures que le capteur reçoit et correspnd à la vision du robot */
	int i,j;
	for (i=0;i<TAILLE_MATRICE;i++)
	{
		for (j=0;j<TAILLE_MATRICE;j++)
		{
			printf("%.1f " ,camera->img[TAILLE_MATRICE*i+j]);
		}
		printf("%s","\n");
	}
}

void camera_indice_chaleur(Camera* camera) //retourne l'indice du tableau de la zone la plus chaude entre 0 et 7
{

	float temp_sum[TAILLE_MATRICE];
	int i,j;
	for (i = 0;i<TAILLE_MATRICE;i++) //boucle initialisation tableau à sa 1ère valeur
	{
	    temp_sum[i] = 1.0/(TAILLE_MATRICE-1)*camera->img[i];
	}
	
	
	for (i = 1; i<TAILLE_MATRICE-1;i++) // calcul de la somme de chacune des colonnes de l'image
	{	
		for (j = 0; j < TAILLE_MATRICE; j++) {
			temp_sum[j] = 1.0 / (TAILLE_MATRICE -1)* camera->img[TAILLE_MATRICE * i + j] + temp_sum[j];
		}
	}
	
	int max_temp = 0; //indice de la colonne la plus chaude
	float val_max_temp = temp_sum[0];
	for (i = 0;i<TAILLE_MATRICE;i++)
	{
	    if (temp_sum[i]>val_max_temp)
	    {
		max_temp = i;
		val_max_temp = temp_sum[i]; //valeur qui définit s'il y a une source chaude
	    }
	}

	if (val_max_temp < COEFF_TEMP*(camera->temp_amb) || val_max_temp > 40)
	{
		max_temp = -1;
	}
	camera->cibles[camera->debut_tab]=max_temp;
	camera->debut_tab=(camera->debut_tab+1)%(camera->moyenne);
	camera->indice=max_temp;
	#if DEBUG
		printf("Valeur moyenne colonne : %f \n", val_max_temp);
		printf("indice : %i \n", max_temp);
	#endif
}

void camera_get_temp_amb(Camera* camera)
{	
	int flag;
	unsigned char rd[2];
	unsigned char wr[1];
	wr[0] = 0x0E; // adresse du registre de temperature ambiante
	flag = write(camera->i2cFile, wr, 1);
	if (flag != 1) { perror("Erreur requete I2C"); exit(1); }
	flag = read(camera->i2cFile, rd, 2);
	if (flag != 2) { perror("Erreur lecture I2C"); }
	/*  */
	if (rd[1] < 0x08) 
	{
		camera->temp_amb = (256 * (float)rd[1] + (float)rd[0]) * 0.0625;
	}
	else {
		camera->temp_amb = (256 * (float)rd[1] + (float)rd[0] - 4096) * 0.0625;
	}
}


void camera_moyenne(Camera* camera)
{
	int temp=0;
	int counter=0;
	int i,j;
	
	for (i=0;i<camera->moyenne;i++)
	{
		for (j=0;j<camera->moyenne;j++)
		{
			if (camera->cibles[j]==camera->cibles[i])
			{
				counter++;
			}
		}
		if (counter>temp)
		{
			temp=counter;
			camera->cible=camera->cibles[i];
		}
		counter=0;
	}
	
	#if DEBUG
	printf("Tab cibles : ");
	for (i=0;i<camera->moyenne;i++){printf("%i ",camera->cibles[i]);}
	printf("\n");
	#endif
}

void camera_update(Camera* camera)
{
	clock_gettime(CLOCK_MONOTONIC,&(camera->t2));
	if ((((double)((camera->t2).tv_sec)+(double)((camera->t2).tv_nsec)*1.0e-9) - ((double)((camera->t1).tv_sec)+(double)((camera->t1).tv_nsec)*1.0e-9))>camera->timer)
	{	
		camera_get_image(camera);
		camera_get_temp_amb(camera);
		camera_min_image(camera);
		camera_max_image(camera);
		usleep(1);
		camera_indice_chaleur(camera);
		camera_moyenne(camera);
		#if DEBUG
			printf("Cible après moyennage : %i \n", camera->cible);
		#endif
		clock_gettime(CLOCK_MONOTONIC,&(camera->t1)); // On update t1	
	}
}


void camera_settimer(Camera* camera,double timer){camera->timer = timer;}


//	Fonctions utilisées pour l'interface web 

void camera_gen_image(Camera* camera) // Affiche une image au format SVG
{	
	float p1,p2,p3;
	int bleu, vert, rouge,x, y, i;
	p1 = camera->min + (camera->max - camera->min)/4;
	p2 = camera->min + (camera->max - camera->min)/2;
	p3 = camera->min + 3*(camera->max - camera->min)/4;
	printf("<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 80 80\">\n");
	for (i=0;i<NB_PIXELS;i++){
	// Choix couleur
	if (camera->img[i] < camera->min){rouge=0; vert=0; bleu=255;}
	if (camera->img[i] >= camera->min && camera->img[i] < p1){rouge=0; vert=(int)((camera->img[i]-camera->min)*4/((camera->max-camera->min)*255)); bleu=255;}
	if (camera->img[i] >= p1 && camera->img[i] < p2){rouge=0; vert=255; bleu=255 - (int)((camera->img[i]-p1)*4/((camera->max-camera->min))*255);}
	if (camera->img[i] >= p2 && camera->img[i] < p3){rouge= (int)((camera->img[i]-p2)*4/((camera->max-camera->min))*255); vert=255; bleu=0;}
	if (camera->img[i] >= p3){rouge=255 ; vert=255-(int)((camera->img[i]-p3)*4/(camera->max-camera->min)*255); bleu=0;}

	x =(int)(i%8*10);
	y= (int)(i/8*10);
	printf("<rect fill=\"rgb(%i,%i,%i)\" x=\"%i\" y=\"%i\" width=\"10\" height=\"10\" />\n",rouge,vert,bleu,x,y);
	printf("<text class=\"temp\" font-size=\"3\" class=\"test\" x=\"%i\" y=\"%i\">%0.2f</text>\n",x+1,y+5,camera->img[i]);
	}
	printf("</svg>");
}

void camera_max_image(Camera* camera)
{	
	int i;
	camera->max = camera->img[0];
	for (i=0;i<NB_PIXELS;i++){
		if (camera->img[i] > camera->max){
		camera->max = camera->img[i];
		}
	}
}

void camera_min_image(Camera* camera)
{
	int i;
	camera->min = camera->img[0];
	for (i=0;i<NB_PIXELS;i++){
		if (camera->img[i] < camera->min){
		camera->min = camera->img[i];
		}
	}
}
