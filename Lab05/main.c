#include "lab1/leds.h"
#include "lab5/switches.h"
#include "lab5/timers.h"
#include <lab2/uart.h>
#include <ti/devices/msp/msp.h>
#include <uart_extras.c>

void part1(void);
void TIMG6_IRQHandler(void);
void TIMG12_IRQHandler(void);
void GROUP1_IRQHandler(void);

#define TIMG6_PRESCALER 255u
// Final value after computing with the formula from equation 28 in the TRM
#define TIMG6_LOAD 31249u

// Value determined after computing equation 28 from the TRM
#define TIMG12_LOAD 31999u

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

	// Check if LED2 is on
	if (led2Active)
	{
		// Increment the time count
		timeCount++;

		// Check if the counter has reached .5 second/1 Hz
		if (timeCount % 500 == 0)
		{
			// Toggle LED2 and change the color
			led2Color = (led2Color + 1) % numColors;

			// Switch case to change the LED color
			switch (led2Color % numColors)
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

// ISR for the GROUP1 interrupt this handles both switch 1 and switch 2 together
void GROUP1_IRQHandler(void)
{
	// Check the index of GPIOA to see if switch 1 was pressed
	uint32_t interruptIndexGPIOA = GPIOA->CPU_INT.IIDX;

	// If switch 1 was pressed toggle LED1
	if (interruptIndexGPIOA == GPIO_CPU_INT_IIDX_STAT_DIO18)
	{
		led1Active = !led1Active;
		if(!led1Active)
		{
			LED1_set(OFF);
		}
	}
	// Check the index of GPIOB to see if switch 2 was pressed
	uint32_t interruptIndexGPIOB = GPIOB->CPU_INT.IIDX;
	// If switch 2 was pressed toggle LED2
	if (interruptIndexGPIOB == GPIO_CPU_INT_IIDX_STAT_DIO21)
	{
		// Check if LED2 is on
		if (!led2Active)
		{
			// Toggle the state of LED2
			led2Active = 1;
			timeCount = 0;

			// Switch case to change the LED color
			switch (led2Color % numColors)
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
			led2Active = 0;
    LED2_set(NONE);

    UART0_put("Elapsed time: ");
    UART0_printDec((int)timeCount);
    UART0_put(" ms\r\n");

    led2Color = (led2Color + 1) % numColors;
		}
	}
}

void part1()
{
	// Initialize all the peripherals
	UART0_init();
	S1_init_interrupt();
	S2_init_interrupt();
	LED1_init();
	LED2_init();
	TIMG6_init(TIMG6_LOAD, TIMG6_PRESCALER);
	TIMG12_init(TIMG12_LOAD);

	// Infinite loop to keep the program running and allow interrupts to happen
	while (1)
	{
	}
}

int main(void)
{
	part1();
}
