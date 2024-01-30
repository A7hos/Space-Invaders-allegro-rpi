/**************************************************************************
 * MATERIA: Programacion I
 * CUATRIMESTRE: 2Q 2023
 * INTEGRANTES: Athos Expósito Sabán, Juan Francisco Compagnucci y Cristobal Kramer
 * PROFESORES:Nicolás Magliola y Pablo Vacatello
 * JUEGO: SPACE INVADERS en allegro 5
***************************************************************************/
#include <stdio.h>
#include <stdbool.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h> //manejo de fonts
#include <allegro5/allegro_ttf.h> //Manejo de ttfs

#include "allconst.h"
#include "allmenu.h"
#include "allgame.h"
#include "allwinlose.h"

/***********************
 * PROTOTIPOS PRIVADOS *
************************/
static int init(void);                                                              //funcion de inicializacion general
static void destroy(void);                                                          //funcion que libera la memoria reservada

/*******************************
 * VARIABLES GLOBALES PUBLICAS *
********************************/
ALLEGRO_DISPLAY *display = NULL;                                                    //puntero del display
ALLEGRO_EVENT_QUEUE *event_queue = NULL;                                            //puntero de la cola de evenetos
ALLEGRO_TIMER *timer = NULL;                                                        //puntero del timer de fps
ALLEGRO_SAMPLE *boton_sfx = NULL;                                                   //puntero al sonido de boton
bool do_exit = false;                                                               //variable de escape para cerrar el programa

/**************************************************************************
 * FUNCION: main del progrma general en allegro 5
 * RECIBE: -
 * DEVUELVE: si hubo error
***************************************************************************/
int main(void)
{
    int r = OK;

    if(init() == ERROR)                                                             //inicializamos y verificamos si hay errores
    {
        return ERROR;
    }

    do
    {
        r = OK;
        r = menu();                                                                 //llamamos a menu y esperamos a ver que devuelve

        if(r == PLAY)                                                               //si en el menu se selecciono play iniciamos el juego
        {
            do
            {
                r = OK;
                r = game();                                                         //llamamos a juego y verificamos que devuelve
                if(r == LOSE || r == WIN)                                           //si se pasa el nivel o pierde por completo mostramos el score
                {
                    al_rest(0.25);
                    if(ptr_winlose(r) == ERROR)                                     //verificamos si a la hora de mostrar el puntaje hubo algun error
                    {
                        r = QUIT;                                                   //si hubo error salimos
                    }
                    
                }
            }
            while((r == RESTART) || (r == WIN));                                    //si se gana o en la pausa se toca restart se reinicia el juego, solo que si se gana el puntaje se mantiene
        }
    }
    while((r != QUIT) && ((r == LOSE) || (r == MENU)));                             //si en el menu se selecciono quit cerramos el juego, y si perdimos vovemos al menu

    destroy();                                                                      //liberamos la memoria
    return OK;
}

/**************************************************************************
 * FUNCION: inicializa allegro 5
 * RECIBE: -
 * DEVUELVE: si hubo error
***************************************************************************/
static int init(void)
{
    if(!al_init())                                                                  //iniciamos allegro
    {
        fprintf(stderr, "failed to initialize allegro!\n");
        return ERROR;
    }
    if(!al_init_font_addon())                                                       //inicializamos font addon
    {
        fprintf(stderr, "failed to initialize allegro!\n");
        al_uninstall_system();
        return ERROR;
    }
    if(!al_init_ttf_addon())                                                        //inicializamos el ttf (True Type Font) addon
    {
        fprintf(stderr, "failed to initialize allegro!\n");
        al_uninstall_system();
        return ERROR;
    }
    if(!al_install_mouse())                                                         //inicializamos el mouse
    {
        fprintf(stderr, "failed to initialize the mouse!\n");
        al_uninstall_system();
        return ERROR;
    }
    if(!al_install_keyboard())                                                      //inicializamos el teclado
    {
        fprintf(stderr, "failed to initialize the keyboard!\n");
        al_uninstall_system();
        return ERROR;
    }
    if(!al_install_audio())                                                         //iniciamos el audio
    {
        fprintf(stderr, "failed to initialize audio!\n");
        al_uninstall_system();
        return ERROR;
    }
    if(!al_init_acodec_addon())                                                     //iniciamos los codecs de audio
    {
        fprintf(stderr, "failed to initialize audio codecs!\n");
        al_uninstall_system();
        al_uninstall_audio();

        return ERROR;
    }
    if(!al_reserve_samples(20))                                                     //reservamos la cantidad de samples
    {
        fprintf(stderr, "failed to reserve samples!\n");
        al_uninstall_system();
        al_uninstall_audio();
        return ERROR;
    }
    if(!al_init_image_addon())                                                      //inicializamos el addon de imagenes
    {
        fprintf(stderr, "Unable to start image addon \n");
        al_uninstall_system();
        al_uninstall_audio();
        return ERROR;
    }
    if(!al_init_primitives_addon())                                                 //inicializamos el addon de primitivas
    {
        fprintf(stderr, "failed to initialize primitives!\n");
        al_uninstall_system();
        al_uninstall_audio();
        al_shutdown_image_addon();
        return ERROR;
    }

/*-----------------------------------------------------------------------------------------------------------------*/

    timer = al_create_timer(1.0 / FPS);                                             //fps del display
    if(!timer)
    {
        fprintf(stderr, "failed to create timer!\n");
        al_uninstall_system();
        al_uninstall_audio();
        al_shutdown_image_addon();
        al_shutdown_primitives_addon();
        return ERROR;
    }

    event_queue = al_create_event_queue();                                          //creamos cola de eventos
    if(!event_queue)
    {
        fprintf(stderr, "failed to create event_queue!\n");
        al_uninstall_system();
        al_uninstall_audio();
        al_shutdown_image_addon();
        al_shutdown_primitives_addon();
        al_destroy_timer(timer);
        return ERROR;
    }

    display = al_create_display(SCREEN_W, SCREEN_H);                                //creamos display
    if(!display)
    {
        fprintf(stderr, "failed to create display!\n");
        al_uninstall_system();
        al_uninstall_audio();
        al_shutdown_image_addon();
        al_shutdown_primitives_addon();
        al_destroy_timer(timer);
        al_destroy_event_queue(event_queue);
        return ERROR;
    }

    boton_sfx = al_load_sample("res/boton sfx.wav");                                //cargamos el sonido del boton
    if(!boton_sfx)
    {
        fprintf(stderr, "Audio boton sfx.wav sample not loaded!\n");
        al_uninstall_system();
        al_uninstall_audio();
        al_shutdown_image_addon();
        al_shutdown_primitives_addon();
        al_destroy_timer(timer);
        al_destroy_event_queue(event_queue);
        return ERROR;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));    //registramos el display
    al_register_event_source(event_queue, al_get_timer_event_source(timer));        //registramos el timer de fps
    al_register_event_source(event_queue, al_get_keyboard_event_source());          //regristramos el teclado
    al_register_event_source(event_queue, al_get_mouse_event_source());             //registramos el mouse

    al_start_timer(timer);                                                          //iniciamos el timer de fps

    return OK;
}

/**************************************************************************
 * FUNCION: libera la memoria reservada
 * RECIBE: -
 * DEVUELVE: -
***************************************************************************/
static void destroy(void)
{
    al_uninstall_system();
    al_uninstall_audio();
    al_shutdown_image_addon();
    al_shutdown_primitives_addon();
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    al_destroy_sample(boton_sfx);
}