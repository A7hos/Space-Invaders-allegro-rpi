#include "libsconst.h"
#include "mundo.h"

/*******************************
 * VARIABLES GLOBALES PUBLICAS *
********************************/
int mundo[FILA][FILA];

/**************************************************************************
 * FUNCION: inicializa el mundo o tablero de juego
 * RECIBE: -
 * DEVUELVE: -
***************************************************************************/
void crear_mundo(void)
{
	int i, l;

	for(l = 0; l < FILA; l++)					//completamos con cero la matriz
    {
        for(i = 0; i < FILA; i++)
        {
            mundo[l][i] = 0;
        }
    }

	for(i = 0; i < ALIENS; i++)
	{
		mundo[3][2+i]= ALI_3;					// completamos una fila con monstruos nivel 3
	}
	for(l = 1; l < 3; l++)
	{
		for(i = 0; i < ALIENS; i++)
		{
			mundo[3+l][2+i] = ALI_2;			// completamos dos filas con monstruos nivel 2
		}
	}
	for(l = 3; l < 5; l++)
	{
		for(i = 0; i < ALIENS; i++)
		{
			mundo[3+l][2+i] = ALI_1;			//completamos dos filas con monstruos nivel 1
		}
	}
	
	mundo[11][3] = BLOQUES;						//inicializacion bloques protectores
	mundo[11][7] = BLOQUES;
	mundo[11][8] = BLOQUES;
	mundo[11][12] = BLOQUES;
	mundo[12][2] = BLOQUES;
	mundo[12][3] =BLOQUES;
	mundo[12][4]= BLOQUES;
	mundo[12][7] = BLOQUES;
	mundo[12][8] = BLOQUES;
	mundo[12][11] = BLOQUES;
	mundo[12][12] = BLOQUES;
	mundo[12][13] = BLOQUES;
	mundo[13][2] = BLOQUES;
	mundo[13][3] = BLOQUES;
	mundo[13][4] =BLOQUES;
	mundo[13][7] = BLOQUES;
	mundo[13][8] = BLOQUES;
	mundo[13][11] = BLOQUES;
	mundo[13][12] = BLOQUES;
	mundo[13][13] = BLOQUES;					//finalizacion creacion bloques
	
	mundo[15][6] = TANQUE;						//creacion del tanque
	mundo[15][7] = TANQUE;
	mundo[15][8] = TANQUE;
}


