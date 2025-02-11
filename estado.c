#include <stdio.h>
#include "estado.h"

#define MAX_BUFFER		10240

char *estado2str(ESTADO e) {
	static char buffer[MAX_BUFFER];
	unsigned char *p = (unsigned char *) &e;
	unsigned int i;

	buffer[0] = 0;

	for(i = 0; i < sizeof(ESTADO); i++)
		sprintf(buffer, "%s%02x", buffer, p[i]);
	
	return buffer;
}

ESTADO str2estado(char *argumentos) {
	ESTADO e;
	unsigned int i;
	unsigned int d;
	unsigned char *p = (unsigned char *) &e;


	for(i = 0; i < sizeof(ESTADO); i++, argumentos += 2) {

		sscanf(argumentos, "%2x", &d);
		p[i] = (unsigned char) d;
	}
	
	return e;
}

