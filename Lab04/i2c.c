#include "../lib/lab4/I2C/i2c.h"
#include <ti/devices/msp/msp.h>
#include "../lib/sysctl.c"

void I2C1_init(uint16_t targetAddress)
{
	if ((I2C1->GPRCM.PWREN & I2C_PWREN_ENABLE_MASK) == 0)
	{
		// Force it to reset
		I2C1->GPRCM.RSTCTL = I2C_RSTCTL_KEY_UNLOCK_W | I2C_RSTCTL_RESETASSERT_ASSERT;

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
	I2C1 ->CLKSEL |= I2C_CLKSEL_BUSCLK_SEL_ENABLE;
	
	// Perform CLK division
	I2C1 ->CLKDIV |= I2C_CLKDIV_RATIO_DIV_BY_1;
	
	// Disable analog glitch suppression
	I2C1->GFCTL |= I2C_GFCTL_AGFEN_DISABLE;
	
	// Disable the controller
	I2C1 ->MASTER.MCR =  I2C_MCR_ACTIVE_DISABLE;
	
	// Clear the control register
	I2C1 ->MASTER.MCTR = 0;
	
	// Set the TPR, this math comes from 20.2.1.1, gives you the number automatically
	I2C1->MASTER.MTPR = 7;
	
	// FIFO RX and TX triggers
	I2C1->MASTER.MFIFOCTL = I2C_MFIFOCTL_RXTRIG_LEVEL_1 | I2C_MFIFOCTL_TXTRIG_EMPTY;
	
	// Disable clock stretching, step 8
	I2C1->MASTER.MCR |= I2C_MCR_CLKSTRETCH_DISABLE;
	
	// Set the target address that gets passed through
	I2C1->MASTER.MSA = targetAddress;
	
	// Re enable the controller for use
	I2C1 ->MASTER.MCR |= I2C_MCR_ACTIVE_ENABLE;
}
	
void I2C1_putchar(unsigned char ch)
{
		while(((I2C1->MASTER.MFIFOSR & I2C_MFIFOSR_TXFIFOCNT_MASK)>>I2C_MFIFOSR_TXFIFOCNT_OFS)== 0)
		{
			;
		}
		I2C1 -> MASTER.MTXDATA = ch;
}



void I2C1_put(unsigned char *data, uint16_t data_size)
{
		// Check if the data in the pointer is empty or if the size of the data is non existent
    if(data_size == 0 || data == 0)
		{
			return;
		}
		
		I2C1->MASTER.MSA = (I2C1->MASTER.MSA & I2C_MSA_SADDR_MASK) | I2C_MSA_DIR_TRANSMIT;
		
		for(int i = 0; i<data_size; i++)
		{
			I2C1_putchar(data[i]);
		}
		
		uint32_t transactionLength = ((uint32_t)data_size << I2C_MCTR_MBLEN_OFS) & I2C_MCTR_MBLEN_MASK;
		
		I2C1->MASTER.MCTR= transactionLength| I2C_MCTR_START_ENABLE | I2C_MCTR_STOP_ENABLE | I2C_MCTR_BURSTRUN_ENABLE;
		
		while(I2C1->MASTER.MSR & I2C_MSR_BUSY_MASK)
		{
			;
		}
}
	