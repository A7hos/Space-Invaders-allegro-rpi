#ifndef SCORE_H
#define SCORE_H

/**********************
 * VARIABLES PUBLICAS *
***********************/
extern int scor;                    //variable global donde guardamos el score de la partida para que pueda ser vista y en caso de ser necesario modificada

/***********************
 * PROTOTIPOS PUBLICOS *
************************/
void score(int contacto);           //funcion encargada del score, recibe que alien se impacto para sumar dicho puntaje

#endif // SCORE_H
