#ifndef MUNDO_H
#define MUNDO_H

/*********************************
 * DEFINIMOS CONSTANTES PUBLICAS *
**********************************/
#define ALIENS 11                   //cantidad de aliens por fila
#define FILA 16                     //ancho y alto de la matriz (es cuadrada)

/**********************
 * VARIABLES PUBLICAS *
***********************/
extern int mundo[FILA][FILA];       //matriz global usada como tablero de juego principal

/***********************
 * PROTOTIPOS PUBLICOS *
************************/
void crear_mundo(void);             //funcion la cual le da el estado inicial al mundo o tablero de juego

#endif // MUNDO_H
