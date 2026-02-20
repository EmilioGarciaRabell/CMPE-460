#include <lab5/timers.h>
#include <ti/devices/msp/msp.h>

/**
 * @brief Timer G0 module initialization. General purpose timer
 * @note Timer G0 is in Power Domain 0. Check page 3 of the Data Sheet
 */
void TIMG0_init(uint32_t period, uint32_t prescaler)
{
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
	// Select BUSCLK TODO: Verify in a later stage if this clk is the best choice (MFCLK or LFCLK)
	TIMG0->CLKSEL |= GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE;

	// Perform clock division
	TIMG0->CLKDIV |= GPTIMER_CLKDIV_RATIO_DIV_BY_1;

	// Set prescalers code below was our previous attempt
	// TIMG0->COMMONREGS.CPS &= ~GPTIMER_CPS_PCNT_MASK;
	TIMG0->COMMONREGS.CPS = prescaler;

	// Enable TIMCLK
	TIMG0->COMMONREGS.CCLKCTL |= GPTIMER_CCLKCTL_CLKEN_MASK;
	
	TIMG0->COUNTERREGS.CTRCTL &= ~GPTIMER_CTRCTL_CM_MASK;

	// Configure timer for down counting mode
	TIMG0->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_CM_DOWN;

	// Enable periodic mode
	TIMG0->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_REPEAT_MASK;

	// Enable counter value
	TIMG0->COUNTERREGS.CTRCTL &= ~GPTIMER_CTRCTL_CVAE_MASK;

	// Set the LOAD to the period, pass in pcount
	TIMG0->COUNTERREGS.LOAD = period;

	__disable_irq();

	// Disable interrupt for PA18
	TIMG0->CPU_INT.ICLR |= GPTIMER_CPU_INT_ICLR_Z_CLR;

	// Enable interrupt for PA18
	TIMG0->CPU_INT.IMASK |= GPTIMER_CPU_INT_IMASK_Z_SET;

	// Register interrupt with NVIC
	NVIC_EnableIRQ(TIMG0_INT_IRQn);

	// Enable interrupts
	__enable_irq();
}

/**
 * @brief Timer G6 module initialization. General purpose timer
 */
void TIMG6_init(uint32_t period, uint32_t prescaler)
{

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
	TIMG6->CLKSEL |= GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE;

	// Perform clock division
	TIMG6->CLKDIV |= GPTIMER_CLKDIV_RATIO_DIV_BY_8;

	// Set prescalers version below was our previous code
	// TIMG6->COMMONREGS.CPS &= ~GPTIMER_CPS_PCNT_MASK;
	TIMG6->COMMONREGS.CPS = prescaler;

	// Enable TIMCLK
	TIMG6->COMMONREGS.CCLKCTL |= GPTIMER_CCLKCTL_CLKEN_MASK;

	TIMG6->COUNTERREGS.CTRCTL &= ~GPTIMER_CTRCTL_CM_MASK;

	// Configure timer for down counting mode
	TIMG6->COUNTERREGS.CTRCTL = GPTIMER_CTRCTL_CM_DOWN;

	// Enable periodic mode
	TIMG6->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_REPEAT_MASK;

	// Enable counter value
	TIMG6->COUNTERREGS.CTRCTL &= ~GPTIMER_CTRCTL_CVAE_MASK;
	
	TIMG6->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_DISABLED;

	// Set the LOAD to the period
	TIMG6->COUNTERREGS.LOAD = period;
	
	TIMG6->COMMONREGS.CCLKCTL |= GPTIMER_CCLKCTL_CLKEN_ENABLED;

	__disable_irq();

	// Disable interrupt for PA18
	TIMG6->CPU_INT.ICLR |= GPTIMER_CPU_INT_ICLR_Z_CLR;

	// Enable interrupt for PA18
	TIMG6->CPU_INT.IMASK |= GPTIMER_CPU_INT_IMASK_Z_SET;

	// Register interrupt with NVIC
	NVIC_EnableIRQ(TIMG6_INT_IRQn);

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
	TIMG12->CLKSEL |= GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE;

	// Perform clock division
	TIMG12->CLKDIV |= GPTIMER_CLKDIV_RATIO_DIV_BY_1; // TODO: figure out if this needs to change

	// Set prescalers
	// TIMG12->COMMONREGS.CPS &= ~GPTIMER_CPS_PCNT_MASK;

	// Enable TIMCLK
	TIMG12->COMMONREGS.CCLKCTL |= GPTIMER_CCLKCTL_CLKEN_MASK;

	TIMG12->COUNTERREGS.CTRCTL &= ~GPTIMER_CTRCTL_CM_MASK;

	// Configure timer for down counting mode
	TIMG12->COUNTERREGS.CTRCTL = GPTIMER_CTRCTL_CM_DOWN;

	// Enable periodic mode
	TIMG12->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_REPEAT_MASK;

	// Enable counter value
	TIMG12->COUNTERREGS.CTRCTL &= ~GPTIMER_CTRCTL_CVAE_MASK;
	
	TIMG12->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_DISABLED;

	// Set the LOAD to the period
	TIMG12->COUNTERREGS.LOAD = period;
	
	TIMG12->COMMONREGS.CCLKCTL |= GPTIMER_CCLKCTL_CLKEN_ENABLED;

	__disable_irq();

	// Disable interrupt for PA18
	TIMG12->CPU_INT.ICLR |= GPTIMER_CPU_INT_ICLR_Z_CLR;

	// Enable interrupt for PA18
	TIMG12->CPU_INT.IMASK |= GPTIMER_CPU_INT_IMASK_Z_SET;

	// Register interrupt with NVIC
	NVIC_EnableIRQ(TIMG12_INT_IRQn);

	TIMG12->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_ENABLED;
	
	// Enable interrupts
	__enable_irq();

}
