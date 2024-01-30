#include <stdio.h>
#include <stdbool.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h> //manejo de fonts
#include <allegro5/allegro_ttf.h> //Manejo de ttfs

#include "allconst.h"
#include "allmenu.h"

/***********************
 * PROTOTIPOS PRIVADOS *
************************/
static int ini(void);                                                       //inicializamos el menu
static void destroy(void);                                                  //liberamos lamemoria reservada

/*******************************
 * VARIABLES GLOBALES PRIVADAS *
********************************/
static ALLEGRO_FONT *font1 = NULL;                                          //puntero a la fuente 1
static ALLEGRO_FONT *font2 = NULL;                                          //puntero a la fuente 2
static ALLEGRO_FONT *fonts = NULL;                                          //puntero a la fuente START
static ALLEGRO_BITMAP *titulo = NULL;                                       //puntero a el bitmap que contiene el titulo del juego
static ALLEGRO_BITMAP *boton = NULL;                                        //puntero al bitmap que contiene el boton generico
static ALLEGRO_SAMPLE *space_invaders_ost = NULL;                           //puntero a la cancion de fondo del menu

/**************************************************************************
 * FUNCION: menu principal del juego
 * RECIBE: -
 * DEVUELVE: si hubo error
***************************************************************************/
int menu(void)
{
    int retu = OK, x = 0, y = 0;
    enum MYKEYS {KEY_ENTER};                                                //teclas a usar
    bool key[1] = {false};                                                  //Estado de teclas, true cuando esta apretada

    if(ini() == ERROR)                                                      //inicializamos el menu y verificamos si hubo error
    {
        return ERROR;
    }

    //dibujamos la pantalla (con dibujarla una vez alcanza)
/*------------------------------------------------------------------------------------------------------------------------------------*/
    al_play_sample(space_invaders_ost, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_scaled_bitmap(titulo,
            0, 0, al_get_bitmap_width(titulo), al_get_bitmap_height(titulo),
            (SCREEN_W / 2 ) - (al_get_display_width(display)/(2*2)), 0, al_get_display_width(display)/2, al_get_display_height(display)/2, 
            0);
    al_draw_scaled_bitmap(boton,
            0, 0, al_get_bitmap_width(boton), al_get_bitmap_height(boton),
            (SCREEN_W / 2) - (al_get_display_width(display)/(4*2)), (SCREEN_H / 2), al_get_display_width(display)/4, al_get_display_height(display)/7, 
            0);
    al_draw_scaled_bitmap(boton,
            0, 0, al_get_bitmap_width(boton), al_get_bitmap_height(boton),
            25, (SCREEN_H - 75), 100, 50, 
            0);
    
    al_draw_text(fonts, al_map_rgb(198, 218, 45), (SCREEN_W / 2 ), ((SCREEN_H / 2) + (SCREEN_H / 20)), ALLEGRO_ALIGN_CENTER, "START");
    al_draw_text(font2, al_map_rgb(198, 218, 45), 45, (SCREEN_H - 60), ALLEGRO_ALIGN_LEFT, "Quit");
    al_draw_text(font1, al_map_rgb(198, 218, 45), (SCREEN_W / 2), (SCREEN_H / 1.45), ALLEGRO_ALIGN_CENTER, "press start or enter");
    al_flip_display();

/*------------------------------------------------------------------------------------------------------------------------------------*/

    while(!do_exit && retu == OK)                                           //bucle principal del menu principal
    {
        ALLEGRO_EVENT state;
        if(al_get_next_event(event_queue, &state))                          //verificamos la lista de eventos
        {
            if(state.type == ALLEGRO_EVENT_TIMER)                           //por cada instancia del reloj creado verificamos
            {
                if(key[KEY_ENTER])                                          //relizamos la accion luego de presionar la tecla enter
                {
                    al_play_sample(boton_sfx, 0.25, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    retu = PLAY;
                }
            }
            else if(state.type == ALLEGRO_EVENT_MOUSE_AXES || state.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)    //verificamos evento del mouse
            {

                x = state.mouse.x;                                          //guardamos coordenadas
                y = state.mouse.y;

                if(x >= 383 && x <= 639 && y >= 383 && y <= 491)
                {
                    if(state.mouse.button & 1)                              //si se presiono play iniciamos el juego
                    {
                        al_play_sample(boton_sfx, 0.25, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        retu = PLAY;
                    }
                }
                else if(x >= 25 && x <= 125 && y >= 693 && y <= 743)
                {
                    if(state.mouse.button & 1)                              //si se presiona quit salimos del juego
                    {
                        retu = QUIT;
                    }
                }
            }
            else if(state.type == ALLEGRO_EVENT_DISPLAY_CLOSE)              //verificamos si se cerro el display
            {
                do_exit = true;
            }
            else if(state.type == ALLEGRO_EVENT_KEY_DOWN)                   //verificamos si se presiono alguna tecla
            {
                switch (state.keyboard.keycode)
                {
                     case ALLEGRO_KEY_ENTER:
                        key[KEY_ENTER] = true;
                        break;
                }
            }
            else if(state.type == ALLEGRO_EVENT_KEY_UP)                     //verificamos si se dejo de presionar alguna tecla
            {
                switch(state.keyboard.keycode)
                {
                    case ALLEGRO_KEY_ENTER:
                        key[KEY_ENTER] = false;
                        break;
                }
            }
        }
    }

    destroy();                                                              //liberamos memoria
    return retu;
}

/**************************************************************************
 * FUNCION: asigna los punteros a los archivos externos a utilizar
 * RECIBE: -
 * DEVUELVE: si hubo error
***************************************************************************/
static int ini(void)
{
    titulo = al_load_bitmap("res/titulo.png");                              //bitmap que contiene el titulo
    if(!titulo)
    {
        fprintf(stderr, "Unable to load titulo\n");
        return ERROR;
    }
    
    boton = al_load_bitmap("res/boton.png");                                //bitmaop que contiene el boton
    if(!boton)
    {
        fprintf(stderr, "Unable to load boton\n");
        al_destroy_bitmap(titulo);
        return ERROR;
    }

    font1 = al_load_ttf_font("res/Good Old DOS.ttf", 35, 0);                //creamos la fuente 1
    if(!font1)
    {
        fprintf(stderr, "Could not load 'Good Old DOS.ttf'.\n");
        al_destroy_bitmap(titulo);
        al_destroy_bitmap(boton);
        return ERROR;
    }

    font2 = al_load_ttf_font("res/Good Old DOS.ttf", 21, 0);                //creamos la fuente 2
    if(!font2)
    {
        fprintf(stderr, "Could not load 'Good Old DOS.ttf'.\n");
        al_destroy_bitmap(titulo);
        al_destroy_bitmap(boton);
        al_destroy_font(font1);
        return ERROR;
    }

    fonts = al_load_ttf_font("res/Good Old DOS.ttf", 50, 0);                //creamos la fuente START
    if(!fonts)
    {
        fprintf(stderr, "Could not load 'Good Old DOS.ttf'.\n");
        al_destroy_bitmap(titulo);
        al_destroy_bitmap(boton);
        al_destroy_font(font1);
        al_destroy_font(font2);
        return ERROR;
    }

    space_invaders_ost = al_load_sample("res/space invaders.wav");          //cargamos la musica de fondo del menu
    if(!space_invaders_ost)
    {
        fprintf(stderr, "Audio space invaders.wav sample not loaded!\n");
        al_destroy_bitmap(titulo);
        al_destroy_bitmap(boton);
        al_destroy_font(font1);
        al_destroy_font(font2);
        al_destroy_font(fonts);
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
    al_destroy_bitmap(titulo);
    al_destroy_bitmap(boton);
    al_destroy_font(font1);
    al_destroy_font(font2);
    al_destroy_font(fonts);
    al_destroy_sample(space_invaders_ost);
}