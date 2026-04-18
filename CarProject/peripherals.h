/**
 * ******************************************************************************
 * @file    : PERIPHERALS.h
 * @brief   : OLED display helper for line-scan camera data
 * @details : Mirrors the MATLAB pipeline: raw ? 5-pt smooth ? edge detection,
 *            then renders each stage to the OLED using draw_1d_data / OLED API.
 *
 * @author  Cristian Morataya & Emilio Garcia Rabell
 * ******************************************************************************
 */

#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#include <stdint.h>

/* Which view to render on the single OLED screen */
typedef enum {
    DISPLAY_MODE_RAW    = 0,   /* raw ADC values               */
    DISPLAY_MODE_SMOOTH = 1,   /* 5-point moving average       */
    DISPLAY_MODE_EDGE   = 2,   /* binary edge-detected output  */
} DisplayMode;

/**
 * @brief  Initialise the OLED. Call once at startup before Display_update().
 */
void Display_init(void);

/**
 * @brief  Process one camera frame and push it to the OLED.
 *
 * @param  pixels   Pointer to the 128-element ADC array from Camera_getData().
 * @param  mode     Which processing stage to show on the OLED.
 */
void Display_update(const uint16_t *pixels, DisplayMode mode);

/**
 * @brief  Cycle through RAW ? SMOOTH ? EDGE on each call.
 *         Convenient if you want to rotate views with a button press.
 */
void Display_cycleMode(void);

int select_mode(void);
void s_delay(void);
#endif /* DISPLAY_H */