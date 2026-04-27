/**
 * ******************************************************************************
 * @file    : i2c.c
 * @brief   : I2C driver for the OLED display
 * @details : This file implements the I2C driver that will be called by the OLED file
 *
 * @author Emilio Garcia Rabell & Cristian Morataya
 * @date 2/12/2026
 * ******************************************************************************
 */

#include "lab4/I2C/i2c.h"
#include <ti/devices/msp/msp.h>
#include <sysctl.h>

/**
 * @brief Initialize I2C1
 * @param[in] targetAddress - Passed by OLED file. I2C target/listener address
 * @note OLED file looks for this exact prototype
 */
void I2C1_init(uint16_t targetAddress)
{
	if ((I2C1->GPRCM.PWREN & I2C_PWREN_ENABLE_MASK) == 0)
	{
		// Force it to reset
		I2C1->GPRCM.RSTCTL = I2C_RSTCTL_KEY_UNLOCK_W | I2C_RSTCTL_RESETASSERT_ASSERT | I2C_RSTCTL_RESETSTKYCLR_CLR;

		// Enable power after the reset
		I2C1->GPRCM.PWREN = I2C_PWREN_KEY_UNLOCK_W | I2C_PWREN_ENABLE_ENABLE;

		for (volatile int i = 0; i < 10; i++)
		{
		}
	}
	// Set the SDA pin
	IOMUX->SECCFG.PINCM[IOMUX_PINCM16] |= IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM16_PF_I2C1_SDA;
	IOMUX->SECCFG.PINCM[IOMUX_PINCM16] |= IOMUX_PINCM_INENA_ENABLE;

	// Set the SCL pin
	IOMUX->SECCFG.PINCM[IOMUX_PINCM15] |= IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM15_PF_I2C1_SCL;
	IOMUX->SECCFG.PINCM[IOMUX_PINCM15] |= IOMUX_PINCM_INENA_ENABLE;

	// Select the BUS_CLK
	I2C1->CLKSEL = I2C_CLKSEL_BUSCLK_SEL_ENABLE;

	// Perform CLK division
	I2C1->CLKDIV = I2C_CLKDIV_RATIO_DIV_BY_1;

	// Disable analog glitch suppression AND with mask and invert the mask in order to set the corresponding bits to disable
	I2C1->GFCTL &= ~I2C_GFCTL_AGFEN_MASK;

	// Clear the control register
	I2C1->MASTER.MCTR = 0;

	// Set the TPR, this math comes from 20.2.1.1, gives you the number automatically, try 0x1F if 7 doesnt work
	I2C1->MASTER.MTPR = 7;

	// FIFO RX and TX triggers
	I2C1->MASTER.MFIFOCTL = I2C_MFIFOCTL_RXTRIG_LEVEL_1 | I2C_MFIFOCTL_TXTRIG_EMPTY;

	// Disable clock stretching, step 8
	I2C1->MASTER.MCR &= ~I2C_MCR_CLKSTRETCH_MASK;

	// Set the target address that gets passed through
	I2C1->MASTER.MSA |= targetAddress << I2C_MSA_SADDR_OFS;

	// Re enable the controller for use
	I2C1->MASTER.MCR |= I2C_MCR_ACTIVE_ENABLE;
}

/**
 * @brief Sends a single character byte over I2C1
 * @param[in] ch - Byte to send
 */
void I2C1_putchar(unsigned char ch)
{
	// Wait until there is space in the FIFO
	while (!((I2C1->MASTER.MFIFOSR & I2C_MFIFOSR_TXFIFOCNT_MASK) >= I2C_MFIFOSR_TXFIFOCNT_MINIMUM))
		;

	// Write the data to the FIFO
	I2C1->MASTER.MTXDATA = ch;
}

/**
 * @brief Send full character string over I2C until limit is reached
 * @param[in] data - String pointer to data to send
 * @param[in] data_size - Amount of bytes to transmit
 */
void I2C1_put(unsigned char *data, uint16_t data_size)
{

	// Set where the data its going
	I2C1->MASTER.MSA |= I2C_MSA_DIR_TRANSMIT;

	uint32_t transactionLength = ((uint32_t)data_size << I2C_MCTR_MBLEN_OFS) & I2C_MCTR_MBLEN_MAXIMUM;

	// Transfer over the data
	I2C1->MASTER.MCTR |= transactionLength | I2C_MCTR_START_ENABLE | I2C_MCTR_STOP_ENABLE | I2C_MCTR_BURSTRUN_ENABLE;

	// Write the data to the FIFO
	for (int i = 0; i < data_size; i++)
	{
		I2C1_putchar(data[i]);
	}

	// Wait until the transfer is done
	while (!(I2C1->MASTER.MSR & I2C_MSR_IDLE_MASK))
		;

	// Disable the module after the transfer
	I2C1->MASTER.MCTR &= ~(I2C_MCTR_MBLEN_MASK | I2C_MCTR_BURSTRUN_MASK);
}
