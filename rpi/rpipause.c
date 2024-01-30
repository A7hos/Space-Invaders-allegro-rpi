#include <stdio.h>
#include <time.h>

#include "/home/pi/libs/joydisp/disdrv.h"
#include "/home/pi/libs/joydisp/joydrv.h"

#include "rpiconst.h"
#include "rpipause.h"

/***********************
 * PROTOTIPOS PRIVADOS *
************************/
static void letras(void);                   //funcion que contiene las letras de la pausa, contiene: "P", "R", "M/H" y "Q"

/**********************
 * VARIABLES GLOBALES *
***********************/
unsigned int t_pausa;                       //variable que guarda el tiempo acumulado en segundos transcurrido en la pausa, se resetea respecto al estado de la nodriza

/**************************************************************************
 * FUNCION: muestra "P" "R" "M/H" y "Q"
 * RECIBE: -
 * DEVUELVE: estado seleccionado, RESUME, RESTART, MENU o QUIT
***************************************************************************/
int pause(void)
{
    int retu = OK, flag = 1;                                    //variable a devolver y flag de delay
    time_t tiempo_ini;                                          //variable donde guardamos la hora inicial
    time_t tiempo_fin;                                          //variable donde guardamos la hora final
    joyinfo_t coord = {0,0,J_NOPRESS};							//coordenadas medidas del joystick
	dcoord_t pos = {7, 6};                                      //coordenadas iniciales del selector
    dcoord_t npos = pos;                                        //donde guardaremos las nuevas coordenadas del selector

    tiempo_ini = time(NULL);                                    //guardamos la hora inicial
    disp_clear();                                               //limpiamos el display
    letras();                                                   //mostramos las letras
    disp_update();                                              //actualizamos el display

    do
    {
        coord = joy_read();										//Guarda las coordenadas medidas

        if(flag >= 1)                                           //usamos a flag como contador para generar un delay en ciertas acciones del bucle, que no sucedan tan rapido
		{
			if(flag >= 30)                                      //cantidad de delay 
			{
				flag = 0;
			}
			else
			{
				flag++;
			}
		}

        if(coord.y > THRESHOLD)
        {
            npos.x = 7;                                         //si movemos el joystick hacia arriba movemos el selector
            npos.y = 6;
        }
        if(coord.y < -THRESHOLD)
        {
            npos.x = 7;                                         //si movemos el joystick hacia abajo movemos el selector
            npos.y = 9;
        }
        if(coord.x > THRESHOLD)
        {
            npos.x = 9;                                         //si movemos el joystick hacia la derecha movemos el selector
            npos.y = 8;
        }
        if(coord.x < -THRESHOLD)
        {
            npos.x = 5;                                         //si movemos el joystick hacia la izquierda movemos el selector
            npos.y = 8;
        }

        //al precionar el boton del stick y dependiendo del selector
        if((coord.sw == J_PRESS) && (npos.x == 7) && (npos.y == 6) && (flag == 0))
        {
            retu = RESUME;                                      //si el selector esta en "P" se devuelve RESUME para reanudar el juego
        }
        if((coord.sw == J_PRESS) && (npos.x == 5) && (npos.y == 8) && (flag == 0))
        {
            retu = RESTART;                                     //si el selector esta en "R" se devuelve RESTART para reiniciar el juego
        }
        if((coord.sw == J_PRESS) && (npos.x == 9) && (npos.y == 8) && (flag == 0))
        {
            retu = MENU;                                        //si el selector esta en "M/H" se devuelve NENU para volver al menu del juego
        }
        if((coord.sw == J_PRESS) && (npos.x == 7) && (npos.y == 9) && (flag == 0))
        {
            retu = QUIT;                                        //si el selector esta en "Q" se devuelve QUIT para salir del juego
        }

		disp_write(pos, D_OFF);                                 //borramos la posicion vieja del selector
		disp_write(npos, D_ON);                                 //mostramos la posicion nueva del selector
        disp_update();                                          //actualizamos el display
        pos = npos;                                             //actualizamos la posicion del selector con la posicion actual
    }
    while(retu == OK);                                          //luego de seleccionar alguna de las opciones "RESUME, RESTAR, MENU o QUIT" salimos

    tiempo_fin = time(NULL);                                    //guardamos la hora fianl
    t_pausa += (tiempo_fin - tiempo_ini);                       //guardamos la cantidad de segundos acumulados trascurridos en el menu

    return retu;                                                //devolvemos lo seleccionado
}

/**************************************************************************
 * FUNCION: contiene las letras hardcodeadas
 * RECIBE: -
 * DEVUELVE: -
***************************************************************************/
static void letras(void)
{
    dcoord_t pos;                       //letra P
    pos.x = 6;
    pos.y = 1;
    disp_write(pos, D_ON);
    pos.x = 7;
    disp_write(pos, D_ON);
    pos.x = 8;
    disp_write(pos, D_ON);
    pos.x = 6;
    pos.y = 2;
    disp_write(pos, D_ON);
    pos.x = 8;
    disp_write(pos, D_ON);
    pos.x = 6;
    pos.y = 3;
    disp_write(pos, D_ON);
    pos.x = 7;
    disp_write(pos, D_ON);
    pos.x = 8;
    disp_write(pos, D_ON);
    pos.x = 6;
    pos.y = 4;
    disp_write(pos, D_ON);

    pos.x = 1;                          //letra R
    pos.y = 6;
    disp_write(pos, D_ON);
    pos.x = 2;
    disp_write(pos, D_ON);
    pos.x = 3;
    disp_write(pos, D_ON);
    pos.x = 1;
    pos.y = 7;
    disp_write(pos, D_ON);
    pos.x = 3;
    disp_write(pos, D_ON);
    pos.x = 1;
    pos.y = 8;
    disp_write(pos, D_ON);
    pos.x = 2;
    disp_write(pos, D_ON);
    pos.x = 1;
    pos.y = 9;
    disp_write(pos, D_ON);
    pos.x = 3;
    disp_write(pos, D_ON);

    pos.x = 11;                         //letra M/H
    pos.y = 6;
    disp_write(pos, D_ON);
    pos.x = 13;
    disp_write(pos, D_ON);
    pos.x = 11;
    pos.y = 7;
    disp_write(pos, D_ON);
    pos.x = 12;
    disp_write(pos, D_ON);
    pos.x = 13;
    disp_write(pos, D_ON);
    pos.x = 11;
    pos.y = 8;
    disp_write(pos, D_ON);
    pos.x = 12;
    disp_write(pos, D_ON);
    pos.x = 13;
    disp_write(pos, D_ON);
    pos.x = 11;
    pos.y = 9;
    disp_write(pos, D_ON);
    pos.x = 13;
    disp_write(pos, D_ON);

    pos.x = 6;                          //letra Q
    pos.y = 11;
    disp_write(pos, D_ON);
    pos.x = 7;
    disp_write(pos, D_ON);
    pos.x = 8;
    disp_write(pos, D_ON);
    pos.x = 6;
    pos.y = 12;
    disp_write(pos, D_ON);
    pos.x = 8;
    disp_write(pos, D_ON);
    pos.x = 6;
    pos.y = 13;
    disp_write(pos, D_ON);
    pos.x = 8;
    disp_write(pos, D_ON);
    pos.x = 6;
    pos.y = 14;
    disp_write(pos, D_ON);
    pos.x = 7;
    disp_write(pos, D_ON);
    pos.x = 8;
    disp_write(pos, D_ON);
    pos.x = 9;
    disp_write(pos, D_ON);
}