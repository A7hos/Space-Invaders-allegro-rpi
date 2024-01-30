#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h> //manejo de fonts
#include <allegro5/allegro_ttf.h> //Manejo de ttfs

#include "allconst.h"
#include "allgame.h"
#include "allpause.h"
#include "../libs/score.h"
#include "../libs/vidas.h"

/*************************
 * TYPEDEF Y ESTRUCTURAS *
**************************/
typedef struct                                                  //estructura correspondiente a los aliens
{
    float x;                                                    //coordenada x1
    float y;                                                    //coordenada y1
    float i;                                                    //ancho
    float j;                                                    //alto
    bool vida;                                                  //si el alien esta vivo o no
    int pts;                                                    //puntos del alien
}alien_t;

typedef struct                                                  //estructura correspondiente a los disparos
{
    float bx;                                                    //coordenada x1
    float by;                                                    //coordenada y1
    float bi;                                                    //ancho
    float bj;                                                    //alto
    bool vida;                                                   //si el disparo esta vivo o no
}bala_t;

typedef struct                                                  //estructura correspondiente a el cañon del jugador
{
    float x;                                                    //coordenada x1
    float y;                                                    //coordenada y1
    float i;                                                    //ancho
    float j;                                                    //alto
}canon_t;

typedef struct                                                  //estructura correspondiente a los escudos/defensas
{
    float ex;                                                   //coordenada x1
    float ey;                                                   //coordenada y1
    float ei;                                                   //ancho
    float ej;                                                   //alto
    int vida;                                                   //vidas restantes de las defensas
}escudo_t;

/***********************
 * PROTOTIPOS PRIVADOS *
************************/
static int ini(void);                                           //funcion que inicializa el juego
static void destroy(void);                                      //libera la memoria reservada
static void reset(int estado);                                  //resetea algunos parametros a la hora de reiniciar o volver a jugar
static int lose(void);                                          //determina cuando se pierde
static void ptr_score_vidas(void);                              //dibuja en pantalla el score y las vidas
static void disparo(float dis_x, float dis_y, int i);           //dibuja en pantalla los disparos
static bool verifico(int i);                                    //verifica el estado de los disparos
static bool veri_escudo(int i);                                 //verifica si los disparos impactaron las barreras/escudos
static bool encuentro(float x1, float y1, float i1, float j1, float x2, float y2, float i2, float j2);      //determina cuando dos objetos chocan
static void init_aliens(void);                                  //inicializa los aliens
static void aliens(void);                                       //dibuja los aliens
static void movi_aliens(int estado);                            //mueve los aliens de posicion
static void init_barreras(void);                                //inicializa las barreras/escudos
static void barreras(void);                                     //dibuja en pantalla las barreras/escudos

/*******************************
 * VARIABLES GLOBALES PRIVADAS *
********************************/
static ALLEGRO_TIMER *tempo = NULL;                             //puntero al reloj en segundos
static ALLEGRO_BITMAP *canonbit = NULL;                         //puntero al bitmap del cañon
static ALLEGRO_BITMAP *ten_pts = NULL;                          //puntero al bitmap del alien de nivel 1
static ALLEGRO_BITMAP *two_pts = NULL;                          //puntero al bitmap del alien de nivel 2
static ALLEGRO_BITMAP *thr_pts = NULL;                          //puntero al bitmap del alien de nivel 3
static ALLEGRO_BITMAP *nodri_pts = NULL;                        //puntero al bitmap de la nave nodriza
static ALLEGRO_FONT *fontsc = NULL;                             //puntero a la fuente del score y vidas
static ALLEGRO_SAMPLE *gwyn_ost = NULL;                         //puntero a la cancion de fondo del juego
static ALLEGRO_SAMPLE *nodriza_sfx = NULL;                      //puntero al sonido de la nodriza cuando pasa
static ALLEGRO_SAMPLE *exp_nodriza_sfx = NULL;                  //puntero al sonido de la explosion de la nodriza
static ALLEGRO_SAMPLE *explosion_sfx = NULL;                    //puntero al sonido de la explosion de los aliens
static ALLEGRO_SAMPLE *laser_jugador = NULL;                    //puntero al sonido del laser del jugador
static ALLEGRO_SAMPLE *laser_aliens = NULL;                     //puntero al sonido del laser de los aliens
static ALLEGRO_SAMPLE *menos_vida_sfx = NULL;                   //puntero al sonido cuado se resta una vida
static ALLEGRO_SAMPLE_INSTANCE *nodriza_instance = NULL;        //puntero a la instancia de sonido de la nodriza

static alien_t mali[CANT_ALIENS+1];                             //arreglo donde guardaremos la informacion de los aliens y la nodriza
static escudo_t escudo[CANT_ESCU];                              //arreglo donde guardaremos la informacion de los escudos
static bala_t bala[2];                                          //bala[0] representa el disparo del jugador, mientras que bala[1] el de los aliens
static canon_t canon;                                           //variable donde guardaremos la informacion del cañon del jugador
static int cant_aliens = CANT_ALIENS;                           //variable donde guardaremos los aliens restantes
static int segundos;                                            //contador en segundos desde el inicio del programa

