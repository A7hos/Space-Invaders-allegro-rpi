#include "libsconst.h"
#include "misil.h"
#include "impact.h"

/**************************************************************************
 * FUNCION: se encarga del disparo del jugador, si debe moverse, impacto, etc
 * RECIBE: columna donde debe moverse el disparo y la matriz del mundo o tablero de juego
 * DEVUELVE: si esta listo o no para disparar de nuevo
***************************************************************************/
int misil(int i, int mundo[16][16])
{
	//con cada llamado a la funcion se analiza y mueve una casilla / celda
	int flag = 0;											//flag usado para saber si la bala existe o no, y permitir que se vuelva a disparar
	static int l = 14;										//usamos como indice para desplazar verticalmente el disparo

	if(mundo[l][i] == BALA)									//en caso de que el disparo exista
	{
		if(mundo_buffer[l-1][i] == ALI_BALA)				//verificamos si impacto con la bala de un alien
		{
			mundo_buffer[l-1][i] = 0;						//de ser asi matamos la dos balas
			mundo[l][i] = 0;
			flag = 1;										//activamos el flag para indicar que se puede volver a disparar
		}
		else if(mundo[l-1][i] != 0)							//si impacto con cualquier otra cosa
		{
			impact(mundo[l-1][i], l-1, i, mundo);			//llamamos a impact para ver con que impacto
			mundo[l][i] = 0;								//matamos la bala
			flag = 1;										//activamos el flag para indicar que se puede volver a disparar
		}
		else												//en caso de que no halla impactado con nada
		{
			mundo[l-1][i] = BALA;							//desplazamos la bala
			mundo[l][i] = 0;
			l--;											//actualizamos el indice
		}
	}
	else													//si el disparo no existe
	{
		flag = 1;											//ponemos el flag en 1 para que se pueda dispaar
	}
	
	if(l == 0 || flag == 1)									//si el disparo impacto, no existe o se choco con el borde superior
	{
		l = 14;												//reseteamos las variables e indicamos que esta listo para disparar
		mundo[0][i] = 0;
		flag = 1;
	}

	return flag;											//devolvemos si se puede disparar o no
}
