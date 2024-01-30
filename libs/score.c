#include <stdlib.h>
#include <time.h>

#include "libsconst.h"
#include "score.h"

/*******************************
 * VARIABLES GLOBALES PUBLICAS *
********************************/
int scor;										//variable global donde guardamos el score de la partida para que pueda ser vista y en caso de ser necesario modificada

/**************************************************************************
 * FUNCION: se encarga del puntaje de la partida
 * RECIBE: con que tipo de alien impacto / murio
 * DEVUELVE: -
***************************************************************************/
void score(int contacto)
{
	srand(time(NULL));							//semilla del random
	
	if (contacto == ALI_1)						//verifico si impacto alien de nivel 1
	{
		scor += ALI_1;
	}
	else if (contacto == ALI_2)					//verifico si impacto alien de nivel 2
	{
		scor += ALI_2;
	}
	else if (contacto == ALI_3)					//verifico si impacto alien de nivel 3
	{
		scor += ALI_3;
	}
	else if (contacto == ALI_N)					// verifico si impacto la nave nodriza 
	{
		scor += rand()%51+50;					//valor random entre 50-100
	}
}

