CC = /sec/buildroot/output/host/usr/bin/arm-linux-gcc

all: /targetfs/usr/local/bin/ultrason /targetfs/usr/local/bin/moteur /targetfs/usr/local/bin/camera

/targetfs/usr/local/bin/moteur:	moteur
	cp moteur /targetfs/usr/local/bin/

/targetfs/usr/local/bin/camera:	camera
	cp camera /targetfs/usr/local/bin/

/targetfs/usr/local/bin/ultrason:	ultrason
	cp ultrason /targetfs/usr/local/bin/

moteur:	moteur.c
	$(CC) -o moteur moteur.c

camera:	camera.c
	$(CC) -o camera camera.c

ultrason:	ultrason.c
	$(CC) -o ultrason ultrason.c