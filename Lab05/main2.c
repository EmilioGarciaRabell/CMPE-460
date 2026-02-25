//#include "lab1/leds.h"
//#include "lab5/switches.h"
//#include "lab5/timers.h"
//#include <lab2/uart.h>
//#include "lab5/adc12.h"
//#include <ti/devices/msp/msp.h>
//#include <uart_extras.c>
//#include <sysctl.h>

//#define TIMG6_PRESCALER 32u


//void part2(void);
//void print_hexa(uint32_t dec_val);

//void part2()
//{
//	
//	uint32_t TIMG6LoadValue = (SYSCTL_SYSCLK_getULPCLK() / 8 / (TIMG6_PRESCALER + 1)) - 1;
//	// Initialize all the peripherals
//	UART0_init();
//	S1_init_interrupt();
//	S2_init_interrupt();
//	LED1_init();
//	LED2_init();
//	TIMG6_init(TIMG6LoadValue, TIMG6_PRESCALER);
//	ADC0_init();

//	// Infinite loop to keep the program running and allow interrupts to happen
//	while (1)
//	{
//	}
//}

//void TIMG6_IRQHandler(void){
//	uint32_t adc_result = ADC0_getVal();
//	
//	// Use the input voltage divide by the maximum value
//	float voltage = adc_result * (3.3f / 4.0950f)/100;
//	//float voltage = (((adc_result*3300)/ 4095 - 500) / 10);
//	//float voltage = (3.3f / 4096.0f) * ((float)adc_result - 0.5f);

//	// Celcius conversion the 0.5 is the offset from the datasheet
//	float tempCelcius = (voltage -0.5f);
//	
//	// Convert to farenheit, regular conversion formula
//	float tempFarenheit = (tempCelcius*9.0f/5.0f)+32.0f;
//	
//	// print decimal ADC result
//	//UART0_put("\n\r Decimal ADC Value: ");
//	//UART0_printDec(adc_result);
//	
//	// Print hex ADC result
//	//print_hexa(adc_result);
//	
//	// Print out both temperatures in C and F
//  UART0_put("\n\r Temp (C): ");
//  UART0_printDec((int32_t)tempCelcius);
//	UART0_put("\n\r Temp (F): ");
//  UART0_printDec((int32_t)tempFarenheit);
//}


//void print_hexa(uint32_t dec_val){
//	int i = 1, j, temp;
//	char hexa_num[100];
//	while (dec_val != 0) {
//        temp = dec_val % 16;
//      
//        // converting decimal number 
//        // in to a hexa decimal
//        // number
//        if (temp < 10)
//            temp = temp + 48;
//        else
//            temp = temp + 55;
//        hexa_num[i++] = temp;
//        dec_val = dec_val / 16;
//    }
//    // printing the hexa decimal number
//    UART0_put("\n\r Hexadecimal ADC Value: ");
//    for (j = i - 1; j > 0; j--)
//        UART0_putchar(hexa_num[j]);

//    

//}


//int main(){
//	part2();
//}

// Code for sensors