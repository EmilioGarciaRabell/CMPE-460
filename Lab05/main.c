#include "lab5/camera.h"
#include "lab2/uart.h"
#include <uart_extras.h>

int main(void)
{
	UART0_init();
	Camera_init();
	
	while(1)
	{
		if(Camera_isDataReady())
		{
			uint16_t *incomingData = Camera_getData();
			
			UART0_put("-1\r\n");
			
			for(int i = 0; i<128; i++)
			{
				UART0_printDec(incomingData[i]);
				UART0_put("\r\n");
			}
			UART0_put("-2\r\n");

            Camera_clearDataReady();
		}
	}
}