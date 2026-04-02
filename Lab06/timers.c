/**
 * ******************************************************************************
 * @file    : timers.c
 * @brief   : Timers module file
 * @details : Timers initialization and configuration
 *
 * @author Cristian Morataya & Emilio Garcia Rabell
 * @date 2/26/2026
 * ******************************************************************************
 */

#include <lab6/timers.h>
#include <ti/devices/msp/msp.h>

/**
 * @brief Timer G0 module initialization. General purpose timer
 * @note Timer G0 is in Power Domain 0. Check page 3 of the Data Sheet
 */
void TIMG0_init(uint32_t period, uint32_t prescaler)
{
	// Check if the timer is already powered on
	if ((TIMG0->GPRCM.PWREN & GPTIMER_PWREN_ENABLE_MASK) == 0)
	{
		// Force it to reset
		TIMG0->GPRCM.RSTCTL = GPTIMER_RSTCTL_KEY_UNLOCK_W | GPTIMER_RSTCTL_RESETASSERT_ASSERT;

		// Enable power after the reset
		TIMG0->GPRCM.PWREN = GPTIMER_PWREN_KEY_UNLOCK_W | GPTIMER_PWREN_ENABLE_ENABLE;

		for (volatile int i = 0; i < 10; i++)
		{
		}
	}
	// Select BUSCLK
	TIMG0->CLKSEL = GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE;

	// Perform clock division
	TIMG0->CLKDIV = GPTIMER_CLKDIV_RATIO_DIV_BY_1;

	// Set prescalers version below was our previous code
	// TIMG6->COMMONREGS.CPS &= ~GPTIMER_CPS_PCNT_MASK;
	TIMG0->COMMONREGS.CPS = prescaler;

	// Set the LOAD to the period
	TIMG0->COUNTERREGS.LOAD = period;

	// Enable counter value
	TIMG0->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_CVAE_LDVAL;

	// Make the counter count down, repeat and also disable it
	TIMG0->COUNTERREGS.CTRCTL = GPTIMER_CTRCTL_CM_DOWN | GPTIMER_CTRCTL_REPEAT_REPEAT_1 | GPTIMER_CTRCTL_EN_DISABLED;

	// Enable TIMCLK
	TIMG0->COMMONREGS.CCLKCTL |= GPTIMER_CCLKCTL_CLKEN_ENABLED;

	// Disable interrupts
	__disable_irq();

	// Register interrupt with NVIC
	NVIC_EnableIRQ(TIMG0_INT_IRQn);
	// Clear interrupt for PA18
	TIMG0->CPU_INT.ICLR |= GPTIMER_CPU_INT_ICLR_Z_CLR;

	// Enable interrupt for PA18
	TIMG0->CPU_INT.IMASK |= GPTIMER_CPU_INT_IMASK_Z_SET;

	// Enable the timer
	TIMG0->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_ENABLED;

	// Enable interrupts
	__enable_irq();
}

/**
 * @brief Timer G6 module initialization. General purpose timer
 */
void TIMG6_init(uint32_t period, uint32_t prescaler)
{
	// Check if the timer is already powered on
	if ((TIMG6->GPRCM.PWREN & GPTIMER_PWREN_ENABLE_MASK) == 0)
	{
		// Force it to reset
		TIMG6->GPRCM.RSTCTL = GPTIMER_RSTCTL_KEY_UNLOCK_W | GPTIMER_RSTCTL_RESETASSERT_ASSERT;

		// Enable power after the reset
		TIMG6->GPRCM.PWREN = GPTIMER_PWREN_KEY_UNLOCK_W | GPTIMER_PWREN_ENABLE_ENABLE;

		for (volatile int i = 0; i < 10; i++)
		{
		}
	}
	// Select BUSCLK TODO: Verify in a later stage if this clk is the best choice (MFCLK or LFCLK)
	TIMG6->CLKSEL = GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE;

	// Perform clock division
	TIMG6->CLKDIV = GPTIMER_CLKDIV_RATIO_DIV_BY_8;

	// Set prescalers version below was our previous code
	// TIMG6->COMMONREGS.CPS &= ~GPTIMER_CPS_PCNT_MASK;
	TIMG6->COMMONREGS.CPS = prescaler;

	// Set the LOAD to the period
	TIMG6->COUNTERREGS.LOAD = period;

	// Enable counter value
	TIMG6->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_CVAE_LDVAL;

	// Make the counter count down, repeat and also disable it
	TIMG6->COUNTERREGS.CTRCTL = GPTIMER_CTRCTL_CM_DOWN | GPTIMER_CTRCTL_REPEAT_REPEAT_1 | GPTIMER_CTRCTL_EN_DISABLED;

	// Enable TIMCLK
	TIMG6->COMMONREGS.CCLKCTL |= GPTIMER_CCLKCTL_CLKEN_ENABLED;

	// Disable interrupts
	__disable_irq();

	// Register interrupt with NVIC
	NVIC_EnableIRQ(TIMG6_INT_IRQn);
	// Clear interrupt for PA18
	TIMG6->CPU_INT.ICLR |= GPTIMER_CPU_INT_ICLR_Z_CLR;

	// Enable interrupt for PA18
	TIMG6->CPU_INT.IMASK |= GPTIMER_CPU_INT_IMASK_Z_SET;

	// Enable the timer
	TIMG6->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_ENABLED;

	// Enable interrupts
	__enable_irq();
}

