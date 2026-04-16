#include <ti/devices/msp/msp.h>
#include <sysctl.h>
#include <math.h>
#include "lab5/camera.h"
#include "lab6/timers.h"
#include "lab2/uart.h"
#include "uart_extras.h"
#include "track_settings.h"
#include "car_setup.h"
#include "config.h"
#include "motor_settings.h"

CarSettings config = {
    .steer = {
        .center_pwm = 92.5f,
        .min_pwm = 86.0f,    
        .max_pwm = 99.0f,    
        .deadzone = 4,
        .Kp = 0.055f,       // Slightly increased to compensate for squared error
        .Kd = 0.25f,        
        .Ki = 0.0f,
        .i_limit = 10.0f
    },
    .drive = {
        .base_speed = 38.0,
        .speed_sensitivity = 0.2f,
        .diff_k = 0.20f,
        .min_speed = 12.0f
    },
    .track = {
        .center_reference = 73  // Hardware center from your UART data
    }
};

int main(void)
{   
    Camera_init();
    DC_EN_init();
    motors_setup(); 
    UART0_init();

    float last_error = 0.0f;
    float filtered_error = 0.0f;
    float alpha = 0.80f; 
    static int last_valid_center = 73;

    while (1)
    {
        if (!Camera_isDataReady()) continue;

        uint16_t *pixels = Camera_getData();
        int current_center = find_center_line(pixels);
        
        //  Map actual sensor range (15-115) to full theoretical range (0-127)
        float mapped_center;
        if (current_center <= 0 || current_center >= 127) {
            mapped_center = (last_valid_center < config.track.center_reference) ? 0 : 127;
        } else {
            // Stretch the 15-115 range to 0-127 for full control authority
            if (current_center < 15) mapped_center = 0;
            else if (current_center > 115) mapped_center = 127;
            else {
                mapped_center = (float)(current_center - 15) * (127.0f / 100.0f);
            }
            last_valid_center = current_center;
        }

        // Error Calculation & Filtering
        float raw_error = (float)(config.track.center_reference - mapped_center);
        if (fabs(raw_error) < (float)config.steer.deadzone) raw_error = 0.0f;

        filtered_error = (alpha * raw_error) + ((1.0f - alpha) * filtered_error);

        //  PID Logic (Squared P for smoother center)
        float error_sq = (filtered_error * filtered_error) / 64.0f;
        if (filtered_error < 0) error_sq = -error_sq;
        
        float P = config.steer.Kp * error_sq;
        float D = config.steer.Kd * (filtered_error - last_error);
        
        float targetServo = config.steer.center_pwm + P + D;

        // Servo Clamping
        if (targetServo < config.steer.min_pwm) targetServo = config.steer.min_pwm;
        if (targetServo > config.steer.max_pwm) targetServo = config.steer.max_pwm;
        write_servo(targetServo);

        // Motor Control
        double target_speed = config.drive.base_speed - (fabs(filtered_error) * config.drive.speed_sensitivity);
        if (target_speed < config.drive.min_speed) target_speed = config.drive.min_speed;
        
        float diff_offset = filtered_error * config.drive.diff_k; 
        set_speed_fw(target_speed - diff_offset, target_speed + diff_offset);

        last_error = filtered_error;
        Camera_clearDataReady();
    }
}