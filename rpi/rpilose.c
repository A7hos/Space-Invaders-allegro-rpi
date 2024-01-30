#include <unistd.h>

#include "/home/pi/libs/joydisp/disdrv.h"

#include "rpiconst.h"
#include "rpilose.h"
#include "../libs/score.h"

/***********************
 * PROTOTIPOS PRIVADOS *
************************/
static void num(int num, int x, int y);     //funcion que contiene los numeros hardcodeados y recibe el numero deseado y las cordenadas x e y
static void letras(void);                   //funcion que contiene las letras hardcodeadas de la pantalla de lose, contiene la frase: "END SC:"

/**************************************************************************
 * FUNCION: muestra "END SC:" luego de perder y luego el score
 * RECIBE: -
 * DEVUELVE: -
***************************************************************************/
void lose(void)
{
    int digito, x , y;
    int temp = scor;
    
	disp_clear();                           //limpiamos el display
    letras();                               //mostramos las letras "END SC:"
    disp_update();                          //actualizamos el display
    sleep(3);                               //esperamos unos sefundos

	disp_clear();                           //impiamos el display de nuevo
    x = 12;                                 //cordenadas en x de la cifra menos cignificativa del score
    y = 8;                                  //cordenadas en y de la cifra menos cignificativa del score

    do                                      //bucle para mostrar el score ES IMPORTANTE SABER QUE SOLO SE PUEDE VISUALIZAR HASTA UN SCORE CON 8 CIFRAS
    {
        digito = temp % 10;                 //guardamos en digito la cifra menos cignificativa del score
        
        if(x >= 0)                          //hasta llegar al limite del display en x
        {
            num(digito, x, y);              //mostramos el digito en la posicion deseada
            x -= 4;                         //nos desplazamos a la siguiente cifra
        }
        else                                //cuando llegamos al limite en x
        {
            x = 12;                         //restet del x
            y = 2;                          //actuaizmos y
            num(digito, x, y);              //mostramos el digito en la posicion deseada
            x -= 4;                         //nos desplazamos
        }
        temp /= 10;                         //dividimos a temp que contiene una copia del score para saber el proximo digito
    }
    while(temp != 0);                       //hasta que temp valga 0 lo cua significa que terminamos de mostrar el numero

    disp_update();                          //actualizamos el display
    sleep(5);                               //esperamos nuevamente unos segundos

    scor = 0;                               //reset del score
}

