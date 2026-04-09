#include "lab5/camera.h"
#include "lab6/timers.h"
#include <ti/devices/msp/msp.h>
#include <sysctl.h>
#include "track_settings.h"
#include "car_setup.h"
#include "config.h"
#include "motor_settings.h"

CarSettings config = {
    .steer = {
        .center_pwm = 92.5f,
        .min_pwm = 87.5f,    
        .max_pwm = 95.0f,    
        .deadzone = 3,
        .Kp = 0.0333f,
        .Ki = 0.00f,
        .Kd = 0.15f,
        .i_limit = 25.0f
    },
    .drive = {
        .base_speed =30.0,
        .speed_sensitivity = 0.15f,
        .carpet_thresh = 2000
    },
    .track = {
        .center_reference = 64
    }
};

int main(void)
{	
    Camera_init();
    DC_EN_init();
    motors_setup(); 

    float integral = 0.0f;
    float last_error = 0.0f;

    while (1)
    {
        if (!Camera_isDataReady()) continue;

        uint16_t *pixels = Camera_getData();
        
        if (is_carpet(pixels)) 
        {
            stop_motors(); 
            write_servo(config.steer.center_pwm); 
            integral = 0;
            last_error = 0;
            Camera_clearDataReady();
            continue; 
        }

        int current_center = find_center_line(pixels);
        
				float error = (float)(config.track.center_reference - current_center);
				
        if (error > -config.steer.deadzone && error < config.steer.deadzone) {
            error = 0.0f;
        }

        // P term
        float P = config.steer.Kp * error;

        // I term with simple saturation limit
        integral += error;
        if (integral > config.steer.i_limit)  integral = config.steer.i_limit;
        if (integral < -config.steer.i_limit) integral = -config.steer.i_limit;
        float I = config.steer.Ki * integral;

        // D term (simple difference)
        float D = config.steer.Kd * (error - last_error);
        last_error = error;

        // Total output
        float targetServo = config.steer.center_pwm + (P + I + D);

        // Hardware safety clamp
        if (targetServo < config.steer.min_pwm) targetServo = config.steer.min_pwm;
        if (targetServo > config.steer.max_pwm) targetServo = config.steer.max_pwm;
        write_servo(targetServo);

        // Simple speed control
        float error_mag = (error < 0) ? -error : error;
        double current_speed = config.drive.base_speed - (error_mag * config.drive.speed_sensitivity);
        
        set_speed_fw(current_speed, current_speed);

        Camera_clearDataReady();
    }
}