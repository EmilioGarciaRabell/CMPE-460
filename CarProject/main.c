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
#include "peripherals.h"

CarSettings config; // declare structure

int main(void)
{   
		
		// select speed mode
		int mode = select_mode();
		// pass address to configure the mode
		mode_config(&config, mode);
	
    Camera_init();
    DC_EN_init();
    motors_setup(); 
    UART0_init();
		write_servo(config.steer.center_pwm); 
    float last_error = 0.0f;
    float filtered_center = 60.0f;
    const float alpha = 0.45f;

    while (1)
    {
        if (!Camera_isDataReady()) continue;

        uint16_t *pixels = Camera_getData();
       

        // Process Raw Data
			
        int raw_center = find_center_line(pixels);			
				//UART0_printDec(raw_center);
				//UART0_put("\n\r");
			
				if (is_carpet(pixels)) 
        {
            stop_motors(); 
            write_servo(config.steer.center_pwm); 
            Camera_clearDataReady();
            continue; 
        }
				// Finish line stopping. TODO: test it. this works by checking how much black there is
//				if (dark_pixel_count < 60) {
//						stop_motors();
//						write_servo(config.steer.center_pwm);
//						
//						// Stay in this loop forever so the car doesn't move again
//						while(1) {
//								
//						}
//				}
//				
				
        
				
        // Safety: If we lose the line, don't jerk the wheel
        if (raw_center < 2 || raw_center > 122) {
            raw_center = config.track.center_reference;
        }

        // Smooth the Input
        filtered_center = (alpha * (float)raw_center) + ((1.0f - alpha) * filtered_center);

        // Calculate Error (Current - Reference)
        float error = filtered_center - (float)config.track.center_reference;
        float abs_error = fabs(error);

        // Gain Scheduling
        float active_Kp, active_Kd;
        if (abs_error > 12.0f) { // "12" is halfway to your sensor edge
            active_Kp = config.steer.Kp_turn; 
            active_Kd = config.steer.Kd_turn;  
        } else {
            active_Kp = config.steer.Kp; 
            active_Kd = config.steer.Kd;  
        }

        // 5. PD Logic
        float P = active_Kp * error;
        float D = active_Kd * (error - last_error);

        // Servo Output (Standard: PWM increases to turn Right)
        float targetServo = config.steer.center_pwm + (P + D);
        
        // Clamp to hardware limits
        if (targetServo < config.steer.min_pwm) targetServo = config.steer.min_pwm;
        if (targetServo > config.steer.max_pwm) targetServo = config.steer.max_pwm;
        
        write_servo(targetServo);

        // Motor Logic
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