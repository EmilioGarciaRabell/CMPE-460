/**
 * ******************************************************************************
 * @file    : main.c
 * @brief   : Main file for Lab 4 demos
 * @details : This main file handles both mains for part 2 and part 3, part 2 displays three different strings on the OLED and part 3 displays ASCII art on the OLED that loops with switch presses
 *
 * @author Emilio Garcia Rabell & Cristian Morataya
 * @date 2/12/2026
 * ******************************************************************************
 */

#include "../lib/lab1/switches.h"
#include "../lib/lab4/I2C/oled.h"

// All functions for part 2 and part 3
void part2(void);
void part3(void);
void print_ascii(const char *ascii);
bool DisplayCameraData = false;
bool DisplayThreshCameraData = false;
unsigned char OLED_TEXT_ARR[1024];
unsigned char OLED_GRAPH_ARR[1024];

// delay constant
#define DELAY_COUNT 2000000

void s_delay(void);

// Delay function to hold the OLED for a bit
void s_delay(void)
{
    for (volatile long i = 0; i < DELAY_COUNT; i++)
        ;
}

// Main function for part 2 which displays three different strings on the OLED
void part2()
{   
    // Initialize the OLED
    OLED_Init();
    // Clear the display
    OLED_display_clear();
    OLED_draw_line(1, 1, (unsigned char *)"Hello World");
    OLED_draw_line(2, 2, (unsigned char *)"How are you?");
    OLED_draw_line(3, 3, (unsigned char *)"Goodbye");

    // Write the strings to the display
    while (1)
    {
        OLED_write_display(OLED_TEXT_ARR);
        s_delay();
    }
}

// Main function for part 3 which has three different ASCII graphics and loops through it with switch presses
void part3()
{

    int press_count = 0;

    char *cat =
        "     \n"
        "     /\\_/\\ \n"
        "    ( o o )\n"
        "     > ^ < \0";

    char *robot =
        "     \n"
        "    [o_o] \n"
        "    /|_|\\ \n"
        "    /   \\ \0";

    char *ghost =
        "   .-.   \n"
        "  (o o)  \n"
        "  | O |  \n"
        "  |   |  \n"
        " /'---'\\ \0";

    // Initialize the OLED
    OLED_Init();
    // Clear the display
    OLED_display_clear();
    // Initialize the switch
    S1_init();

    // Print the first ASCII art
    print_ascii(cat);
    s_delay();

    // Loop through the ASCII art with switch presses
    while (1)
    {
        if (S1_pressed())
        {
            press_count++;

            if (press_count == 1)
            {
                print_ascii(robot);
            }
            else if (press_count == 2)
            {
                print_ascii(ghost);
            }
            else if (press_count == 3)
            {
                print_ascii(cat);
                press_count = 0;
            }
            // Delay to hold ASCII art
            s_delay();
        }
    }
}

// Function to print out the ASCII art
void print_ascii(const char *ascii)
{
    // Clear the display   
    OLED_display_clear();
    int row = 0;
    int col = 0;
    
    // Loop until you get to the end of the string with a null terminator
    while (*ascii != '\0')
    {
        // If it is at the end, go to the next row and reset the column
        if (*ascii == '\n')
        {
            row++;
            col = 0;
        }
        else
        {
            OLED_draw(row, col, *ascii);
            col++;
        }
        ascii++;
        // Write the ASCII art to the display
        OLED_write_display(OLED_TEXT_ARR);
    }
    s_delay();
}

// Main function to call either part 2 or part 3
int main(void)
{
    part2();
}