/**
 * @brief Timer G12 module initialization. General purpose timer
 * @note Timer G12 has no prescaler
 */
void TIMG12_init(uint32_t period)
{
	// Check if the timer is already powered on
	if ((TIMG12->GPRCM.PWREN & GPTIMER_PWREN_ENABLE_MASK) == 0)
	{
		// Force it to reset
		TIMG12->GPRCM.RSTCTL = GPTIMER_RSTCTL_KEY_UNLOCK_W | GPTIMER_RSTCTL_RESETASSERT_ASSERT;

		// Enable power after the reset
		TIMG12->GPRCM.PWREN = GPTIMER_PWREN_KEY_UNLOCK_W | GPTIMER_PWREN_ENABLE_ENABLE;

		for (volatile int i = 0; i < 10; i++)
		{
		}
	}
	// Select BUSCLK TODO: Verify in a later stage if this clk is the best choice (MFCLK or LFCLK)
	TIMG12->CLKSEL = GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE;

	// Perform clock division
	TIMG12->CLKDIV = GPTIMER_CLKDIV_RATIO_DIV_BY_1;

	// Set prescalers version below was our previous code
	// TIMG6->COMMONREGS.CPS &= ~GPTIMER_CPS_PCNT_MASK;
	// TIMG12->COMMONREGS.CPS = prescaler;

	// Set the LOAD to the period
	TIMG12->COUNTERREGS.LOAD = period;

	// Enable counter value
	TIMG12->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_CVAE_LDVAL;

	// Make the counter count down, repeat and also disable it
	TIMG12->COUNTERREGS.CTRCTL = GPTIMER_CTRCTL_CM_DOWN | GPTIMER_CTRCTL_REPEAT_REPEAT_1 | GPTIMER_CTRCTL_EN_DISABLED;

	// Enable TIMCLK
	TIMG12->COMMONREGS.CCLKCTL |= GPTIMER_CCLKCTL_CLKEN_ENABLED;

	// Disable interrupts
	__disable_irq();

	// Register interrupt with NVIC
	NVIC_EnableIRQ(TIMG12_INT_IRQn);
	// Clear interrupt for PA18
	TIMG12->CPU_INT.ICLR |= GPTIMER_CPU_INT_ICLR_Z_CLR;

	// Enable interrupt for PA18
	TIMG12->CPU_INT.IMASK |= GPTIMER_CPU_INT_IMASK_Z_SET;

	// Enable the timer
	TIMG12->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_ENABLED;

	// Enable interrupts
	__enable_irq();
}

/**
 * @brief Timer A0 module PWM initialization
 * @param[in] pin - Timer PWM output pin / channel
 * @param[in] period - Timer load value
 * @param[in] prescaler - Timer prescale value
 * @param[in] percenetDutyCycle - PWM duty cycle positive
 * @note Store period to be able to adjust duty cycle percentage later
 */
