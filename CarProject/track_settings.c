/**
 * ******************************************************************************
 * @file    : track_settings.c
 * @brief   : Track settings module file
 * @details : Functions for processing track data
 *
 * @author Emilio Garcia Rabell & Cristian Morataya
 * @date 4/26/2026
 * ******************************************************************************
 */

#include "lab5/camera.h"
#include "lab6/timers.h"
#include <ti/devices/msp/msp.h>
#include <sysctl.h>
#include "track_settings.h"
#include "lab2/uart.h"
#include "uart_extras.h"
#include "config.h"

// Algorithm to find the center line, its a weighted average of the pixels and compares it to a threshold
int find_center_line(uint16_t *in)
{
    // Find the minimum and maximum pixel values to calculate the threshold
    uint16_t mn = in[0], mx = in[0];
    for (int i = 1; i < 128; i++)
    {
        if (in[i] < mn)
            mn = in[i];
        if (in[i] > mx)
            mx = in[i];
    }

    // If the difference from the max and min is small then return to the center
    if ((mx - mn) < 150)
    {
        return config.track.center_reference;
    }

    // Find the weighted average of the pixels above the threshold
    uint16_t threshold = (uint16_t)(((uint32_t)mx + mn) / 2u);
    uint32_t weighted_sum = 0;
    uint32_t total_weight = 0;
    int temp_count = 0;

    // Go through the pixels to see if they are above the threshold
    for (int i = 0; i < 128; i++)
    {
        // if the pixel is higher than the threshold then add it to the sum
        if (in[i] > threshold)
        {
            uint16_t weight = in[i] - threshold;
            weighted_sum += (uint32_t)(i * weight);
            total_weight += (uint32_t)weight;
            temp_count++;
        }
    }

    // If 0 then return the center reference otherwise return the weighted average divided by the average position
    if (total_weight == 0)
        return config.track.center_reference;

    return (int)(weighted_sum / total_weight);
}

// Function to determine if the car is on the "carpet" or anything dark really 
int is_carpet(uint16_t *pixels)
{
    uint32_t sum = 0;

    // Go through the pixels and add them up to find the average
    for (int i = 0; i < 128; i++)
    {
        sum += pixels[i];
    }
    //	UART0_printDec(sum/128);
    //  UART0_put("\n\r");
    // If the average is less than 800 then its on the carpet otherwise its not
    if ((sum / 128) < 800)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
