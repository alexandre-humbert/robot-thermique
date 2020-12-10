#ifndef H_CAMERA
#define H_CAMERA

#define I2C_PERIPH "/dev/i2c-1" //Bus de la camera
#define ADDR 0x68 //Adresse de la camera
#define MAX_BUFF_SIZE_RD 128
#define MAX_BUFF_SIZE_WR 8
#define NB_PIXELS 64
#define TAILLE_MATRICE 8
#define TAILLE_TEMP_SUM 8
#define COEFF_TEMP 1.04
void i2c1_init(int *i2cFile);
void get_image(int i2cfile, char* rd, char* wr );
void affiche_image(char* rd);
int indice_chaleur(char* rd,float temp_amb);
float get_temp_amb(int i2cFile, char* rd, char* wr);
void gen_image(char* I2C_RD_Buf,int max, int min);
int max_image(char* I2C_RD_Buf);
int min_image(char* I2C_RD_Buf);
#endif