/**************************************************************************
 * FUNCION: se encarga de manejar el juego
 * RECIBE: -
 * DEVUELVE: estado correspondiente a las acciones WIN, LOSE, QUIT, ERROR, etc
***************************************************************************/
int game(void)
{
    int retu = OK, alien = 0, p;
    enum MYKEYS {KEY_SPACE, KEY_A, KEY_D, KEY_LEFT, KEY_RIGHT, KEY_ESCAPE, KEY_P};  //teclas a usar
    bool key_pressed[8] = {false, false, false, false, false, false, false, false}; //Estado de teclas, true cuando esta apretada
    bool redraw = false;                                        //indica cuando debemos redibujar
    float disp_x = 0, disp_y = 0;                               //variables donde temporalmente guardaremos la posicion del disparo
    float dispa_x = 0, dispa_y = 0;                             //variables donde temporalmente guardaremos la posicion del disparo de los aliens
    
    bala[0].vida  = false;                                      //inicializamos los disparos en false para asegurar que no halla errores
    bala[1].vida  = false;
    
    canon.x = SCREEN_W / 2.0 - CANON_SIZE_X / 2.0;              //posicion inicial del cañon
    canon.y = SCREEN_H - CANON_SIZE_Y * 2.0;
    canon.i = CANON_SIZE_X;                                     //guardamos el ancho
    canon.j = CANON_SIZE_Y;                                     //y el alto

    srand(time(NULL));						                    //inicializamos semilla

    if(ini() == ERROR)                                          //inicializamos los recursos del juego y verificamos posibles errorres
    {
        retu = ERROR;
    }
    else                                                        //si todo esta ok
    {
        al_register_event_source(event_queue, al_get_timer_event_source(tempo));    //registramos el timer en segundos reales desde que inicia la partida
        al_start_timer(tempo);                                                      //iniciamos el timer

        al_play_sample(gwyn_ost, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);       //iniciamos la musica de fondo

        init_aliens();                                          //inicializamos los aliens
        init_barreras();                                        //inicializamos las barreras/escudos
    }

    while(!do_exit && retu == OK)                               //bucle principal del juego
    {
        ALLEGRO_EVENT ev;
        if(al_get_next_event(event_queue, &ev))                 //tomamos el siguiente evento de la cola de eventos
        {
            if(ev.type == ALLEGRO_EVENT_TIMER)                  //cuando el evento es el timer
            {
                if(ev.timer.source == timer)                    //si el evento es el timer de fps
                {
                    redraw = true;                              //activamos el redibujar y relizamos la accion correspondiente a las teclas presionadas debajo
                
                    if((key_pressed[KEY_A] || key_pressed[KEY_LEFT]) && canon.x >= MOVE_RATE)
                    {
                        canon.x -= MOVE_RATE;                   //mover cañon a la izquierda
                    }
                    if((key_pressed[KEY_D] || key_pressed[KEY_RIGHT]) && canon.x <= SCREEN_W - CANON_SIZE_X - MOVE_RATE)
                    {
                        canon.x += MOVE_RATE;                   //mover el cañon a la derecha
                    }
                }
                if(ev.timer.source == tempo)                    //cuando el evento corresponde al timer tempo en segundos
                {
                    segundos++;                                 //incrementamos los segundos trascurridos
                }
                
            }
            else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)     //verificamos si se cerro el display
            {
                do_exit = true;
            }
            else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)          //caundo se presiona una tecla
            {
                switch(ev.keyboard.keycode)
                {
                    case ALLEGRO_KEY_A:
                        key_pressed[KEY_A] = true;
                        break;

                    case ALLEGRO_KEY_D:
                        key_pressed[KEY_D] = true;
                        break;

                    case ALLEGRO_KEY_LEFT:
                        key_pressed[KEY_LEFT] = true;
                        break;

                    case ALLEGRO_KEY_RIGHT:
                        key_pressed[KEY_RIGHT] = true;
                        break;

                    case ALLEGRO_KEY_SPACE:                     //si fue el spacebar y el disparo no existe lo creamos
                        if(bala[0].vida == false)
                        {
                            al_play_sample(laser_jugador, 0.3, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            disp_x = canon.x;
                            disp_y = canon.y;
                            bala[0].vida  = true;
                        }
                        break;
                }
            }
            else if(ev.type == ALLEGRO_EVENT_KEY_UP)            //si se dejo de presionar una tecla
            {
                switch(ev.keyboard.keycode)
                {
                    case ALLEGRO_KEY_A:
                        key_pressed[KEY_A] = false;
                        break;

                    case ALLEGRO_KEY_D:
                        key_pressed[KEY_D] = false;
                        break;

                    case ALLEGRO_KEY_LEFT:
                        key_pressed[KEY_LEFT] = false;
                        break;

                    case ALLEGRO_KEY_RIGHT:
                        key_pressed[KEY_RIGHT] = false;
                        break;
                    case ALLEGRO_KEY_ESCAPE:                    //verificamos si se pesiona esc o p para la pausa
                    case ALLEGRO_KEY_P:
                        al_stop_sample_instance(nodriza_instance);
                        al_play_sample(boton_sfx, 0.25, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        p = pause();                            //dependiendo del estado seleccionado en pausa
                        if(p == ERROR)                          //salimos si hubo error
                        {
                            retu = ERROR;
                        }
                        else if(p == RESTART)                   //reiniciamos la partida
                        {
                            reset(RESTART);
                            scor = 0;    
                            retu = RESTART;
                        }
                        else if(p == MENU)                      //volvemos al menu
                        {
                            reset(MENU);
                            scor = 0; 
                            retu = MENU;
                        }
                        else if(p == QUIT)                      //salimos del juego
                        {
                            do_exit = true;
                        }
                        break;
                }
            }
        }
        if(redraw && al_is_event_queue_empty(event_queue))      //redibujamos el display
        {
            redraw = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));             //ponemos todo en negro
            
            barreras();                                         //dibujamos las barreras
            aliens();                                           //dibujamos los aliens y el cañon
            al_draw_scaled_bitmap(canonbit,
                    0, 0, al_get_bitmap_width(canonbit), al_get_bitmap_height(canonbit),
                    canon.x, canon.y, canon.i, canon.j,
                    0);

            if(bala[0].vida  == true)                           //en caso de que el disparo del jugador exista lo desplazamos y verificamos
            {   
                disparo(disp_x, disp_y, 0);                     //dibujamos el disparo
                bala[0].vida  = !verifico(0);                   //verificamos
                disp_y -= 10;                                   //desplazamos
            }

            if(bala[1].vida  == true)                           //en caso de que el disparo de los aliens existalo desplazamos y verificamos
            {   
                disparo(dispa_x, dispa_y, 1);                   //dibuajmos el disparo enemigo
                bala[1].vida  = !verifico(1);                   //verificamos
                dispa_y += 5;                                   //desplazamos
            }
            else if((bala[1].vida == false) && (segundos != 0) && (cant_aliens != 0))   //sino existe
            {
                do
                {
                    alien = rand() % 55;                        //elejimos un alien aleatorio
                }
                while(mali[alien].vida != true);                //nos aseguramos que el alien eljido exista
                
                al_play_sample(laser_aliens, 0.125, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                bala[1].vida = true;                            //creamos el disparo
                dispa_x = mali[alien].x;                        //guardamos las coordenadas del disparo en x
                dispa_y = (mali[alien].y + mali[alien].j);      //en y
            }

            ptr_score_vidas();                                  //mostramos en pantalla el score y las vidas

            if(cant_aliens == 0)                                //verificamos si se gano
            {
                vidas(ADD_VIDA);                                //agregamos una vida al jugador
                reset(WIN);                                     //reseteamos variables necesarias
                retu = WIN;
            }
            else if(lose() == LOSE)                             //verificamos si se perdio
            {
                reset(LOSE);                                    //reseteamos variables necesarias
                retu = LOSE;
            }
            
            al_flip_display();                                  //actualizamos el display
        }
    }

    destroy();                                                  //liberamos la memoria al salir
    return retu;                                                //devolvemos el estado correspondiente
}

