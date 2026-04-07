#include "lab5/camera.h"
#include "lab6/timers.h"
#include <ti/devices/msp/msp.h>
#include <sysctl.h>
#include "track_settings.h"
#include "car_setup.h"
#include "config.h"
#include "motor_settings.h"

// Define the actual values for the central config
CarSettings config = {
    .steer = {
        .center_pwm = 92.5f,
        .min_pwm = 94.5f,    
        .max_pwm = 90.0f,    
        .error_min = -64.0f,
        .error_max = 64.0f,
        .alpha = 0.8f,
        .deadzone = 5
    },
    .drive = {
        .base_speed = 27.0,
        .max_boost = 10.0,
        .carpet_thresh = 2000
    },
    .track = {
        .edge_threshold = 200,
        .single_edge_off = 10,
        .center_reference = 64
    }
};

int main(void)
{
    // Initialize Hardware
    Camera_init();
    DC_EN_init();
    motors_setup(); 

    static float filteredServo = 92.5f;

    while (1)
    {
        // Wait for camera data
        if (!Camera_isDataReady()) continue;

        uint16_t *pixels = Camera_getData();
        
        // Stop if it encounters carpet
        if (is_carpet(pixels)) 
        {
            stop_motors();             // Motor module function
            write_servo(config.steer.center_pwm); 
            Camera_clearDataReady();
            continue; 
        }

        // Track sensing
        int current_center = find_center_line(pixels);
        int error = current_center - config.track.center_reference;
        
        // Apply deadzone from config
        if (error > -config.steer.deadzone && error < config.steer.deadzone) {
            error = 0;
        }

        // Steering calculation
        double target_angle = calculate_steering_angle(error, filteredServo);
        filteredServo = (float)target_angle;

        write_servo(filteredServo);

        // speed calculation
        double current_speed = calculate_speed(error);

        // Apply speed to motors 
        set_speed_fw(current_speed, current_speed);

        Camera_clearDataReady();
    }
}