void TIMA0_PWM_init(uint8_t pin, uint32_t period, uint32_t prescaler, double percentDutyCycle)
{
	uint32_t ccValue = (uint32_t)((percentDutyCycle / 100.0) * period);

	// Check if the timer is already powered on
	if ((TIMA0->GPRCM.PWREN & GPTIMER_PWREN_ENABLE_MASK) == 0)
	{
		// Force it to reset
		TIMA0->GPRCM.RSTCTL = GPTIMER_RSTCTL_KEY_UNLOCK_W | GPTIMER_RSTCTL_RESETASSERT_ASSERT;

		// Enable power after the reset
		TIMA0->GPRCM.PWREN = GPTIMER_PWREN_KEY_UNLOCK_W | GPTIMER_PWREN_ENABLE_ENABLE;

		for (volatile int i = 0; i < 10; i++)
		{
		}
	}
	// Select BUSCLK
	TIMA0->CLKSEL = GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE;

	// Perform clock division
	TIMA0->CLKDIV = GPTIMER_CLKDIV_RATIO_DIV_BY_1;

	// Set prescalers version below was our previous code
	// TIMG6->COMMONREGS.CPS &= ~GPTIMER_CPS_PCNT_MASK;
	TIMA0->COMMONREGS.CPS = prescaler;

	// Set the LOAD to the period
	TIMA0->COUNTERREGS.LOAD = period;

	// Enable counter value
	TIMA0->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_CVAE_LDVAL;

	// Make the counter count down, repeat and also disable it
	TIMA0->COUNTERREGS.CTRCTL = GPTIMER_CTRCTL_CM_DOWN | GPTIMER_CTRCTL_REPEAT_REPEAT_1 | GPTIMER_CTRCTL_EN_DISABLED;

	// Enable TIMCLK
	TIMA0->COMMONREGS.CCLKCTL |= GPTIMER_CCLKCTL_CLKEN_ENABLED;

	// Initialize the IOMUX (check what pin is selected and go from there (look at gitlab for table for corresponding pins)
	if (pin == 0)
	{
		IOMUX->SECCFG.PINCM[IOMUX_PINCM25] = IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM25_PF_TIMA0_CCP0;
	}
	else if (pin == 1)
	{
		IOMUX->SECCFG.PINCM[IOMUX_PINCM29] = IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM29_PF_TIMA0_CCP1;
	}
	else if (pin == 2)
	{
		IOMUX->SECCFG.PINCM[IOMUX_PINCM43] = IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM43_PF_TIMA0_CCP2;
	}
	else if (pin == 3)
	{
		IOMUX->SECCFG.PINCM[IOMUX_PINCM30] = IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM30_PF_TIMA0_CCP3;
	}

	// Determine whether it is channel 0/1 or 2/3
	uint8_t registerIndex = pin % 2;

	// Set timxcc, configure duty cycle
	if (pin < 2)
	{
		TIMA0->COUNTERREGS.CC_01[registerIndex] = ccValue;
	}
	else
	{
		TIMA0->COUNTERREGS.CC_23[registerIndex] = ccValue;
	}

	// Set compare mode, depending on if it is channel 0/1 or 2/3
	if (pin < 2)
	{
		TIMA0->COUNTERREGS.CCCTL_01[registerIndex] = GPTIMER_CCCTL_01_COC_COMPARE;
	}
	else
	{
		TIMA0->COUNTERREGS.CCCTL_23[registerIndex] = GPTIMER_CCCTL_23_COC_COMPARE;
	}

	// Configure chinese comunist party as an output for the cc block, based off the pin selected
	if (pin == 0)
	{
		TIMA0->COMMONREGS.CCPD |= GPTIMER_CCPD_C0CCP0_OUTPUT;
	}
	else if (pin == 1)
	{
		TIMA0->COMMONREGS.CCPD |= GPTIMER_CCPD_C0CCP1_OUTPUT;
	}
	else if (pin == 2)
	{
		TIMA0->COMMONREGS.CCPD |= GPTIMER_CCPD_C0CCP2_OUTPUT;
	}
	else if (pin == 3)
	{
		TIMA0->COMMONREGS.CCPD |= GPTIMER_CCPD_C0CCP3_OUTPUT;
	}

	// set CCP for compare, zero, load events, sw and fault events
	if (pin < 2)
	{
		TIMA0->COUNTERREGS.CCACT_01[registerIndex] |= GPTIMER_CCACT_01_LACT_CCP_HIGH;
		TIMA0->COUNTERREGS.CCACT_01[registerIndex] |= GPTIMER_CCACT_01_CDACT_CCP_LOW;
	}
	else
	{
		TIMA0->COUNTERREGS.CCACT_23[registerIndex] |= GPTIMER_CCACT_23_LACT_CCP_HIGH;
		TIMA0->COUNTERREGS.CCACT_23[registerIndex] |= GPTIMER_CCACT_23_CDACT_CCP_LOW;
	}

	// Set CCP0 to 0 to select signal generator output
	// Configure polarity of the signal using CCPOINV
	// Set low while disabled
	if (pin < 2)
	{
		TIMA0->COUNTERREGS.OCTL_01[registerIndex] = GPTIMER_OCTL_01_CCPO_FUNCVAL | GPTIMER_OCTL_01_CCPOINV_NOINV | GPTIMER_OCTL_01_CCPIV_LOW;
	}
	else
	{
		TIMA0->COUNTERREGS.OCTL_23[registerIndex] = GPTIMER_OCTL_23_CCPO_FUNCVAL | GPTIMER_OCTL_23_CCPOINV_NOINV | GPTIMER_OCTL_23_CCPIV_LOW;
	}

	// Enable counter
	TIMA0->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_ENABLED;
}

