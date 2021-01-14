#include "moteur.h"
#include <stdio.h>
#include <string.h>

void main(int argc, const char* argv[])
{
	Moteur moteur;
	moteur_init(&moteur);
	if (argc>0){
		moteur_commande(&moteur,argv[1]);
	}
}