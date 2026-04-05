#include "lab5/camera.h"
#include "lab6/timers.h"
#include <ti/devices/msp/msp.h>
#include <sysctl.h>

// Enable the pins for the motors
void DC_EN_init(void)
{
	// Try to enable GPIOB for the left DC_EN if not enabled already
	if ((GPIOB->GPRCM.PWREN & GPIO_PWREN_ENABLE_MASK) == 0)
	{
		// Force it to reset
		GPIOB->GPRCM.RSTCTL = GPIO_RSTCTL_KEY_UNLOCK_W | GPIO_RSTCTL_RESETASSERT_ASSERT;

		// Enable power after the reset
		GPIOB->GPRCM.PWREN = GPIO_PWREN_KEY_UNLOCK_W | GPIO_PWREN_ENABLE_ENABLE;

		for (volatile int i = 0; i < 10; i++)
		{
		}
	}

	// Try to enable GPIOA for the right DC_EN if not already enabled
	if ((GPIOA->GPRCM.PWREN & GPIO_PWREN_ENABLE_MASK) == 0)
	{
		// Force it to reset
		GPIOA->GPRCM.RSTCTL = GPIO_RSTCTL_KEY_UNLOCK_W | GPIO_RSTCTL_RESETASSERT_ASSERT;

		// Enable power after the reset
		GPIOA->GPRCM.PWREN = GPIO_PWREN_KEY_UNLOCK_W | GPIO_PWREN_ENABLE_ENABLE;

		for (volatile int i = 0; i < 10; i++)
		{
		}
	}

	// PINCM register to connect it as IO (Left DC_EN)
	IOMUX->SECCFG.PINCM[IOMUX_PINCM45] = IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM45_PF_GPIOB_DIO19;
	// PINCM register to connect as IO (Right DC_EN)
	IOMUX->SECCFG.PINCM[IOMUX_PINCM47] = IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM47_PF_GPIOA_DIO22;

	// Make both pins an output by setting them
	GPIOB->DOE31_0 |= (1 << 19);
	GPIOA->DOE31_0 |= (1 << 22);

	// Set the GPIO module so that the register is enabled, does not set any value to it though
	GPIOB->DOUTSET31_0 = (1 << 19);
	GPIOA->DOUTSET31_0 = (1 << 22);
}

// Function to find the center line on the track with the camera input data
int find_center_line(uint16_t *pixels)
{
	// Set the center to 64 because its in the middle of 128 and is just a reference point for starting
	static int previousCenter = 64;

	// To reduce noise a smoothing filter can be used to find edges easier
	uint16_t smoothingFilter[128];

	// Make the first and last value of the filter to get the range
	smoothingFilter[0] = pixels[0];
	smoothingFilter[128 - 1] = pixels[128 - 1];

	// Loop through and apply the filter to the pixels by getting the average of the current pixel and the average on both sides of it
	for (int i = 1; i < 128 - 1; i++)
	{
		// Calculate the average
		smoothingFilter[i] = (pixels[i - 1] + pixels[i] + pixels[i + 1]) / 3;
	}

	// Set the left and right as a placeholder before finding the actual edges
	int leftEdge = -1;
	int rightEdge = -1;

	// Loop through the filter to find the left edge
	for (int i = 11; i < 128 - 10; i++)
	{
		if ((int)smoothingFilter[i] - (int)smoothingFilter[i - 1] > 200)
		{
			// Set the value on the left
			leftEdge = i;
			break;
		}
	}
	// Loop through the filter to find the right edge
	for (int i = 128 - 11; i >= 10; i--)
	{
		if ((int)smoothingFilter[i] - (int)smoothingFilter[i + 1] > 200)
		{
			// Set the value on the right
			rightEdge = i;
			break;
		}
	}

	// Check if either the car has veered off to the left or right and then correct or if its fine
	if (leftEdge != -1 && rightEdge != -1 && rightEdge > leftEdge)
	{
		previousCenter = (leftEdge + rightEdge) / 2;
	}
	else if (leftEdge != -1 && rightEdge == -1)
	{
		previousCenter = leftEdge + 20;
	}
	else if (rightEdge != -1 && leftEdge == -1)
	{
		previousCenter = rightEdge - 20;
	}

	// Return the value in the center
	return previousCenter;
}

// Function to check if the car is on the carpet by seeing if the camera data is mostly dark
int is_carpet(uint16_t *pixels)
{
	uint32_t sum = 0;

	// Loop through the pixels and add them up to get the average
	for (int i = 0; i < 128; i++)
	{
		sum += pixels[i];
	}

	// If the average is less than 500 then its on the carpet otherwise its not
	if ((sum / 128) < 500)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

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
        double baseSpeed = 24
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