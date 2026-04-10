#include "lab5/camera.h"
#include "lab6/timers.h"
#include <ti/devices/msp/msp.h>
#include <sysctl.h>
#include "motor_settings.h"
#include "config.h"

#include "lab5/camera.h"
#include "lab6/timers.h"
#include <ti/devices/msp/msp.h>
#include "sysctl.h"
#include "motor_settings.h"
#include "config.h"

void motors_setup() {
    uint32_t mclk = (uint32_t)SYSCTL_SYSCLK_getMCLK();

    // Set up timer for the servo (50Hz)
    uint32_t TIMA1Load = (mclk / (9 + 1) / 50) - 1;
    TIMA1_PWM_init(0, TIMA1Load, 9, config.steer.center_pwm);

    // Set up timer for the DC motors (10kHz)
    uint32_t TIMA0Load = (mclk / (0 + 1) / 10000) - 1;
    TIMA0_PWM_init(0, TIMA0Load, 0, 0.0); 
    TIMA0_PWM_init(1, TIMA0Load, 0, 0.0); 
    TIMA0_PWM_init(2, TIMA0Load, 0, 0.0); 
    TIMA0_PWM_init(3, TIMA0Load, 0, 0.0); 
    
    stop_motors();
}

void set_speed_fw(double speed_right, double speed_left) {
    TIMA0_PWM_DutyCycle(0, 0.0);
    TIMA0_PWM_DutyCycle(1, speed_right);
    TIMA0_PWM_DutyCycle(2, 0.0);
    TIMA0_PWM_DutyCycle(3, speed_left);
}

void set_speed_bw(double speed_right, double speed_left) {
    TIMA0_PWM_DutyCycle(0, speed_right);
    TIMA0_PWM_DutyCycle(1, 0.0);
    TIMA0_PWM_DutyCycle(2, speed_left);
    TIMA0_PWM_DutyCycle(3, 0.0);
}

void stop_motors() {
    TIMA0_PWM_DutyCycle(0, 0.0);
    TIMA0_PWM_DutyCycle(1, 0.0);
    TIMA0_PWM_DutyCycle(2, 0.0);
    TIMA0_PWM_DutyCycle(3, 0.0);
}

void write_servo(double angle) {
    TIMA1_PWM_DutyCycle(0, angle);
}

double calculate_speed(float error) {
    float abs_error = (error < 0) ? -error : error;
    
    // Reduces base speed as error increases
    double speed = config.drive.base_speed - (abs_error * config.drive.speed_sensitivity);
    
    // Ensure speed doesn't drop below zero
    if (speed < 0) speed = 0;
    
    return speed;
}