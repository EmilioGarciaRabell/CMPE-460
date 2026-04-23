/**
 * ******************************************************************************
 * @file    : uart.c
 * @brief   : UART0 and UART1 module file
 * @details : UART initialization and send/recieve functions
 *
 * @author Cristian Morataya
 * @date 1/30/2026
 * ******************************************************************************
 */

#include <lab2/uart.h>
#include <ti/devices/msp/msp.h>
#include "../lib/sysctl.h"

/**
 * @brief Initialze UART0, start by checking that it is powered on and then setup the IOMUX, baud rate, clock, etc.
 */
void UART0_init(void)
{
	if ((UART0->GPRCM.PWREN & UART_PWREN_ENABLE_MASK) == 0)
	{
		// Force it to reset
		UART0->GPRCM.RSTCTL = UART_RSTCTL_KEY_UNLOCK_W | UART_RSTCTL_RESETASSERT_ASSERT;

		// Enable power after the reset
		UART0->GPRCM.PWREN = UART_PWREN_KEY_UNLOCK_W | UART_PWREN_ENABLE_ENABLE;

		for (volatile int i = 0; i < 10; i++)
		{
		}
	}
	// UART RX channel (recieves data)
	IOMUX->SECCFG.PINCM[IOMUX_PINCM22] |= IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM22_PF_UART0_RX;
	// Enable bit for input/recieving
	IOMUX->SECCFG.PINCM[IOMUX_PINCM22] |= IOMUX_PINCM_INENA_ENABLE;

	// UART TX channel (only transmits data so no enable bit/INENA macro)
	IOMUX->SECCFG.PINCM[IOMUX_PINCM21] |= IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM21_PF_UART0_TX;

	// Select the BUS_CLK
	UART0->CLKSEL |= UART_CLKSEL_BUSCLK_SEL_ENABLE;

	// Perform CLK division
	UART0->CLKDIV |= UART_CLKDIV2_RATIO_DIV_BY_1;

	// Disable UART
	UART0->CTL0 &= ~UART_CTL0_ENABLE_DISABLE;

	// Set oversampling
	UART0->CTL0 |= UART_CTL0_HSE_OVS16;

	// Transit enable
	UART0->CTL0 |= UART_CTL0_TXE_ENABLE;

	// Recieve enable
	UART0->CTL0 |= UART_CTL0_RXE_ENABLE;

	// FIFO enable
	UART0->CTL0 |= UART_CTL0_FEN_ENABLE;

	// Disable before doing the math for the baud rate
	UART0->CTL0 |= UART_CTL0_ENABLE_DISABLE;

	// Doing the baurd-rate divisor from section 18.2.3.4 in TRM
	float BRD = SYSCTL_SYSCLK_getULPCLK() / (BAUD_RATE * 16);

	// Get the integer/whole value portion of the BRD
	int brdInt = (int)BRD;

	// Get the fractional value of the BRD
	float brdFloat = BRD - (float)brdInt;

	// Write to the IBRD register
	UART0->IBRD = brdInt;

	// Write to the FBRD register
	UART0->FBRD = brdFloat;

	UART0->LCRH |= UART_LCRH_PEN_DISABLE | UART_LCRH_STP2_DISABLE | UART_LCRH_WLEN_DATABIT8;

	// Re-enable UART after disenabling it earlier
	UART0->CTL0 |= UART_CTL0_ENABLE_ENABLE;
}

/**
 * @brief Put a character over UART0
 * @param[in] ch - Character to print
 */
void UART0_putchar(char ch)
{
	// Loop until the queue is not full
	while (UART0->STAT & UART_STAT_TXFF_MASK)
	{
		;
	}
	// Transmit the outgoing char
	UART0->TXDATA = ch;
}

/**
 * @brief Retrieve a single character from UART0
 */
char UART0_getchar(void)
{
	// Loop until there is some input
	while ((UART0->STAT & UART_STAT_RXFE_SET))
	{
		;
	}
	return (UART0->RXDATA);
}

/**
 * @brief Send a full character string over UART0
 * @param[in] ptr_str - Pointer to the string to print
 */
void UART0_put(char *ptr_str)
{
	// Go through string in memory and iterate until the null terminator is reached
	while (*ptr_str != '\0')
	{
		// Call put char to put in the current character
		UART0_putchar(*ptr_str);
		// Move onto the next character in the address
		ptr_str++;
	}
}

