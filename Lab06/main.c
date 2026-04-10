/**
 * ******************************************************************************
 * @file    : main.c
 * @brief   : Main file for Lab 6
 * @details : Contains the main functions for part 1A, 1B, 2, 3 and 4 of Lab 6
 *
 * @author Cristian Morataya & Emilio Garcia Rabell
 * @date 3/17/2026
 * ******************************************************************************
 */

#include "lab6/timers.h"
#include <ti/devices/msp/msp.h>
#include <sysctl.c>

// Declarations of the different parts of the lab
void part1A(void);
void part1B(void);
void part2(void);
void part3(void);
void part4(void);
void DC_EN_init(void);

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

// Second function for part 1 the DC motor with an H-bridge and two PWM signals to move it forward and backward
void part1B()
{
		/* Enable the Motor Driver Pins (Enable/Sleep) */
    DC_EN_init(); 
		uint32_t mclk = (uint32_t)SYSCTL_SYSCLK_getMCLK();
    /* Calculate 10kHz Load Value based on 32MHz Clock */
    /* (32,000,000 / 10,000) - 1 = 3199 */
    //uint32_t TIMA0LoadValue = (SYSCTL_SYSCLK_getMCLK() / 10000) - 1;
		uint32_t TIMA0LoadValue = (mclk / (0 + 1) / 10000) - 1;
    /* Initialize PWM Channels */
    TIMA0_PWM_init(0, TIMA0LoadValue, 0, 0.0);
    TIMA0_PWM_init(1, TIMA0LoadValue, 0, 0.0);

    

    /* IMPORTANT: Start the Timer Counter */
    /* Without this, PWM signals might be inconsistent or stuck */
   // DL_TimerA_startCounter(TIMA0); 

    while (1)
    {
        /* Test with 10% - if this is 'fast', your timer clock 
           is likely not running at the speed you calculated */
        TIMA0_PWM_DutyCycle(0, 10.0f); 
        TIMA0_PWM_DutyCycle(1, 0.0f);  
    }
}
int main(void)
{
    part1B();
}
