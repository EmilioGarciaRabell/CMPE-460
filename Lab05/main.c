
#include "lab1/leds.h"
#include "lab5/switches.h"
#include "lab5/timers.h"
#include "lab2/uart.h"
void part1(void);


void part1(){
	UART0_init();
	S1_init_interrupt();
	S2_init_interrupt();
	LED1_init();
	LED2_init();
	TIMG0_init();
	TIMG6_init();
	TIMG12_init();
}

int main (void)
{
	;
}

