/**
 * ******************************************************************************
 * @file    : config.h
 * @brief   : Car configuration header files
 * @details : Configuration and global variables for the car
 *
 * @author Emilio Garcia Rabell & Cristian Morataya
 * @date 4/26/2026
 * ******************************************************************************
 */

#ifndef CONFIG_H
#define CONFIG_H

// Structure for the car settings
typedef struct
{
    float center_pwm;
    float min_pwm;
    float max_pwm;
    int deadzone;

    // Different PID values for straight and turning
    float Kp;      
    float Kd;      
    float Kp_turn; 
    float Kd_turn; 
    float Ki;

    float i_limit;
} SteeringConfig;

// Another structure for the motor settings
typedef struct
{
    double base_speed;
    float speed_sensitivity;
    float diff_k;   
    float min_speed; 
    uint32_t carpet_thresh;
} DriveConfig;

// Structure for the track settings
typedef struct
{
    int center_reference;
} TrackConfig;

// Main structure for the car settings
typedef struct
{
    SteeringConfig steer;
    DriveConfig drive;
    TrackConfig track;
} CarSettings;

extern CarSettings config;

#endif