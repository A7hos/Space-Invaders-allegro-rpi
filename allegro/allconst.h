#ifndef ALLCONST_H
#define ALLCONST_H

/*******************************
 * INCLUIMOS LIBRERIA DE AUDIO *
********************************/
#include <allegro5/allegro_audio.h> 
#include <allegro5/allegro_acodec.h> //Extensiones con acodec .wav, .flac, .ogg, .it, .mod, .s3m, .xm. 

/*********************************
 * DEFINIMOS CONSTANTES GLOBALES *
**********************************/
#define OK 0
#define ERROR -1
#define RESUME 1
#define RESTART 2
#define MENU 3
#define QUIT 4
#define WIN 5
#define LOSE 6

#define CANT_ALIENS 55                              //cantidad de aliens
#define MOV_ALIENS 55                               //usada en la velocidad de movimiento de los aliens

#define FPS 60.0                                    //frames por segundo
#define SCREEN_W 1024                               //tamaño pantalla
#define SCREEN_H 768
#define HORIZ (SCREEN_W/16)                         //dividimos a la pantalla en 16x16 "celdas"
#define VERTI (SCREEN_H/16)

#define ALIN_X (HORIZ - HORIZ/7)                    //tamaño nodriza
#define ALIN_Y (VERTI - VERTI/4)

#define ALI3_X (HORIZ - HORIZ/2)                    //tamaño alien nivel 3
#define ALI3_Y (VERTI - VERTI/5)

#define ALI2_X (HORIZ - HORIZ/3.5)                  //tamaño alien nivel 2
#define ALI2_Y (VERTI - VERTI/5)

#define ALI1_X (HORIZ - HORIZ/4)                    //tamaño alien nivel 1
#define ALI1_Y (VERTI - VERTI/5)

/**********************
 * VARIABLES PUBLICAS *
***********************/
extern bool do_exit;                                //variable de escape para cerrar el programa
extern ALLEGRO_DISPLAY *display;                    //puntero del display
extern ALLEGRO_EVENT_QUEUE *event_queue;            //puntero de la cola de evenetos
extern ALLEGRO_TIMER *timer;                        //puntero del timer de fps
extern ALLEGRO_SAMPLE *boton_sfx;                   //puntero al sonido de boton

#endif // ALLCONST_H
