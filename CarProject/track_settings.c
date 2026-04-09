#include "lab5/camera.h"
#include "lab6/timers.h"
#include <ti/devices/msp/msp.h>
#include <sysctl.h>
#include "track_settings.h"

/*The functions living under this module will manage any track settings, 
	like finding the center and checking if the car is on the track*/
	
	
	// Function to find the center line on the track with the camera input data
int find_center_line(uint16_t *pixels)
{
    static int previousCenter = 64;

    uint16_t smoothingFilter[128];

    // Keep edges
    smoothingFilter[0] = pixels[0];
    smoothingFilter[127] = pixels[127];

    // --- SMOOTHING ---
    for (int i = 1; i < 127; i++)
    {
        smoothingFilter[i] = (pixels[i - 1] + pixels[i] + pixels[i + 1]) / 3;
    }

    int leftEdge = -1;
    int rightEdge = -1;

    int threshold = 180; 

    // --- FIND LEFT EDGE (dark -> light) ---
    for (int i = 12; i < 118; i++)
    {
        int diff = (int)smoothingFilter[i] - (int)smoothingFilter[i - 1];

        if (diff > threshold)
        {
            leftEdge = i;
            break;
        }
    }

    // --- FIND RIGHT EDGE (light -> dark) ---
    for (int i = 116; i > 10; i--)
    {
        int diff = (int)smoothingFilter[i] - (int)smoothingFilter[i + 1];

        if (diff > threshold)
        {
            rightEdge = i;
            break;
        }
    }

    // --- CENTER CALCULATION ---
    if (leftEdge != -1 && rightEdge != -1 && rightEdge > leftEdge)
    {
        previousCenter = (leftEdge + rightEdge) / 2;
    }
    else if (leftEdge != -1 && rightEdge == -1)
    {
        // Estimate using track width (~40 pixels)
        previousCenter = leftEdge + 20;
    }
    else if (rightEdge != -1 && leftEdge == -1)
    {
        previousCenter = rightEdge - 20;
    }

    // Clamp to valid range
    if (previousCenter < 0) previousCenter = 0;
    if (previousCenter > 127) previousCenter = 127;

    return previousCenter;
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
	if ((sum / 128) < 2000)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
