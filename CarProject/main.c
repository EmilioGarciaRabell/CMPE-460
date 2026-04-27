/**
 * ******************************************************************************
 * @file    : main.c
 * @brief   : Main module file
 * @details : Main function and overall control logic for the car
 *
 * @author Cristian Morataya & Emilio Garcia Rabell
 * @date 4/26/2026
 * ******************************************************************************
 */

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

// Car settings structure for the parameters
CarSettings config;

int main(void)
{

    // Variable to know what mode we are in
    int mode = select_mode();
    // Pass to the mode selection function
    mode_config(&config, mode);

    // Initialize everything we need to run the car
    Camera_init();
    DC_EN_init();
    motors_setup();
    UART0_init();

    // Center the servo at the start
    write_servo(config.steer.center_pwm);
    
    // Error variable for PID
    float last_error = 0.0f;
    // Variables for the moving average filter
    float filtered_center = 60.0f;
    const float alpha = 0.45f;

    while (1)
    {
        // Wait for new camera data
        if (!Camera_isDataReady())
            continue;

        // Get the data
        uint16_t *pixels = Camera_getData();

        // Send data over to find the center line
        int raw_center = find_center_line(pixels);

        // Debugging prints
        // UART0_printDec(raw_center);
        // UART0_put("\n\r");

        // Check if it is on carpet or not, if it is stop motors, center the servo and wait for data
        if (is_carpet(pixels))
        {
            stop_motors();
            write_servo(config.steer.center_pwm);
            Camera_clearDataReady();
            continue;
        }
    
        // Boundaries for the center, if its not in range then go back to the reference center 
        if (raw_center < 2 || raw_center > 122)
        {
            raw_center = config.track.center_reference;
        }

        // Smooth the input
        filtered_center = (alpha * (float)raw_center) + ((1.0f - alpha) * filtered_center);

        // Find the error by comparing the filtered center to the reference center
        float error = filtered_center - (float)config.track.center_reference;
        float abs_error = fabs(error);

        // Dynamic PID, if the error is big enough then become more aggressive with the turning and vice versa
        float active_Kp, active_Kd;
        // 12 is the value that best worked as the threshold
        if (abs_error > 12.0f)
        {
            active_Kp = config.steer.Kp_turn;
            active_Kd = config.steer.Kd_turn;
        }
        else
        {
            active_Kp = config.steer.Kp;
            active_Kd = config.steer.Kd;
        }

        // PID but without I, only was needed for demo 3
        float P = active_Kp * error;
        float D = active_Kd * (error - last_error);

        // Calculate where the servo would go with the PID output
        float targetServo = config.steer.center_pwm + (P + D);

        // Make sure to be within the servo range that we found in the previous lab
        if (targetServo < config.steer.min_pwm)
            targetServo = config.steer.min_pwm;
        if (targetServo > config.steer.max_pwm)
            targetServo = config.steer.max_pwm;

        write_servo(targetServo);

        // Set the speed based on error, big error means go slow, small means we're probably centered and go fast
        double current_speed = config.drive.base_speed - (abs_error * config.drive.speed_sensitivity);
        if (current_speed < config.drive.min_speed)
            current_speed = config.drive.min_speed;

        // Diff steering, if positive then turn the right wheel and vice versa
        float diff_offset = error * config.drive.diff_k;
        set_speed_fw(current_speed + diff_offset, current_speed - diff_offset);

        // Change the previous error 
        last_error = error;
        Camera_clearDataReady();
    }
}