/*------------------------------------------------------------------------------------------------------------------------------------*/

/**************************************************************************
 * FUNCION: asigna los punteros a los archivos externos a utilizar
 * RECIBE: -
 * DEVUELVE: si hubo error
***************************************************************************/
static int ini(void)
{
    tempo = al_create_timer(1.0);                               //creamos el timer en segundos
    if(!tempo)
    {
        fprintf(stderr, "failed to create timer!\n");
        return ERROR;
    }

    canonbit = al_load_bitmap("res/canon.png");                 //cargamos el bitmap con la imagen del cañon
    if(!canonbit)
    {
        fprintf(stderr, "Unable to load canon\n");
        al_destroy_timer(tempo);
        return ERROR;
    }

    ten_pts = al_load_bitmap("res/10pts.png");                 //cargamos el bitmap con la imagen del alien nivel 1
    if(!ten_pts)
    {
        fprintf(stderr, "Unable to load 10pts\n");
        al_destroy_timer(tempo);
        al_destroy_bitmap(canonbit);
        return ERROR;
    }

    two_pts = al_load_bitmap("res/20pts.png");                 //cargamos el bitmap con la imagen del alien nivel 2
    if(!two_pts)
    {
        fprintf(stderr, "Unable to load 20pts\n");
        al_destroy_timer(tempo);
        al_destroy_bitmap(canonbit);
        al_destroy_bitmap(ten_pts);
        return ERROR;
    }

    thr_pts = al_load_bitmap("res/30pts.png");                 //cargamos el bitmap con la imagen del alien nivel 3
    if(!thr_pts)
    {
        fprintf(stderr, "Unable to load 30pts\n");
        al_destroy_timer(tempo);
        al_destroy_bitmap(canonbit);
        al_destroy_bitmap(ten_pts);
        al_destroy_bitmap(two_pts);
        return ERROR;
    }

    nodri_pts = al_load_bitmap("res/nodri.png");                //cargamos el bitmap con la imagen de la nodriza
    if(!nodri_pts)
    {
        fprintf(stderr, "Unable to load nodri\n");
        al_destroy_timer(tempo);
        al_destroy_bitmap(canonbit);
        al_destroy_bitmap(ten_pts);
        al_destroy_bitmap(two_pts);
        al_destroy_bitmap(thr_pts);
        return ERROR;
    }

    fontsc = al_load_ttf_font("res/Good Old DOS.ttf", 25, 0);   //creamos la fuente del score y vidas
    if(!fontsc)
    {
        fprintf(stderr, "Could not load 'Good Old DOS.ttf'.\n");
        al_destroy_timer(tempo);
        al_destroy_bitmap(canonbit);
        al_destroy_bitmap(ten_pts);
        al_destroy_bitmap(two_pts);
        al_destroy_bitmap(thr_pts);
        al_destroy_bitmap(nodri_pts);
        return ERROR;
    }

    gwyn_ost = al_load_sample("res/back gwyn.wav");             //cargamos la musica de fondo del juego
    if(!gwyn_ost)
    {
        fprintf(stderr, "Audio back gwyn.wav sample not loaded!\n");
        al_destroy_timer(tempo);
        al_destroy_bitmap(canonbit);
        al_destroy_bitmap(ten_pts);
        al_destroy_bitmap(two_pts);
        al_destroy_bitmap(thr_pts);
        al_destroy_bitmap(nodri_pts);
        al_destroy_font(fontsc);
        return ERROR;
    }

    nodriza_sfx = al_load_sample("res/nodriza.wav");            //cargamos el sonido de la nodriza
    if(!nodriza_sfx)
    {
        fprintf(stderr, "Audio nodriza.wav sample not loaded!\n");
        al_destroy_timer(tempo);
        al_destroy_bitmap(canonbit);
        al_destroy_bitmap(ten_pts);
        al_destroy_bitmap(two_pts);
        al_destroy_bitmap(thr_pts);
        al_destroy_bitmap(nodri_pts);
        al_destroy_font(fontsc);
        al_destroy_sample(gwyn_ost);
        al_destroy_sample(nodriza_sfx);
        return ERROR;
    }

    exp_nodriza_sfx = al_load_sample("res/explosion nodriza.wav");  //cargamos el sonido de la explosion de la nodriza
    if(!exp_nodriza_sfx)
    {
        fprintf(stderr, "Audio explosion nodriza.wav sample not loaded!\n");
        al_destroy_timer(tempo);
        al_destroy_bitmap(canonbit);
        al_destroy_bitmap(ten_pts);
        al_destroy_bitmap(two_pts);
        al_destroy_bitmap(thr_pts);
        al_destroy_bitmap(nodri_pts);
        al_destroy_font(fontsc);
        al_destroy_sample(gwyn_ost);
        al_destroy_sample(nodriza_sfx);
        return ERROR;
    }

    explosion_sfx = al_load_sample("res/explosion.wav");        //cargamos el sonido de explosion
    if(!explosion_sfx)
    {
        fprintf(stderr, "Audio explosion.wav sample not loaded!\n");
        al_destroy_timer(tempo);
        al_destroy_bitmap(canonbit);
        al_destroy_bitmap(ten_pts);
        al_destroy_bitmap(two_pts);
        al_destroy_bitmap(thr_pts);
        al_destroy_bitmap(nodri_pts);
        al_destroy_font(fontsc);
        al_destroy_sample(gwyn_ost);
        al_destroy_sample(nodriza_sfx);
        al_destroy_sample(exp_nodriza_sfx);
        return ERROR;
    }

    laser_jugador = al_load_sample("res/laser jugador.wav");    //cargamos el sonido del laser del jugador
    if(!laser_jugador)
    {
        fprintf(stderr, "Audio laser jugador.wav sample not loaded!\n");
        al_destroy_timer(tempo);
        al_destroy_bitmap(canonbit);
        al_destroy_bitmap(ten_pts);
        al_destroy_bitmap(two_pts);
        al_destroy_bitmap(thr_pts);
        al_destroy_bitmap(nodri_pts);
        al_destroy_font(fontsc);
        al_destroy_sample(gwyn_ost);
        al_destroy_sample(nodriza_sfx);
        al_destroy_sample(exp_nodriza_sfx);
        al_destroy_sample(explosion_sfx);
        return ERROR;
    }

    laser_aliens = al_load_sample("res/laser aliens.wav");      //cargamos el sonido del laser de los aliens
    if(!laser_aliens)
    {
        fprintf(stderr, "Audio laser aliens.wav sample not loaded!\n");
        al_destroy_timer(tempo);
        al_destroy_bitmap(canonbit);
        al_destroy_bitmap(ten_pts);
        al_destroy_bitmap(two_pts);
        al_destroy_bitmap(thr_pts);
        al_destroy_bitmap(nodri_pts);
        al_destroy_font(fontsc);
        al_destroy_sample(gwyn_ost);
        al_destroy_sample(nodriza_sfx);
        al_destroy_sample(exp_nodriza_sfx);
        al_destroy_sample(explosion_sfx);
        al_destroy_sample(laser_jugador);
        return ERROR;
    }

    menos_vida_sfx = al_load_sample("res/daño.wav");            //cargamos el sonido de restar vidas
    if(!menos_vida_sfx)
    {
        fprintf(stderr, "Audio daño.wav sample not loaded!\n");
        al_destroy_timer(tempo);
        al_destroy_bitmap(canonbit);
        al_destroy_bitmap(ten_pts);
        al_destroy_bitmap(two_pts);
        al_destroy_bitmap(thr_pts);
        al_destroy_bitmap(nodri_pts);
        al_destroy_font(fontsc);
        al_destroy_sample(gwyn_ost);
        al_destroy_sample(nodriza_sfx);
        al_destroy_sample(exp_nodriza_sfx);
        al_destroy_sample(explosion_sfx);
        al_destroy_sample(laser_jugador);
        al_destroy_sample(laser_aliens);
        return ERROR;
    }

    nodriza_instance = al_create_sample_instance(nodriza_sfx);  //creamos la instancia para el sonido de la nodriza
    if(!nodriza_instance)
    {
        fprintf(stderr, "Audio nodriza_instance could not be loaded!\n");
        al_destroy_timer(tempo);
        al_destroy_bitmap(canonbit);
        al_destroy_bitmap(ten_pts);
        al_destroy_bitmap(two_pts);
        al_destroy_bitmap(thr_pts);
        al_destroy_bitmap(nodri_pts);
        al_destroy_font(fontsc);
        al_destroy_sample(gwyn_ost);
        al_destroy_sample(nodriza_sfx);
        al_destroy_sample(exp_nodriza_sfx);
        al_destroy_sample(explosion_sfx);
        al_destroy_sample(laser_jugador);
        al_destroy_sample(laser_aliens);
        al_destroy_sample(menos_vida_sfx);
        return ERROR;
    }
    al_attach_sample_instance_to_mixer(nodriza_instance, al_get_default_mixer());   //acoplamos la instancia

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
    al_destroy_bitmap(canonbit);
    al_destroy_bitmap(ten_pts);
    al_destroy_bitmap(two_pts);
    al_destroy_bitmap(thr_pts);
    al_destroy_bitmap(nodri_pts);
    al_destroy_font(fontsc);
    al_destroy_sample(gwyn_ost);
    al_destroy_sample(nodriza_sfx);
    al_destroy_sample(exp_nodriza_sfx);
    al_destroy_sample(explosion_sfx);
    al_destroy_sample(laser_jugador);
    al_destroy_sample(laser_aliens);
    al_destroy_sample(menos_vida_sfx);
    al_destroy_sample_instance(nodriza_instance);
}

