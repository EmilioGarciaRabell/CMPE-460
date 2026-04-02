/**
 * ******************************************************************************
 * @file    : main.c
 * @brief   : Main file for Lab 6
 * @details : Contains the main functions for part 1A, 1B, 2, 3 and 4 of Lab 6
 *
 * @author Cristian Morataya & Emilio Garcia Rabell
 * @date 3/17/2026
 * ******************************************************************************
 */

#include "lab6/timers.h"
#include <ti/devices/msp/msp.h>
#include <sysctl.h>

// Declarations of the different parts of the lab
void part1A(void);
void part1B(void);
void part2(void);
void part3(void);
void part4(void);

// Helper function for counting up or down
int count_op(int count, int op);

// Timer for a 10 ms delay
static void delay_10ms(void)
{
    for (volatile long i = 0; i < 320000; i++)
    {
    }
}

// Timer for a 1 s delay
static void delay_1s(void)
{
    for (volatile long i = 0; i < 3200000; i++)
    {
    }
}

// Part 1 function to create a 20% duty cycle signal at 10k kHz
void part1A()
{
    // Calculate the load value
    uint32_t TIMA0LoadValue = (SYSCTL_SYSCLK_getMCLK() / (0 + 1) / 10000) - 1;

    // Initialize the PWM for a 20% duty cycle
    TIMA0_PWM_init(0, TIMA0LoadValue, 0, 80.0);

    // Loop so that the program just runs
    while (1)
    {
    }
}

// Helper function to count up or down based on the value of op
int count_op(int count, int op)
{
    if (op)
    {
        return ++count;
    }
    return --count;
}

// Second function for part 1 the DC motor with an H-bridge and two PWM signals to move it forward and backward
void part1B()
{
    // Calculate the load value
    uint32_t TIMA0LoadValue = (SYSCTL_SYSCLK_getMCLK() / (0 + 1) / 10000) - 1;

    // Initialize the PWM for both channels with 0% duty cycle to start
    TIMA0_PWM_init(0, TIMA0LoadValue, 0, 0.0);
    TIMA0_PWM_init(1, TIMA0LoadValue, 0, 0.0);

    // Variables for tracking the control of the motor and the signals
    int opl = 1;
    int opr = 0;
    int countl = 0;
    int countr = 100;
    int forward = 1;

    while (1)
    {
        // Check if we are moving forward
        if (forward)
        {
            TIMA0_PWM_DutyCycle(0, countl);
            TIMA0_PWM_DutyCycle(1, 0.0);
        }
        else
        {
            TIMA0_PWM_DutyCycle(0, 0.0);
            TIMA0_PWM_DutyCycle(1, countr);
        }

        // Delay by 10 ms and update the counts based on the direction
        delay_10ms();
        countl = count_op(countl, opl);
        countr = count_op(countr, opr);

        // If we are done counting down from 100 to 0 or from 0 to 100 change the direction for the right side
        if (countr >= 100 || countr < 0)
        {
            opr = !opr;
            countr = count_op(countr, opr);
            if (opr == 1)
            {
                forward = !forward;
            }
        }

        // If we are done counting down from 100 to 0 or from 0 to 100 change the direction for the left side
        if (countl >= 100 || countl < 0)
        {
            opl = !opl;
            countl = count_op(countl, opl);
            if (opl == 1)
            {
                forward = !forward;
            }
        }
    }
}

