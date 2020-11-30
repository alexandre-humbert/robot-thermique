#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#define I2C_PERIPH "/dev/i2c-2" //Peripheric name in /dev
#define ADDR_SRF02 0x70 //Peripheric address
#define MAX_BUFF_SIZE 64
main()
{
	int i2cFile;
	int flag, value, distance;
	unsigned char I2C_WR_Buf[MAX_BUFF_SIZE]; // Contains sent data
	unsigned char I2C_RD_Buf[MAX_BUFF_SIZE]; // Contains read data
	i2cFile = open(I2C_PERIPH, O_RDWR);
	if (i2cFile < 0) { perror("i2cOpen error"); exit(1); }
	flag = ioctl(i2cFile, I2C_SLAVE_FORCE, ADDR_SRF02);
	if (flag < 0) { perror("i2cSetAddress error:"); exit(1); }
	I2C_WR_Buf[0] = 0x00; I2C_WR_Buf[1] = 0x51;
	flag = write(i2cFile, I2C_WR_Buf, 2);
	if (flag != 2) { perror("Error in First Send_I2C"); exit(1); }
	sleep(1);
	I2C_WR_Buf[0] = 0x02;
	flag = write(i2cFile, I2C_WR_Buf, 1);
	if (flag != 1) { perror("Error in Second Send_I2C"); exit(1); }
	flag = read(i2cFile, I2C_RD_Buf, 2);
	if (flag != 2) { perror("Read Error I2C_Byte"); }
	distance = 256*(int)I2C_RD_Buf[0] +  (int)I2C_RD_Buf[1];
	printf(" Distance = %i cm\n", distance);
	close(i2cFile);
}