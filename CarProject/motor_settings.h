/**
 * ******************************************************************************
 * @file    : motor_settings.h
 * @brief   : Motor settings header file
 * @details : Motor setup and functions to control the motors and servo
 *
 * @author Emilio Garcia Rabell & Cristian Morataya
 * @date 4/26/2026
 * ******************************************************************************
 */

#ifndef MOTOR_SETTINGS_H
#define MOTOR_SETTINGS_H

// Function to setup the servo with the correct PWM settings just like the previous lab
void motors_setup(void);

// Function to set the speed of the motor depending on whether it needs the left or right motor
void set_speed_fw(double speed_left, double speed_right);

// Function to set the speed of the motor depending on whether it needs the left or right motor but backwards
void set_speed_bw(double speed_left, double speed_right);

// Function to stop the motors
void stop_motors(void);

// Function to change the servo position
void write_servo(double angle);

#endif