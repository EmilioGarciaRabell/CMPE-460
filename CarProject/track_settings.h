#ifndef CAR_SETTINGS_H
#define CAR_SETTINGS_H

#include <stdint.h>

/**
 * @brief Manages track analysis, including center-line detection 
 * and surface type verification.
 */

/**
 * @brief Processes camera pixel data to find the center line of the track.
 * * Uses a basic smoothing filter and edge detection to determine the track 
 * boundaries. If one edge is lost, it estimates the center based on the 
 * remaining edge.
 * * @param pixels Pointer to an array of 128 uint16_t camera pixel values.
 * @return int   The calculated center position (0-127). Returns the 
 * previous center if no edges are detected.
 */
int find_center_line(uint16_t *pixels);

/**
 * @brief Determines if the car is currently over a dark surface (carpet).
 * * Calculates the average brightness of the sensor data. If the average 
 * falls below a specific threshold, the surface is considered carpet.
 * * @param pixels Pointer to an array of 128 uint16_t camera pixel values.
 * @return int   1 if the surface is carpet (dark), 0 otherwise.
 */
int is_carpet(uint16_t *pixels);

// Global variable for the finish line detection
extern int dark_pixel_count;

#endif /* TRACK_MANAGEMENT_H */