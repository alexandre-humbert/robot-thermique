#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


main(int argc, const char* argv[]) {

	FILE* uart;
	uart = fopen("/dev/ttyO1", "w");
	fprintf(uart, "%s\n",argv[1]);
	fclose(uart);

}
