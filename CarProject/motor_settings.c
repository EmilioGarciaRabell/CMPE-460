/**
 * ******************************************************************************
 * @file    : motor_settings.c
 * @brief   : Motor settings module file
 * @details : Motor setup and functions to control the motors and servo
 *
 * @author Emilio Garcia Rabell & Cristian Morataya
 * @date 4/26/2026
 * ******************************************************************************
 */

#include "lab5/camera.h"
#include "lab6/timers.h"
#include <ti/devices/msp/msp.h>
#include <sysctl.h>
#include "motor_settings.h"
#include "config.h"

// Function to setup the servo with the correct PWM settings just like the previous lab
void motors_setup()
{
    uint32_t mclk = (uint32_t)SYSCTL_SYSCLK_getMCLK();

    uint32_t TIMA1Load = (mclk / (9 + 1) / 50) - 1;
    TIMA1_PWM_init(0, TIMA1Load, 9, config.steer.center_pwm);
    uint32_t TIMA0Load = (mclk / (0 + 1) / 10000) - 1;
    TIMA0_PWM_init(0, TIMA0Load, 0, 0.0);
    TIMA0_PWM_init(1, TIMA0Load, 0, 0.0);
    TIMA0_PWM_init(2, TIMA0Load, 0, 0.0);
    TIMA0_PWM_init(3, TIMA0Load, 0, 0.0);

    stop_motors();
}

// Function to set the speed of the motor depending on whether it needs the left or right motor
void set_speed_fw(double speed_right, double speed_left)
{
    TIMA0_PWM_DutyCycle(0, 0.0);
    TIMA0_PWM_DutyCycle(1, speed_right);
    TIMA0_PWM_DutyCycle(2, 0.0);
    TIMA0_PWM_DutyCycle(3, speed_left);
}

// Function to set the speed of the motor depending on whether it needs the left or right motor but backwards
void set_speed_bw(double speed_right, double speed_left)
{
    TIMA0_PWM_DutyCycle(0, speed_right);
    TIMA0_PWM_DutyCycle(1, 0.0);
    TIMA0_PWM_DutyCycle(2, speed_left);
    TIMA0_PWM_DutyCycle(3, 0.0);
}


// Function to stop the motors
void stop_motors()
{
    TIMA0_PWM_DutyCycle(0, 0.0);
    TIMA0_PWM_DutyCycle(1, 0.0);
    TIMA0_PWM_DutyCycle(2, 0.0);
    TIMA0_PWM_DutyCycle(3, 0.0);
}

// Function to change the servo position
void write_servo(double angle)
{
    TIMA1_PWM_DutyCycle(0, angle);
}
