#ifndef H_ULTRASON
#define H_ULTRASON

#define I2C_PERIPH_2 "/dev/i2c-2" // Bus du caputeur ultrason
#define ADDR_SRF02 0x70 //Adress du capteur ultrason
#define MAX_BUFF_SIZE 4 
void i2c2_init(int* i2cFile);

int mesure_distance(int i2cFile, char* I2C_WR_Buf, char* I2C_RD_Buf);

#endif