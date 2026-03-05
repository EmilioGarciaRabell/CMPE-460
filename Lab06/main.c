#include "lab6/timers.h"
#include <ti/devices/msp/msp.h>
#include <sysctl.h>

void part1A(void);
void part1B(void);
void part2(void);
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

void part2()
{
    GPIOB->GPRCM.RSTCTL = GPIO_RSTCTL_KEY_UNLOCK_W | GPIO_RSTCTL_RESETASSERT_ASSERT;
    GPIOB->GPRCM.PWREN  = GPIO_PWREN_KEY_UNLOCK_W  | GPIO_PWREN_ENABLE_ENABLE;
    for (volatile int i = 0; i < 10; i++) {}

    IOMUX->SECCFG.PINCM[IOMUX_PINCM12] = IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM12_PF_GPIOB_DIO00;
    IOMUX->SECCFG.PINCM[IOMUX_PINCM23] = IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM23_PF_GPIOB_DIO06;
    IOMUX->SECCFG.PINCM[IOMUX_PINCM24] = IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM24_PF_GPIOB_DIO07;
    IOMUX->SECCFG.PINCM[IOMUX_PINCM33] = IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM33_PF_GPIOB_DIO16;

    GPIOB->DOESET31_0 = (1 << 0) | (1 << 6) | (1 << 7) | (1 << 16);

    GPIOB->DOUTCLR31_0 = (1 << 0) | (1 << 6) | (1 << 7) | (1 << 16);

    int forward = 1;
    int phase = 0;

    while (1)
    {
        GPIOB->DOUTCLR31_0 = (1 << 0) | (1 << 6) | (1 << 7) | (1 << 16);

        if (forward)
        {
            if      (phase == 0) { GPIOB->DOUTSET31_0 = (1 << 6);  phase++; }
            else if (phase == 1) { GPIOB->DOUTSET31_0 = (1 << 7);  phase++; }
            else if (phase == 2) { GPIOB->DOUTSET31_0 = (1 << 0);  phase++; }
            else                 { GPIOB->DOUTSET31_0 = (1 << 16); phase=0; }
        }
        else
        {
            if      (phase == 0) { GPIOB->DOUTSET31_0 = (1 << 16); phase++; }
            else if (phase == 1) { GPIOB->DOUTSET31_0 = (1 << 0);  phase++; }
            else if (phase == 2) { GPIOB->DOUTSET31_0 = (1 << 7);  phase++; }
            else                 { GPIOB->DOUTSET31_0 = (1 << 6);  phase=0; }
        }

        delay_10ms();
    }
}
int main(void)
{
	part2();
}
