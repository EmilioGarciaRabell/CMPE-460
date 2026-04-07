/**
 * ******************************************************************************
 * @file    : uart.c
 * @brief   : UART module 
 * @details : UART initialization and interaction
 * 
 * @author Emilio Garcia Rabell
 * @date 1/29/26
 * ******************************************************************************
*/




#include <lab2/uart.h>
#include <ti/devices/msp/msp.h>
#include "../lib/sysctl.h"

void UART0_init(void)
{
	// Enable power and reset UART0 if not already enabled 
	if ((UART0->GPRCM.PWREN & UART_PWREN_ENABLE_MASK) == 0)
	{
		// Reset UART0 
		UART0->GPRCM.RSTCTL |= UART_RSTCTL_RESETASSERT_ASSERT |
							   UART_RSTCTL_KEY_UNLOCK_W |
							   UART_RSTCTL_RESETSTKYCLR_CLR;

		// Enable power to UART0
		UART0->GPRCM.PWREN |= UART_PWREN_KEY_UNLOCK_W |
							  UART_PWREN_ENABLE_ENABLE;
	}


	// Configure IOMUX 
	
	// Configure RX pin
	IOMUX->SECCFG.PINCM[IOMUX_PINCM22] |= (IOMUX_PINCM_PC_CONNECTED |
										   IOMUX_PINCM22_PF_UART0_RX |
										   IOMUX_PINCM_INENA_ENABLE);

	// Configure TX pin
	IOMUX->SECCFG.PINCM[IOMUX_PINCM21] |= (IOMUX_PINCM_PC_CONNECTED |
										   IOMUX_PINCM21_PF_UART0_TX);

	// Clock configuration
	
	// Select BUSCLK 
	UART0->CLKSEL = UART_CLKSEL_BUSCLK_SEL_ENABLE;

	// Set clock divider
	UART0->CLKDIV = UART_CLKDIV2_RATIO_DIV_BY_1;

	// UART configuration
	
	// Disable UART before configuration
	UART0->CTL0 &= ~UART_CTL0_ENABLE_DISABLE;

	// Configure UART: 16x oversampling, enable TX/RX, enable FIFO
	UART0->CTL0 = UART_CTL0_HSE_OVS16 |
				  UART_CTL0_TXE_ENABLE |
				  UART_CTL0_RXE_ENABLE |
				  UART_CTL0_FEN_ENABLE;

	// Disable UART 
	UART0->CTL0 |= UART_CTL0_ENABLE_DISABLE;

	// Baud rate configuration
	float brd = SYSCTL_SYSCLK_getULPCLK() /
				(OVERSAMPLING_RATE * BAUD_RATE);

	int brd_int = (int)brd;
	float brd_dec = brd - (float)brd_int;

	// Baud rate registers 
	UART0->IBRD = brd_int;
	UART0->FBRD = brd_dec; /* TODO: fractional equation may be incorrect */

	// Line control configuration 
	
	// 8 data bits, 1 stop bit, no parity 
	UART0->LCRH |= UART_LCRH_PEN_DISABLE |
				   UART_LCRH_STP2_DISABLE |
				   UART_LCRH_WLEN_DATABIT8;

	// Re-enable UART 
	UART0->CTL0 |= UART_CTL0_ENABLE_ENABLE;
}

/**
 * @brief Put a character over UART0
 * @param[in] ch - Character to print
 */
void UART0_putchar(char ch)
{
	//  Wait until the Transmit FIFO is not full,
	while (UART0->STAT & UART_STAT_TXFF_MASK)
	{
	}
	// transmit the character
	UART0->TXDATA = ch;
}

/**
 * @brief Retrieve a single character from UART0
 */
char UART0_getchar(void)
{
	// Wait until RX FIFO is NOT empty
	while (UART0->STAT & UART_STAT_RXFE_SET)
	{
		// wait
	}

	return (UART0->RXDATA);
}

/**
 * @brief Send a full character string over UART0
 * @param[in] ptr_str - Pointer to the string to print
 */
void UART0_put(char *ptr_str)
{
	// Traverse each character in the string until a null terminator is hit
	while (*ptr_str != '\0')
	{
		// call the putchar function to transmit
		UART0_putchar(*ptr_str);
		ptr_str++;
	}
}