/*------------------------------------------------------------------------------------------------------------------------------------*/

/**************************************************************************
 * FUNCION: resetea algunos paramentros a la hora de rejugar
 * RECIBE: -
 * DEVUELVE: -
***************************************************************************/
static void reset(int estado)
{
    if(estado != WIN)
    {
        vidas(RESET_VIDA);          //y en caso de qu no se halla ganado reseteamos las vidas
    }
    segundos = 0;                   //volvemos los segundos a 0
    cant_aliens = CANT_ALIENS;      //reseteamos la cantidad de aliens
    movi_aliens(estado);            //reseteamos el movimiento de los aliens
}

/**************************************************************************
 * FUNCION: determina si se perdio
 * RECIBE: -
 * DEVUELVE: true si se perdio, false si no
***************************************************************************/
static int lose(void)
{
    int i, retu = OK;

    for(i = 0; i < 55; i++)                                             //verificamos si algun alien que este vivo llego donde esta el cañon
    {
        if(mali[i].vida == true)
        {
            if((mali[i].y + mali[i].j) > (SCREEN_H - CANON_SIZE_Y*2))   //si llega = LOSE
            {
                retu = LOSE;
            }
        }
    }
    if(vidas(STATUS_VIDA) == 0)                                         //verificamos las vidas restantes
    {
        retu = LOSE;
    }

    return retu;
}

