/**
 * ******************************************************************************
 * @file    : car_setup.h
 * @brief   : Car setup module header file
 * @details : Initialization and configuration of car settings
 *
 * @author Emilio Garcia Rabell & Cristian Morataya
 * @date 4/26/2026
 * ******************************************************************************
 */

#ifndef CAR_SETUP_H
#define CAR_SETUP_H
#include "config.h"
#include <ti/devices/msp/msp.h>

// Initialize the DC EN pins to drive the motors
void DC_EN_init(void);

// Function to select the different modes for the car
void mode_config(CarSettings *c, int mode);
#endif 