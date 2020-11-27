#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


main() {
	system("stty -F /dev/ttyO1 9600");
	FILE* uart;
	
	uart = fopen("/dev/ttyO1", "w");
	fprintf(uart, "av\n");
	fclose(uart);
	sleep(4);
	uart = fopen("/dev/ttyO1", "w");
	fprintf(uart, "st\n");
	fclose(uart);

	sleep(1);
	uart = fopen("/dev/ttyO1", "w");
	fprintf(uart, "bw\n");
	fclose(uart);
	sleep(4);
	uart = fopen("/dev/ttyO1", "w");
	fprintf(uart, "st\n");
	fclose(uart);

}
