#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "libsconst.h"
#include "dispara.h"
#include "misil.h"
#include "impact.h"

/**************************************************************************
 * FUNCION: gestiona el disparo del jugador
 * RECIBE: matriz del mundo o tablero de juego
 * DEVUELVE: si esta listo o no para disparar de nuevo
***************************************************************************/
int shoot(int mundo[16][16])
{
	int i = 0, flag = 0;									//indice de busqueda y flag de escape
	static int j, flag2 = 1;								//j se usa como indice para saber donde se dispara, y flag2 indica cuando esta listo para disparar, 1 significa que se puede disparar y 0 que el disparo existe
	
	if(flag2 == 1)											//si el disparo no existe
	{
		while((i < 16) && (flag == 0))						//buscamos donde se encuentra el tanque para disparar
		{
			if(mundo[15][i] == TANQUE)						//una vez encontrado el tanque
			{
				if(mundo_buffer[14][i+1] != ALI_BALA)		//si a la hora de disparar no se tiene un disparo enemigo enfrente
				{
					mundo[14][i+1] = BALA;					//generamos el disparo
					flag2 = 0;								//desactivamos el flag2 para indicar que no se puede disparar mientras exista otro disparo
					flag = 1;								//usamos flag para sallir del bucle
					j = i+1;								//guardamos la coordenada de la columna donde se genero el disparo
				}
				else										//si se tiene un disparo enemigo enfrente
				{
					mundo_buffer[14][i+1] = 0;				//matamos el disparo del enemigo
				}
			}
			else											//sino encontramos el tanque
			{
				i++;										//incrementamos el indice
			}
		}
	}
	else													//si el disparo existe
	{
		flag2 = misil(j, mundo);							//llamamos a misil para que desplace el disparo y nos diga cuando impacto
	}

	return flag2;											//devolvemos si se puede o no disparar
}

/**************************************************************************
 * FUNCION: gestiona el disparo de los aliens
 * RECIBE: matriz del mundo o tablero de juego
 * DEVUELVE: -
***************************************************************************/
void shoot_alien(int mundo[16][16])
{
	int i = 0, j, flag = 1;									//indices y flag de escape
	static int x, y, flag2 = 0;								//x y son indices estaticos para mantener el disparo de los aliens en las coordenadas deseadas, y flag2 permite que solo exista un disparo de aliens por vez

	srand(time(NULL));										//inicializamos la semilla

	if(flag2 == 0)											//si no existe el disparo de los aliens lo generamos
	{
		do													//buscamos dentro del bucle que el alien a disparar exista
		{
			i = rand() % 16;								//seleccionamos una columna al azar para que busque la existencia de los aliens

			for(j = 0; (j < 16) && (flag == 1); j++)		//buscamos en los aliens de nivel 1
			{
				if((mundo[j][i] == ALI_1) && ((mundo[j+1][i] < 5) || (mundo[j+1][i] == BALA)))		//si hay aliens con vida
				{
					if(mundo[j+1][i] == BALA)				//verificamos si hay una bala del jugador cerca
					{
						mundo[j+1][i] = 0;					//de ser asi "dispara el alien" y se borra la bala
						flag = 0;							//salimos del bucle
					}
					else if(mundo[j+1][i] != 0)				//verificamos si hay uns barrera protectora cerca
					{
						mundo[j+1][i]--;					//decrementamos el nivel de la barrera
						flag = 0;							//salimos del bucle
					}
					else									//sino se encontro lo anterior
					{
						mundo_buffer[j+1][i] = ALI_BALA;	//generamos la bala del alien
						flag = 0;							//salimos del bucle
						x = i;								//guardamos los indices en los estaticos para usar luego en diferentes llamados a la funcion
						y = j + 1;
					}
				}
			}
			if(flag == 1)									//en caso de que no halla ningun alien vivo en el nivel 1 buscamos en el nivel 2
			{
				for(j = 0; (j < 16) && (flag == 1); j++)	//repetimos el proceso con los aliens de nivel 2
				{
					if((mundo[j][i] == ALI_2) && ((mundo[j+1][i] < 5) || (mundo[j+1][i] == BALA)))
					{
						if(mundo[j+1][i] == BALA)
						{
							mundo[j+1][i] = 0;
							flag = 0;
						}
						else if(mundo[j+1][i] != 0)
						{
							mundo[j+1][i]--;
							flag = 0;
						}
						else
						{
							mundo_buffer[j+1][i] = ALI_BALA;
							flag = 0;
							x = i;
							y = j + 1;
						}
					}
				}
			}
			if(flag == 1)									//en caso de que no halla ningun alien vivo en el nivel 2 buscamos en el nivel 3
			{
				for(j = 0; (j < 16) && (flag == 1); j++)	//repetimos el proceso con los aliens de nivel 3
				{
					if((mundo[j][i] == ALI_3) && ((mundo[j+1][i] < 5) || (mundo[j+1][i] == BALA)))
					{
						if(mundo[j+1][i] == BALA)
						{
							mundo[j+1][i] = 0;
							flag = 0;
						}
						else if(mundo[j+1][i] != 0)
						{
							mundo[j+1][i]--;
							flag = 0;
						}
						else
						{
							mundo_buffer[j+1][i] = ALI_BALA;
							flag = 0;
							x = i;
							y = j + 1;
						}
					}
				}
			}
		}
		while((flag == 1) && (cant_aliens != 0));			//en caso de que en la columna elegida no halla ningun alien volvemos a buscar en otra, y si no hay ningun alien vivo salimos del bucle
		flag2 = 1;											//activamos el flag2 para indicar que los aliens dispararon
	}
	else													//cuando el disparo de los aliens esta activo
	{
		if((mundo_buffer[y][x] == ALI_BALA) && (y < 15))	//si la bala existe y no llego a abajo de todo
		{
			if(mundo_buffer[y+1][x] != 0)					//verificamos si impacta con algo
			{
				impact_alien(mundo[y+1][x], y+1, x, mundo);	//en caso de hacerlo llamamos a impact_alien para que verifique con que impacto el disparo
				mundo_buffer[y][x] = 0;						//matamos el disparo
				flag2 = 0;									//desactivamos el flag para indicar que se vuelva a disparar
			}
			else											//si no impacto con nada
			{
				mundo_buffer[y][x] = 0;						//desplazamos el disparo
				mundo_buffer[y+1][x] = ALI_BALA;
			}
			y++;											//incrementamos el indice estatico
		}
		else												//si la bala no existe o llego hasta abajo
		{
			mundo_buffer[y][x] = 0;							//nos aseguramos de que deje de existir la bala
			flag2 = 0;										//desactivamos el flag para indicar que se vuelva a disparar
		}
	}
}

