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
/*
Servo notes:
    - 95 is the farthest the servo will move to the right
    - 90 is the farthest it moves to the left
*/

/* Simple, balanced configuration */
CarSettings config = {
    .steer = {
        .center_pwm = 92.5f,
        .min_pwm = 90.0f,    
        .max_pwm = 95.0f,    
        .deadzone = 4,
        .Kp = 0.02f, // 4/10/26 0.085 was a good value
        .Ki = 0.000001f,
        .Kd = 0.1f,
        .i_limit = 10.0f
    },
    .drive = {
        .base_speed = 37.0,
        .speed_sensitivity = 0.2,
        .carpet_thresh = 2000
    },
    .track = {
        .center_reference = 64
 
   }
};

int main(void)
{   
    /* Hardware Initialization */
    Camera_init();
    DC_EN_init();
    motors_setup(); 
		UART0_init();

    float integral = 0.0f;
    float last_error = 0.0f;

    while (1)
    {
			

        /* Synchronization with camera sensor */
        if (!Camera_isDataReady()) continue;

        uint16_t *pixels = Camera_getData();
        
        /* Emergency stop on carpet */
        if (is_carpet(pixels)) 
        {
            stop_motors(); 
            write_servo(config.steer.center_pwm); 
            integral = 0;
            last_error = 0;
            Camera_clearDataReady();
            continue; 
        }

        /* Line detection and center calculation */
        int current_center = find_center_line(pixels);
				
				UART0_printDec(current_center);
				UART0_put("\r\n");
        
        /* Default to center if line is lost */
        if (current_center <= 0 || current_center >= 127) {
            current_center = 64;
        }

        /* Error calculation */
        float error = (float)(config.track.center_reference - current_center);
				if (fabs(error) < (float)config.steer.deadzone) {
            error = 0.0f;
        }
        /* P: Proportional - Immediate correction */
        float P = config.steer.Kp * error;

        /* I: Integral - Corrects long-term drift */
        integral += error;
        if (integral > config.steer.i_limit)  integral = config.steer.i_limit;
        if (integral < -config.steer.i_limit) integral = -config.steer.i_limit;
        float I = config.steer.Ki * integral;

        /* D: Derivative - Dampens oscillations */
        float D = config.steer.Kd * (error - last_error);

        /* Calculate final servo output with safety bounds */
        float targetServo = config.steer.center_pwm + P + D;
				
//				UART0_printDec(targetServo);
//				UART0_put("\r\n");
        if (targetServo < config.steer.min_pwm) targetServo = config.steer.min_pwm;
        if (targetServo > config.steer.max_pwm) targetServo = config.steer.max_pwm;
        
        write_servo(targetServo);

        /* Linear speed reduction based on error magnitude */
        float abs_error = (error < 0) ? -error : error;
        double current_speed = config.drive.base_speed - (abs_error * config.drive.speed_sensitivity);
        
        /* Ensure motors always have minimum power */
        if (current_speed < 12.0) current_speed = 12.0;
				
				float diff_offset = error * 0.3f; // Adjust 0.25f to change how aggressive the diff is

				double left_speed = current_speed + diff_offset;
				double right_speed = current_speed - diff_offset;

				// Safety: Clamp the speeds so they don't go negative or exceed 100%
				if (left_speed < 10.0) left_speed = 10.0;
				if (right_speed < 10.0) right_speed = 10.0;
				
				
				
        set_speed_fw(right_speed , left_speed + 0.5 );

        /* Store state for next iteration */
        last_error = error;
        Camera_clearDataReady();
    }
}