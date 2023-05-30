//-------| src/led.c |-------//
#include "led.h"
#include "dot.h"

static short * led;

void init_led(short * address) {
	led = address;
}

void led_down_shift() {
	
	int i;
	for( i=1; i<=16 ; i++ ) {
		*led = (short)~( ((unsigned long)0xFF << i) >> 8 );
		usleep(50000);
	}
}

void led_up_shift() {
	
	int i;
	for( i=1; i<=16 ; i++ ) {
		*led = (short)~((unsigned long)0xFF00 >> i);
		usleep(50000);
	}
}

void led_blink_all() {
	int i;
	for( i=1; i<=24 ; i++ ) {
		if ( i%2 == 1 ) {
			*led = (short)~0xFF;
		}
		else {
			*led = (short)~0x00;
		}
		usleep(100000);
	}
}

void led_blink_flower() {
	int i;
	for( i=1; i<=24 ; i++ ) {
		if ( i%2 == 1 ) {
			*led = (short)~0xFF;
			dot_write(2);
		}
		else {
			*led = (short)~0x00;
			dot_clear();
		}
		usleep(100000);
	}
}

void led_clear() {
	*led = (short)~0;
}

void led_all() {
	*led = (short)~0xFF;
}

void led_bit(int number) {
	*led = (short)~(number << 4);
}

void led_level(int level) {
	*led = (short)~((unsigned long)0xFF00 >> (level*2));
}
