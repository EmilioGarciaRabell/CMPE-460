/**
 * ******************************************************************************
 * @file    : track_settings.h
 * @brief   : Track settings header module file
 * @details : Functions for processing track data
 *
 * @author Emilio Garcia Rabell & Cristian Morataya
 * @date 4/26/2026
 * ******************************************************************************
 */

#ifndef CAR_SETTINGS_H
#define CAR_SETTINGS_H

// Algorithm to find the center line, its a weighted average of the pixels and compares it to a threshold
int find_center_line(uint16_t *pixels);

// Function to check if the car is on the carpet or not
int is_carpet(uint16_t *pixels);

#endif