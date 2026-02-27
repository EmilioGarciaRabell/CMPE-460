/**
 * ******************************************************************************
 * @file    : adc12.c
 * @brief   : ADC12 module file
 * @details : ADC12 initialization and configuration as well as a function to get the value of the conversion
 *
 * @author Emilio Garcia Rabell & Cristian Morataya
 * @date 2/26/2026
 * ******************************************************************************
 */

#include "lab5/switches.h"
#include <ti/devices/msp/msp.h>
#include "lab5/adc12.h"

/**
 * @brief Initialize ADC0
 */
void ADC0_init(void)
{
	if ((ADC0->ULLMEM.GPRCM.PWREN & ADC12_PWREN_ENABLE_MASK) == 0)
	{
		// Force it to reset
		ADC0->ULLMEM.GPRCM.RSTCTL = ADC12_RSTCTL_KEY_UNLOCK_W | ADC12_RSTCTL_RESETASSERT_ASSERT;

		// Enable power after the reset
		ADC0->ULLMEM.GPRCM.PWREN = ADC12_PWREN_KEY_UNLOCK_W | ADC12_PWREN_ENABLE_ENABLE;

		// Quick delay because the TRM says that you have to wait 4 clock cycles
		for (volatile int i = 0; i < 10; i++)
		{
		}
	}

	// Select the Ultra Low Power clock and the highest possible frequency range
	ADC0->ULLMEM.GPRCM.CLKCFG |= ADC12_CLKCFG_SAMPCLK_ULPCLK;
	ADC0->ULLMEM.CLKFREQ |= ADC12_CLKFREQ_FRANGE_RANGE40TO48;

	// Set the power down mode so that ADC remains on after conversion
	ADC0->ULLMEM.CTL0 |= ADC12_CTL0_PWRDN_MANUAL;

	// Set SCLKDIV bits to divide-by-8
	ADC0->ULLMEM.CTL0 |= ADC12_CTL0_SCLKDIV_DIV_BY_8;

	// Set single channel single conversion mode with auto software trigger
	ADC0->ULLMEM.CTL1 &= ADC12_CTL1_CONSEQ_OFS;
	ADC0->ULLMEM.CTL1 |= ADC12_CTL1_SAMPMODE_OFS;
	ADC0->ULLMEM.CTL1 &= ADC12_CTL1_TRIGSRC_OFS;

	// Map channel 0 to memory result register 0.
	ADC0->ULLMEM.MEMCTL[0] = ADC12_MEMCTL_CHANSEL_CHAN_0;
	ADC0->ULLMEM.SCOMP0 = 0;		// specify sample time
	ADC0->ULLMEM.CPU_INT.IMASK = 0; // ensure there are no interrupts triggered
}

/**
 * @brief Retrieve the value from the ADC0
 * @note The ADC channel in use is set during initialization
 *       The channel is not the same as where the module stores the value
 * @return ADC0 processed value
 */
uint32_t ADC0_getVal()
{

	// Enable ADC
	ADC0->ULLMEM.CTL0 |= ADC12_CTL0_ENC_ON;
	// Trigger and Sample Mode ADC Usage Matrix
	ADC0->ULLMEM.CTL1 |= ADC12_CTL1_SC_MASK;
	// Set SC bit to start the sample phase
	ADC0->ULLMEM.CTL1 |= ADC12_CTL1_SC_START;

	// Wait for the ADC to not be bussy
	while (ADC0->ULLMEM.STATUS & ADC12_STATUS_BUSY_ACTIVE)
		;

	// Read the result from the mapped memory register
	return ADC0->ULLMEM.MEMRES[0];
}