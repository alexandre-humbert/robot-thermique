#ifndef H_CAMERA
#define H_CAMERA
#define I2C_PERIPH "/dev/i2c-1" //Bus de la camera
#define ADDR 0x68 //Adresse de la camera
#define MAX_BUFF_SIZE_RD 128 
#define MAX_BUFF_SIZE_WR 8
#define NB_PIXELS 64 //Matrice de 8*8 valeurs
#define TAILLE_MATRICE 8 //nombre de colonnes = 8
#define TAILLE_TEMP_SUM 8
#define COEFF_TEMP 1.04
#include <time.h>

typedef struct Camera Camera; //Raccourci : au lieu de devoir écrire "struct Camera" on peut écrire "Camera" directement.
struct Camera
        {
		int i2cFile; // Int pour I2C
		float img[NB_PIXELS]; // Tableau qui stocke ce que la matrice renvoie sous la forme de float
		float temp_amb; // Stocke la temperature ambiente
		float sum_pix; // Stocke la somme de tout le tableau
		float max; // max du tableau
		float min; // min du tableau
		int cible; // Stock l'indice de la colonne du tableau la plus chaude après moyennage
		int indice; // Stock l'indice de la colonne du tableau la plus chaude avant moyennage
		int* cibles; // Tableau pour le moyennage : exemple : on trouve une source chaude en indice a donc on incrémente cible[a]. On repète l'opération 3 fois (moyennage 3). Puis on cherche la valeur la plus élevée de ce tableau qu'on va stocker dans cible. 
		int moyenne; // Stocke le nombre de moyennage que l'on veut faire. 
		int debut_tab; //Indice du tableau tournant
		double timer;
		struct timespec t1; // Stock un temps
		struct timespec t2; // Stock un deuxième temps
		// Avec ces deux temps, on va pouvoir créer un chronomètre pour éviter d'utiliser la fonction Sleep() pour contourner l'obstacle en tout sécurité.
		// Avec les sleep() on ne vérifiait pas que le robot ne rentre pas dans un autre obstacle pendant la séquence. D'ou l'utilisation de ce chronomètre.
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
