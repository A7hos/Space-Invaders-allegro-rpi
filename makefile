#Compilacion
CC := gcc
CONFIG := -Wall -O2
#ALLEGRO const
ALL_OBJ := allegro/allmain.o allegro/allmenu.o allegro/allpause.o allegro/allwinlose.o allegro/allgame.o libs/score.o libs/vidas.o
ALL_LIBS := -lallegro -lallegro_font -lallegro_ttf -lallegro_color -lallegro_image -lallegro_primitives -lallegro_audio -lallegro_acodec
ALL_NAME := allinvaders
#Raspberry Pi const
RPI_OBJ := /home/pi/libs/joydisp/disdrv.o /home/pi/libs/joydisp/joydrv.o rpi/rpimain.o rpi/rpimenu.o rpi/rpipause.o rpi/rpilose.o rpi/rpigame.o libs/mundo.o libs/movimiento.o libs/dispara.o libs/misil.o libs/impact.o libs/vidas.o libs/score.o
RPI_NAME := rpiinvaders

#ALLEGRO compilacion
${ALL_NAME}: ${ALL_OBJ}
	${CC} ${CONFIG} ${ALL_OBJ} ${ALL_LIBS} -o ${ALL_NAME}
allegro/allmain.o: allegro/allmain.c allegro/allconst.h allegro/allmenu.h allegro/allwinlose.h allegro/allgame.h 
	${CC} ${CONFIG} -c allegro/allmain.c -o allegro/allmain.o
allegro/allmenu.o: allegro/allmenu.c allegro/allconst.h allegro/allmenu.h
	${CC} ${CONFIG} -c allegro/allmenu.c -o allegro/allmenu.o
allegro/allpause.o: allegro/allpause.c allegro/allconst.h allegro/allpause.h
	${CC} ${CONFIG} -c allegro/allpause.c -o allegro/allpause.o
allegro/allwinlose.o: allegro/allwinlose.c allegro/allconst.h allegro/allwinlose.h libs/score.h
	${CC} ${CONFIG} -c allegro/allwinlose.c -o allegro/allwinlose.o
allegro/allgame.o: allegro/allgame.c allegro/allconst.h allegro/allgame.h allegro/allpause.h libs/score.h libs/vidas.h libs/score.h
	${CC} ${CONFIG} -c allegro/allgame.c -o allegro/allgame.o

#Raspberry Pi compilacion
${RPI_NAME}: ${RPI_OBJ}
	${CC} ${CONFIG} ${RPI_OBJ} -o ${RPI_NAME}
rpi/rpimain.o: rpi/rpimain.c rpi/rpiconst.h rpi/rpimenu.h rpi/rpilose.h rpi/rpigame.h
	${CC} ${CONFIG} -c rpi/rpimain.c -o rpi/rpimain.o
rpi/rpimenu.o: rpi/rpimenu.c rpi/rpiconst.h rpi/rpimenu.h
	${CC} ${CONFIG} -c rpi/rpimenu.c -o rpi/rpimenu.o
rpi/rpipause.o: rpi/rpipause.c rpi/rpiconst.h rpi/rpipause.h
	${CC} ${CONFIG} -c rpi/rpipause.c -o rpi/rpipause.o
rpi/rpilose.o: rpi/rpilose.c rpi/rpiconst.h rpi/rpilose.h libs/score.h
	${CC} ${CONFIG} -c rpi/rpilose.c -o rpi/rpilose.o
rpi/rpigame.o: rpi/rpigame.c rpi/rpiconst.h rpi/rpigame.h rpi/rpipause.h libs/libsconst.h libs/mundo.h libs/movimiento.h libs/dispara.h libs/vidas.h libs/score.h
	${CC} ${CONFIG} -c rpi/rpigame.c -o rpi/rpigame.o

#LIBS
libs/score.o: libs/score.c libs/libsconst.h libs/score.h
	${CC} ${CONFIG} -c libs/score.c -o libs/score.o
libs/vidas.o: libs/vidas.c libs/vidas.h
	${CC} ${CONFIG} -c libs/vidas.c -o libs/vidas.o
libs/mundo.o: libs/mundo.c libs/libsconst.h libs/mundo.h
	${CC} ${CONFIG} -c libs/mundo.c -o libs/mundo.o
libs/movimiento.o: libs/movimiento.c libs/libsconst.h libs/movimiento.h libs/score.h
	${CC} ${CONFIG} -c libs/movimiento.c -o libs/movimiento.o
libs/dispara.o: libs/dispara.c libs/libsconst.h libs/dispara.h libs/misil.h libs/impact.h
	${CC} ${CONFIG} -c libs/dispara.c -o libs/dispara.o
libs/misil.o: libs/misil.c libs/libsconst.h libs/misil.h libs/impact.h
	${CC} ${CONFIG} -c libs/misil.c -o libs/misil.o
libs/impact.o: libs/impact.c libs/libsconst.h libs/impact.h libs/score.h libs/vidas.h
	${CC} ${CONFIG} -c libs/impact.c -o libs/impact.o

#ALLEGRO tools
allrun: ${ALL_NAME}
	./${ALL_NAME}
allclean:
	rm libs/*.o
	rm allegro/*.o
	rm ${ALL_NAME}
	clear

#Raspberry Pi tools
rpirun: ${RPI_NAME}
	sudo ./${RPI_NAME}
rpiclean:
	rm libs/*.o
	rm rpi/*.o
	rm ${RPI_NAME}
	clear
reboot:
	sudo reboot
off:
	sudo shutdown -h now