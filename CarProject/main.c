#include "lab5/camera.h"
#include "lab6/timers.h"
#include <ti/devices/msp/msp.h>
#include <sysctl.h>
#include "track_settings.h"
#include "car_setup.h"
#include "config.h"
#include "motor_settings.h"
#include <math.h>
#include "lab2/uart.h"
#include "uart_extras.h"

// Configuration settings for car steering, speed, and track reference
CarSettings config = {
    .steer = {
        .center_pwm = 92.5f,
        .min_pwm = 86.0f,    
        .max_pwm = 99.0f,    
        .deadzone = 4
			,
        .Kp = 0.02,        
        .Kd = 0.38,         
        .Kp_turn = 0.095f,   
        .Kd_turn = 0.08f,    
        .Ki = 0.0000001f,
        .i_limit = 10.0f
    },
    .drive = {
        .base_speed = 40.0,
        .speed_sensitivity = 0.22,//0.4f,
        .diff_k = 0.15f,
        .min_speed = 12.0f,
        .carpet_thresh = 2000
    },
    .track = {
        .center_reference = 64
    }
};

int main(void)
{   
    // Initialize hardware peripherals
    Camera_init();
    DC_EN_init();
    motors_setup(); 
    UART0_init();

    float integral = 0.0f;
    float last_error = 0.0f;

    while (1)
    {
        // Wait for new camera frame
        if (!Camera_isDataReady()) continue;

        uint16_t *pixels = Camera_getData();
        
        // Safety check for off-track/carpet detection
        if (is_carpet(pixels)) 
        {
            stop_motors(); 
            write_servo(config.steer.center_pwm); 
            integral = 0;
            last_error = 0;
            Camera_clearDataReady();
            continue; 
        }

        // Process pixel data to find line position
        int current_center = find_center_line(pixels);
        
        // Revert to center reference if line is lost
        if (current_center <= 0 || current_center >= 127) {
            current_center = config.track.center_reference;
        }

        // Calculate steering error
        float error = (float)(config.track.center_reference - current_center);
        float abs_error = fabs(error);

        // Apply deadzone to prevent jitter on straights
        if (abs_error < (float)config.steer.deadzone) {
            error = 0.0f;
        }
        
        float active_Kp, active_Kd, active_diff_k;
				UART0_printDec(current_center);
				UART0_put("\r\n");
        // Dynamic Gain Scheduling: Switch parameters based on turn severity
        if (abs_error > 35.0f) 
        {
            // Aggressive turning parameters for curves
            active_Kp = config.steer.Kp_turn; 
            active_Kd = config.steer.Kd_turn;  
            active_diff_k = 0.14f; 
        } 
        else 
        {
            // Stable parameters for straight sections
            active_Kp = config.steer.Kp; 
            active_Kd = config.steer.Kd;  
            active_diff_k = 0.0f; 
        }
        
        // PID calculation
        float P = active_Kp * error;

        integral += error;
        if (integral > config.steer.i_limit)  integral = config.steer.i_limit;
        if (integral < -config.steer.i_limit) integral = -config.steer.i_limit;
        float I = config.steer.Ki * integral;

        float D = active_Kd * (error - last_error);

        // Apply calculated output to steering servo
        float targetServo = config.steer.center_pwm + P + I + D;
        
        // Clamp servo values to physical hardware limits
        if (targetServo < config.steer.min_pwm) targetServo = config.steer.min_pwm;
        if (targetServo > config.steer.max_pwm) targetServo = config.steer.max_pwm;
        
        write_servo(targetServo);

        // Adjust base speed based on error (slow down in turns)
        double current_speed = config.drive.base_speed - (abs_error * config.drive.speed_sensitivity);
        
        if (current_speed < config.drive.min_speed) current_speed = config.drive.min_speed;
        
        // Calculate differential speed for motors
        float diff_offset = error * active_diff_k; 

        double left_speed = current_speed + diff_offset;
        double right_speed = current_speed - diff_offset;

        // Ensure motors maintain minimum operational power
        if (left_speed < 10.0) left_speed = 10.0;
        if (right_speed < 10.0) right_speed = 10.0;
        
        // Update motor speeds
        set_speed_fw(right_speed, left_speed + 0.5);

        // Update state for next loop iteration
        last_error = error;
        Camera_clearDataReady();
    }
}