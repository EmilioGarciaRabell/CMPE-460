// #include "lab1/leds.h"
// #include "lab5/switches.h"
// #include "lab5/timers.h"
// #include <lab2/uart.h>
// #include "lab5/adc12.h"
// #include <ti/devices/msp/msp.h>
// #include <uart_extras.c>
// #define TIMG6_LOAD 31249u
// #define TIMG6_PRESCALER 32
// void part2()
// {
// 	// Initialize all the peripherals
// 	UART0_init();
// 	S1_init_interrupt();
// 	S2_init_interrupt();
// 	LED1_init();
// 	LED2_init();
// 	TIMG6_init(TIMG6_LOAD, TIMG6_PRESCALER);
// 	ADC0_init();

// 	// Infinite loop to keep the program running and allow interrupts to happen
// 	while (1)
// 	{
// 	}
// }

// void TIMG6_IRQHandler(void){
// 	uint32_t adc_result = ADC0_getVal();
// 	// print decimal ADC result
// 	UART0_printDec(adc_result);
	
// 	// Print hex ADC result
	
// }
