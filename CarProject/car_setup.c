#include "lab5/camera.h"
#include "lab6/timers.h"
#include <ti/devices/msp/msp.h>
#include <sysctl.h>
#include "car_setup.h"

/* The functions living here are needed for the initialization of the car*/

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