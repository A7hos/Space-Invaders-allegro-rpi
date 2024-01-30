#include "vidas.h"

/**************************************************************************
 * FUNCION: se encarga de las vidas del jugador
 * RECIBE: que se debe realizar con las vidas
 * DEVUELVE: cantidad de vidas restantes
***************************************************************************/
int vidas(int situacion)
{
	static int vida = VIDAS;

	if(situacion == ADD_VIDA)						//incrementamos la cantidad de vidas
	{
		vida++;
	}
	else if(situacion == SUBS_VIDA)					//reducimos la cantidad de vidas
	{
		vida--;
	}
	else if(situacion == RESET_VIDA)				//volvemos la cantidad de vidas a la default
	{
		vida = RESET_VIDA;
	}
	return vida;									//devolvemos la cantidad de vidas
}
