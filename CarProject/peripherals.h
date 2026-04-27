/**
 * ******************************************************************************
 * @file    : peripherals.h
 * @brief   : OLED helper header file that displays camera data and mode selection
 * @details : Functions to initialize the OLED display, update it with camera data, and select the car mode at the start
 *
 * @author Cristian Morataya & Emilio Garcia Rabell
 * @date 4/27/2026
 * ******************************************************************************
 */

#ifndef PERIPHERALS_H
#define PERIPHERALS_H
#include <stdint.h>

// Enum for the different display modes
typedef enum
{
    DISPLAY_MODE_RAW = 0,
    DISPLAY_MODE_SMOOTH = 1,
    DISPLAY_MODE_EDGE = 2,
} DisplayMode;

// Function to initialize the display
void Display_init(void);

// Function to update the display with the data from the camera
void Display_update(const uint16_t *pixels, DisplayMode mode);

// Function to cycle through the display modes
void Display_cycleMode(void);

// Function to select the mode at the start of the car
int select_mode(void);

// Delay helper function
void s_delay(void);

#endif