CC := /usr/local/arm-linux-4.1.1/bin/arm-linux-gcc

OUTPUT		= grow_plant
SRCS		= ./src/main.c \
				./src/led.c\
				./src/dot.c\
				./src/fnd.c\
				./src/clcd.c\
				./src/keypad.c\

CFLAGS		= -I./lib


$(OUTPUT) : $(SRCS)
	$(CC) $(CFLAGS) -o $@ $(SRCS)


clean:
	@rm -f $(OUTPUT)
