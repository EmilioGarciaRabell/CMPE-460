#include "lab5/camera.h"
#include "lab6/timers.h"
#include <ti/devices/msp/msp.h>
#include <sysctl.h>
#include "track_settings.h"
#include "lab2/uart.h"
#include "uart_extras.h"

/*The functions living under this module will manage any track settings, 
	like finding the center and checking if the car is on the track*/
	
	
	// Function to find the center line on the track with the camera input data
int find_center_line(uint16_t *in)
{
    uint16_t mn = in[0], mx = in[0];
    int i;
    for (i = 1; i < 128; i++)
    {
        if (in[i] < mn) mn = in[i];
        if (in[i] > mx) mx = in[i];
    }

    // Midpoint threshold
    uint16_t threshold = (uint16_t)(((uint32_t)mx + mn) / 2u);

    uint32_t weighted_sum = 0;
    uint32_t total_weight = 0;

    for (i = 0; i < 128; i++)
    {
        // Check if the pixel is part of the line (darker than threshold)
        if (in[i] < threshold)
        {
            // Calculate "weight" based on how dark the pixel is
            // More dark = higher weight
            uint16_t weight = threshold - in[i]; 
            
            weighted_sum += (uint32_t)(i * weight);
            total_weight += (uint32_t)weight;
        }
    }

    if (total_weight == 0)
    {
        return 64; 
    }

    // This division now includes the "intensity" of the line
    return (int)(weighted_sum / total_weight);
}

// Function to check if the car is on the carpet by seeing if the camera data is mostly dark
int is_carpet(uint16_t *pixels)
{
	uint32_t sum = 0;

	// Loop through the pixels and add them up to get the average
	for (int i = 0; i < 128; i++)
	{
		sum += pixels[i];
	}

	// If the average is less than 500 then its on the carpet otherwise its not
	if ((sum / 128) < 300)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
