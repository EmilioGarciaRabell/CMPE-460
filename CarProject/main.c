#include "lab5/camera.h"
#include "lab6/timers.h"
#include <ti/devices/msp/msp.h>
#include <sysctl.h>
#include "track_settings.h"
#include "car_setup.h"


// Main function to set up the timers, camera, etc.
int main(void)
{
	// Initialize the camera and the motors
	Camera_init();
	DC_EN_init();

	// Set up timer that will control the servo and the motors
	uint32_t TIMA1Load = (SYSCTL_SYSCLK_getMCLK() / (9 + 1) / 50) - 1;
	TIMA1_PWM_init(0, TIMA1Load, 9, 92.5);

	uint32_t TIMA0Load = (SYSCTL_SYSCLK_getMCLK() / (0 + 1) / 10000) - 1;

	TIMA0_PWM_init(0, TIMA0Load, 0, 0.0);
	TIMA0_PWM_init(1, TIMA0Load, 0, 0.0);
	TIMA0_PWM_init(2, TIMA0Load, 0, 0.0);
	TIMA0_PWM_init(3, TIMA0Load, 0, 0.0);


    static float filteredServo = 92.5f;

    while (1)
    {
        if (!Camera_isDataReady()) continue;

        uint16_t *pixels = Camera_getData();
        int error = find_center_line(pixels) - 64;
        
        // 1. DEADBAND
        if (error > -3 && error < 3) error = 0;

        // 2. REDUCED GAIN MAPPING (Numbers preserved as requested)
        float in_min  = -64.0f; 
        float in_max  =  64.0f; 
        float out_min = 96.5f;  
        float out_max = 88.75f;  

        float targetServo = (float)(error - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;

        // 3. HEAVY SMOOTHING
        float alpha = 0.10f; 
        filteredServo = (targetServo * alpha) + (filteredServo * (1.0f - alpha));

        // 4. Set Hardware
        TIMA1_PWM_DutyCycle(0, (double)filteredServo);

        // 5. ADJUSTABLE SPEED LOGIC
        int absError = (error < 0) ? -error : error;
        
        // Base speed for corners and Boost speed for straights
        double baseSpeed = 24;
					; 
        double maxBoost  = 7; // How much extra speed to add on straights
        
        // Calculate speed: 
        // When error is 0, speed = 26.3 + 12.0 = 38.3
        // When error is 64, speed = 26.3 + 0.0 = 26.3
        double speed = baseSpeed + (maxBoost * (1.0 - (double)absError / 64.0));

        TIMA0_PWM_DutyCycle(0, 0.0);
        TIMA0_PWM_DutyCycle(1, speed);
        TIMA0_PWM_DutyCycle(2, 0.0);
        TIMA0_PWM_DutyCycle(3, speed);

        Camera_clearDataReady();
    }
}