void UART1_init(void)
{
	// Enable power and reset UART0 if not already enabled 
	if ((UART1->GPRCM.PWREN & UART_PWREN_ENABLE_MASK) == 0)
	{
		// Reset UART0 
		UART1->GPRCM.RSTCTL |= UART_RSTCTL_RESETASSERT_ASSERT |
							   UART_RSTCTL_KEY_UNLOCK_W |
							   UART_RSTCTL_RESETSTKYCLR_CLR;

		// Enable power to UART0
		UART1->GPRCM.PWREN |= UART_PWREN_KEY_UNLOCK_W |
							  UART_PWREN_ENABLE_ENABLE;
	}


	// Configure IOMUX 
	
	// Configure RX pin
	IOMUX->SECCFG.PINCM[IOMUX_PINCM20] |= (IOMUX_PINCM_PC_CONNECTED |
										   IOMUX_PINCM20_PF_UART1_RX |
										   IOMUX_PINCM_INENA_ENABLE);

	// Configure TX pin
	IOMUX->SECCFG.PINCM[IOMUX_PINCM19] |= (IOMUX_PINCM_PC_CONNECTED |
										   IOMUX_PINCM19_PF_UART1_TX);

	// Clock configuration
	
	// Select BUSCLK 
	UART1->CLKSEL = UART_CLKSEL_BUSCLK_SEL_ENABLE;

	// Set clock divider
	UART1->CLKDIV = UART_CLKDIV2_RATIO_DIV_BY_1;

	// UART configuration
	
	// Disable UART before configuration
	UART1->CTL0 &= ~UART_CTL0_ENABLE_DISABLE;

	// Configure UART: 16x oversampling, enable TX/RX, enable FIFO
	UART1->CTL0 = UART_CTL0_HSE_OVS16 |
				  UART_CTL0_TXE_ENABLE |
				  UART_CTL0_RXE_ENABLE |
				  UART_CTL0_FEN_ENABLE;

	// Disable UART 
	UART1->CTL0 |= UART_CTL0_ENABLE_DISABLE;

	// Baud rate configuration
	float brd = SYSCTL_SYSCLK_getULPCLK() /
				(OVERSAMPLING_RATE * BAUD_RATE);

	int brd_int = (int)brd;
	float brd_dec = brd - (float)brd_int;

	// Baud rate registers 
	UART1->IBRD = brd_int;
	UART1->FBRD = brd_dec; /* TODO: fractional equation may be incorrect */

	// Line control configuration 
	
	// 8 data bits, 1 stop bit, no parity 
	UART1->LCRH |= UART_LCRH_PEN_DISABLE |
				   UART_LCRH_STP2_DISABLE |
				   UART_LCRH_WLEN_DATABIT8;

	// Re-enable UART 
	UART1->CTL0 |= UART_CTL0_ENABLE_ENABLE;
}

/**
 * @brief Put a character over UART0
 * @param[in] ch - Character to print
 */
void UART1_putchar(char ch)
{
	//  Wait until the Transmit FIFO is not full,
	while (UART1->STAT & UART_STAT_TXFF_MASK)
	{
	}
	// transmit the character
	UART1->TXDATA = ch;
}

/**
 * @brief Retrieve a single character from UART0
 */
char UART1_getchar(void)
{
	// Wait until RX FIFO is NOT empty
	while (UART1->STAT & UART_STAT_RXFE_SET)
	{
		// wait
	}

	return (UART1->RXDATA);
}

/**
 * @brief Send a full character string over UART0
 * @param[in] ptr_str - Pointer to the string to print
 */
void UART1_put(char *ptr_str)
{
	// Traverse each character in the string until a null terminator is hit
	while (*ptr_str != '\0')
	{
		// call the putchar function to transmit
		UART1_putchar(*ptr_str);
		ptr_str++;
	}
}


int UART0_dataAvailable(){
	 if (UART0->STAT & UART_STAT_RXFE_SET)
    {
        return 0;
    }
    return 1;
}

int UART1_dataAvailable(){
	 if (UART1->STAT & UART_STAT_RXFE_SET)
    {
        return 0;
    }
    return 1;
}
