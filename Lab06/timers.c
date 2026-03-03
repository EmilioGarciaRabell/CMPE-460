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
void TIMA0_PWM_init(uint8_t pin, uint32_t period, uint32_t prescaler, double percentDutyCycle){
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
	
	// Initialize the IOMUX
	IOMUX->SECCFG.PINCM[IOMUX_PINCM25] |= IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM25_PF_GPIOB_DIO08;

	// Enable channel 0 output
	TIMA0 -> COMMONREGS.CCPD |= GPTIMER_CCPD_C0CCP0_OUTPUT;
	
	// Output Generator
	
	// Downcounting is already set
	
	// Set timxcc, configure duty cycle
	TIMA0 -> COUNTERREGS.CC_01[0] = ccValue;
	
	// Set compare mode
	TIMA0 -> COUNTERREGS.CCCTL_01[0] = 1; // Check that the index is correct
	
	// Configure chinese comunist party as an output for the cc block
	TIMA0 -> COMMONREGS.CCPD = GPTIMER_CCPD_C0CCP0_OUTPUT; // Check this is correct
	
	// set CCP for compare, zero, load events, sw and fault events
	TIMA0 -> COUNTERREGS.CCACT_01[0] = GPTIMER_CCACT_01_CDACT_MASK;
	TIMA0 -> COUNTERREGS.CCACT_01[0] = GPTIMER_CCACT_01_LACT_MASK;
	// Set CCPO to 0 to select signal generator output
	TIMA0 -> COUNTERREGS.OCTL_01[0] = GPTIMER_CCPD_C0CCP0_OFS;
	
	// Enable corresponding CCP out
	TIMA0 -> COMMONREGS.ODIS = GPTIMER_ODIS_C0CCP0_CCP_OUTPUT_OCTL;
	
	// Configure polarity of the signal unsing CCPOINV
	TIMA0 -> COMMONREGS.CCPD |= GPTIMER_OCTL_01_CCPOINV_NOINV; // TODO: Change
	
	// Enable counter
	TIMA0 -> COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_MASK;
	
}


/**
 * @brief Timer A1 module PWM initialization
 * @param[in] pin - Timer PWM output pin / channel
 * @param[in] period - Timer load value
 * @param[in] prescaler - Timer prescale value
 * @param[in] percenetDutyCycle - PWM duty cycle positive
 * @note Store period to be able to adjust duty cycle percentage later
*/
void TIMA1_PWM_init(uint8_t pin, uint32_t period, uint32_t prescaler, double percentDutyCycle);


/**
 * @brief Change PWM duty cycle for all Timer A0 channels
 * @param[in] pin - Timer PWM output pin / channel
 * @param[in] percentDutyCycle - Duty cycle to change to
*/
void TIMA0_PWM_DutyCycle(uint8_t pin, double percentDutyCycle){
	
	uint32_t loadValue = TIMA0 -> COUNTERREGS.LOAD;
	
	// calculate count value
	uint32_t ccValue = (uint32_t)((percentDutyCycle / 100.0) * loadValue);
	
	// 3. Write to the appropriate CC register based on the channel (pin)
	// pin 0 or 1 uses CC_01 array index 0 or 1
	// pin 2 or 3 uses CC_23 array index 0 or 1
	if (pin < 2) {
        TIMA0->COUNTERREGS.CC_01[pin] = ccValue;
	} else if (pin < 4) {
			TIMA0->COUNTERREGS.CC_23[pin - 2] = ccValue;
	}
}


/**
 * @brief Change PWM duty cycle for all Timer A1 channels
 * @param[in] pin - Timer PWM output pin / channel
 * @param[in] percentDutyCycle - Duty cycle to change to
*/
void TIMA1_PWM_DutyCycle(uint8_t pin, double percentDutyCycle);






