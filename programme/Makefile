CC = /sec/buildroot/output/host/usr/bin/arm-linux-gcc

all: /targetfs/usr/local/bin/robot


/targetfs/usr/local/bin/robot:	build/robot
	cp build/robot /targetfs/usr/local/bin/




build/robot: build/robot.o build/moteur.o build/camera.o build/ultrason.o
	$(CC) -o build/robot build/robot.o build/moteur.o build/camera.o build/ultrason.o




build/robot.o:	robot.c
	$(CC) -c -o build/robot.o robot.c

build/moteur.o:	moteur.c moteur.h
	$(CC) -c -o build/moteur.o moteur.c

build/camera.o:	camera.c camera.h
	$(CC) -c -o build/camera.o camera.c

build/ultrason.o:	ultrason.c ultrason.h
	$(CC) -c -o build/ultrason.o ultrason.c


clean:
	rm build/*