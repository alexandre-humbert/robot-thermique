#include "ultrason.h"
#include <stdio.h>

void main(int argc, const char* argv[])
{
	Ultrason ultrason;
	ultrason_init(&ultrason);
	printf("Distance : %i cm \n", ultrason.distance);
	printf("Luminosté : %i \n", ultrason.luminosite);
}