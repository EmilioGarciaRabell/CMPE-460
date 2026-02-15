/**
 * ******************************************************************************
 * @file    : switches.c
 * @brief   : Switches module file
 * @details : Switches initialization and interaction
 *
 * @author Cristian Morataya
 * @date 1/21/2026
 * ******************************************************************************
 */
#include "lab5/switches.h"
#include "cmsis_armclang.h"
#include <ti/devices/msp/msp.h>

/**
 * @brief Switch 1 polling initialization, starts by checking if it is on or not, then connects them to the I/O port and sets it up for use
 */
void S1_init(void)
{
	if ((GPIOA->GPRCM.PWREN & GPIO_PWREN_ENABLE_MASK) == 0)
	{
		// Force it to reset
		GPIOA->GPRCM.RSTCTL = GPIO_RSTCTL_KEY_UNLOCK_W | GPIO_RSTCTL_RESETASSERT_ASSERT;

		// Enable power after the reset
		GPIOA->GPRCM.PWREN = GPIO_PWREN_KEY_UNLOCK_W | GPIO_PWREN_ENABLE_ENABLE;

		// Quick delay because the TRM says that you have to wait 4 clock cycles
		for (volatile int i = 0; i < 10; i++)
		{
		}
	}

	// Access the PINCM register for S1 and connect it as port I/O, use the PC_CONNECTED and GPIOA_DIO18 macros (they have the same values as the lab manual in part C)
	IOMUX->SECCFG.PINCM[IOMUX_PINCM40] |= IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM40_PF_GPIOA_DIO18;

	// Enable input bit
	IOMUX->SECCFG.PINCM[IOMUX_PINCM40] |= IOMUX_PINCM_INENA_ENABLE;

	// Enable pull down resistor
	IOMUX->SECCFG.PINCM[IOMUX_PINCM40] |= IOMUX_PINCM_PIPD_ENABLE;
}

/**
 * @brief Switch 2 polling initialization, starts by checking if it is on or not, then connects them to the I/O port and sets it up for use
 */
void S2_init(void)
{
	if ((GPIOB->GPRCM.PWREN & GPIO_PWREN_ENABLE_MASK) == 0)
	{
		// Force it to reset
		GPIOB->GPRCM.RSTCTL = GPIO_RSTCTL_KEY_UNLOCK_W | GPIO_RSTCTL_RESETASSERT_ASSERT;

		// Enable power after the reset
		GPIOB->GPRCM.PWREN = GPIO_PWREN_KEY_UNLOCK_W | GPIO_PWREN_ENABLE_ENABLE;

		// Quick delay because the TRM says that you have to wait 4 clock cycles
		for (volatile int i = 0; i < 10; i++)
		{
		}
	}

	// Access the PINCM register for S2 and connect it as port I/O, use the PC_CONNECTED and GPIOB_DIO21 macros
	IOMUX->SECCFG.PINCM[IOMUX_PINCM49] |= IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM49_PF_GPIOB_DIO21;

	// Enable input bit
	IOMUX->SECCFG.PINCM[IOMUX_PINCM49] |= IOMUX_PINCM_INENA_ENABLE;

	// Enable pull up resistor
	IOMUX->SECCFG.PINCM[IOMUX_PINCM49] |= IOMUX_PINCM_PIPU_ENABLE;

	// Invert the input since the switch is active low
	IOMUX->SECCFG.PINCM[IOMUX_PINCM49] |= IOMUX_PINCM_INV_ENABLE;
}

/**
 * @brief Check if switch 1 was pressed
 */
int S1_pressed(void)
{
	// Check the DIO bit for PIN18 to see what it returns, basically used to check the current state
	if (GPIOA->DIN31_0 & (1u << 18))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 * @brief Check if switch 2 was pressed
 */
int S2_pressed(void)
{
	// Check the DIO bit for PIN21 to see what it returns, check the current state
	if (GPIOB->DIN31_0 & (1u << 21))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 * @brief Switch 1 interrupt initialization
 * @note Use NVIC_EnableIRQ() to register IRQn with the NVIC
 *       Check out `cmsis_armclang.h`
 * @hint Keep the polarity in mind
*/
void S1_init_interrupt(void)
{
	__disable_irq();
	S1_init();
	// Clear interrupt for PA18
	GPIOA->CPU_INT.ICLR |= GPIO_GEN_EVENT1_ICLR_DIO18_CLR;
	
	// Enable interrupt for PA18
	GPIOA->CPU_INT.IMASK &= ~GPIO_GEN_EVENT1_IMASK_DIO18_SET;
	
	// Set polarity for the interrupt
	GPIOA->POLARITY31_16 |= GPIO_POLARITY31_16_DIO18_RISE;
	
	// Register interrupt with NVIC 
	NVIC_EnableIRQ(GPIOA_INT_IRQn);
	
	// Enable interrupts
	__enable_irq();
	
}

/**
 * @brief Switch 2 interrupt initialization
 * @note Use NVIC_EnableIRQ() to register IRQn with the NVIC
 *       Check out `cmsis_armclang.h`
*/
void S2_init_interrupt(void)
{
	intrinsic void __disable_irq();
	S2_init();
	// Clear interrupt for PA21
	GPIOB->CPU_INT.ICLR |= GPIO_GEN_EVENT1_ICLR_DIO21_CLR;
	
	// Enable interrupt for PA18
	GPIOB->CPU_INT.IMASK &= ~GPIO_GEN_EVENT1_IMASK_DIO21_SET;
	
	// Set polarity for the interrupt
	GPIOB->POLARITY31_16 |= GPIO_POLARITY31_16_DIO21_LOW;
	
	// Register interrupt with NVIC 
	NVIC_EnableIRQ(GPIOB_INT_IRQn);
	
	// Enable interrupts
	intrinsic void __enable_irq();
}
