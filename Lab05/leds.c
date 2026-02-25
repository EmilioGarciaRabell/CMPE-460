/**
 * ******************************************************************************
 * @file    : leds.c
 * @brief   : LEDs module file
 * @details : LED initialization and setting methods
 *
 * @author Cristian Morataya
 * @date 1/21/2026
 * ******************************************************************************
 */

#include <lab1/leds.h>
#include <ti/devices/msp/msp.h>

/**
 * @brief Initialze LED1, starts by checking if it is on or not, then connects them to the I/O port and sets it up for use
 */

void LED1_init(void)
{
	// Check if the power is enabled, using the mask tells you if it is or not
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

	// Access the PINCM register for LED1 and connect it as port I/O, use the PC_CONNECTED as 0x80 and GPIOA_DIO00 as 0x01 (same as lab manual part C)
	IOMUX->SECCFG.PINCM[IOMUX_PINCM1] |= IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM1_PF_GPIOA_DIO00;

	// Make the drive strength high for LED1
	IOMUX->SECCFG.PINCM[IOMUX_PINCM1] |= IOMUX_PINCM_DRV_DRVVAL1;

	// Invert it since it is active low (look at excel table)
	IOMUX->SECCFG.PINCM[IOMUX_PINCM1] |= IOMUX_PINCM_INV_ENABLE;

	// Set the GPIO module so that the register is enabled, does not set any value to it though
	GPIOA->DOESET31_0 |= (1u << 0);

	// Set the pin to either an on or off state, in this case we're clearing to be off
	GPIOA->DOUTCLR31_0 |= (1u << 0);
}

/**
 * @brief Initialize LED2, starts by checking if it is on or not, then connects them to the I/O port and sets it up for use, in this case it accounts for the RGB
 */
void LED2_init(void)
{
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

	// Access the PINCM register for LED2 RED and connect it as port I/O, use the PC_CONNECTED for this PICM and GPIOB_DIO26 since thats the pin number
	IOMUX->SECCFG.PINCM[IOMUX_PINCM57] |= IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM57_PF_GPIOB_DIO26;

	// Make the drive strength high
	IOMUX->SECCFG.PINCM[IOMUX_PINCM57] |= IOMUX_PINCM_DRV_DRVVAL1;

	// Repeat the same as LED2 RED but this time for LED2 GREEN
	IOMUX->SECCFG.PINCM[IOMUX_PINCM58] |= IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM58_PF_GPIOB_DIO27;

	// Make the drive strength high
	IOMUX->SECCFG.PINCM[IOMUX_PINCM58] |= IOMUX_PINCM_DRV_DRVVAL1;

	// Repeat the same as LED2 RED but this time for LED2 BLUE
	IOMUX->SECCFG.PINCM[IOMUX_PINCM50] |= IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM50_PF_GPIOB_DIO22;

	// Make the drive strength high
	IOMUX->SECCFG.PINCM[IOMUX_PINCM50] |= IOMUX_PINCM_DRV_DRVVAL1;

	// Set the GPIO module so that the register is enabled, does not set any value to it though, do it for all colors
	GPIOB->DOESET31_0 |= (1u << 26);
	GPIOB->DOESET31_0 |= (1u << 27);
	GPIOB->DOESET31_0 |= (1u << 22);

	// Set the pin to either an on or off state, in this case we're clearing to be off
	GPIOB->DOUTCLR31_0 |= (1u << 26);
	GPIOB->DOUTCLR31_0 |= (1u << 27);
	GPIOB->DOUTCLR31_0 |= (1u << 22);
}

/**
 * @brief Set LED1 output state
 */
void LED1_set(enum State state)
{
	// Check if the desired state is ON, if so set to be on
	if (state == ON)
	{
		GPIOA->DOUTSET31_0 |= (1u << 0);
	}
	// Check if the desired state if OFF, if so set to be off
	else if (state == OFF)
	{
		GPIOA->DOUTCLR31_0 |= (1u << 0);
	}
	// Toggle whatever state it was in
	else if (state == TOGGLE)
	{
		GPIOA->DOUTTGL31_0 |= (1u << 0);
	}
}

/**
 * @brief Set LED2 output state, makes sure all color combinations are handled
 */
void LED2_set(enum Color color)
{
	// First turn off the LED so that there's not a stray LED on
	GPIOB->DOUTCLR31_0 |= (1u << 26) | (1u << 27) | (1u << 22);

	// Go through all the different colors and turn on the corresponding combination, or if its just one just turn on the correct LED (could use a case statement to be cleaner)
	if (color == RED)
	{
		GPIOB->DOUTSET31_0 |= (1u << 26);
	}
	else if (color == GREEN)
	{
		GPIOB->DOUTSET31_0 |= (1u << 27);
	}
	else if (color == BLUE)
	{
		GPIOB->DOUTSET31_0 |= (1u << 22);
	}
	else if (color == CYAN)
	{
		GPIOB->DOUTSET31_0 |= (1u << 27);
		GPIOB->DOUTSET31_0 |= (1u << 22);
	}
	else if (color == MAGENTA)
	{
		GPIOB->DOUTSET31_0 |= (1u << 26);
		GPIOB->DOUTSET31_0 |= (1u << 22);
	}
	else if (color == YELLOW)
	{
		GPIOB->DOUTSET31_0 |= (1u << 26);
		GPIOB->DOUTSET31_0 |= (1u << 27);
	}
	else if (color == WHITE)
	{
		GPIOB->DOUTSET31_0 |= (1u << 26);
		GPIOB->DOUTSET31_0 |= (1u << 27);
		GPIOB->DOUTSET31_0 |= (1u << 22);
	}
	else if (color == NONE)
	{
		GPIOB->DOUTCLR31_0 |= (1u << 26);
		GPIOB->DOUTCLR31_0 |= (1u << 27);
		GPIOB->DOUTCLR31_0 |= (1u << 22);
	}
}