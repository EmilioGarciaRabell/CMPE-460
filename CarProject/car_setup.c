#include "lab5/camera.h"
#include "lab6/timers.h"
#include <ti/devices/msp/msp.h>
#include <sysctl.h>
#include "car_setup.h"
#include "config.h"

/* The functions living here are needed for the initialization of the car*/

// wavy vars
float wavy_sensitivity_mid = 0.4f;

float wavy_sensitivity_pro = 0.9f;
float wavy_dif_pro = 0.5f;
float wavy_speed_pro = 47.3;


//straight vars
float straight_sensitivity_pro = 0.76f;
float straight_dif_pro = 0.4f;
float straight_speed_pro = 49;


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


void mode_config(CarSettings *c, int mode) {
	
				c->track.center_reference = 64;
				c->steer.center_pwm = 92.5f;
        c->steer.min_pwm = 90.0f;
        c->steer.max_pwm = 95.0f;
				c->drive.min_speed = 23.0f;
    // Mode 1: Pro Mode (The values you shared - Fastest)
    if (mode == 1) {
        
        c->steer.deadzone = 2;
        c->steer.Kp = 0.055f;
        c->steer.Kd = 0.31f;
        c->steer.Kp_turn = 0.07; //0.07f;
        c->steer.Kd_turn = 0.28; //0.28f;
        c->steer.Ki = 0.0f;
        c->steer.i_limit = 10.0f;

        c->drive.base_speed = straight_speed_pro;
        c->drive.speed_sensitivity = wavy_sensitivity_pro;
        c->drive.diff_k = straight_dif_pro;
                
    }
    // Mode 2: Mid Mode (Stability focused)
    else if (mode == 2) {
   
        c->steer.deadzone = 2;
        c->steer.Kp = 0.055f;
        c->steer.Kd = 0.31f;
        c->steer.Kp_turn = 0.07f;
        c->steer.Kd_turn = 0.28f;
        c->steer.Ki = 0.0f;
        c->steer.i_limit = 10.0f;

        c->drive.base_speed = 40.0; // Slower base for consistency
        c->drive.speed_sensitivity = wavy_sensitivity_mid;
        c->drive.diff_k = 0.28f;
        
    }
    // Mode 3: Safe Mode (Slow)
    else {

        c->steer.deadzone = 2;
        c->steer.Kp = 0.055f;
        c->steer.Kd = 0.31f;
        c->steer.Kp_turn = 0.07f;
        c->steer.Kd_turn = 0.28f;
        c->steer.Ki = 0.0f;
        c->steer.i_limit = 10.0f;

        c->drive.base_speed = 32.0; 
        c->drive.speed_sensitivity = 0.0f;
        c->drive.diff_k = 0.10f;
        
        
    }
}
