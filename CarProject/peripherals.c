 /**
 * ******************************************************************************
 * @file    : peripherals.c
 * @brief   : OLED display helper for line-scan camera data
 * @details : Replicates the MATLAB pipeline from plot_cameras_serial.m:
 *              1. Raw pixel data
 *              2. 5-point moving average (smoothData)
 *              3. Midpoint-threshold binary edge detection (edgeData)
 *            Each stage feeds draw_1d_data() / OLED_DisplayCameraData() so
 *            the result appears on the 128x64 OLED in real time.
 *
 * @author  Cristian Morataya & Emilio Garcia Rabell
 * ******************************************************************************
 */

#include "peripherals.h"
#include "oled.h"   /* adjust path to wherever oled.h lives in your tree */
#include <string.h>
#include <stdint.h>
#include "lab5/switches.h"
/* -- constants ---------------------------------------------------------------- */
#define NUM_PIXELS     128
#define ADC_MAX        0x3FFFu   /* 14-bit ADC ceiling used by draw_1d_data     */
#define SMOOTH_WINDOW  5         /* must be odd; mirrors movmean(data, 5)        */
// delay constant
#define DELAY_COUNT 2000000

/* -- module state ------------------------------------------------------------- */
static DisplayMode  s_mode          = DISPLAY_MODE_EDGE;   /* default: edge view */
static uint16_t     s_smooth[NUM_PIXELS];
static uint16_t     s_edge[NUM_PIXELS];

/* -- private helpers ----------------------------------------------------------- */

/**
 * @brief  5-point symmetric moving average, matching MATLAB movmean(data,5).
 *         Edge pixels use a smaller window (same as MATLAB's default shrink).
 *
 * @param  in   Source array  (128 x uint16_t, raw ADC)
 * @param  out  Destination   (128 x uint16_t, smoothed)
 */
static void smooth_data(const uint16_t *in, uint16_t *out)
{
    int half = SMOOTH_WINDOW / 2;   /* = 2 */
    int i, j;
    for (i = 0; i < NUM_PIXELS; i++)
    {
        uint32_t sum   = 0;
        int      count = 0;
        for (j = i - half; j <= i + half; j++)
        {
            if (j >= 0 && j < NUM_PIXELS)
            {
                sum += in[j];
                count++;
            }
        }
        out[i] = (uint16_t)(sum / count);
    }
}

/**
 * @brief  Midpoint binary threshold — mirrors edgeData() in the .m file.
 *         Pixels above threshold ? ADC_MAX (logic 1), others ? 0 (logic 0).
 *         Output is scaled to the full ADC range so draw_1d_data() maps it
 *         correctly to the top/bottom of the 64-pixel OLED height.
 *
 * @param  in   Smoothed array  (128 x uint16_t)
 * @param  out  Binary array    (128 x uint16_t, values are 0 or ADC_MAX)
 */
static void edge_data(const uint16_t *in, uint16_t *out)
{
    /* find min / max */
    uint16_t mn = in[0], mx = in[0];
    int i;
    for (i = 1; i < NUM_PIXELS; i++)
    {
        if (in[i] < mn) mn = in[i];
        if (in[i] > mx) mx = in[i];
    }
    uint16_t threshold = (uint16_t)(((uint32_t)mx + mn) / 2u);

    for (i = 0; i < NUM_PIXELS; i++)
        out[i] = (in[i] > threshold) ? ADC_MAX : 0u;
}

/* -- public API --------------------------------------------------------------- */

void Display_init(void)
{
    OLED_Init();
    /* Optionally show a splash — comment out if you want a clean start */
    OLED_Print(1, 1, "Camera Ready");
}

void Display_update(const uint16_t *pixels, DisplayMode mode)
{
    smooth_data(pixels, s_smooth);
    edge_data(s_smooth, s_edge);

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



void Display_cycleMode(void)
{
    s_mode = (DisplayMode)((s_mode + 1) % 3);
}

void s_delay(void)
{
    for (volatile long i = 0; i < DELAY_COUNT; i++)
        ;
}


int select_mode() {
    OLED_Init(); // Ensure display is ready
    S1_init();
    S2_init();

    char *modes[] = { "PRO MODE", "MID MODE", "ECO MODE" };
    int current_selection = 0;
    int num_modes = 3;
    int redraw = 1;

    while(1) {
        if (redraw) {
            OLED_display_clear();
            
            for (int i = 0; i < num_modes; i++) {
                // ROW: Use i+1 because the library math is (x-1). 
                // i=0 becomes row 1, i=1 becomes row 2, etc.
                int display_row = i + 1; 
                
                if (i == current_selection) {
                    // COL: Use 1 for the arrow (first character slot)
                    OLED_Print(display_row, 1, "->"); 
                }
                
                // COL: Use 4 for text (starts at the 4th character slot)
                OLED_Print(display_row, 4, modes[i]); 
            }
            redraw = 0;
        }

        if (S1_pressed()) {
            current_selection = (current_selection + 1) % num_modes;
            redraw = 1;
            s_delay(); // Robust debounce for the race-day switches
        }

        if (S2_pressed()) {
            s_delay();
            OLED_display_clear();
            return current_selection + 1; // Returns 1, 2, or 3
        }
    }
}