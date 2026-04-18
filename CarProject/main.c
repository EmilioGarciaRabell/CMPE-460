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

CarSettings config = {
    .steer = {
        .center_pwm = 92.5f,
        .min_pwm = 90.0f,    // Restored your full range for tight turns
        .max_pwm = 95.0f,   
        .deadzone = 2,       // Low deadzone since we have a smoothing filter
        
        // PID GAINS: Since error is now max 20, these need to be higher than before
        .Kp = 0.055f,         // Proportional
        .Kd = 0.31f,         // Derivative
        .Kp_turn = 0.07f,    // Aggressive curve entry
        .Kd_turn = 0.28f,    
        .Ki = 0.0f,
        .i_limit = 10.0f
    },
    .drive = {
        .base_speed = 48.0,
        .speed_sensitivity = 0.47f,
        .diff_k = 0.18f,
        .min_speed = 23.0f
    },
    .track = {
        .center_reference = 64 // UPDATED: Your physical center is 60
    }
};

int main(void)
{   
    Camera_init();
    DC_EN_init();
    motors_setup(); 
    UART0_init();

    float integral = 0.0f;
    float last_error = 0.0f;
    float filtered_center = 60.0f;
    const float alpha = 0.45f;

    while (1)
    {
        if (!Camera_isDataReady()) continue;

        uint16_t *pixels = Camera_getData();
        
        if (is_carpet(pixels)) 
        {
            stop_motors(); 
            write_servo(config.steer.center_pwm); 
            integral = 0; last_error = 0;
            Camera_clearDataReady();
            continue; 
        }

        // 1. Process Raw Data
        int raw_center = find_center_line(pixels);			
				
        UART0_printDec(raw_center);
				UART0_put("\n\r");
				
        // Safety: If we lose the line, don't jerk the wheel
        if (raw_center < 2 || raw_center > 122) {
            raw_center = config.track.center_reference;
        }

        // 2. Smooth the Input
        filtered_center = (alpha * (float)raw_center) + ((1.0f - alpha) * filtered_center);

        // 3. Calculate Error (Current - Reference)
        // If line is at 80 (Right), error is 20.
        // If line is at 40 (Left), error is -20.
        float error = filtered_center - (float)config.track.center_reference;
        float abs_error = fabs(error);

        // 4. Gain Scheduling
        float active_Kp, active_Kd;
        if (abs_error > 12.0f) { // "12" is halfway to your sensor edge
            active_Kp = config.steer.Kp_turn; 
            active_Kd = config.steer.Kd_turn;  
        } else {
            active_Kp = config.steer.Kp; 
            active_Kd = config.steer.Kd;  
        }

        // 5. PID Logic
        float P = active_Kp * error;
        
        integral += error;
        if (integral > config.steer.i_limit)  integral = config.steer.i_limit;
        if (integral < -config.steer.i_limit) integral = -config.steer.i_limit;
        float I = config.steer.Ki * integral;

        float D = active_Kd * (error - last_error);

        // 6. Servo Output (Standard: PWM increases to turn Right)
        float targetServo = config.steer.center_pwm + (P + I + D);
        
        // Clamp to hardware limits
        if (targetServo < config.steer.min_pwm) targetServo = config.steer.min_pwm;
        if (targetServo > config.steer.max_pwm) targetServo = config.steer.max_pwm;
        
        write_servo(targetServo);

        // 7. Motor Logic
        double current_speed = config.drive.base_speed - (abs_error * config.drive.speed_sensitivity);
        if (current_speed < config.drive.min_speed) current_speed = config.drive.min_speed;

        // Differential: if error is positive (turning right), 
        // slow down Right motor, speed up Left motor.
        float diff_offset = error * config.drive.diff_k;

        set_speed_fw(current_speed + diff_offset, current_speed - diff_offset);

        // Update state
        last_error = error;
        Camera_clearDataReady();
    }
}