/**************************************************************************
 * FUNCION: dibuja el score y las vidas en el display
 * RECIBE: -
 * DEVUELVE: -
***************************************************************************/
static void ptr_score_vidas(void)
{
    char pun[20];                                                       //usamos el arreglo para guardar un string y lugo pasar a la funcion de texto
    sprintf(pun, "score: %d", scor);
    al_draw_text(fontsc, al_map_rgb(25, 255, 25), 5, 5, ALLEGRO_ALIGN_LEFT, pun);                   //puntaje
    sprintf(pun, "lives: %d", vidas(STATUS_VIDA));
    al_draw_text(fontsc, al_map_rgb(25, 255, 25), (SCREEN_W - 200), 5, ALLEGRO_ALIGN_LEFT, pun);    //vidas
    al_draw_filled_rectangle(0, 34, SCREEN_W, 35, al_map_rgb(25, 255, 25));                         //linea separadora
}

/*------------------------------------------------------------------------------------------------------------------------------------*/

/**************************************************************************
 * FUNCION: verifica el estado de los disparos
 * RECIBE: el disparo a verificar [0] disparo del jugador, [1] disparo aliens
 * DEVUELVE: true si impacto con algo, false si no lo hizo
***************************************************************************/
static bool verifico(int i)
{
    int cont = 0;
    bool valor = false;
    if(i == 0)                                                      //verificamos el disparo del jugador
    {
        while((cont < 56) && (valor == false))                      //vemos si impacto con algun alien
        {
            if(mali[cont].vida == true)                             //en valor erificamos si impacto pasando las coordenadas
            {
                valor = encuentro(mali[cont].x, mali[cont].y, mali[cont].i, mali[cont].j, bala[i].bx, bala[i].by, bala[i].bi, bala[i].bj);
            }
            if(valor == true)                                       //si impacto
            {
                if(cont == 55)                                      //si fue con la nodriza la hacemos explotar
                {
                    al_stop_sample_instance(nodriza_instance);                                      //frenamos el ruido de la nodriza
                    al_play_sample(exp_nodriza_sfx, 0.25, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);   //sonido explosion
                    mali[cont].vida = false;
                    score(mali[cont].pts);                          //sumamos el score
                }
                else                                                //si fue con cualquier otro alien
                {
                    al_play_sample(explosion_sfx, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);      //sonido explosion
                    mali[cont].vida = false;                        //muere
                    score(mali[cont].pts);                          //sumamos el score correspondiente
                    cant_aliens--;                                  //descontamos la cantidad de aliens
                }
            }
            cont++;
        }
    }
    else                                                            //verificamos el disparo de los aliens, si impacto con el jugador y si lo hizo
    {
        if((valor == false) && (encuentro(canon.x, canon.y, canon.i, canon.j, bala[i].bx, bala[i].by, bala[i].bi, bala[i].bj) == true))
        {
            al_play_sample(menos_vida_sfx, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);             //sonido de menos vidas
            vidas(SUBS_VIDA);                                       //restamos las vidas
            valor = true;                                           //devolvemos true indicaqando que impacto
        }
    }
    //en caso de que no halla impactado ningun disparo anteriormente
    if((valor == false) && (veri_escudo(i) == true))                                                //verificamos si impacto con los escudos/defensas
    {
        valor = true;
    }
    if((valor == false) && ((bala[i].by < 35) || (bala[i].by + bala[i].bj > SCREEN_H)))             //verificamos si se fue de los limites de la pantalla
    {
        valor = true;
    }
    if((valor == false) && (bala[0].vida == true) && (bala[1].vida == true))                        //verificamos si los disparos chocaron entre si
    {
        valor = encuentro(bala[1].bx, bala[1].by, bala[1].bi, bala[1].bj, bala[0].bx, bala[0].by, bala[0].bi, bala[0].bj);
        if(valor == true)
        {
            bala[0].vida = false;
            bala[1].vida = false;
        }
    }
    return valor;
}

