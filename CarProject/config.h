#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

typedef struct {
    float center_pwm;     
    float min_pwm;        
    float max_pwm;        
    int deadzone;         
    
    float Kp;        // Straight/Normal Kp
    float Kd;        // Straight/Normal Kd
    float Kp_turn;   // Aggressive Turn Kp
    float Kd_turn;   // Lower resistance Kd for Turns
    float Ki;

    float i_limit;        
} SteeringConfig;

typedef struct {
    double base_speed;    
    float speed_sensitivity; 
    float diff_k;          // Differential steering gain
    float min_speed;       // Minimum motor floor
    uint32_t carpet_thresh; 
} DriveConfig;

typedef struct {
    int center_reference; 
} TrackConfig;

typedef struct {
    SteeringConfig steer;
    DriveConfig    drive;
    TrackConfig    track;
} CarSettings;

extern CarSettings config;

#endif