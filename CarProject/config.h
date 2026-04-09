#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

typedef struct {
    float center_pwm;     
    float min_pwm;        
    float max_pwm;        
    int deadzone;         
    
    float Kp;
    float Ki;
    float Kd;

    float i_limit;        
} SteeringConfig;

typedef struct {
    double base_speed;    
    float speed_sensitivity; 
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