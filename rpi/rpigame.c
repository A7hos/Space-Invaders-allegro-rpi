#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include "/home/pi/libs/joydisp/disdrv.h"
#include "/home/pi/libs/joydisp/joydrv.h"

#include "rpiconst.h"
#include "rpigame.h"
#include "rpipause.h"
#include "../libs/libsconst.h"
#include "../libs/mundo.h"
#include "../libs/movimiento.h"
#include "../libs/dispara.h"
#include "../libs/vidas.h"
#include "../libs/score.h"

/*********************************
 * DEFINIMOS CONSTANTES PRIVADAS *
**********************************/
#define DESTROY_N 1
#define VOID 0

/*************************
 * TYPEDEF Y ESTRUCTURAS *
**************************/
typedef struct													//estructura correspondiente al cañon/nave del jugador
{
	dcoord_t c_pos[3];
}canon_t;

/***********************
 * PROTOTIPOS PRIVADOS *
************************/
static void can_init(void);										//inicializamos el cañon con los datos del mundo
static void copy(void);											//copia los datos de mundo a mundo_buffer sin los aliens
static int lose(void);											//determina si se perdio y devuelve LOSE o OK
static void print_disp(void);									//mostramos los datos del mundo en el display
static void print_vidas(void);									//mostramos las vidas en el display
static void mov_canon(void);									//se encarga del movimiento del cañon
static void mov_aliens(void);									//se encarga del movimiento de los aliens
static void disparo(void);										//se encarga del disparo del jugador
static void disparo_aliens(void);								//se encarga del disparo de los aliens
static bool nodriza(int estado);								//se encarga de el movimiento y estado de la nave nodriza y devuelve si esta viva o no
static void ini_time(void);										//guarda la hora en hora inicial pata usar de contador
static bool time_elapsed(unsigned int seg);						//determina si trascurrio el tiempo recibido en segundos y devuelve true o false

/*******************************
 * VARIABLES GLOBALES PRIVADAS *
********************************/
static canon_t canon;											//sera donde se guarde la posicion del cañon para luego copiarla a la matriz
static time_t tiempo_inicial;									//guardaremos la hora de inicio del juego para usar de timer, y se ira actualizando dependiendo la situacion

/*******************************
 * VARIABLES GLOBALES PUBLICAS *
********************************/
int mundo_buffer[16][16];										//guarda una copia del mundo sin los aliens y con el disparo agregado de los aliens ya que el disparo de los aliens solo existira aca
int cant_aliens = CANT_ALIENS;									//variable que contendra la cantidad de aliens en todo momento

/**************************************************************************
 * FUNCION: se encarga de manejar el juego en general
 * RECIBE: -
 * DEVUELVE: diversos estados dependiendo de la situacion, WIN, LOSE, RESUME, RESTART, MENU o QUIT
***************************************************************************/
int game(void)
{
	int p, retu = OK, flag = 1;									//p sera el estado de la pausa, retu la variable a devolver, y flag es usado como delay
	joyinfo_t coord = {0,0,J_NOPRESS};							//coordenadas medidas del joystick
	
	t_pausa = 0;												//nos aseguramos que al iniciar el tiempo acumulado en la pausa sea 0
	flag_mov = 1;												//flag que al estar en 1 fuerza el movimiento inicial hacia la derecha
	nodriza(DESTROY_N);											//nos aseguramos que la nodriza este correctamente inicializada
	cant_aliens = CANT_ALIENS;									//nos aseguramos de resetear la cantidad de aliens
	ini_time();													//llamamos a la funcion para guardar la hora de inicio del programa y asi usarla de timer

	disp_clear();												//limpia el display
	crear_mundo();												//inicializamos el mundo en su estado inicial
	can_init();													//inicializamos canon para que contenga la posicion de el cañon en la matriz y posteriormente actualizarla

	do
	{
		coord = joy_read();										//Guarda las coordenadas medidas
		disp_update();											//Actualiza el display con el contenido del buffer
		
		if(flag >= 1)											//usamos a flag como contador para generar un delay en ciertas acciones del bucle, que no sucedan tan rapido
		{
			if(flag >= 30)										//cantidad de delay 
			{
				flag = 0;
			}
			else
			{
				flag++;
			}
		}

        //al precionar el boton del stick
		if((coord.sw == J_PRESS) && (flag == 0))
		{
			flag = 1;
			p = pause();										//guardamos el estado devuelto por pausa
            if(p == RESTART)									//al recibir RESTART
            {
				vidas(RESET_VIDA);								//volvemos la cantidad de vidas a la inicial
                scor = 0;										//y el score a 0
                retu = RESTART;
            }
            else if(p == MENU)									//al recibir MENU
            {
				vidas(RESET_VIDA);
                scor = 0;
                retu = MENU;
            }
            else if(p == QUIT)									//al recibir QUIT
            {
                retu = QUIT;
            }
		}

		mov_canon();											//verificamos si se movio el cañon y actualizamos su posicion en la matriz
		mov_aliens();											//llamamos para mover los aliens respecto a la velocidad configurada dentro de la funcion
		copy();													//copiamos los datos de mundo a mundo_buffer sin copiar los aliens
		disparo();												//verificamos si se disparo y como se debe mover el disparo
		copy();													//copiamos de nuevo los datos para que esten actualizados en todo momento los disparos
		disparo_aliens();										//verificamos si los aliens deben disparar y como se debe mover su disparo

		disp_clear();											//limpiamos el display
		print_disp();											//mostramos la matriz/mundo en la matriz de led
		print_vidas();											//mostramos las vidas en la matriz de led

		if(cant_aliens == 0)									//verificamos si se mataron todos los aliens, en tal caso se gano el nivel
        {
            vidas(ADD_VIDA);									//agregamos una vida por ganar el nivel
            retu = WIN;
        }
        else if(lose() == LOSE)									//si no se gano verificamos si se perdio, y en tal caso:
        {
			vidas(RESET_VIDA);									//reseteamos las vidas
            retu = LOSE;
        }
	}
	while(retu == OK);											//en caso que se deba retornar algun estado devido a la pausa o por perder o ganar, salimos del bucle
	
	disp_clear();												//limpiamos el display al salir
    disp_update();												//actualizamos el display

	return retu;												//devolvemos el estado correspondiente
}

