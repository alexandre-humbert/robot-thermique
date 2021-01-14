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
#include <time.h>

typedef struct Camera Camera;
struct Camera
        {
		int i2cFile;
		float img[NB_PIXELS];
		float temp_amb;
		float sum_pix;
		float max;
		float min;
		int cible;
		int indice;
		int* cibles;
		int moyenne;
		int debut_tab;
		double timer;
		struct timespec t1;
		struct timespec t2;
        };
        
void camera_init(Camera* camera,int moyenne);
void camera_get_image(Camera* camera);
void camera_afficher_image(Camera* camera);
void camera_indice_chaleur(Camera* camera);
void camera_get_temp_amb(Camera* camera);
void camera_gen_image(Camera* camera);
void camera_max_image(Camera* camera);
void camera_min_image(Camera* camera);
void camera_update(Camera* camera);
void camera_moyenne(Camera* camera);
void camera_settimer(Camera* camera,double timer);
#endif
