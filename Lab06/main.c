#include "lab6/timers.h"
#include <ti/devices/msp/msp.h>
#include <sysctl.h>

void part1A(void);
void part1B(void);
int count_op(int count, int op);

static void delay_10ms(void)
{
	for (volatile long i = 0; i < 320000; i++)
	{
	}
}

void part1A()
{
	// Calculate the load value
	uint32_t TIMA0LoadValue = (SYSCTL_SYSCLK_getMCLK() / (0 + 1) / 10000) - 1;

	TIMA0_PWM_init(0, TIMA0LoadValue, 0, 80.0);

	while (1)
	{
	}
}

int count_op(int count, int op){
    if(op){
        return ++count;
    }
    return --count;
}

void part1B()
{
	// Calculate the load value
	uint32_t TIMA0LoadValue = (SYSCTL_SYSCLK_getMCLK() / (0 + 1) / 10000) - 1;

	TIMA0_PWM_init(0, TIMA0LoadValue, 0, 0.0);
  TIMA0_PWM_init(1, TIMA0LoadValue, 0, 0.0);
    int opl = 1;
		int opr = 0;
    int countl = 0;
		int countr = 100;
    int forward = 1;
	
	while (1)
    {
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

        delay_10ms();
        countl = count_op(countl, opl);
				countr = count_op(countr, opr);
				
        if (countr >= 100 || countr < 0)
        {
            opr = !opr;
            countr = count_op(countr, opr);
            if (opr == 1)
            {
                forward = !forward;
            }
        }
				
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

int main(void)
{
	part1B();
}
