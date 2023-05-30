// stem -> LED

#include "stem.h"

static short * stem;

// init
void init_stem(short * address) {
	stem = address;
}

// clear
void stem_clear() {
	*stem = (short)~0;
}

// all
void stem_all() {
	*stem = (short)~0xFF;
}

// level
void stem_level(int level) {
	*stem = (short)~((unsigned long)0xFF00 >> (level*2));
}

// game over
void stem_game_over() {
	int i;
	for( i=1; i<=16 ; i++ ) {
		if ( i%2 == 1 ) {
			*stem = (short)~0xFF;
		}
		else {
			*stem = (short)~0x00;
		}
		usleep(100000);
	}
}

// game fail
void stem_fail() {
	
	int i;
	for( i=1; i<=16 ; i++ ) {
		*stem = (short)~( ((unsigned long)0xFF << i) >> 8 );
		usleep(50000);
	}
}