/**
 * @brief Init for UART1, same exact thing as uart0 except that the PINCM numbers are different
 */
void UART1_init(void)
{
	if ((UART1->GPRCM.PWREN & UART_PWREN_ENABLE_MASK) == 0)
	{
		// Force it to reset
		UART1->GPRCM.RSTCTL = UART_RSTCTL_KEY_UNLOCK_W | UART_RSTCTL_RESETASSERT_ASSERT;

		// Enable power after the reset
		UART1->GPRCM.PWREN = UART_PWREN_KEY_UNLOCK_W | UART_PWREN_ENABLE_ENABLE;

		for (volatile int i = 0; i < 10; i++)
		{
		}
	}
	// UART RX channel (recieves data)
	IOMUX->SECCFG.PINCM[IOMUX_PINCM20] |= IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM20_PF_UART1_RX;
	// Enable bit for input/recieving
	IOMUX->SECCFG.PINCM[IOMUX_PINCM20] |= IOMUX_PINCM_INENA_ENABLE;

	// UART TX channel (only transmits data so no enable bit/INENA macro)
	IOMUX->SECCFG.PINCM[IOMUX_PINCM19] |= IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM19_PF_UART1_TX;

	// Select the BUS_CLK
	UART1->CLKSEL |= UART_CLKSEL_BUSCLK_SEL_ENABLE;

	// Perform CLK division
	UART1->CLKDIV |= UART_CLKDIV2_RATIO_DIV_BY_1;

	// Disable UART
	UART1->CTL0 &= ~UART_CTL0_ENABLE_DISABLE;

	// Set oversampling
	UART1->CTL0 |= UART_CTL0_HSE_OVS16;

	// Transit enable
	UART1->CTL0 |= UART_CTL0_TXE_ENABLE;

	// Recieve enable
	UART1->CTL0 |= UART_CTL0_RXE_ENABLE;

	// FIFO enable
	UART1->CTL0 |= UART_CTL0_FEN_ENABLE;

	// Disable before doing the math for the baud rate
	UART1->CTL0 |= UART_CTL0_ENABLE_DISABLE;

	// Doing the baurd-rate divisor from section 18.2.3.4 in TRM
	float BRD = SYSCTL_SYSCLK_getULPCLK() / (BAUD_RATE * 16);

	// Get the integer/whole value portion of the BRD
	int brdInt = (int)BRD;

	// Get the fractional value of the BRD
	float brdFloat = BRD - (float)brdInt;

	// Write to the IBRD register
	UART1->IBRD = brdInt;

	// Write to the FBRD register
	UART1->FBRD = brdFloat;

	UART1->LCRH |= UART_LCRH_PEN_DISABLE | UART_LCRH_STP2_DISABLE | UART_LCRH_WLEN_DATABIT8;

	// Re-enable UART after disenabling it earlier
	UART1->CTL0 |= UART_CTL0_ENABLE_ENABLE;
}

/**
 * @brief Put a character over UART1
 * @param[in] ch - Character to print
 */
void UART1_putchar(char ch)
{
	while (UART1->STAT & UART_STAT_TXFF_MASK)
	{
		;
	}
	// Transmit the outgoing char
	UART1->TXDATA = ch;
}

/**
 * @brief Retrieve a single character from UART1
 */
char UART1_getchar(void)
{
	while (UART1->STAT & UART_STAT_RXFE_SET)
	{
		;
	}
	return (UART1->RXDATA);
}

/**
 * @brief Send a full character string over UART1
 * @param[in] ptr_str - Pointer to the string to print
 */
void UART1_put(char *ptr_str)
{
	// Go through string in memory and iterate until the null terminator is reached
	while (*ptr_str != '\0')
	{
		// Call put char to put in the current character
		UART1_putchar(*ptr_str);
		// Move onto the next character in the address
		ptr_str++;
	}
}

/**
 * @brief Function to check if there is something to be read in UART0
 */
int UART0_dataAvailable(void)
{
	// Check the stat register to see if the FIFO is empty
	if (UART0->STAT & UART_STAT_RXFE_SET)
	{
		return false;
	}
	else
	{
		return true;
	}
}

/**
 * @brief Function to check if there is something to be read in UART1
 */
int UART1_dataAvailable(void)
{
	// Check the stat register to see if the FIFO is empty
	if (UART1->STAT & UART_STAT_RXFE_SET)
	{
		return false;
	}
	else
	{
		return true;
	}
}