/**************************************************************************
 * FUNCION: guarda el estado del cañon dentro de mundo en canon
 * RECIBE: -
 * DEVUELVE: -
***************************************************************************/
static void can_init(void)
{
	int x, ind_canon = 0;
	
	for(x = 0; (x < FILA) && (ind_canon < 3); x++)
	{
		if(mundo[15][x] == TANQUE)								//cuando se encuentra TANQUE guardamos las coordenadas
		{
			canon.c_pos[ind_canon].x = x;
			canon.c_pos[ind_canon].y = 15;
			ind_canon++;
		}
	}
}

/**************************************************************************
 * FUNCION: copia mundo a mundo_buffer sin los aliens
 * RECIBE: -
 * DEVUELVE: -
***************************************************************************/
static void copy(void)
{
	int i, j;

	for(j = 0; j < 16; j++)
	{
		for(i = 0; i < 16; i++)
		{
			if((mundo[j][i] != ALI_1) && (mundo[j][i] != ALI_2) &&  (mundo[j][i] != ALI_3) && (mundo_buffer[j][i] != ALI_BALA))			//nos aseguramos de no sobreescribir ALI_BALA en mundo_buffer
			{
				mundo_buffer[j][i] = mundo[j][i];				//copiamos celda a celda
			}
			else if(mundo_buffer[j][i] != ALI_BALA)				//rellenamos espacios vacios con ceros
			{
				mundo_buffer[j][i] = 0;
			}
		}
	}
}

/**************************************************************************
 * FUNCION: verificamos si se perdio
 * RECIBE: -
 * DEVUELVE: LOSE si se perdio, OK si no
***************************************************************************/
static int lose(void)
{
    int i, retu = OK;

	for(i = 0; i < 16; i++)										//verificamos si los aliens llegaron a donde esta el jugador
	{
		if((mundo[14][i] == ALI_3) || (mundo[14][i] == ALI_2) || (mundo[14][i] == ALI_1))
		{
			retu = LOSE;
		}
	}
    if(vidas(STATUS_VIDA) == 0)									//verificamos si el jugador se quedo sin vidas
    {
        retu = LOSE;
    }

    return retu;
}

/**************************************************************************
 * FUNCION: mostramos lo que ocurre en la matriz de led
 * RECIBE: -
 * DEVUELVE: -
***************************************************************************/
static void print_disp(void)
{
	int x, y;
	dcoord_t pos;

	//recorremos la matriz mundo buscando cualquier cosa que deba mostrarse y mundo_buffer en busca de el disparo de los aliens
	for(y = 0; y < FILA; y++)
	{
		for(x = 0; x < FILA; x++)
		{
			if((mundo[y][x] != 0) || (mundo_buffer[y][x] == ALI_BALA))
			{
				pos.x = x;
				pos.y = y;
				disp_write(pos, D_ON);							//prendemos el led en esa posicion
			}
		}
	}
}