/**************************************************************************
 * FUNCION: contiene los nuemros hardcodeados
 * RECIBE: el numero deseado y las cordenadas x e y de la matriz de led
 * DEVUELVE: -
***************************************************************************/
static void num(int num, int x, int y)
{
    dcoord_t pos;
    switch(num)
    {
        case 1:
            pos.x = 0 + x;
            pos.y = 0 + y;
            disp_write(pos, D_ON);
            pos.x = 1 + x;
            disp_write(pos, D_ON);
            pos.x = 1 + x;
            pos.y = 1 + y;
            disp_write(pos, D_ON);
            pos.x = 1 + x;
            pos.y = 2 + y;
            disp_write(pos, D_ON);
            pos.x = 1 + x;
            pos.y = 3 + y;
            disp_write(pos, D_ON);
            pos.x = 0 + x;
            pos.y = 4 + y;
            disp_write(pos, D_ON);
            pos.x = 1 + x;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            disp_write(pos, D_ON);
            break;
        case 2:
            pos.x = 0 + x;
            pos.y = 4 + y;
            disp_write(pos, D_ON);
            pos.x = 1 + x;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            disp_write(pos, D_ON);
            pos.x = 0 + x;
            pos.y = 2 + y;
            disp_write(pos, D_ON);
            pos.x = 1 + x;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            disp_write(pos, D_ON);
            pos.x = 0 + x;
            pos.y = 0 + y;
            disp_write(pos, D_ON);
            pos.x = 1 + x;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            pos.y = 1 + y;
            disp_write(pos, D_ON);
            pos.x = 0 + x;
            pos.y = 3 + y;
            disp_write(pos, D_ON);
            break;
        case 3:
            pos.x = 0 + x;
            pos.y = 4 + y;
            disp_write(pos, D_ON);
            pos.x = 1 + x;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            disp_write(pos, D_ON);
            pos.x = 0 + x;
            pos.y = 0 + y;
            disp_write(pos, D_ON);
            pos.x = 1 + x;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            pos.y = 1 + y;
            disp_write(pos, D_ON);
            pos.y = 3 + y;
            disp_write(pos, D_ON);
            pos.x = 1 + x;
            pos.y = 2 + y;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            disp_write(pos, D_ON);
            break;
        case 4:
            pos.x = 0 + x;
            pos.y = 0 + y;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            disp_write(pos, D_ON);
            pos.x = 0 + x;
            pos.y = 1 + y;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            disp_write(pos, D_ON);
            pos.x = 0 + x;
            pos.y = 2 + y;
            disp_write(pos, D_ON);
            pos.x = 1 + x;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            disp_write(pos, D_ON);
            pos.y = 3 + y;
            disp_write(pos, D_ON);
            pos.y = 4 + y;
            disp_write(pos, D_ON);
            break;
        case 5:
            pos.x = 0 + x;
            pos.y = 4 + y;
            disp_write(pos, D_ON);
            pos.x = 1 + x;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            disp_write(pos, D_ON);
            pos.x = 0 + x;
            pos.y = 2 + y;
            disp_write(pos, D_ON);
            pos.x = 1 + x;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            disp_write(pos, D_ON);
            pos.x = 0 + x;
            pos.y = 0 + y;
            disp_write(pos, D_ON);
            pos.x = 1 + x;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            disp_write(pos, D_ON);
            pos.x = 0 + x;
            pos.y = 1 + y;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            pos.y = 3 + y;
            disp_write(pos, D_ON);
            break;
        case 6:
            pos.x = 0 + x;
            pos.y = 4 + y;
            disp_write(pos, D_ON);
            pos.x = 1 + x;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            disp_write(pos, D_ON);
            pos.x = 0 + x;
            pos.y = 2 + y;
            disp_write(pos, D_ON);
            pos.x = 1 + x;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            disp_write(pos, D_ON);
            pos.x = 0 + x;
            pos.y = 0 + y;
            disp_write(pos, D_ON);
            pos.x = 1 + x;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            disp_write(pos, D_ON);
            pos.x = 0 + x;
            pos.y = 1 + y;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            pos.y = 3 + y;
            disp_write(pos, D_ON);
            pos.x = 0 + x;
            disp_write(pos, D_ON);
            break;
        case 7:
            pos.x = 0 + x;
            pos.y = 0 + y;
            disp_write(pos, D_ON);
            pos.x = 1 + x;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            disp_write(pos, D_ON);
            pos.y = 1 + y;
            disp_write(pos, D_ON);
            pos.y = 2 + y;
            disp_write(pos, D_ON);
            pos.y = 3 + y;
            disp_write(pos, D_ON);
            pos.y = 4 + y;
            disp_write(pos, D_ON);
            break;
        case 8:
            pos.x = 0 + x;
            pos.y = 4 + y;
            disp_write(pos, D_ON);
            pos.x = 1 + x;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            disp_write(pos, D_ON);
            pos.x = 0 + x;
            pos.y = 2 + y;
            disp_write(pos, D_ON);
            pos.x = 1 + x;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            disp_write(pos, D_ON);
            pos.x = 0 + x;
            pos.y = 0 + y;
            disp_write(pos, D_ON);
            pos.x = 1 + x;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            disp_write(pos, D_ON);
            pos.x = 0 + x;
            pos.y = 1 + y;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            pos.y = 3 + y;
            disp_write(pos, D_ON);
            pos.x = 0 + x;
            disp_write(pos, D_ON);
            break;
        case 9:
            pos.x = 0 + x;
            pos.y = 4 + y;
            disp_write(pos, D_ON);
            pos.x = 1 + x;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            disp_write(pos, D_ON);
            pos.x = 0 + x;
            pos.y = 2 + y;
            disp_write(pos, D_ON);
            pos.x = 1 + x;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            disp_write(pos, D_ON);
            pos.x = 0 + x;
            pos.y = 0 + y;
            disp_write(pos, D_ON);
            pos.x = 1 + x;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            disp_write(pos, D_ON);
            pos.x = 0 + x;
            pos.y = 1 + y;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            pos.y = 3 + y;
            disp_write(pos, D_ON);
            break;
        case 0:
            pos.x = 0 + x;
            pos.y = 4 + y;
            disp_write(pos, D_ON);
            pos.x = 1 + x;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            disp_write(pos, D_ON);
            pos.x = 0 + x;
            pos.y = 2 + y;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            disp_write(pos, D_ON);
            pos.x = 0 + x;
            pos.y = 0 + y;
            disp_write(pos, D_ON);
            pos.x = 1 + x;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            disp_write(pos, D_ON);
            pos.x = 0 + x;
            pos.y = 1 + y;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            disp_write(pos, D_ON);
            pos.x = 2 + x;
            pos.y = 3 + y;
            disp_write(pos, D_ON);
            pos.x = 0 + x;
            disp_write(pos, D_ON);
            break;
    }
}

