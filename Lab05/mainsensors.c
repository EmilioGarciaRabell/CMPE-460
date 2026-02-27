/**
 * ******************************************************************************
 * @file    : main.c
 * @brief   : Main file for Lab 5 part 2 demo
 * @details : This main file handles part 2 of the lab which functions by reading the ADC value and converting it to hex and then to temperature in both Celsius and Fahrenheit
 *
 * @author Emilio Garcia Rabell & Cristian Morataya
 * @date 2/26/2026
 * ******************************************************************************
 */

#include "lab1/leds.h"
#include "lab5/switches.h"
#include "lab5/timers.h"
#include <lab2/uart.h>
#include "lab5/adc12.h"
#include <ti/devices/msp/msp.h>
#include <uart_extras.c>
#include <sysctl.h>

// Prescaler for the timer
#define TIMG6_PRESCALER 32u

// Function declarations
void part2(void);
void print_hexa(uint32_t dec_val);

// Main function where everything is initialized and runs in an infinite loop so that interrupts can happen
void part2()
{
    // Get the load value for the timer with the system clock, prescaler and desired frequency
    uint32_t TIMG6LoadValue = (SYSCTL_SYSCLK_getULPCLK() / 8 / (TIMG6_PRESCALER + 1)) - 1;
    // Initialize all the peripherals
    UART0_init();
    S1_init_interrupt();
    S2_init_interrupt();
    LED1_init();
    LED2_init();
    TIMG6_init(TIMG6LoadValue, TIMG6_PRESCALER);
    ADC0_init();

    // Infinite loop to keep the program running and allow interrupts to happen
    while (1)
    {
    }
}

// ISR for the TIMG6 timer, this is where the ADC value is read and turned into hex and then temperature
void TIMG6_IRQHandler(void)
{
    uint32_t adc_result = ADC0_getVal();

    // Use the input voltage divide by the maximum value
    float voltage = adc_result * (3.3f / 4.0950f) / 100;

    // Celcius conversion the 0.5 is the offset from the datasheet
    float tempCelcius = (voltage - 0.5f);

    // Convert to farenheit, regular conversion formula
    float tempFarenheit = (tempCelcius * 9.0f / 5.0f) + 32.0f;

    // print decimal ADC result
    UART0_put("\n\r Decimal ADC Value: ");
    UART0_printDec(adc_result);

    // Print hex ADC result
    print_hexa(adc_result);

    // Print out both temperatures in C and F
    UART0_put("\n\r Temp (C): ");
    UART0_printDec((int32_t)tempCelcius);
    UART0_put("\n\r Temp (F): ");
    UART0_printDec((int32_t)tempFarenheit);
}

// Helper function to convert the decimal value to hex and print it out
void print_hexa(uint32_t dec_val)
{
    int i = 1, j, temp;
    // Array to store hexadecimal number
    char hexa_num[100];
    while (dec_val != 0)
    {
        temp = dec_val % 16;

        // converting decimal number
        // in to a hexa decimal
        // number
        if (temp < 10)
            temp = temp + 48;
        else
            // adding 55 to convert to ASCII character
            temp = temp + 55;
        hexa_num[i++] = temp;
        dec_val = dec_val / 16;
    }
    // printing the hexa decimal number
    UART0_put("\n\r Hexadecimal ADC Value: ");
    for (j = i - 1; j > 0; j--)
        UART0_putchar(hexa_num[j]);
}

int main()
{
    part2();
}