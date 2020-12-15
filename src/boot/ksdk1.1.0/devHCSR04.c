#include <stdint.h>

#include "fsl_spi_master_driver.h"
#include "fsl_port_hal.h"

#include "SEGGER_RTT.h"
#include "gpio_pins.h"
#include "warp.h"
#include "devHCSR04.h"


enum
{
	kHCSR04PinEcho		= GPIO_MAKE_PIN(HW_GPIOA, 3),
	kHCSR04PinTrig		= GPIO_MAKE_PIN(HW_GPIOA, 4),
};

int
takeReading()
{
	// Set the trigger pin high for 10 microseconds
	GPIO_DRV_SetPinOutput(kHCSR04PinTrig);
	OSA_TimeDelay(10);
	GPIO_DRV_ClearPinOutput(kHCSR04PinTrig);
	int counter;

	//start timer
	while (GPIO_DRV_ReadPinInput(kHCSR04PinEcho) == 1)
	{
		OSA_TimeDelay(1);
		counter += 1;
		continue
	}
	//stop timer

	int distance = counter * 0.01715;

	SEGGER_RTT_printf(0, "%d\n", distance);
	return distance
}