//-------| src/main.c |-------//
#include "main.h"

#define MAX_LEVEL 4
#define WATER_DELAY 2000000 // 2 sec


static off_t IEB_DOT[MAX_DOT] = {
	IEB_DOT1,
	IEB_DOT2,
	IEB_DOT3,
	IEB_DOT4,
	IEB_DOT5
};
static off_t IEB_FND[MAX_FND] = {
	IEB_FND0,
	IEB_FND1,
	IEB_FND2,
	IEB_FND3,
	IEB_FND4,
	IEB_FND5,
	IEB_FND6,
	IEB_FND7
};

static int fd;
static int map_counter = 0;
static void * map_data[100];
static seclection_t sel; 

int water_count = 0;
int game_time = 10;
truth_t game_over = FALSE;

void water_plant() {
	if( water_count < MAX_LEVEL ) {
		water_count++;
		led_level(water_count);
	}
	
	if( water_count == MAX_LEVEL ) {
		clcd_clear_display();
		clcd_write_string("WINNER WINNER");
		clcd_set_DDRAM(0x40);	// 2nd line
		clcd_write_string("CHICKEN DINNER!!");
		dot_write(0);
		usleep(500000);
		dot_write(1);
		usleep(500000);
		dot_write(2);
		led_blink_flower();
		dot_down_shift(2);
		led_down_shift();
		sleep(3);
		clcd_clear_display();
		fnd_clear();
		led_clear();
		game_over = TRUE;
	}
}

void decrease_time() {
	fnd_write(game_time, 0);
	game_time--;
	usleep(1000000);
	
	if( game_time < 0 && !game_over ) {
		clcd_clear_display();
		clcd_write_string("LOSER T^T");
		clcd_set_DDRAM(0x40);	// 2nd line
		clcd_write_string("SHAME ON YOU!");
		led_blink_all();
		led_down_shift();
		sleep(3);
		clcd_clear_display();
		fnd_clear();
		led_clear();
		game_over = TRUE;
	}
}

int main(int argc, char* argv[]) {
	
	int i;
	short * led, * dot[MAX_DOT], * fnd[MAX_FND];
	short * clcd_cmd, * clcd_data, * keypad_out, * keypad_in;
	
	fd = open("/dev/mem", O_RDWR|O_SYNC);
	if (fd == -1) {
		fprintf(stderr, "Cannot open /dev/mem file");
		exit(EXIT_FAILURE);
	}
	
	led = mapper(IEB_LED, PROT_WRITE);
	for( i=0; i<MAX_DOT; i++ ) {
		dot[i] = mapper(IEB_DOT[i], PROT_WRITE);
	}
	for( i=0; i<MAX_FND; i++ ) {
		fnd[i] = mapper(IEB_FND[i], PROT_WRITE);
	}
	clcd_cmd  = mapper(IEB_CLCD_CMD, PROT_WRITE);
	clcd_data = mapper(IEB_CLCD_DATA, PROT_WRITE);
	keypad_out  = mapper(IEB_KEY_W, PROT_WRITE);
	keypad_in = mapper(IEB_KEY_R, PROT_READ);
	
	init_led(led);
	init_dot(dot);
	init_fnd(fnd);
	init_clcd(clcd_cmd, clcd_data);
	init_keypad(keypad_out, keypad_in);
	
	select_mode();
	
	if( sel.exit == 1 ) { return 0; }
	
	while( !game_over ) {
	
		int key_count, key_value;
		key_count = keypad_read(&key_value);
		
		if( !game_over ) {
			decrease_time();
		
			if( key_count == 1 ) {
				if( key_value == 0 ) {
					water_plant();
				}
			}
			else if( key_count > 1 ) {
				break;
			}
		}
	}
	
	unmapper();
	close(fd);
	return 0;
}

short * mapper(off_t offset, int prot) {
	map_data[map_counter] = mmap(NULL, sizeof(short), prot, MAP_SHARED, fd, offset);
	if ( map_data[map_counter] == MAP_FAILED ) {
		fprintf(stderr, "Cannot do mmap()");
		emergency_closer();
	}
	return (short *)map_data[map_counter++];
}

void unmapper() {
	int i;
	for( i=0; i<map_counter; i++) {
		munmap(map_data[i], sizeof(short));
	}
	map_counter = 0;
}

void emergency_closer() {
	unmapper();
	close(fd);
	exit(EXIT_FAILURE);
}

void select_mode() {
	int i;  char buf[100];
	char clcd_str[20] = "";
	
	led_clear();
	dot_clear();
	fnd_clear();
	clcd_clear_display();
	
	printf("\n");
	printf("************************************\n");
	printf("*                                  *\n");
	printf("*         GROW YOUR PLANT          *\n");
	printf("*                                  *\n");
	printf("*       Try not to kill it         *\n");
	printf("*                                  *\n");
	printf("************************************\n\n");
	
	sel.all = 0xFF;
	sel.exit = 0;
	
	clcd_write_string("Press 2 : Start");
	clcd_set_DDRAM(0x40);
	clcd_write_string("Press 3 : Quit");
	
	while( 1 ) {
		int key_count, key_value;
		key_count = keypad_read(&key_value);
	
		if( key_count == 1 ) {
			if( key_value == 1 ) {	// Press 2 : Start
				clcd_clear_display();
				clcd_write_string("Grow your plant!!");
				clcd_set_DDRAM(0x40);
				clcd_write_string("Press 1 : Water");
				break;
			}
			else if ( key_value == 2 ) {	// Press 3 : Quit
				clcd_clear_display();
				clcd_write_string("Freakin' coward!");
				clcd_set_DDRAM(0x40);
				clcd_write_string("Go home noob!");
				sleep(3);
				clcd_clear_display();
				sel.exit = 1;
				break;
			}
		}
		else if( key_count > 1 ) {
			sel.exit = 1;
			break;
		}
	}
}
