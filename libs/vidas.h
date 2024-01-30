#ifndef VIDAS_H
#define VIDAS_H

/*********************************
 * DEFINIMOS CONSTANTES PUBLICAS *
**********************************/
#define VIDAS 3
#define STATUS_VIDA 0
#define ADD_VIDA 1
#define SUBS_VIDA -1
#define RESET_VIDA VIDAS

/***********************
 * PROTOTIPOS PUBLICOS *
************************/
int vidas(int situacion);           //funcion la cual se encarga de las vidas, recibe lo que debe suceder con el contador de vidas devuelve la cantidad de vidas

#endif // VIDAS_H
