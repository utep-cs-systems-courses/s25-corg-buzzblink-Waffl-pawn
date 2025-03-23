# makfile configuration
COMMON_OBJECTS  = toyMain.o buzzer.o transition_state_asm.o
CPU             = msp430g2553
CFLAGS          = -mmcu=${CPU} -I../h -L/opt/ti/msp430_gcc/include
LDFLAGS		= -L/opt/ti/msp430_gcc/include

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-as

all: toy-c.elf toy-s.elf 

#additional rules for filest
toy-c.elf: ${COMMON_OBJECTS} transition_state_c.o
	${CC} ${CFLAGS} -o $@ $^ libTimer.a
toy-s.elf: ${COMMON_OBJECTS} transition_state_asm.o
	${CC} ${CFLAGS} -o $@ $^ libTimer.a

load-c: toy-c.elf
	msp430loader.sh	$^
load-s: toy-s.elf
	msp430loader.sh $^



clean: 
	rm -f *.o *.elf

toy.c: buzzer.h led.h
buzzer.c: buzzer.h