/**
 * @brief Timer A1 module PWM initialization
 * @param[in] pin - Timer PWM output pin / channel
 * @param[in] period - Timer load value
 * @param[in] prescaler - Timer prescale value
 * @param[in] percenetDutyCycle - PWM duty cycle positive
 * @note Store period to be able to adjust duty cycle percentage later
 */
void TIMA1_PWM_init(uint8_t pin, uint32_t period, uint32_t prescaler, double percentDutyCycle)
{
	uint32_t ccValue = (uint32_t)((percentDutyCycle / 100.0) * period);

	// Check if the timer is already powered on
	if ((TIMA1->GPRCM.PWREN & GPTIMER_PWREN_ENABLE_MASK) == 0)
	{
		// Force it to reset
		TIMA1->GPRCM.RSTCTL = GPTIMER_RSTCTL_KEY_UNLOCK_W | GPTIMER_RSTCTL_RESETASSERT_ASSERT;

		// Enable power after the reset
		TIMA1->GPRCM.PWREN = GPTIMER_PWREN_KEY_UNLOCK_W | GPTIMER_PWREN_ENABLE_ENABLE;

		for (volatile int i = 0; i < 10; i++)
		{
		}
	}
	// Select BUSCLK
	TIMA1->CLKSEL = GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE;

	// Perform clock division
	TIMA1->CLKDIV = GPTIMER_CLKDIV_RATIO_DIV_BY_1;

	// Set prescalers version below was our previous code
	// TIMG6->COMMONREGS.CPS &= ~GPTIMER_CPS_PCNT_MASK;
	TIMA1->COMMONREGS.CPS = prescaler;

	// Set the LOAD to the period
	TIMA1->COUNTERREGS.LOAD = period;

	// Enable counter value
	TIMA1->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_CVAE_LDVAL;

	// Make the counter count down, repeat and also disable it
	TIMA1->COUNTERREGS.CTRCTL = GPTIMER_CTRCTL_CM_DOWN | GPTIMER_CTRCTL_REPEAT_REPEAT_1 | GPTIMER_CTRCTL_EN_DISABLED;

	// Enable TIMCLK
	TIMA1->COMMONREGS.CCLKCTL |= GPTIMER_CCLKCTL_CLKEN_ENABLED;

	// Initialize the IOMUX (check what pin is selected and go from there (look at gitlab for table for corresponding pins)
	if (pin == 0)
	{
		IOMUX->SECCFG.PINCM[IOMUX_PINCM17] = IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM17_PF_TIMA1_CCP0;
	}

	// Determine whether it is channel 0/1 or 2/3
	uint8_t registerIndex = pin % 2;

	// Set timxcc, configure duty cycle
	if (pin < 2)
	{
		TIMA1->COUNTERREGS.CC_01[registerIndex] = ccValue;
	}
	else
	{
		TIMA1->COUNTERREGS.CC_23[registerIndex] = ccValue;
	}

	// Set compare mode, depending on if it is channel 0/1 or 2/3
	if (pin < 2)
	{
		TIMA1->COUNTERREGS.CCCTL_01[registerIndex] = GPTIMER_CCCTL_01_COC_COMPARE;
	}
	else
	{
		TIMA1->COUNTERREGS.CCCTL_23[registerIndex] = GPTIMER_CCCTL_23_COC_COMPARE;
	}

	// Configure chinese comunist party as an output for the cc block, based off the pin selected
	if (pin == 0)
	{
		TIMA1->COMMONREGS.CCPD |= GPTIMER_CCPD_C0CCP0_OUTPUT;
	}
	else if (pin == 1)
	{
		TIMA1->COMMONREGS.CCPD |= GPTIMER_CCPD_C0CCP1_OUTPUT;
	}
	else if (pin == 2)
	{
		TIMA1->COMMONREGS.CCPD |= GPTIMER_CCPD_C0CCP2_OUTPUT;
	}
	else if (pin == 3)
	{
		TIMA1->COMMONREGS.CCPD |= GPTIMER_CCPD_C0CCP3_OUTPUT;
	}

	// set CCP for compare, zero, load events, sw and fault events
	if (pin < 2)
	{
		TIMA1->COUNTERREGS.CCACT_01[registerIndex] |= GPTIMER_CCACT_01_LACT_CCP_HIGH;
		TIMA1->COUNTERREGS.CCACT_01[registerIndex] |= GPTIMER_CCACT_01_CDACT_CCP_LOW;
	}
	else
	{
		TIMA1->COUNTERREGS.CCACT_23[registerIndex] |= GPTIMER_CCACT_23_LACT_CCP_HIGH;
		TIMA1->COUNTERREGS.CCACT_23[registerIndex] |= GPTIMER_CCACT_23_CDACT_CCP_LOW;
	}

	// Set CCP0 to 0 to select signal generator output
	// Configure polarity of the signal using CCPOINV
	// Set low while disabled
	if (pin < 2)
	{
		TIMA1->COUNTERREGS.OCTL_01[registerIndex] = GPTIMER_OCTL_01_CCPO_FUNCVAL | GPTIMER_OCTL_01_CCPOINV_NOINV | GPTIMER_OCTL_01_CCPIV_LOW;
	}
	else
	{
		TIMA1->COUNTERREGS.OCTL_23[registerIndex] = GPTIMER_OCTL_23_CCPO_FUNCVAL | GPTIMER_OCTL_23_CCPOINV_NOINV | GPTIMER_OCTL_23_CCPIV_LOW;
	}

	// Enable counter
	TIMA1->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_ENABLED;
}

