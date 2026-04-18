#ifndef CAR_SETUP_H
#define CAR_SETUP_H
#include "config.h"
#include <ti/devices/msp/msp.h>

/**
 * @brief Initializes and enables the GPIO pins required for the DC Motor Drivers.
 * * This function performs the following steps:
 * 1. Checks if GPIOB and GPIOA power domains are enabled; if not, it resets 
 * and enables them.
 * 2. Configures the IOMUX for the specific Motor Enable (DC_EN) pins:
 * - Left Motor: GPIOB DIO19 (PINCM45)
 * - Right Motor: GPIOA DIO22 (PINCM47)
 * 3. Sets both pins as digital outputs.
 * 4. Sets the output high to enable the motor controllers.
 */
void DC_EN_init(void);

void mode_config(CarSettings *c, int mode);
#endif /* CAR_SETUP_H */