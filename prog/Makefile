CC = /sec/buildroot/output/host/usr/bin/arm-linux-gcc

/targetfs/usr/local/bin/moteur:	moteur
	cp moteur /targetfs/usr/local/bin/

/targetfs/usr/local/bin/camera:	camera
	cp camera /targetfs/usr/local/bin/

moteur:	moteur.c
	$(CC) -o moteur moteur.c

camera:	camera.c
	$(CC) -o camera camera.c