/**
 * @brief Change PWM duty cycle for all Timer A0 channels
 * @param[in] pin - Timer PWM output pin / channel
 * @param[in] percentDutyCycle - Duty cycle to change to
 */
void TIMA0_PWM_DutyCycle(uint8_t pin, double percentDutyCycle)
{
	uint32_t loadValue = TIMA0->COUNTERREGS.LOAD;

	// calculate count value
	uint32_t ccValue = (uint32_t)((percentDutyCycle / 100.0) * loadValue);

	// Check what channel it is and assign the count value
	if (pin < 2)
	{
		TIMA0->COUNTERREGS.CC_01[pin % 2] = ccValue;
	}
	else if (pin < 4)
	{
		TIMA0->COUNTERREGS.CC_23[pin % 2] = ccValue;
	}
}

/**
 * @brief Change PWM duty cycle for all Timer A1 channels
 * @param[in] pin - Timer PWM output pin / channel
 * @param[in] percentDutyCycle - Duty cycle to change to
 */
void TIMA1_PWM_DutyCycle(uint8_t pin, double percentDutyCycle)
{
	uint32_t loadValue = TIMA1->COUNTERREGS.LOAD;

	// calculate count value
	uint32_t ccValue = (uint32_t)((percentDutyCycle / 100.0) * loadValue);

	// Check what channel it is and assign the count value
	if (pin < 2)
	{
		TIMA1->COUNTERREGS.CC_01[pin % 2] = ccValue;
	}
	else if (pin < 4)
	{
		TIMA1->COUNTERREGS.CC_23[pin % 2] = ccValue;
	}
}
