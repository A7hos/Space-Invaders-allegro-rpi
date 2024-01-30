#include "/home/pi/libs/joydisp/disdrv.h"					///home/pi/libs/joydisp/
#include "/home/pi/libs/joydisp/joydrv.h"

#include "rpiconst.h"
#include "rpimenu.h"

/***********************
 * PROTOTIPOS PRIVADOS *
************************/
static void letras(void);                   //funcion que contiene las letras del menu, contiene: "SI", "P", "Q"

/**************************************************************************
 * FUNCION: muestra "SI" \n "P" \n "Q"
 * RECIBE: -
 * DEVUELVE: estado seleccionado, PLAY o QUIT
***************************************************************************/
int menu(void)
{
    int retu = OK, flag = 1;                                    //variable a devolver y flag de delay
    joyinfo_t coord = {0,0,J_NOPRESS};							//coordenadas medidas del joystick
	dcoord_t pos = {4, 7};                                      //coordenadas iniciales del selector
    dcoord_t npos = pos;                                        //donde guardaremos las nuevas coordenadas del selector

    disp_clear();                                               //limpiamos el display
    letras();                                                   //mostramos las letras
    disp_update();

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
            npos.y = 7;                                         //si movemos el joystick hacia arriba movemos el selector
        }
        if(coord.y < -THRESHOLD)
        {
            npos.y = 12;                                        //si movemos el joystick hacia abajo movemos el selector
        }
        
        //al precionar el boton del stick y dependiendo del selector
        if((coord.sw == J_PRESS) && (npos.y == 7) && (flag == 0))
        {
            retu = PLAY;                                        //si el selector esta en "P" se devuelve PLAY para iniciar el juego
        }
        if((coord.sw == J_PRESS) && (npos.y == 12) && (flag == 0))
        {
            retu = QUIT;                                        //si el selector esta en "Q" se devuelve QUIT para cerrar el juego
        }
        
		disp_write(pos, D_OFF);                                 //borramos la posicion vieja del selector
		disp_write(npos, D_ON);                                 //mostramos la posicion nueva del selector
        disp_update();                                          //actualizamos el display
        pos = npos;                                             //actualizamos la posicion del selector con la posicion actual
    }
    while(retu == OK);                                          //luego de seleccionar alguna de las opciones "PLAY o QUIT" salimos

    return retu;                                                //devolvemos lo seleccionado
}

/**************************************************************************
 * FUNCION: contiene las letras hardcodeadas
 * RECIBE: -
 * DEVUELVE: -
***************************************************************************/
static void letras(void)
{
    dcoord_t pos;        //letra Q
    pos.x = 6;
    pos.y = 14;
    disp_write(pos, D_ON);
    pos.x = 7;
    disp_write(pos, D_ON);
    pos.x = 8;
    disp_write(pos, D_ON);
    pos.x = 9;
    disp_write(pos, D_ON);
    pos.x = 6;
    pos.y = 13;
    disp_write(pos, D_ON);
    pos.x = 8;
    disp_write(pos, D_ON);
    pos.x = 6;
    pos.y = 12;
    disp_write(pos, D_ON);
    pos.x = 8;
    disp_write(pos, D_ON);
    pos.x = 6;
    pos.y = 11;
    disp_write(pos, D_ON);
    pos.x = 7;
    disp_write(pos, D_ON);
    pos.x = 8;
    disp_write(pos, D_ON);

    pos.x = 6;                  //letra P
    pos.y = 9;
    disp_write(pos, D_ON);
    pos.x = 6;
    pos.y = 8;
    disp_write(pos, D_ON);
    pos.x = 7;
    disp_write(pos, D_ON);
    pos.x = 8;
    disp_write(pos, D_ON);
    pos.x = 6;
    pos.y = 7;
    disp_write(pos, D_ON);
    pos.x = 8;
    disp_write(pos, D_ON);
    pos.x = 6;
    pos.y = 6;
    disp_write(pos, D_ON);
    pos.x = 7;
    disp_write(pos, D_ON);
    pos.x = 8;
    disp_write(pos, D_ON);

    pos.x = 4;                  //letra S
    pos.y = 4;
    disp_write(pos, D_ON);
    pos.x = 5;
    disp_write(pos, D_ON);
    pos.x = 6;
    disp_write(pos, D_ON);
    pos.x = 7;
    disp_write(pos, D_ON);
    pos.x = 6;
    pos.y = 3;
    disp_write(pos, D_ON);
    pos.x = 5;
    pos.y = 2;
    disp_write(pos, D_ON);
    pos.x = 4;
    pos.y = 1;
    disp_write(pos, D_ON);
    pos.x = 5;
    disp_write(pos, D_ON);
    pos.x = 6;
    disp_write(pos, D_ON);
    pos.x = 7;
    disp_write(pos, D_ON);

    pos.x = 9;                  //letra I
    pos.y = 4;
    disp_write(pos, D_ON);
    pos.x = 10;
    disp_write(pos, D_ON);
    pos.x = 11;
    disp_write(pos, D_ON);
    pos.x = 10;
    pos.y = 3;
    disp_write(pos, D_ON);
    pos.y = 2;
    disp_write(pos, D_ON);
    pos.x = 9;
    pos.y = 1;
    disp_write(pos, D_ON);
    pos.x = 10;
    disp_write(pos, D_ON);
    pos.x = 11;
    disp_write(pos, D_ON);
}