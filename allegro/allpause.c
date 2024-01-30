#include <stdio.h>
#include <stdbool.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h> //manejo de fonts
#include <allegro5/allegro_ttf.h> //Manejo de ttfs
#include <allegro5/allegro_audio.h> 
#include <allegro5/allegro_acodec.h> //Extensiones con acodec .wav, .flac, .ogg, .it, .mod, .s3m, .xm. 

#include "allconst.h"
#include "allpause.h"

/***********************
 * PROTOTIPOS PRIVADOS *
************************/
static int ini(void);                                                   //inicializa el menu de pausa
static void destroy(void);                                              //liberamos la memoria

/*******************************
 * VARIABLES GLOBALES PRIVADAS *
********************************/
static ALLEGRO_FONT *font1 = NULL;                                      //puntero a la fuente 1
static ALLEGRO_FONT *font2 = NULL;                                      //puntero a la fuente 2
static ALLEGRO_FONT *fontp = NULL;                                      //puntero a la fuete pausa
static ALLEGRO_BITMAP *boton = NULL;                                    //puntero al bitmap del boton generico

/**************************************************************************
 * FUNCION: menu de pausa del juego
 * RECIBE: -
 * DEVUELVE: si hubo error
***************************************************************************/
int pause(void)
{
    int retu = OK, x = 0, y = 0;
    
    if(ini() == ERROR)                                                  //inicializamos y verificamos si hubo error
    {
        return ERROR;
    }

    //dibujamos la pantalla de pausa (con dibujarla una vez basta)
/*------------------------------------------------------------------------------------------------------------------------------------*/

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(fontp, al_map_rgb(255, 100, 45), (SCREEN_W / 2), (SCREEN_H / 17), ALLEGRO_ALIGN_CENTER, "PAUSE");
    for(int i = 0; i < 3; i++)
    {
        al_draw_scaled_bitmap(boton,
                0, 0, al_get_bitmap_width(boton), al_get_bitmap_height(boton),
                (SCREEN_W / 2) - (al_get_display_width(display)/(4*2)), 160 + 190*i, al_get_display_width(display)/4, al_get_display_height(display)/7, 
                0);
    }
    al_draw_scaled_bitmap(boton,
            0, 0, al_get_bitmap_width(boton), al_get_bitmap_height(boton),
            25, (SCREEN_H - 75), 100, 50, 
            0);
    al_draw_text(font1, al_map_rgb(198, 218, 45), (SCREEN_W / 2), (SCREEN_H / 4), ALLEGRO_ALIGN_CENTER, "Resume");
    al_draw_text(font1, al_map_rgb(198, 218, 45), (SCREEN_W / 2), (SCREEN_H / 2), ALLEGRO_ALIGN_CENTER, "Restart");
    al_draw_text(font1, al_map_rgb(198, 218, 45), (SCREEN_W / 2), (SCREEN_H / 4) + (SCREEN_H / 2), ALLEGRO_ALIGN_CENTER, "Menu");
    al_draw_text(font2, al_map_rgb(198, 218, 45), 45, (SCREEN_H - 60), ALLEGRO_ALIGN_LEFT, "Quit");
    al_flip_display();

/*------------------------------------------------------------------------------------------------------------------------------------*/

    while(!do_exit && retu == OK)                                       //bucle principal del menu de pausa
    {
        ALLEGRO_EVENT state;
        if(al_get_next_event(event_queue, &state))                      //verificamos el evento siguiente
        {
            
            if(state.type == ALLEGRO_EVENT_MOUSE_AXES || state.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)     //verificamos evento del mouse
            {
                x = state.mouse.x;                                      //guardamos las coordenadas
                y = state.mouse.y;

                if(x >= 383 && x <= 639 && y >= 156 && y <= 270)
                {
                    if(state.mouse.button & 1)                          //si se presiono resume reanudamos el juego
                    {
                        al_play_sample(boton_sfx, 0.25, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        retu = RESUME;
                    }
                }
                else if(x >= 383 && x <= 639 && y >= 349 && y <= 457)
                {
                    if(state.mouse.button & 1)                          //si se presiono restart reiniciamos el juego
                    {
                        al_play_sample(boton_sfx, 0.25, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        retu = RESTART;
                    }
                }
                else if(x >= 383 && x <= 639 && y >= 540 && y <= 649)
                {
                    if(state.mouse.button & 1)                          //si se presiono menu volvemos al menu principal
                    {
                        al_play_sample(boton_sfx, 0.25, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        retu = MENU;
                    }
                }
                else if(x >= 25 && x <= 125 && y >= 693 && y <= 743)
                {
                    if(state.mouse.button & 1)                          //si se presiono quit cerramos el juego
                    {
                        retu = QUIT;
                    }
                }
            }
            else if(state.type == ALLEGRO_EVENT_DISPLAY_CLOSE)          //verificamos si se cerro el display
            {
                do_exit = true;
            }
            else if(state.type == ALLEGRO_EVENT_KEY_UP)                 //verificamos si se dejo de presionar una tecla
            {
                switch(state.keyboard.keycode)
                {
                    case ALLEGRO_KEY_ESCAPE:                            //al presionar esc o p podemos reanudar el juego, el evento detecta cuando se dejo de presionar para no generar conflicto
                    case ALLEGRO_KEY_P:
                        al_play_sample(boton_sfx, 0.25, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        retu = RESUME;
                        break;
                }
            }
        }
    }

    destroy();                                                          //liberamos la memoria reservada
    return retu;
}

/**************************************************************************
 * FUNCION: asigna los punteros a los archivos externos a utilizar
 * RECIBE: -
 * DEVUELVE: si hubo error
***************************************************************************/
static int ini(void)
{
    boton = al_load_bitmap("res/boton.png");                            //bitmap que contiene el boton
    if(!boton)
    {
        fprintf(stderr, "Unable to load boton\n");
        return ERROR;
    }
    
    font1 = al_load_ttf_font("res/Good Old DOS.ttf", 45, 0);            //creamos la fuente 1
    if(!font1)
    {
        fprintf(stderr, "Could not load 'Good Old DOS.ttf'.\n");
        al_destroy_bitmap(boton);
        return ERROR;
    }

    font2 = al_load_ttf_font("res/Good Old DOS.ttf", 21, 0);            //creamos la fuente 2
    if(!font2)
    {
        fprintf(stderr, "Could not load 'Good Old DOS.ttf'.\n");
        al_destroy_bitmap(boton);
        al_destroy_font(font1);
        al_destroy_font(font2);
        return ERROR;
    }

    fontp = al_load_ttf_font("res/Good Old DOS 3D.ttf", 90, 0);         //creamos la fuente pausa 
    if(!fontp)
    {
        fprintf(stderr, "Could not load 'Good Old DOS 3D.ttf'.\n");
        al_destroy_bitmap(boton);
        al_destroy_font(font1);
        al_destroy_font(font2);
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
    al_destroy_bitmap(boton);
    al_destroy_font(font1);
    al_destroy_font(font2);
    al_destroy_font(fontp);
}