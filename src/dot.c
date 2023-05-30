//-------| src/dot.c |-------//
#include "dot.h"


static unsigned short dot_flower[3][MAX_DOT] = {
	{0x00, 0x00, 0x0F, 0x00, 0x00}, // flower 1
	{0x00, 0x1C, 0x1F, 0x1C, 0x00}, // flower 2
	{0x2A, 0x1C, 0x7F, 0x1C, 0x2A}, // flower 3
};

static short * dot[MAX_DOT];

void init_dot(short * address[]) {
	int i;
	for( i=0; i<MAX_DOT; i++ ) {
		dot[i] = address[i];
	}
}

void dot_clear() {
	int i;
	for(i=0; i<MAX_DOT; i++){
		*dot[i] = 0;
	}
	usleep(0); // for Ximulator
}

void dot_write(int number) {
	int i;
	for(i=0; i<MAX_DOT; i++){
		*dot[i] = dot_flower[number][i];
	}
	usleep(0); // for Ximulator
}

void dot_down_shift(int decimal) {
	int shift, i;
	for( shift=0; shift<=7; shift++ ) {
		for(i=0; i<MAX_DOT; i++){
			*dot[i] = dot_flower[decimal][i] >> shift;
		}
		usleep(100000);
	}
}
