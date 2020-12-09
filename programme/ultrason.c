#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "ultrason.h"

void i2c2_init(int* i2cFile)
{
	int flag;
	*i2cFile = open(I2C_PERIPH_2, O_RDWR);
	if (*i2cFile < 0) { perror("I2C - Erreur ouverture"); exit(1); }
	flag = ioctl(*i2cFile, I2C_SLAVE_FORCE, ADDR_SRF02);

	if (flag < 0) { perror("I2C - Erreur adresse"); exit(1); }
}

int mesure_distance(int i2cFile, char* I2C_WR_Buf, char* I2C_RD_Buf){
	int flag,distance;

	I2C_WR_Buf[0] = 0x00; 
	I2C_WR_Buf[1] = 0x51;

	flag = write(i2cFile, I2C_WR_Buf, 2);
	if (flag != 2) { perror("Error in First Send_I2C"); exit(1); }
	usleep(100000);

	I2C_WR_Buf[0] = 0x02;
	flag = write(i2cFile, I2C_WR_Buf, 1);

	if (flag != 1) { perror("Error in Second Send_I2C"); exit(1); }
	flag = read(i2cFile, I2C_RD_Buf, 2);

	if (flag != 2) { perror("Read Error I2C_Byte"); }
	distance = 256*(int)I2C_RD_Buf[0] +  (int)I2C_RD_Buf[1];

	printf(" Distance = %i cm\n", distance);

	return distance;
}