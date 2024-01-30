#ifndef ALLGAME_H
#define ALLGAME_H

/*********************************
 * DEFINIMOS CONSTANTES PUBLICAS *
**********************************/
#define CANON_SIZE_X 60                             //tamaño del cañon
#define CANON_SIZE_Y 30
#define MOVE_RATE 10.0                              //velocidad de movimieto del cañon

#define CANT_ESCU (36)                              //cantidad de escudos
#define ESCU_X ((SCREEN_W / 8)*1.5)                 //tamaño de los escudos
#define ESCU_Y (SCREEN_H / 2 + SCREEN_H / 4)

/***********************
 * PROTOTIPOS PUBLICOS *
************************/
int game(void);                                     //funcion encargada de manejar juego 

#endif // ALLGAME_H
