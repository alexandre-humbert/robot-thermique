#include "moteur.h"
#include "ultrason.h"
#include "camera.h"

main(int argc, const char* argv[])
{

	Camera camera;
	Ultrason ultrason;
	Moteur moteur;
	
	/*INITIALISATION CAMERA THERMIQUE*/	
	camera_init(&camera, 3); //On moyenne sur 3 valeurs 
	
	/* initialisation ultrason */
	ultrason_init(&ultrason,0x07,0x15);

	/* INITIALISTION MOTEUR */
	moteur_init(&moteur);
	
	/* Affichage distance, luminosité de la pièce, et indice de la source chaude (-1 si y'en a pas)) */
	printf("<p>Distance : %i cm</p>",ultrason.distance);
	printf("<p>Luminosité : %i</p>",ultrason.luminosite);
	printf("<p>Source chaude en position : %i</p>",camera.indice);
	
	/* Affichage graphique de la matrice renvoyée par le capteur thermique */
	camera_gen_image(&camera);


}
