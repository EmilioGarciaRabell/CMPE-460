/**
 * ******************************************************************************
 * @file    : main.c
 * @brief   : Main file for Lab 5 part 3 demo
 * @details : This is the main file for the part 3 demo which checks for data all the time and prints it out
 *
 * @author Emilio Garcia Rabell & Cristian Morataya
 * @date 2/26/2026
 * ******************************************************************************
 */
#include "lab5/camera.h"
#include "lab2/uart.h"
#include <uart_extras.h>

int main(void)
{
	// Initialize both the UART and the camera
	UART0_init();
	Camera_init();

	// Infinite loop to check if there is new camera data
	while (1)
	{
		if (Camera_isDataReady())
		{
			// Make a pointer to the camera data
			uint16_t *incomingData = Camera_getData();

			// Print out a start marker with -1
			UART0_put("-1\r\n");

			// Loop through the 128 pixels 
			for (int i = 0; i < 128; i++)
			{
				// Print out the values througb UARTs
				UART0_printDec(incomingData[i]);
				UART0_put("\r\n");
			}
			// Print out the end marker
			UART0_put("-2\r\n");

			// Clear the data for the next round of data
			Camera_clearDataReady();
		}
	}
}