#include "libsconst.h"
#include "movimiento.h"
#include "score.h"

/*******************************
 * VARIABLES GLOBALES PUBLICAS *
********************************/
int flag_mov;								//flag usado para asegurar que el movimiento siempre comience en el mismo sentido

/**************************************************************************
 * FUNCION: se encarga del movimiento de los aliens
 * RECIBE: matriz del mundo o tablero de juego
 * DEVUELVE: -
***************************************************************************/
void move(int mundo[16][16])
{
	static int flag;						//flag el cual va determinando el sentido del movimiento a medida que corre el juego
	int i, j;

	if(flag_mov == 1)						//aseguramos el sentido del movimiento
	{
		flag = 0;
		flag_mov = 0;
	}

	if(flag == 0)							//se muevan los aliens a derecha
	{
		for(i = 15; i >= 0; i--)
		{
			for(j = 0; j < 16; j++)
			{
				if((mundo[j][i] == 30) || (mundo[j][i] == 20) || (mundo[j][i] == 10))
				{
					if(mundo[j][i+1] == BALA)			//AVISO: se debio agregar esta verificacion para solucionar un bug, en el cual si la bala impactaba a los aliens de costado no sucedia lo esperado
					{
						score(mundo[j][i]);				//sumamos el score de ser necesario
						mundo[j][i+1] = 0;				//matamos el alien y la bala
						mundo[j][i] = 0;
						cant_aliens--;					//descontamos el alien de ser necesario
					}
					else								//movemos los aliens
					{
						mundo[j][i+1] = mundo[j][i];
						mundo[j][i] = 0;
					}
				}
			} 
		}
		for(i = 15; i >= 0; i--)			//verifico no salirnos del arreglo y ver si deben descender y cambiar direccion
		{
			if((mundo[i][15] == 30) || (mundo[i][15] == 20) || (mundo[i][15] == 10))
			{
				flag = 1;
			}
		}
	}
	else if(flag == 2)						//se mueven los aliens a izquierda
	{
		for(i = 0; i < 16; i++)
		{
			for(j = 0; j < 16; j++)
			{
				if((mundo[j][i] == 30) || (mundo[j][i] == 20) || (mundo[j][i] == 10))
				{
					if(mundo[j][i-1] == BALA)			//AVISO: se debio agregar esta verificacion para solucionar un bug, en el cual si la bala impactaba a los aliens de costado no sucedia lo esperado
					{
						score(mundo[j][i]);				//sumamos el score de ser necesario
						mundo[j][i-1] = 0;				//matamos el alien y la bala
						mundo[j][i] = 0;
						cant_aliens--;					//descontamos el alien de ser necesario
					}
					else								//movemos los aliens
					{
						mundo[j][i-1] = mundo[j][i];
						mundo[j][i] = 0;
					}
				}
			} 
		}
		for(i = 15; i >= 0; i--)			//verifico no salirnos del arreglo y ver si deben descender y cambiar direccion
		{
			if((mundo[i][0] == 30) || (mundo[i][0] == 20) || (mundo[i][0] == 10))
			{
				flag = 1;
			}
		}
	}
	else if(flag == 1)						//muevo los monstruos hacia abajo
	{
		for(j = 15; j >= 0; j--)
		{
			for(i = 15; i >= 0; i--)
			{
				if((mundo[j][i] == 30) || (mundo[j][i] == 20) || (mundo[j][i] == 10))
				{
					if(mundo[j+1][i] == BALA)			//AVISO: se debio agregar esta verificacion para solucionar un bug, en el cual si la bala impactaba a los aliens de costado no sucedia lo esperado
					{
						score(mundo[j][i]);				//sumamos el score de ser necesario
						mundo[j+1][i] = 0;				//matamos el alien y la bala
						mundo[j][i] = 0;
						cant_aliens--;					//descontamos el alien de ser necesario
					}
					else								//movemos los aliens
					{
						mundo[j+1][i] = mundo[j][i];
						mundo[j][i] = 0;
					}
				}
			} 
		}

		for(i = 15; i >= 0; i--)			//verifico para que direccion deben de moverse los monstruos
		{
			if((mundo[i][15] == 30) || (mundo[i][15] == 20) || (mundo[i][15] == 10))
			{
				flag = 2;					//mover hacia la izquierda
			}
			else if((mundo[i][0] == 30) || (mundo[i][0] == 20) || (mundo[i][0] == 10))
			{
				flag = 0;					//mover hacia la derecha
			}
		}
	}
}
