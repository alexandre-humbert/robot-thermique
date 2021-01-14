#include "camera.h"
#include <stdio.h>

void main(int argc, const char* argv[])
{
	Camera camera;
	camera_init(&camera,5);
	printf("Température ambiante : %0.2f \n", camera.temp_amb);
	printf("Valeur min : %0.2f \n", camera.min);
	printf("Valeur max : %0.2f \n", camera.max);
	printf("Position  : %i \n", camera.indice);
	camera_afficher_image(&camera);
}