/**************************************************************************
 * FUNCION: mostramos la cantidad de vidass en la matriz de led
 * RECIBE: -
 * DEVUELVE: -
***************************************************************************/
static void print_vidas(void)
{
	int i, vida;
	dcoord_t pos;
	pos.y = 0;
	vida = vidas(STATUS_VIDA);									//guardamos la cantidad de vidas
	
	//mostraremos la cantidad de vidas en forma de puntos encendidos en la parte superior izquierda
	for(i = 0; i < vida; i++)
	{
		pos.x = i;
		disp_write(pos, D_ON);									//prendemos los led
	}
}

/**************************************************************************
 * FUNCION: actualiza el movimiento del cañon y guarda la nueva posicion en mundo
 * RECIBE: -
 * DEVUELVE: -
***************************************************************************/
static void mov_canon(void)
{
	int i;
	static int flag;											//flag de delay
	joyinfo_t coord = {0,0,J_NOPRESS};							//coordenadas medidas del joystick
	
	coord = joy_read();											//Guarda las coordenadas medidas
		
	if(flag >= 1)												//usamos a flag como contador para generar un delay en ciertas acciones del bucle, que no sucedan tan rapido
	{
		if(flag >= 15)											//cantidad de delay, determina la velocidad de movimiento del jugador
		{
			flag = 0;
		}
		else
		{
			flag++;
		}
	}

	if((coord.x > THRESHOLD) && (canon.c_pos[2].x < DISP_MAX_X) && (flag == 0))
	{
		for(i = 0; i < 3; i++)									//si el stick se mueve hacia la derecha
		{
			mundo[15][canon.c_pos[i].x] = 0;					//borramos la posicion vieja en la matriz
			canon.c_pos[i].x++;									//actualizamos la posicion nueva en la estructura
		}
		flag = 1;
	}
	if((coord.x < -THRESHOLD) && (canon.c_pos[0].x > DISP_MIN) && (flag == 0))
	{
		for(i = 0; i < 3; i++)									//si el stick se mueve hacia la izquierda
		{
			mundo[15][canon.c_pos[i].x] = 0;					//borramos la posicion vieja en la matriz
			canon.c_pos[i].x--;									//actualizamos la posicion nueva en la estructura
		}
		flag = 1;
	}
	for(i = 0; i < 3; i++)
	{
		mundo[15][canon.c_pos[i].x] = TANQUE;					//actualizamos la posicion nueva en la matriz
	}
}

/**************************************************************************
 * FUNCION: realiza movimiento de los aliens
 * RECIBE: -
 * DEVUELVE: -
***************************************************************************/
static void mov_aliens(void)
{
	static int flag;											//flag de delay

	if(flag >= 1)												//usamos a flag como contador para generar un delay en ciertas acciones del bucle, que no sucedan tan rapido
	{
		if(flag >= (500 / (MOV_ALIENS/(cant_aliens+1)+1)))		//cantidad de delay, determinara la velocidad de movimiento de los aliens, la cual varia segun la cantidad restante
		{
			flag = 0;
		}
		else
		{
			flag++;
		}
	}

	if(flag == 0)												//cuando el delay lo permita llamamos a move la cual mueve a los aliens una celda por cada llamado
	{
		move(mundo);
		flag = 1;
	}

	if(time_elapsed(15) == true)								//caundo se cumpla el tiempo pasado a la funcion time_elapsed pasara la nodriza
	{
		if(nodriza(VOID) == false)								//una vez que termine de pasar la nodriza o sea destruida
		{
			ini_time();											//reseteamos el timer tomando de nuevo una hora incial
		}
		t_pausa = 0;											//nos aseguramos que el tiempo en pausa no se incremente mientras pasa la nave
	}
}

