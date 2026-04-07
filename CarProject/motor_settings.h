#ifndef MOTOR_SETTINGS_H
#define MOTOR_SETTINGS_H
/* Initialization */
void motors_setup(void);

/* Movement Control */
void set_speed_fw(double speed_left, double speed_right);
void set_speed_bw(double speed_left, double speed_right);
void stop_motors(void);

/* Steering Control */
void write_servo(double angle);

/* Calculations */
double calculate_steering_angle(int error, float current_filtered_servo);
double calculate_speed(int error);

#endif /* MOTOR_SETTINGS_H */