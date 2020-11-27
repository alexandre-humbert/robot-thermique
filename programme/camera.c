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
{sleep(3);
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
printf("L'indice de la colonne la plus chaude est %d, valeur = %f \n", max_temp, val_max_temp);

}