/**************************************************************************
 * FUNCION: verifica si algun disparo impacto el escudo
 * RECIBE: el disparo a verificar [0] disparo del jugador, [1] disparo aliens
 * DEVUELVE: true si impacto con algo, false si no lo hizo
***************************************************************************/
static bool veri_escudo(int i)
{
    int cont = 0;
    bool retu = false;

    while((cont < CANT_ESCU) && (retu == false))                //en el bucle analizamos si impacto
    {
            
        if(escudo[cont].vida != 0)                              //verificamos solo escudos disponibles
        {
           retu = encuentro(escudo[cont].ex, escudo[cont].ey, escudo[cont].ei, escudo[cont].ej,  bala[i].bx, bala[i].by, bala[i].bi, bala[i].bj);
        }
        if(retu == true)                                        //si impacto restamos vidas al escudo
        {
            escudo[cont].vida--;
        }
        cont++;
    }
    
    return retu;
}

/**************************************************************************
 * FUNCION: determina cuando dos objetos chocan
 * RECIBE: las coordenadas x, y, ancho y alto de ambos objetos
 * DEVUELVE: true si impacto con algo, false si no lo hizo
***************************************************************************/
static bool encuentro(float x1, float y1, float i1, float j1, float x2, float y2, float i2, float j2)
{
    return ((x1 < x2 + i2) && (x2 < x1 + i1) && (y1 < y2 + j2) && (y2 < y1 + j1));
}

/**************************************************************************
 * FUNCION: determina cuando dos objetos chocan
 * RECIBE: las coordenadas x, y del disparo y de quien es el disparo [0] jugador, [1] aliens
 * DEVUELVE: -
***************************************************************************/
static void disparo(float dis_x, float dis_y, int i)
{
    if(i == 0)                                              //dibujamos el disparo del jugador
    {
        al_draw_filled_rectangle((dis_x + CANON_SIZE_X/2 - 2.5), (dis_y - 22), (dis_x + CANON_SIZE_X/2 + 2.5), (dis_y - 2), al_color_name("darkred"));
        bala[i].bx = dis_x + CANON_SIZE_X/2 - 2.5;          //x
        bala[i].by = dis_y - 22;                            //y
        bala[i].bi = 5;                                     //ancho
    }
    else                                                    //dibujamos el disparo de los aliens
    {
        al_draw_filled_rectangle((dis_x + ALI3_X/2 - 5), (dis_y + 2), (dis_x + ALI3_X/2 + 2), (dis_y + 25), al_color_name("yellow"));
        bala[i].bx = dis_x + ALI3_X/2 - 5;                  //x
        bala[i].by = dis_y + 2;                             //y
        bala[i].bi = 10;                                    //ancho
    }
    bala[i].bj = 20;                                        //alto
}

