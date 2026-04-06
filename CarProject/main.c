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
        
        if (is_carpet(pixels)) 
        {
            // STOP MOTORS IMMEDIATELY
            TIMA0_PWM_DutyCycle(0, 0.0);
            TIMA0_PWM_DutyCycle(1, 0.0);
            TIMA0_PWM_DutyCycle(2, 0.0);
            TIMA0_PWM_DutyCycle(3, 0.0);
            
            // Optional: Center the wheels while stopped
            TIMA1_PWM_DutyCycle(0, 92.5); 
            
            Camera_clearDataReady();
            continue; // Skip the rest of the loop and check again
        }

        // --- EXISTING TRACK LOGIC ---
        int error = find_center_line(pixels) - 64;
        
        if (error > -5 && error < 5) error = 0;

        float in_min  = -64.0f; 
        float in_max  =  64.0f; 
        float out_min = 94.8f;  
        float out_max = 90.0f;  

        float targetServo = (float)(error - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;

        float alpha = 0.20f; 
        filteredServo = (targetServo * alpha) + (filteredServo * (1.0f - alpha));

        TIMA1_PWM_DutyCycle(0, (double)filteredServo);

        // Speed Logic
        int absError = (error < 0) ? -error : error;
        double baseSpeed = 27.0; 
        double maxBoost  = 10; 
        double speed = baseSpeed + (maxBoost * (1.0 - (double)absError / 64.0));

        TIMA0_PWM_DutyCycle(0, 0.0);
        TIMA0_PWM_DutyCycle(1, speed);
        TIMA0_PWM_DutyCycle(2, 0.0);
        TIMA0_PWM_DutyCycle(3, speed);

        Camera_clearDataReady();
    }
}