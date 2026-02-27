/**
 * ******************************************************************************
 * @file    : main.c
 * @brief   : Main file for Lab 5 part 1 demo
 * @details : This main file handles part 1 of the lab which functions by flashing LED1 at a 0.5 sec off 0.5 sec on rate when switch 1 is pressed. Switch 2 will change the color of LED2 and print out the elapsed time between presses
 *
 * @author Emilio Garcia Rabell & Cristian Morataya
 * @date 2/26/2026
 * ******************************************************************************
 */

#include "lab1/leds.h"
#include "lab5/switches.h"
#include "lab5/timers.h"
#include <lab2/uart.h>
#include <ti/devices/msp/msp.h>
#include <uart_extras.c>
#include <sysctl.h>

// Function declarations
void part1(void);
void TIMG6_IRQHandler(void);
void TIMG12_IRQHandler(void);
void GROUP1_IRQHandler(void);

// Prescaler value for the timer
#define TIMG6_PRESCALER 32u

// Variables to keep track of both LEDs and time count
int led1Active = 0;
int led2Active = 0;
int led2Color = 0;
uint32_t timeCount = 0;

// Number of colors from the leds.h file
int numColors = 7;

// ISR for the interrupt, this will toggle LED1 whenever switch 1 is pressed
void TIMG6_IRQHandler(void)
{
	// Clear the interrupt flag
	TIMG6->CPU_INT.ICLR |= GPTIMER_CPU_INT_ICLR_Z_CLR;

	// Toggle LED1 if it is active
	if (led1Active)
	{
		LED1_set(TOGGLE);
	}
}

// ISR for the interrupt, this will toggle LED2 whenever switch 2 is pressed
void TIMG12_IRQHandler(void)
{
	// Clear the interrupt flag
	TIMG12->CPU_INT.ICLR |= GPTIMER_CPU_INT_ICLR_Z_CLR;

	// Increment time count if the LED is active
	if (led2Active)
		timeCount++;
}

// ISR for the GROUP1 interrupt this handles both switch 1 and switch 2 together
void GROUP1_IRQHandler(void)
{
	// Check the index of GPIOA to see if switch 1 was pressed
	uint32_t interruptIndexGPIOA = GPIOA->CPU_INT.IIDX;

	// If switch 1 was pressed toggle LED1
	if (interruptIndexGPIOA == GPIO_CPU_INT_IIDX_STAT_DIO18)
	{
		// Toggle the state of LED1
		led1Active = !led1Active;
		if (!led1Active)
		{
			// Turn off LED1 if it is being turned off
			LED1_set(OFF);
		}
	}
	// Check the index of GPIOB to see if switch 2 was pressed
	uint32_t interruptIndexGPIOB = GPIOB->CPU_INT.IIDX;

	// If switch 2 was pressed toggle LED2
	if (interruptIndexGPIOB == GPIO_CPU_INT_IIDX_STAT_DIO21)
	{
		if (!led2Active)
		{
			// On the first press start the timer and set the LED to the current color
			timeCount = 0;
			led2Active = 1;
			// Go through the colors
			switch (led2Color)
			{
			case 0:
				LED2_set(RED);
				break;
			case 1:
				LED2_set(GREEN);
				break;
			case 2:
				LED2_set(BLUE);
				break;
			case 3:
				LED2_set(CYAN);
				break;
			case 4:
				LED2_set(MAGENTA);
				break;
			case 5:
				LED2_set(YELLOW);
				break;
			case 6:
				LED2_set(WHITE);
				break;
			}
		}
		else
		{
			// Print the elapsed time and as soon as the button is pressed, change over to the next color and reset the timer
			UART0_put("Elapsed time: ");
			UART0_printDec((int)timeCount);
			UART0_put(" ms\r\n");
			timeCount = 0;
			// Change to the next color in the list and set the LED to that color
			led2Color = (led2Color + 1) % numColors;
			switch (led2Color)
			{
			case 0:
				LED2_set(RED);
				break;
			case 1:
				LED2_set(GREEN);
				break;
			case 2:
				LED2_set(BLUE);
				break;
			case 3:
				LED2_set(CYAN);
				break;
			case 4:
				LED2_set(MAGENTA);
				break;
			case 5:
				LED2_set(YELLOW);
				break;
			case 6:
				LED2_set(WHITE);
				break;
			}
		}
	}
}

void part1()
{
	// Declare the timer load values with the system clock, prescaler and desired time
	uint32_t TIMG6LoadValue = (SYSCTL_SYSCLK_getULPCLK() / 8 / (TIMG6_PRESCALER + 1)) - 1;
	uint32_t TIMG12LoadValue = (SYSCTL_SYSCLK_getULPCLK() / 1000) - 1;
	// Initialize all the peripherals
	UART0_init();
	S1_init_interrupt();
	S2_init_interrupt();
	LED1_init();
	LED2_init();
	TIMG6_init(TIMG6LoadValue, TIMG6_PRESCALER);
	TIMG12_init(TIMG12LoadValue);

	// Infinite loop to keep the program running and allow interrupts to happen
	while (1)
	{
	}
}

int main(void)
{
	part1();
}
