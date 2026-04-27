/**
 * ******************************************************************************
 * @file    : peripherals.c
 * @brief   : OLED helper file that displays camera data and mode selection
 * @details : Functions to initialize the OLED display, update it with camera data, and select the car mode at the start
 *
 * @author Cristian Morataya & Emilio Garcia Rabell
 * @date 4/27/2026
 * ******************************************************************************
 */

#include "peripherals.h"
#include "oled.h"
#include <string.h>
#include <stdint.h>
#include "lab5/switches.h"

// Maximum ADC value for the camera
#define ADC_MAX 0x3FFFu
// Window size for the moving average filter
#define SMOOTH_WINDOW 5
#define DELAY_COUNT 2000000

// Variables for the state of the display
static DisplayMode s_mode = DISPLAY_MODE_EDGE;
static uint16_t s_smooth[128];
static uint16_t s_edge[128];

// Function for a smoothing filter on the camera data
static void smooth_data(const uint16_t *in, uint16_t *out)
{
    // Divide the window by 2
    int half = SMOOTH_WINDOW / 2;
    int i, j;

    // Loop through the pixels and find the average of the pixels
    for (i = 0; i < 128; i++)
    {
        uint32_t sum = 0;
        int count = 0;
        // Loop through the window and add the pixels to the sum
        for (j = i - half; j <= i + half; j++)
        {
            if (j >= 0 && j < 128)
            {
                sum += in[j];
                count++;
            }
        }
        // Output is the average of the pixels
        out[i] = (uint16_t)(sum / count);
    }
}

// Get the edge data with comparing to a threshold
static void edge_data(const uint16_t *in, uint16_t *out)
{
    uint16_t mn = in[0], mx = in[0];
    int i;
    // Loop through the pixels
    for (i = 1; i < 128; i++)
    {
        // If the pixel is less than the minimum then update the minimum
        if (in[i] < mn)
            mn = in[i];
        if (in[i] > mx)
            mx = in[i];
    }
    // The threshold is the average of the minimum and maximum
    uint16_t threshold = (uint16_t)(((uint32_t)mx + mn) / 2u);

    // Loop through again and compare to the threshold
    for (i = 0; i < 128; i++)
        out[i] = (in[i] > threshold) ? ADC_MAX : 0u;
}

// Initialize the display and a message to show that the camera is ready
void Display_init(void)
{
    OLED_Init();
    OLED_Print(1, 1, "Camera Ready");
}

// Function to update the display with the data from the camera
void Display_update(const uint16_t *pixels, DisplayMode mode)
{
    // Get both the smoothed and edge data
    smooth_data(pixels, s_smooth);
    edge_data(s_smooth, s_edge);

    // Depending on the mode display the corresponding data on the OLED
    switch (mode)
    {
    case DISPLAY_MODE_RAW:
        OLED_DisplayCameraData((uint16_t *)pixels);
        break;
    case DISPLAY_MODE_SMOOTH:
        OLED_DisplayCameraData(s_smooth);
        break;
    case DISPLAY_MODE_EDGE:
    default:
        OLED_DisplayCameraData(s_edge);
        break;
    }
}

// Function to cycle through the display modes
void Display_cycleMode(void)
{
    s_mode = (DisplayMode)((s_mode + 1) % 3);
}

// Delay helper function
void s_delay(void)
{
    for (volatile long i = 0; i < DELAY_COUNT; i++)
        ;
}

// Function to select the mode at the start of the car
int select_mode()
{
    // Initialize the OLED and the switches
    OLED_Init();
    S1_init();
    S2_init();

    // The different modes
    char *modes[] = {"PRO MODE", "MID MODE", "ECO MODE"};
    int current_selection = 0;
    int num_modes = 3;
    int redraw = 1;

    while (1)
    {
        if (redraw)
        {
            // Clear the display and print the mode selection menu
            OLED_display_clear();

            // Print out the modes
            for (int i = 0; i < num_modes; i++)
            {
                int display_row = i + 1;

                if (i == current_selection)
                {
                    // Pointer to show what selection it is on
                    OLED_Print(display_row, 1, "->");
                }

                OLED_Print(display_row, 4, modes[i]);
            }
            redraw = 0;
        }

        // If switch 1 is pressed then move the arrow down
        if (S1_pressed())
        {
            current_selection = (current_selection + 1) % num_modes;
            redraw = 1;
            s_delay();
        }

        // If switch 2 is pressed then select the mode
        if (S2_pressed())
        {
            s_delay();
            OLED_display_clear();
            return current_selection + 1;
        }
    }
}