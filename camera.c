#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#define I2C_PERIPH "/dev/i2c-1" //Peripheric name in /dev
#define ADDR 0x68 //Peripheric address
#define MAX_BUFF_SIZE 128
#define NB_PIXELS 64
#define TAILLE_MATRICE 8
#define TAILLE_TEMP_SUM 8

main()
{sleep(0);
	int i2cFile;
	int flag, value;
	unsigned char I2C_WR_Buf[MAX_BUFF_SIZE]; // Contains sent data
	unsigned char I2C_RD_Buf[MAX_BUFF_SIZE]; // Contains read data
	i2cFile = open(I2C_PERIPH, O_RDWR);
	if (i2cFile < 0) { perror("i2cOpen error"); exit(1); }
	flag = ioctl(i2cFile, I2C_SLAVE_FORCE, ADDR);
	if (flag < 0) { perror("i2cSetAddress error:"); exit(1); }
/*
	I2C_WR_Buf[0] = 0x00; I2C_WR_Buf[1] = 0x00;
	flag = write(i2cFile, I2C_WR_Buf, 2);
	if (flag != 2) { perror("Error in First Send_I2C"); exit(1); }
*/

	I2C_WR_Buf[0] = 0x80;
	flag = write(i2cFile, I2C_WR_Buf, 1);
	if (flag != 1) { perror("Error in Second Send_I2C"); exit(1); }
	flag = read(i2cFile, I2C_RD_Buf, 128);
	if (flag != 128) { perror("Read Error I2C_Byte"); }

	int i,j;
	for (i=0;i<8;i++){
		for (j=0;j<16;j+=2){
			printf("%.1f " ,(float)I2C_RD_Buf[16*i+j]*0.25);
		}
		printf("%s","\n");
	}

	close(i2cFile);

float temp_sum[TAILLE_TEMP_SUM];

for (i = 0;i<TAILLE_TEMP_SUM;i++)
{
    temp_sum[i] = 0;
}


for (i = 0; i<TAILLE_TEMP_SUM;i++)
{
    temp_sum[0]=(float)I2C_RD_Buf[16*i+0]+temp_sum[0];
    temp_sum[1]=(float)I2C_RD_Buf[16*i+2]+temp_sum[1];
    temp_sum[2]=(float)I2C_RD_Buf[16*i+4]+temp_sum[2];
    temp_sum[3]=(float)I2C_RD_Buf[16*i+6]+temp_sum[3];
    temp_sum[4]=(float)I2C_RD_Buf[16*i+8]+temp_sum[4];
    temp_sum[5]=(float)I2C_RD_Buf[16*i+10]+temp_sum[5];
    temp_sum[6]=(float)I2C_RD_Buf[16*i+12]+temp_sum[6];
    temp_sum[7]=(float)I2C_RD_Buf[16*i+14]+temp_sum[7];
}

int max_temp = 0; // int qui retourne l'indice de la colonne la plus chaude !
float val_max_temp=0;

for (i = 0;i<TAILLE_TEMP_SUM;i++)
{
    if (temp_sum[i]>val_max_temp)
    {
        max_temp = i;
        val_max_temp = temp_sum[i];
    }
}
val_max_temp = val_max_temp*0.25/TAILLE_TEMP_SUM;
if(val_max_temp>26)
{
    printf("Il y a un objet chaud devant moi.");
}

printf("L'indice de la colonne la plus chaude est %i, valeur = %f \n", max_temp, val_max_temp);
// Trouver la temperature minimale et la temperature maximale.
float max, min,p1,p2,p3;
min = (float)I2C_RD_Buf[0];
max = (float)I2C_RD_Buf[0];

for (i=0;i<128;i+=2){
  if ((float)I2C_RD_Buf[i] > max){
  max = (float)I2C_RD_Buf[i];
  }
  if ((float)I2C_RD_Buf[i] < min){
  min = (float)I2C_RD_Buf[i];
  }
}
p1 = min + (max -min)/4;
p2 = min + (max -min)/2;
p3 = min + 3*(max -min)/4;
// Generer une image
FILE* fp;
int bleu, vert, rouge;
int x,y;
x=0;
y=0;
fp = fopen("/var/www/img.svg", "w");
fprintf(fp, "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 8 8\">\n");
for (i=0;i<128;i+=2){
// Choix couleur
if ((float)I2C_RD_Buf[i] >= min && (float)I2C_RD_Buf[i] < p1){rouge=0; vert=(int)(((float)I2C_RD_Buf[i]-min)*4/((max-min)*255)); bleu=255;}
if ((float)I2C_RD_Buf[i] >= p1 && (float)I2C_RD_Buf[i] < p2){rouge=0; vert=255; bleu=255 - (int)(((float)I2C_RD_Buf[i]-p1)*4/((max-min))*255);}
if ((float)I2C_RD_Buf[i] >= p2 && (float)I2C_RD_Buf[i] < p3){rouge= (int)(((float)I2C_RD_Buf[i]-p2)*4/((max-min))*255); vert=255; bleu=0;}
if ((float)I2C_RD_Buf[i] >= p3){rouge=255 ; vert=255-(int)(((float)I2C_RD_Buf[i]-p3)*4/(max-min)*255); bleu=0;}

x =(int)(i%16/2);
y= (int)(i/16);
fprintf(fp, "<rect fill=\"rgb(%i,%i,%i)\" x=\"%i\" y=\"%i\" width=\"1\" height=\"1\" />\n",rouge,vert,bleu,x,y);
}
fprintf(fp, "</svg>");
fclose(fp);
}