/**************************************************************************
 * FUNCION: gestiona el disparo del jugador
 * RECIBE: -
 * DEVUELVE: -
***************************************************************************/
static void disparo(void)
{
	joyinfo_t coord = {0,0,J_NOPRESS};							//coordenadas medidas del joystick
	static int flag, flag2 = 1;									//flag de delay y flag2 se activara y desactivara en funcion de si el disparo existe o no
	
	coord = joy_read();											//Guarda las coordenadas medidas
	
	if(flag >= 1)												//usamos a flag como contador para generar un delay en ciertas acciones del bucle, que no sucedan tan rapido
	{
		if(flag >= 10)											//cantidad de delay, determina la velocidad de movimiento del disparo
		{
			flag = 0;
		}
		else
		{
			flag++;
		}
	}

	//por cada llamada a shoot el disparo se mueve una celda
	if((coord.y > THRESHOLD) && (flag == 0) && (flag2 == 1))	//al mover el stick hacia arriba dispara
	{
		flag2 = shoot(mundo);									//si flag2 es igual a 0 significa que el disparo existe y todavia no impacto
		flag = 1;
	}
	else if((flag == 0) && (flag2 == 0))
	{
		flag2 = shoot(mundo);									//si el flag2 es igual a 1 significa que el disparo esta listo para ser disparado
		flag = 1;
	}
}

/**************************************************************************
 * FUNCION: gestiona el disparo de los aliens
 * RECIBE: -
 * DEVUELVE: -
***************************************************************************/
static void disparo_aliens(void)
{
	static int flag = 1;										//flag de delay
	
	if(flag >= 1)												//usamos a flag como contador para generar un delay en ciertas acciones del bucle, que no sucedan tan rapido
	{
		if(flag >= 30)											//cantidad de delay, determina la velocidad del disparo de los aliens
		{
			flag = 0;
		}
		else
		{
			flag++;
		}
	}

	//por cada llamado a Shoot_alien el disparo se mueve una celda
	if(flag == 0)
	{
		shoot_alien(mundo);										//llamamos a shoot_alien para que genere o mueva el disparo
		flag = 1;
	}
}

/**************************************************************************
 * FUNCION: gestiona el movimiento de la nodriza
 * RECIBE: si edbe ser reseteada con DESTROY_N o "nada" VOID para que realice el movimiento
 * DEVUELVE: true si la nodriza vive, false si muere
***************************************************************************/
static bool nodriza(int estado)
{
	static int i, flag, flag2;									//flag sera el indicador de inicio del movimiento y flag2 sera de delay
	bool retu = true;											//variable de retorno
	
	//por cada llamado nodriza(VOID) se mueve una celda cuando se lo permita el delay
	if(estado == DESTROY_N)										//reseteamos al estado inical
	{
		retu = false;
		mundo[1][i] = 0;
		flag = 0;
		i = 15;
	}
	else
	{
		if(flag2 >= 1)											//usamos a flag2 como contador para generar un delay en ciertas acciones del bucle, que no sucedan tan rapido
		{
			if(flag2 >= 25)										//cantidad de delay, determinara la velocidad de la nave nodriza
			{
				flag2 = 0;
			}
			else
			{
				flag2++;
			}
		}

		if(flag2 == 0)											//si el delay lo permite
		{
			if(flag == 0)										//creamos la nodriza
			{
				i = 15;
				flag = 1;
				mundo[1][i] = ALI_N;
			}
			else												//realizamos el movimiento
			{
				if((i != 0) && (mundo[1][i] == ALI_N))			//verificamos si fue destruida, si no lo fue se sigue desplazando
				{
					i--;
					mundo[1][i] = ALI_N;
					mundo[1][i+1] = 0;
				}
				else											//si fue destruida volvemos al estado inicial
				{
					retu = false;
					mundo[1][i] = 0;
					flag = 0;
					i = 15;
				}
			}
			flag2 = 1;
		}
	}

	return retu;												//retornamos el estado de la nodriza
}

/**************************************************************************
 * FUNCION: inicializa tiempo_inicial con la hora actual
 * RECIBE: -
 * DEVUELVE: -
***************************************************************************/
static void ini_time(void)
{
	tiempo_inicial = time(NULL);								//guardamos la hora para comparar con la actual y crear el timer
}

/**************************************************************************
 * FUNCION: compara el tiempo inicial con el actual para crear el timer
 * RECIBE: cantidad de segundos que deben trascurrir
 * DEVUELVE: true si trascurrio el tiempo, false si no lo hizo
***************************************************************************/
static bool time_elapsed(unsigned int seg)
{
	time_t tiempo_actual = time(NULL);							//guarda la hora actual

	long int tiempo_trascurrido = (tiempo_actual - tiempo_inicial - t_pausa);		//guardamos la diferencia de tiempo en segundos considerando el tiempo de pausa de por medio

	if(tiempo_trascurrido >= seg)								//si paso el tiempo deseado
	{
		t_pausa = 0;											//reseteamos el tiempo acumulado de la pausa
		return true;											//devolvemos true
	}
	else														//sino paso el tiempo
	{
		return false;											//devolvemos false
	}
}