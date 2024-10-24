#include <stdio.h>
#include <stdlib.h>

const char *sdata="OS 2024";
char gdata[128];
char bdata[16] = {1,2,3,4};
main() {
	char * ldata[16];	
	char * ddata;

	ddata = malloc(16);
	printf("sdata: %llx\ngdata: %llx\nbdata: %llx\nldata: %llx\nddata: %llx\n",
			sdata, gdata,bdata,ldata,ddata);
	while (1) {}
	free(ddata);
}
