#ifndef CONFIG_H
#define CONFIG_H

/*  Steering & Servo Configuration  */
typedef struct {
    float center_pwm;     // Default straight position 
    float min_pwm;        // One steering limit
    float max_pwm;        // Opposite steering 
    float error_min;      // Min pixel error 
    float error_max;      // Max pixel error 
    float alpha;          // Low-pass filter smoothing (e.g., 0.8)
    int deadzone;         // Error range to ignore (e.g., 5)
} SteeringConfig;

/*  Motor & Speed Configuration  */
typedef struct {
    double base_speed;    // Cruising speed on straightaways
    double max_boost;     // Additional speed based on error
    uint32_t carpet_thresh; // Average pixel value to trigger stop (e.g., 2000)
} DriveConfig;

/*  Camera & Track Detection Configuration  */
typedef struct {
    int edge_threshold;   // Derivative jump to detect line edge (e.g., 200)
    int single_edge_off;  // Pixels to offset when only one edge is seen (e.g., 10)
    int center_reference; // Camera midpoint 
} TrackConfig;

/*  Master Configuration Struct  */
typedef struct {
    SteeringConfig steer;
    DriveConfig    drive;
    TrackConfig    track;
} CarSettings;

/* * Declaring 'config' as extern allows you to define the actual values in main.c,
 * but let modules like track_settings.c see and use those values.
 */
extern CarSettings config;

#endif /* CONFIG_H */