/*------------------------------------------------------------------------------------------------------------------------------------*/

/**************************************************************************
 * FUNCION: inicializamos los aliens
 * RECIBE: -
 * DEVUELVE: -
***************************************************************************/
static void init_aliens(void)
{
    int i, j, l=0;
    int posx = HORIZ*2;                                 //posicion x
    int pos3y = VERTI*2;                                //posicion y alien nivel 3
    int pos2y = VERTI*3;                                //posicion y alien nivel 2
    int pos1y = VERTI*5;                                //posicion y alien nivel 1
    int posnx = SCREEN_W, posny = VERTI*1;              //posicion nodriza

    for(i = 0; i < 11; i++, posx += HORIZ)              //posicionamos los aliens nivel 3
    {
        mali[l].x = posx;
        mali[l].y = pos3y;
        mali[l].i = ALI3_X;
        mali[l].j = ALI3_Y;
        mali[l].vida = true;                            //asignamos valor de vida true
        mali[l].pts = 30;                               //puntaje del alien
        l++;
    }
    posx = HORIZ*2;
    for(j = 0; j < 2; j++, pos2y += VERTI)              //posicionamos los aliens nivel 2
    {
        for(i = 0; i < 11; i++, posx += HORIZ)
        {
            mali[l].x = posx;
            mali[l].y = pos2y;
            mali[l].i = ALI2_X;
            mali[l].j = ALI2_Y;
            mali[l].vida = true;                        //asignamos valor de vida true
            mali[l].pts = 20;                           //puntaje del alien
            l++;
        }
        posx = HORIZ*2;
    }
    for(j = 0; j < 2; j++, pos1y += VERTI)              //posicionamos los aliens nivel 1
    {
        for(i = 0; i < 11; i++, posx += HORIZ)
        {
            mali[l].x = posx;
            mali[l].y = pos1y;
            mali[l].i = ALI1_X;
            mali[l].j = ALI1_Y;
            mali[l].vida = true;                        //asignamos valor de vida true
            mali[l].pts = 10;                           //puntaje del alien
            l++;
        }
        posx = HORIZ*2;
    }
    
    mali[l].x = posnx;                                  //posicionamos de la nodriza
    mali[l].y = posny;
    mali[l].i = ALIN_X;
    mali[l].j = ALIN_Y;
    mali[l].vida = true;
    mali[l].pts = 40;                                   //codigo del puntaje (NO ES EL PUNTAJE, ya que debe ser aleatorio)
    
}

/**************************************************************************
 * FUNCION: dibujamos los aliens
 * RECIBE: -
 * DEVUELVE: -
***************************************************************************/
static void aliens(void)
{
    int i, j, l=0;

    for(i = 0; i < 11; i++)                             //aliens de nivel 3
    {
        if(mali[l].vida == true)
        {
           al_draw_scaled_bitmap(thr_pts,
                    0, 0, al_get_bitmap_width(thr_pts), al_get_bitmap_height(thr_pts),
                    mali[l].x, mali[l].y, mali[l].i, mali[l].j, 
                    0);
        }
        l++;
    }
    for(j = 0; j < 2; j++)                              //aliens de nivel 2
    {
        for(i = 0; i < 11; i++)
        {
            if(mali[l].vida == true)
            {
                al_draw_scaled_bitmap(two_pts,
                        0, 0, al_get_bitmap_width(two_pts), al_get_bitmap_height(two_pts),
                        mali[l].x, mali[l].y, mali[l].i, mali[l].j, 
                        0);
            }
            l++;
        }
    }
    for(j = 0; j < 2; j++)                              //aliens de nivel 1
    {
        for(i = 0; i < 11; i++)
        {
            if(mali[l].vida == true)
            {
                al_draw_scaled_bitmap(ten_pts,
                        0, 0, al_get_bitmap_width(ten_pts), al_get_bitmap_height(ten_pts),
                        mali[l].x, mali[l].y, mali[l].i, mali[l].j, 
                        0);
            }
            l++;
        }
    }
    if(mali[l].vida == true)                            //nave nodriza
    {
        al_draw_scaled_bitmap(nodri_pts,
                        0, 0, al_get_bitmap_width(nodri_pts), al_get_bitmap_height(nodri_pts),
                        mali[l].x, mali[l].y, mali[l].i, mali[l].j, 
                        0);
    }
    movi_aliens(OK);                                    //llamamos a movi_aliens para que se encargue del movimiento
}

