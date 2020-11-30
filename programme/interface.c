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
#define I2C_PERIPH2 "/dev/i2c-2" //Peripheric name in /dev
#define ADDR_SRF02 0x70 //Peripheric address


main() {
	int i;
	int i2cFile;
	int flag, value;
	unsigned char I2C_WR_Buf[MAX_BUFF_SIZE]; // Contains sent data
	unsigned char I2C_RD_Buf[MAX_BUFF_SIZE]; // Contains read data
	i2cFile = open(I2C_PERIPH, O_RDWR);
	if (i2cFile < 0) { perror("i2cOpen error"); exit(1); }
	flag = ioctl(i2cFile, I2C_SLAVE_FORCE, ADDR);
	if (flag < 0) { perror("i2cSetAddress error:"); exit(1); }

	I2C_WR_Buf[0] = 0x0E;
	flag = write(i2cFile, I2C_WR_Buf, 1);
	if (flag != 1) { perror("Error in First Send_I2C"); exit(1); }
	flag = read(i2cFile, I2C_RD_Buf, 2);
	if (flag != 2) { perror("Read Error I2C_Byte"); }
	printf("<p>Température ambiante : %0.2f</p>", ((float)I2C_RD_Buf[1]*256+(float)I2C_RD_Buf[0])*0.0625);

	I2C_WR_Buf[0] = 0x80;
	flag = write(i2cFile, I2C_WR_Buf, 1);
	if (flag != 1) { perror("Error in Second Send_I2C"); exit(1); }
	flag = read(i2cFile, I2C_RD_Buf, 128);
	if (flag != 128) { perror("Read Error I2C_Byte"); }
	close(i2cFile);

	int distance;
	i2cFile = open(I2C_PERIPH2, O_RDWR);
	if (i2cFile < 0) { perror("i2cOpen error"); exit(1); }
	flag = ioctl(i2cFile, I2C_SLAVE_FORCE, ADDR_SRF02);
	if (flag < 0) { perror("i2cSetAddress error:"); exit(1); }
	I2C_WR_Buf[0] = 0x00; I2C_WR_Buf[1] = 0x51;
	flag = write(i2cFile, I2C_WR_Buf, 2);
	if (flag != 2) { perror("Error in First Send_I2C"); exit(1); }
	usleep(100000);
	I2C_WR_Buf[0] = 0x02;
	flag = write(i2cFile, I2C_WR_Buf, 1);
	if (flag != 1) { perror("Error in Second Send_I2C"); exit(1); }
	flag = read(i2cFile, I2C_RD_Buf, 2);
	if (flag != 2) { perror("Read Error I2C_Byte"); }
	distance = 256*(int)I2C_RD_Buf[0] +  (int)I2C_RD_Buf[1];
	printf("<p>Distance = %i cm</p>", distance);
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
    printf("<p>Objet chaud detecté en position %i.</p>",max_temp);
}
else{
printf("<p>Aucun objet chaud detecté.</p>");

}


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
min=20;
max=35;
p1=23*4;
p2=27*4;
p3=31*4;
// p1 = min + (max -min)/4;
// p2 = min + (max -min)/2;
// p3 = min + 3*(max -min)/4;
// Generer une image
FILE* fp;
int bleu, vert, rouge;
int x,y;
x=0;
y=0;

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