// Main function for the second part of the lab where the stepper motor is controlled with the help of the ULN2083A
void part2()
{
    // Configure the GPIO pins
    GPIOB->GPRCM.RSTCTL = GPIO_RSTCTL_KEY_UNLOCK_W | GPIO_RSTCTL_RESETASSERT_ASSERT;
    GPIOB->GPRCM.PWREN = GPIO_PWREN_KEY_UNLOCK_W | GPIO_PWREN_ENABLE_ENABLE;

    // Delay to make sure it is on
    for (volatile int i = 0; i < 10; i++)
    {
    }

    // Access the PINCM registers for the corresponding pins and enable them
    IOMUX->SECCFG.PINCM[IOMUX_PINCM12] = IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM12_PF_GPIOB_DIO00;
    IOMUX->SECCFG.PINCM[IOMUX_PINCM23] = IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM23_PF_GPIOB_DIO06;
    IOMUX->SECCFG.PINCM[IOMUX_PINCM24] = IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM24_PF_GPIOB_DIO07;
    IOMUX->SECCFG.PINCM[IOMUX_PINCM33] = IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM33_PF_GPIOB_DIO16;

    // Set the pins as outputs and set them low to start (based off PB0, PB6, PB7 and PB16)
    GPIOB->DOESET31_0 = (1 << 0) | (1 << 6) | (1 << 7) | (1 << 16);

    // Set the pins low to start
    GPIOB->DOUTCLR31_0 = (1 << 0) | (1 << 6) | (1 << 7) | (1 << 16);

    // Variables for direction of the stepper motor
    int forward = 1;
    int phase = 0;

    while (1)
    {
        // Set pins to low
        GPIOB->DOUTCLR31_0 = (1 << 0) | (1 << 6) | (1 << 7) | (1 << 16);

        // Check if we're moving forward
        if (forward)
        {
            // Set the corresponding pin high depending if it is the correct phase
            if (phase == 0)
            {
                GPIOB->DOUTSET31_0 = (1 << 6);
                phase++;
            }
            else if (phase == 1)
            {
                GPIOB->DOUTSET31_0 = (1 << 7);
                phase++;
            }
            else if (phase == 2)
            {
                GPIOB->DOUTSET31_0 = (1 << 0);
                phase++;
            }
            else
            {
                GPIOB->DOUTSET31_0 = (1 << 16);
                // After the last phase set it to 0 to start again
                phase = 0;
            }
        }
        else
        {
            // If it is not going forward do the inverse and set the corresponding pin high depending if it is the correct phase
            if (phase == 0)
            {
                GPIOB->DOUTSET31_0 = (1 << 16);
                phase++;
            }
            else if (phase == 1)
            {
                GPIOB->DOUTSET31_0 = (1 << 0);
                phase++;
            }
            else if (phase == 2)
            {
                GPIOB->DOUTSET31_0 = (1 << 7);
                phase++;
            }
            else
            {
                GPIOB->DOUTSET31_0 = (1 << 6);
                // After the last phase set it to 0 to start again
                phase = 0;
            }
        }

        // Delay it for 10 ms, this can be changed to be slower or faster by changing the value in the for loop
        delay_10ms();
    }
}

// Part 3 main function for controlling a servo motor
void part3()
{
    // Calculate the load value
    uint32_t TIMA1LoadValue = (SYSCTL_SYSCLK_getMCLK() / (9 + 1) / 50) - 1;

    // Initialize the PWM for a 7.5% (1.5 ms) duty cycle to start at the middle position
    TIMA1_PWM_init(0, TIMA1LoadValue, 9, 92.5);

    while (1)
    {
        // Change the duty cycle to move the servo to the different positions and delay for 1 second between each change
        // Do 1ms/20ms for 5% duty cycle (clockwise)
        TIMA1_PWM_DutyCycle(0, 95.0);
        delay_1s();
        // Do 1.5 ms/20ms for 7.5% duty cycle (middle)
        TIMA1_PWM_DutyCycle(0, 92.5);
        delay_1s();
        // Do 2 ms/20ms for 10% duty cycle (counter-clockwise)
        TIMA1_PWM_DutyCycle(0, 90.0);
        delay_1s();
        // Do 1.5 ms/20ms for 7.5% duty cycle (middle)
        TIMA1_PWM_DutyCycle(0, 92.5);
        delay_1s();
    }
}

// Part 4 function for control of both the motor and the servo at the same time
void part4()
{
    // Load value for TIMA0, should be 10 kHz
    uint32_t TIMA0LoadValue = (SYSCTL_SYSCLK_getMCLK() / (0 + 1) / 10000) - 1;
    // Channel 0 initialization with 30% duty cycle
    TIMA0_PWM_init(0, TIMA0LoadValue, 0, 70.0);

    // Load value for TIMA1, should be 50 Hz
    uint32_t TIMA1LoadValue = (SYSCTL_SYSCLK_getMCLK() / (9 + 1) / 50) - 1;
    // Initialize the PWM for the servo
    TIMA1_PWM_init(0, TIMA1LoadValue, 9, 92.5);

    while (1)
    {
        // Change the duty cycle to move the servo to the different positions and delay for 1 second between each change
        // Do 1ms/20ms for 5% duty cycle (clockwise)
        TIMA1_PWM_DutyCycle(0, 95.0);
        delay_1s();
        // Do 1.5 ms/20ms for 7.5% duty cycle (middle)
        TIMA1_PWM_DutyCycle(0, 92.5);
        delay_1s();
        // Do 2 ms/20ms for 10% duty cycle (counter-clockwise)
        TIMA1_PWM_DutyCycle(0, 90.0);
        delay_1s();
        // Do 1.5 ms/20ms for 7.5% duty cycle (middle)
        TIMA1_PWM_DutyCycle(0, 92.5);
        delay_1s();
    }
}

int main(void)
{
    part4();
}
