#include "libsconst.h"
#include "impact.h"
#include "score.h"
#include "vidas.h"

/*********************************
 * DEFINIMOS CONSTANTES PRIVADAS *
**********************************/
#define BLOQ_3 3
#define BLOQ_2 2
#define BLOQ_1 1

/**************************************************************************
 * FUNCION: verifica con que impacto el disparo del jugador
 * RECIBE: el objeto con el que impacta, coordenadas "y" y "x", matriz del mundo o tablero de juego
 * DEVUELVE: -
***************************************************************************/
void impact(int objeto, int y, int x, int mundo[16][16])
{
	if(objeto == ALI_1)							//verifico si impacto alien de nivel 1
	{
		mundo[y][x] = 0;
		cant_aliens--;							//restamos la cantidad de aliens
		score(ALI_1);							//sumamos el score del alien de nivel 1
	}
	else if(objeto == ALI_2)					//verifico si impacto alien de nivel 2
	{
		mundo[y][x] = 0;
		cant_aliens--;							//restamos la cantidad de aliens
		score(ALI_2);							//sumamos el score del alien de nivel 2
	}
	else if(objeto == ALI_3)					//verifico si impacto alien de nivel 3	
	{
		mundo[y][x] = 0;
		cant_aliens--;							//restamos la cantidad de aliens
		score(ALI_3);							//sumamos el score del alien de nivel 3
	}
	else if(objeto == ALI_N)					//verifico si impacto la nave nodriza	
	{
		mundo[y][x] = 0;
		score(ALI_N);							//sumamos el score de la nodriza
	}
	else if(objeto == BLOQUES)					//verifico si impacto bloque nivel 4
	{
		mundo[y][x] = 3;						//restamos las vidas del bloque hasta 0
	}
	else if(objeto == BLOQ_3)					//verifico si impacto bloque de nivel 3
	{
		mundo[y][x] = 2;			
	}
	else if(objeto == BLOQ_2)					//verifico si impacto bloque nivel 2
	{
		mundo[y][x] = 1;			
	}
	else if(objeto == BLOQ_1)					//verifico si impacto bloque de nivel 1
	{
		mundo[y][x] = 0;			
	}
}

/**************************************************************************
 * FUNCION: verifica con que impacto el disparo de los aliens
 * RECIBE: el objeto con el que impacta, coordenadas "y" y "x", matriz del mundo o tablero de juego
 * DEVUELVE: -
***************************************************************************/
void impact_alien(int objeto, int y, int x, int mundo[16][16])
{
	if(objeto == BLOQUES)						//verifico si impacto bloque nivel 4
	{
		mundo[y][x] = 3;						//restamos las vidas del bloque hasta 0
	}
	else if(objeto == BLOQ_3)					//verifico si impacto bloque de nivel 3
	{
		mundo[y][x] = 2;			
	}
	else if(objeto == BLOQ_2)					//verifico si impacto bloque nivel 2
	{
		mundo[y][x] = 1;			
	}
	else if(objeto == BLOQ_1)					//verifico si impacto bloque de nivel 1
	{
		mundo[y][x] = 0;			
	}
	else if(objeto == TANQUE)					//verifico si impacto al jugador
	{
		vidas(SUBS_VIDA);						//le quitamos una vida al jugador
	}
	else if(objeto == BALA)						//verifico si impacto la bala del jugador	
	{
		mundo[y][x] = 0;						//matamos la bala del jugador
	}
}
