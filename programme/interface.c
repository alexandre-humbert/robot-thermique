#include "moteur.h"
#include "ultrason.h"
#include "camera.h"

main(int argc, const char* argv[])
{

	Camera camera;
	Ultrason ultrason;
	Moteur moteur;
	
	/*INITIALISATION CAMERA THERMIQUE*/	
	camera_init(&camera,5);
	
	/* initialisation ultrason */
	ultrason_init(&ultrason,0x07,0x15);

	/* INITIALISTION MOTEUR */
	moteur_init(&moteur);
	

	printf("<p>Distance : %i cm</p>",ultrason.distance);
	printf("<p>Luminosité : %i</p>",ultrason.luminosite);
	printf("<p>Source chaude en position : %i</p>",camera.indice);
	camera_gen_image(&camera);


}
