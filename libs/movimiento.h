#ifndef MOVIMIENTO_H
#define MOVIMIENTO_H

/**********************
 * VARIABLES PUBLICAS *
***********************/
extern int flag_mov;                    //flag usado para asegurar que el movimiento siempre comience en el mismo sentido

/***********************
 * PROTOTIPOS PUBLICOS *
************************/
void move(int mundo[16][16]);           //funcion encargada de mover los aliens con cada llamado

#endif // MOVIMIENTO_H
