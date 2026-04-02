#include "lab5/timers.h"
#include <lab2/uart.h>
#include "lab5/adc12.h"
#include <ti/devices/msp/msp.h>
#include <uart_extras.h>
#include <sysctl.h>

// Prescaler for TIMG6, using the same one as in lab 5
#define TIMG6_PRESCALER 32u

// Threshold to detect a beat, this will probably be tweaked with testing
#define PEAK_THRESHOLD 2457u

// variables for the beat detection and to calculate BPM
static volatile int timer_counter = 0;
static volatile int previous_beat_time = 0;
static volatile int above_threshold = 0;
static volatile int average_beats[4] = {0};
static volatile int next_interval = 0;
static volatile int beats_recorded = 0;
static volatile int current_bpm = 0;

// Function to calculate the BPM
int calculate_bpm(void)
{
	// Sum the beat intervals for the recorded beats
	int final_sum = 0;

	// Loop through the recorded beats and sum their intervals
	for (int i = 0; i < beats_recorded; i++)
	{
		final_sum += average_beats[i];
	}

	// Calculate BPM by dividing the number of milliseconds in a minute by the average beat interval
	int bpm_calculation = 60000 / (final_sum / beats_recorded);

	return bpm_calculation;
}

// Interrupt handler for TIMG6, this will be called every 1ms
void TIMG6_IRQHandler(void)
{
	// Increment the timer counter every time the interrupt is called
	timer_counter++;

	// Get the ADC value
	uint32_t adc_Val = ADC0_getVal();

	// Check if the ADC value is above the threshold to detect a beat
	if (adc_Val > PEAK_THRESHOLD)
	{
		// If not above the threshold before, this means we just detected a beat
		if (!above_threshold)
		{
			// Set to 1 to show we are above the threshold
			above_threshold = 1;

			// Calculate the time since the last beat
			int time_from_last_beat = timer_counter - previous_beat_time;

			// Check if the time from the last beat is within range of a heart rate so this translates to 30 BPM to 200 BPM
			if (previous_beat_time != 0 && time_from_last_beat >= 300 && time_from_last_beat <= 2000)
			{
				// Store in array of the last 4 intervals
				average_beats[next_interval] = time_from_last_beat;
				// Go to the next interval if 4 have already been recorded
				next_interval = (next_interval + 1) % 4;

				// If not at 4, just add to the count
				if (beats_recorded < 4)
				{
					beats_recorded++;
				}

				current_bpm = calculate_bpm();
			}
			// Change the previous time to the new time of the beat
			previous_beat_time = timer_counter;
		}
	}
	else
	{	
		// Set to 0 to show it is below the threshold
		above_threshold = 0;
	}
}

// Main function for inits and loop
int main(void)
{
	// Set up the timer
	uint32_t TIMG6LoadValue = (SYSCTL_SYSCLK_getULPCLK() / 8 / (TIMG6_PRESCALER + 1) / 1000) - 1;

	// Initialize UART, timer, and ADC
	UART0_init();
	TIMG6_init(TIMG6LoadValue, TIMG6_PRESCALER);
	ADC0_init();

	// Variable for the last print time
	int previous_print = 0;

	while (1)
	{
		// Check if it has been 1 second since the last print
		if ((timer_counter - previous_print) >= 1000)
		{
			// Update the previous print time to the current time
			previous_print = timer_counter;

			// If beats have been recorded then print the BPM
			if (beats_recorded > 0)
			{
				UART0_put("\n\r Heart Rate is ");
				UART0_printDec(current_bpm);
				UART0_put("BPM");
			}
		}
	}
}