/**************************************************************************
 * FUNCION: se encarga del movimiento de los aliens
 * RECIBE: el estado OK funciona normal, caulquier otro debe reiniciar el sentido del movimiento
 * DEVUELVE: -
***************************************************************************/
static void movi_aliens(int estado)
{
    static int flag, flag_y, flag_n;                            //variables estaticas para mantener su estado
    int i;
    float veloci = 1+(MOV_ALIENS / (cant_aliens + 1))/2;        //velocidad de movimiento de los aliens

    if((estado == RESTART) || (estado == MENU)  || (estado == LOSE) || (estado == WIN))     //reseteamos movimietno
    {
        flag = 0;
        flag_y = 0;
        flag_n = 0;
    }
    else                                                        //estado de funcionamiento normal
    {
        if(flag == 1)                                           //si flag es 1 verificamos movimietno hacia la izquierda
        {
            for(i = 0; i < 55; i++)                             //verificamos si algun alien choco con el borde izquierdo
            {
                if(mali[i].vida == true)                        //solo si estan vivos
                {
                    if(mali[i].x < 0)
                    {
                        flag = 2;
                        flag_y = 0;
                    }
                }
            }
        }
        else if(flag == 0)                                      //si flag es 0 verificamos movimietno hacia la derecha
        {
            for(i = 0; i < 55; i++)                             //verificamos si algun alien choco con el borde derecho
            {
                if(mali[i].vida == true)                        //solo si estan vivos
                {
                    if((mali[i].x + mali[i].i) > SCREEN_W)
                    {
                        flag = 2;
                        flag_y = 1;
                    }
                }
            }
        }

        //dependiendo el estado de el flag dado por la primera parte de la funcion decidimos el sentido del movimiento
        if(flag == 1)
        {
            for(i = 0; i < 55; i++)
            {
                mali[i].x -= veloci;                            //movemos hacia la izquierda
            }
        }
        else if(flag == 0)
        {
            for(i = 0; i < 55; i++)
            {
                mali[i].x += veloci;                            //movemos hacia la derecha
            }
        }
        else if(flag == 2)
        {
            for(i = 0; i < 55; i++)
            {
                mali[i].y += VERTI/3;                           //movemos hacia abajo
            }
            flag = flag_y;
        }

        //movimiento de la nodriza
        if((mali[55].x + mali[55].i) > 0)                       //movemos la nodriza hasta el borde siempre a no ser que se destruya
        {
            if(((segundos % 15 == 0) && (segundos != 0)) || (flag_n == 1))      //cada 15seg hacemoss pasar la nave
            {
                if(segundos % 15 == 0)                          //reproducimos el sonido especial al aparecer en pantalla
                {
                    al_play_sample_instance(nodriza_instance);
                }
                mali[55].x -= 10;                               //movemos hacia la izquierda
                flag_n = 1;                                     //flag para mantener el movimiento
            }
        }
        else                                                    //una vez que llega al limite de la pantalla
        {
            al_stop_sample_instance(nodriza_instance);          //frenamos el audio
            flag_n = 0;                                         //frenamos el movimiento
            mali[55].vida = true;                               //reseteamos la vida en caso de que halla sido destruida
            mali[55].x = SCREEN_W;                              //reseteamos la hubicacion
        }
    }
}

/*------------------------------------------------------------------------------------------------------------------------------------*/

/**************************************************************************
 * FUNCION: inicializamos las defensas/barreras
 * RECIBE: -
 * DEVUELVE: -
***************************************************************************/
static void init_barreras(void)
{
    int i,j,k,cont = 0;
    int posx = ESCU_X;                                  //posicion inicial x
    int posy = ESCU_Y;                                  //posicion inicial y
    int posi = 25;                                      //ancho
    int posj = 25;                                      //alto
    
    for(i = 1; i < 5; i++)                              //recorremos una barrera a la vez
    {
        posy = ESCU_Y;
        for(j = 0; j < 3; j++)                          //recorremos lass columnas
        {
            posx = ESCU_X*i;
            for(k = 0; k < 3; k++)                      //recorremos las filas
            {
                escudo[cont].ex = posx;                 //cordenada x
                escudo[cont].ey = posy;                 //cordenada y
                escudo[cont].ei = posi;                 //ancho
                escudo[cont].ej = posj;                 //alto
                escudo[cont].vida = 4;                  //vidas

                cont++;
                posx += posi;
            }
            posy += posj;
        }
    }
}

/**************************************************************************
 * FUNCION: dibujamos las defensas/barreras
 * RECIBE: -
 * DEVUELVE: -
***************************************************************************/
static void barreras(void)
{
    int cont;

    for(cont = 0; cont < CANT_ESCU; cont++)             //dibujamos las defensas una a una
    {
        switch(escudo[cont].vida)                       //dependiendo la vida que tengan las pintamos de un color diferente
        {
            case 4:                                     //verde
                al_draw_filled_rectangle(escudo[cont].ex, escudo[cont].ey, escudo[cont].ex + escudo[cont].ei, escudo[cont].ey + escudo[cont].ej, al_color_name("green"));
                break;
            case 3:                                     //amarillo
                al_draw_filled_rectangle(escudo[cont].ex, escudo[cont].ey, escudo[cont].ex + escudo[cont].ei, escudo[cont].ey + escudo[cont].ej, al_color_name("yellow"));
                break;
            case 2:                                     //naranja
                al_draw_filled_rectangle(escudo[cont].ex, escudo[cont].ey, escudo[cont].ex + escudo[cont].ei, escudo[cont].ey + escudo[cont].ej, al_color_name("orange"));
                break;
            case 1:                                     //rojo
                al_draw_filled_rectangle(escudo[cont].ex, escudo[cont].ey, escudo[cont].ex + escudo[cont].ei, escudo[cont].ey + escudo[cont].ej, al_color_name("red"));
                break;
        }
    }
}

/*------------------------------------------------------------------------------------------------------------------------------------*/