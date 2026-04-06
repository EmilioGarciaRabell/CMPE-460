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
	// Set the center to 64 because its in the middle of 128 and is just a reference point for starting
	static int previousCenter = 64;

	// To reduce noise a smoothing filter can be used to find edges easier
	uint16_t smoothingFilter[128];

	// Make the first and last value of the filter to get the range
	smoothingFilter[0] = pixels[0];
	smoothingFilter[128 - 1] = pixels[128 - 1];

	// Loop through and apply the filter to the pixels by getting the average of the current pixel and the average on both sides of it
	for (int i = 1; i < 128 - 1; i++)
	{
		// Calculate the average
		smoothingFilter[i] = (pixels[i - 1] + pixels[i] + pixels[i + 1]) / 3;
	}

	// Set the left and right as a placeholder before finding the actual edges
	int leftEdge = -1;
	int rightEdge = -1;

	// Loop through the filter to find the left edge
	for (int i = 11; i < 128 - 10; i++)
	{
		if ((int)smoothingFilter[i] - (int)smoothingFilter[i - 1] > 200)
		{
			// Set the value on the left
			leftEdge = i;
			break;
		}
	}
	// Loop through the filter to find the right edge
	for (int i = 128 - 11; i >= 10; i--)
	{
		if ((int)smoothingFilter[i] - (int)smoothingFilter[i + 1] > 200)
		{
			// Set the value on the right
			rightEdge = i;
			break;
		}
	}

	// Check if either the car has veered off to the left or right and then correct or if its fine
	if (leftEdge != -1 && rightEdge != -1 && rightEdge > leftEdge)
	{
		previousCenter = (leftEdge + rightEdge) / 2;
	}
	else if (leftEdge != -1 && rightEdge == -1)
	{
		previousCenter = leftEdge + 20;
	}
	else if (rightEdge != -1 && leftEdge == -1)
	{
		previousCenter = rightEdge - 20;
	}

	// Return the value in the center
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
	if ((sum / 128) < 500)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
