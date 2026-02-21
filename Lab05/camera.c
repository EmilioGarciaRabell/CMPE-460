#include "lab5/camera.h"
#include "lab5/adc12.h"
#include "lab5/timers.h"
#include <ti/devices/msp/msp.h>
#include <sysctl.h>

void Camera_init(){
	uint32_t TIMG0_Prescaler = 0;
	uint32_t TIMG6_Prescaler = 255;
	uint32_t TIMG0LoadValue = (SYSCTL_SYSCLK_getULPCLK() / 1 / (TIMG0_Prescaler + 1) / 100000) - 1;
	uint32_t TIMG6LoadValue = (SYSCTL_SYSCLK_getULPCLK() / 1 / (TIMG6_Prescaler + 1) / 133) - 1;
	
	if ((GPIOA->GPRCM.PWREN & GPIO_PWREN_ENABLE_MASK) == 0)
	{
		// Force it to reset
		GPIOA->GPRCM.RSTCTL = GPIO_RSTCTL_KEY_UNLOCK_W | GPIO_RSTCTL_RESETASSERT_ASSERT;

		// Enable power after the reset
		GPIOA->GPRCM.PWREN = GPIO_PWREN_KEY_UNLOCK_W | GPIO_PWREN_ENABLE_ENABLE;

		// Quick delay because the TRM says that you have to wait 4 clock cycles
		for (volatile int i = 0; i < 10; i++)
		{
		}
	}
	
	// Configure SI (PA28_ - PINCM3
	IOMUX -> SECCFG.PINCM[IOMUX_PINCM3] = IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM3_PF_GPIOA_DIO28 ;
	// Configure CLK (PA12 - PINCM34
	IOMUX -> SECCFG.PINCM[IOMUX_PINCM34] = IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM34_PF_GPIOA_DIO12;
	
	// Enable outputs in the GPIO module
	GPIOA -> DOESET31_0 |= (1 << 28) | (1 << 12);//GPIO_DOE31_0_DIO0_MASK;
	
	// Intialize ADC
	ADC0_init();
	
	// Initialize TIMG0 with 100kHz
	TIMG0_init(TIMG0LoadValue, TIMG0_Prescaler);
	// Disable TIMG0
	TIMG0->COUNTERREGS.CTRCTL &= ~(0x01);//~GPTIMER_CTRCTL_EN_DISABLED;
	
	// intilize TIMG6 with ~7.6ms integration time
	TIMG6_init(TIMG6LoadValue, TIMG6_Prescaler);
}

uint8_t Camera_isDataReady(){
return cameraData_complete;
}


void TIMG6_IRQHandler(){

}