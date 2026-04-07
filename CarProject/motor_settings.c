#include "lab5/camera.h"
#include "lab6/timers.h"
#include <ti/devices/msp/msp.h>
#include <sysctl.h>
#include "motor_settings.h"
#include "config.h"

void motors_setup() {
    // Set up timer for the servo (50Hz)
    uint32_t TIMA1Load = (SYSCTL_SYSCLK_getMCLK() / (9 + 1) / 50) - 1;
    TIMA1_PWM_init(0, TIMA1Load, 9, config.steer.center_pwm);

    // Set up timer for the DC motors (10kHz)
    uint32_t TIMA0Load = (SYSCTL_SYSCLK_getMCLK() / (0 + 1) / 10000) - 1;
    
    // Initialize all 4 PWM channels to 0.0 initially
    TIMA0_PWM_init(0, TIMA0Load, 0, 0.0);
    TIMA0_PWM_init(1, TIMA0Load, 0, 0.0);
    TIMA0_PWM_init(2, TIMA0Load, 0, 0.0);
    TIMA0_PWM_init(3, TIMA0Load, 0, 0.0);
}

void set_speed_fw(double speed_left, double speed_right) {
    TIMA0_PWM_DutyCycle(0, 0.0);
    TIMA0_PWM_DutyCycle(1, speed_left);
    TIMA0_PWM_DutyCycle(2, 0.0);
    TIMA0_PWM_DutyCycle(3, speed_right);
}

void set_speed_bw(double speed_left, double speed_right) {
    TIMA0_PWM_DutyCycle(0, speed_left);
    TIMA0_PWM_DutyCycle(1, 0.0);
    TIMA0_PWM_DutyCycle(2, speed_right);
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

// Fixed: Now accepts error and current_filtered as inputs
double calculate_steering_angle(int error, float current_filtered) {
    // Pull mapping values from the config struct
    float in_min  = config.steer.error_min; 
    float in_max  = config.steer.error_max; 
    float out_min = config.steer.min_pwm;  
    float out_max = config.steer.max_pwm;  

    float targetServo = (float)(error - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;

    // Apply the alpha filter using the previous filtered value passed from main
    return (double)((targetServo * config.steer.alpha) + (current_filtered * (1.0f - config.steer.alpha)));
}

// Fixed: Now accepts error as input and actually RETURNS the speed
double calculate_speed(int error) {
    int absError = (error < 0) ? -error : error;
    
    // Pull speed values from the config struct
    double base = config.drive.base_speed;
    double boost = config.drive.max_boost;
    
    double speed = base + (boost * (1.0 - (double)absError / 64.0));
    
    return speed; // Must return the value or the car won't move!
}