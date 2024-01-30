#include <stdio.h>
#include <stdbool.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h> //manejo de fonts
#include <allegro5/allegro_ttf.h> //Manejo de ttfs

#include "allconst.h"
#include "allwinlose.h"
#include "../libs/score.h"

/***********************
 * PROTOTIPOS PRIVADOS *
************************/
static int ini(void);                                               //inicializa la pantalla de winlose
static void destroy(void);                                          //libera la memoria reservada

/*******************************
 * VARIABLES GLOBALES PRIVADAS *
********************************/
static ALLEGRO_TIMER *tempo = NULL;                                 //puntero a un timer con periodo = 1, cuenta segundos
static ALLEGRO_FONT *font1 = NULL;                                  //puntero a la fuente 1
static ALLEGRO_FONT *font2 = NULL;                                  //puntero a la fuente 2
static ALLEGRO_FONT *font_wl = NULL;                                //puntero a la fuente winlose
static ALLEGRO_SAMPLE *you_died = NULL;                             //puntero al sonido que se reproduce una vez que se pierde

/**************************************************************************
 * FUNCION: pantalla de winlose del juego
 * RECIBE: -
 * DEVUELVE: si hubo error
***************************************************************************/
int ptr_winlose(int estado)
{
    int retu = OK, segundos = 5;                                    //segundos antes de que desaparesca la pantalla winlose
    bool redraw = false;                                            //inidica que esta listo para redibujar
    char pun[25];                                                   //arreglo donde guardaremos el string que contiene el puntaje a mostrar
    char sec[25];                                                   //arreglo donde guardaremos los segundos restantes antes del cambio de pantalla

    if(ini() == ERROR)                                              //inicializamos y verificamos si hay error
    {
        return ERROR;
    }

    al_register_event_source(event_queue, al_get_timer_event_source(tempo));        //registramos el timer en segundos reales para realizar cuenta atras
    al_start_timer(tempo);                                                          //iniciamos el timer

    if(estado == LOSE)                                                              //verificamos si se perdio para reproducir el sonido
    {
        al_play_sample(you_died, 0.25, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    while(!do_exit && retu == OK && segundos > 0)                                   //bucle principal de la pantalla de winlose
    {
        ALLEGRO_EVENT state;
        if(al_get_next_event(event_queue, &state))                                  //tomamos el siguiente evento
        {
            if(state.type == ALLEGRO_EVENT_TIMER)                                   //esperamos al evento timer
            {
                if(state.timer.source == tempo)                                     //cada 1 segundo decrementamos el contador
                {
                    segundos--;
                }
                redraw = true;                                                      //siempre que el timer de fps lo permita dibujamos
            }
            else if(state.type == ALLEGRO_EVENT_DISPLAY_CLOSE)                      //verificamos si se cerro el display
            {
                do_exit = true;
            }
        }
        else if(redraw && al_is_event_queue_empty(event_queue))                     //cuando la cola de eventos esta vacia y redraw permita redibujar
        {
            redraw = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));                                 //ponemos el fondo negro

            if(estado == WIN)           //si se gana dibujamos la frase YOU WIN y mostramos el puntaje con un conteo atras para el siguiente nivel
            {
                al_draw_text(font_wl, al_map_rgb(10, 255, 50), (SCREEN_W / 2 ), (SCREEN_H / 4), ALLEGRO_ALIGN_CENTER, "YOU WIN");
                sprintf(pun, "Your score: %d", scor);
                al_draw_text(font1, al_map_rgb(255, 255, 255), (SCREEN_W / 2 ), (SCREEN_H / 2 ), ALLEGRO_ALIGN_CENTER, pun);
                sprintf(sec, "Next level in... %d", segundos);
                al_draw_text(font2, al_map_rgb(255, 255, 255), (SCREEN_W / 2 ), (SCREEN_H / 3 )*2, ALLEGRO_ALIGN_CENTER, sec);
    
            }
            else if(estado == LOSE)     //si se pierde dibujamos la frase YOU DIED y mostramos el puntaje con un conteo atras para el menu principal
            {
                al_draw_text(font_wl, al_map_rgb(255, 10, 50), (SCREEN_W / 2 ), (SCREEN_H / 4), ALLEGRO_ALIGN_CENTER, "YOU DIED");
                sprintf(pun, "Your score: %d", scor);
                al_draw_text(font1, al_map_rgb(255, 255, 255), (SCREEN_W / 2 ), (SCREEN_H / 2 ), ALLEGRO_ALIGN_CENTER, pun);
                sprintf(sec, "Menu in... %d", segundos);
                al_draw_text(font2, al_map_rgb(255, 255, 255), (SCREEN_W / 2 ), (SCREEN_H / 3 )*2, ALLEGRO_ALIGN_CENTER, sec);
            }

            al_flip_display();          //actualizamos el display
        }
    }
    if(estado == LOSE)                  //en caso de perder reseteamos el score luego de utilizarlo en la pantalla
    {
        scor = 0; 
    }

    destroy();                          //liberamos la memoria reservada
    return retu;
}

/**************************************************************************
 * FUNCION: asigna los punteros a los archivos externos a utilizar
 * RECIBE: -
 * DEVUELVE: si hubo error
***************************************************************************/
static int ini(void)
{
    tempo = al_create_timer(1.0);                                                   //creamos el reloj en segundos
    if(!tempo)
    {
        fprintf(stderr, "failed to create timer!\n");
        return ERROR;
    }

    font1 = al_load_ttf_font("res/Good Old DOS.ttf", 65, 0);                        //creamos la fuente 1
    if(!font1)
    {
        fprintf(stderr, "Could not load 'Good Old DOS.ttf'.\n");
        al_destroy_timer(tempo);
        return ERROR;
    }

    font2 = al_load_ttf_font("res/Good Old DOS.ttf", 35, 0);                        //creamos la fuente 2
    if(!font2)
    {
        fprintf(stderr, "Could not load 'Good Old DOS.ttf'.\n");
        al_destroy_timer(tempo);
        al_destroy_font(font1);
        return ERROR;
    }

    font_wl = al_load_ttf_font("res/Good Old DOS 3D.ttf", 150, 0);                  //creamos la fuente winlose
    if(!font_wl)
    {
        fprintf(stderr, "Could not load 'Good Old DOS3D.ttf'.\n");
        al_destroy_timer(tempo);
        al_destroy_font(font1);
        al_destroy_font(font2);
        return ERROR;
    }

    you_died = al_load_sample("res/you died.wav");                                  //cargamos el sonido de YOU DIED
    if(!you_died)
    {
        fprintf(stderr, "Audio you died.wav sample not loaded!\n");
        al_destroy_timer(tempo);
        al_destroy_font(font1);
        al_destroy_font(font2);
        al_destroy_font(font_wl);
        return ERROR;
    }
    return OK;
}

/**************************************************************************
 * FUNCION: libera la memoria reservada
 * RECIBE: -
 * DEVUELVE: -
***************************************************************************/
static void destroy(void)
{
    al_destroy_timer(tempo);
    al_destroy_font(font1);
    al_destroy_font(font2);
    al_destroy_font(font_wl);
    al_destroy_sample(you_died);
}