/**************************************************************************
 * FUNCION: contiene las letras hardcodeadas
 * RECIBE: -
 * DEVUELVE: -
***************************************************************************/
static void letras(void)
{
    dcoord_t pos;
    
    pos.x = 2;                              //Letra E
    pos.y = 1;
    disp_write(pos, D_ON);
    pos.x = 3;
    disp_write(pos, D_ON);
    pos.x = 4;
    disp_write(pos, D_ON);
    pos.x = 2;
    pos.y = 2;
    disp_write(pos, D_ON);
    pos.x = 2;
    pos.y = 3;
    disp_write(pos, D_ON);
    pos.x = 3;
    disp_write(pos, D_ON);
    pos.x = 2;
    pos.y = 4;
    disp_write(pos, D_ON);
    pos.x = 2;
    pos.y = 5;
    disp_write(pos, D_ON);
    pos.x = 3;
    disp_write(pos, D_ON);
    pos.x = 4;
    disp_write(pos, D_ON);

    pos.x = 6;                              //Letra N
    pos.y = 1;
    disp_write(pos, D_ON);
    pos.y = 2;
    disp_write(pos, D_ON);
    pos.y = 3;
    disp_write(pos, D_ON);
    pos.y = 4;
    disp_write(pos, D_ON);
    pos.y = 5;
    disp_write(pos, D_ON);
    pos.x = 7;
    pos.y = 2;
    disp_write(pos, D_ON);
    pos.y = 3;
    disp_write(pos, D_ON);
    pos.y = 4;
    disp_write(pos, D_ON);
    pos.x = 8;
    pos.y = 1;
    disp_write(pos, D_ON);
    pos.y = 2;
    disp_write(pos, D_ON);
    pos.y = 3;
    disp_write(pos, D_ON);
    pos.y = 4;
    disp_write(pos, D_ON);
    pos.y = 5;
    disp_write(pos, D_ON);
   
    pos.x = 10;                             //Letra D
    pos.y = 1;
    disp_write(pos, D_ON);
    pos.y = 2;
    disp_write(pos, D_ON);
    pos.y = 3;
    disp_write(pos, D_ON);
    pos.y = 4;
    disp_write(pos, D_ON);
    pos.y = 5;
    disp_write(pos, D_ON);
    pos.x = 11;
    pos.y = 1;
    disp_write(pos, D_ON);
    pos.y = 5;
    disp_write(pos, D_ON);
    pos.x = 12;
    pos.y = 2;
    disp_write(pos, D_ON);
    pos.y = 3;
    disp_write(pos, D_ON);
    pos.y = 4;
    disp_write(pos, D_ON);

    pos.x = 3;                              //Letra S
    pos.y = 8;
    disp_write(pos, D_ON);
    pos.y = 9;
    disp_write(pos, D_ON);
    pos.y = 10;
    disp_write(pos, D_ON);
    pos.y = 12;
    disp_write(pos, D_ON);
    pos.x = 4;
    pos.y = 8;
    disp_write(pos, D_ON);
    pos.y = 10;
    disp_write(pos, D_ON);
    pos.y = 12;
    disp_write(pos, D_ON);
    pos.x = 5;
    pos.y = 8;
    disp_write(pos, D_ON);
    pos.y = 10;
    disp_write(pos, D_ON);
    pos.y = 11;
    disp_write(pos, D_ON);
    pos.y = 12;
    disp_write(pos, D_ON);
    
    pos.x = 7;                              //Letra C
    pos.y = 9;
    disp_write(pos, D_ON);
    pos.y = 10;
    disp_write(pos, D_ON);
    pos.y = 11;
    disp_write(pos, D_ON);
    pos.x = 8;
    pos.y = 8;
    disp_write(pos, D_ON);
    pos.y = 12;
    disp_write(pos, D_ON);
    pos.x = 9;
    pos.y = 8;
    disp_write(pos, D_ON);
    pos.y = 12;
    disp_write(pos, D_ON);

    
    pos.x = 11;                             //Simbolo :
    pos.y = 9;
    disp_write(pos, D_ON);
    pos.y = 11;
    disp_write(pos, D_ON);

}