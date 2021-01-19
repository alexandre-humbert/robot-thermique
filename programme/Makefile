CC = /sec/buildroot/output/host/usr/bin/arm-linux-gcc

all: /targetfs/usr/local/bin/robot /targetfs/usr/local/bin/camera /targetfs/usr/local/bin/ultrason /targetfs/usr/local/bin/moteur /targetfs/usr/local/bin/interface


/targetfs/usr/local/bin/robot:	build/robot
	cp build/robot /targetfs/usr/local/bin/

/targetfs/usr/local/bin/camera: build/camera
	cp build/camera /targetfs/usr/local/bin/

/targetfs/usr/local/bin/ultrason: build/ultrason
	cp build/ultrason /targetfs/usr/local/bin/

/targetfs/usr/local/bin/moteur: build/moteur
	cp build/moteur /targetfs/usr/local/bin/


build/robot: build/robot.o build/moteur.o build/camera.o build/ultrason.o
	$(CC) -g -o build/robot build/robot.o build/moteur.o build/camera.o build/ultrason.o

build/camera: build/camera_dbg.o build/camera.o
	$(CC) -g -o build/camera build/camera_dbg.o build/camera.o

build/ultrason: build/ultrason_dbg.o build/ultrason.o
	$(CC) -g -o build/ultrason build/ultrason_dbg.o build/ultrason.o

build/moteur: build/moteur_dbg.o build/moteur.o
	$(CC) -g -o build/moteur build/moteur_dbg.o build/moteur.o
		

build/robot.o:	robot.c
	$(CC) -c -o build/robot.o robot.c

build/moteur.o:	moteur.c moteur.h
	$(CC) -c -o build/moteur.o moteur.c

build/camera.o:	camera.c camera.h
	$(CC) -c -o build/camera.o camera.c

build/ultrason.o:	ultrason.c ultrason.h
	$(CC) -c -o build/ultrason.o ultrason.c
	

build/camera_dbg.o: camera_dbg.c
	$(CC) -c -o build/camera_dbg.o camera_dbg.c

build/ultrason_dbg.o: ultrason_dbg.c
	$(CC) -c -o build/ultrason_dbg.o ultrason_dbg.c

build/moteur_dbg.o: moteur_dbg.c
	$(CC) -c -o build/moteur_dbg.o moteur_dbg.c

clean:
	rm build/*



