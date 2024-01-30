/**************************************************************************
 * MATERIA: Programacion I
 * CUATRIMESTRE: 2Q 2023
 * INTEGRANTES: Athos Expósito Sabán, Juan Francisco Compagnucci y Cristobal Kramer
 * PROFESORES:Nicolás Magliola y Pablo Vacatello
 * JUEGO: SPACE INVADERS en raspberry pi
***************************************************************************/

#include "/home/pi/libs/joydisp/disdrv.h"
#include "/home/pi/libs/joydisp/joydrv.h"

#include "rpiconst.h"
#include "rpimenu.h"
#include "rpilose.h"
#include "rpigame.h"

/**************************************************************************
 * FUNCION: main del progrma general en raspberry pi
 * RECIBE: -
 * DEVUELVE: si hubo error
***************************************************************************/
int main(void)
{
	int r = OK;

	joy_init();													    //inicializa el joystick
	disp_init();												    //inicializa el display
	
	do
    {
        r = OK;
        r = menu();                                                 //llamamos a menu y esperamos a ver que devuelve

        if(r == PLAY)                                               //si en el menu se selecciono play iniciamos el juego
        {
            do
            {
                r = OK;
                r = game();                                         //llamamos a juego y verificamos que devuelve
                if(r == LOSE)                                       //si se pierde por completo mostramos el score
                {
                    lose();
                }
            }
            while((r == RESTART) || (r == WIN));                    //si se gana o en la pausa se toca R se reinicia el juego, solo que si se gana el puntaje se mantiene
        }
    }
    while((r != QUIT) && ((r == LOSE) || (r == MENU)));             //si se pierde o en la pausa se toca M/H se vuelve al menu
	
	disp_clear();                                                   //Borro el display al salir
    disp_update();
    
	return OK;
}
