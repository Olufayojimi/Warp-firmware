#include <stdint.h>

#include "fsl_spi_master_driver.h"
#include "fsl_port_hal.h"
//#include "fsl_gpio_driver.h"

#include "SEGGER_RTT.h"
#include "gpio_pins.h"
#include "warp.h"
#include "devHCSR04.h"


enum
{
	kHCSR04PinEcho		= GPIO_MAKE_PIN(HW_GPIOB, 10),
	kHCSR04PinTrig		= GPIO_MAKE_PIN(HW_GPIOB, 11),
};



int
takeReading()
{

	GPIO_DRV_SetPinDir(kHCSR04PinEcho, kGpioDigitalInput);
	GPIO_DRV_SetPinDir(kHCSR04PinTrig, kGpioDigitalOutput);

	GPIO_DRV_ClearPinOutput(kHCSR04PinTrig);
	OSA_TimeDelay(5);


	// Set the trigger pin high for 10 microseconds
	GPIO_DRV_SetPinOutput(kHCSR04PinTrig);
	OSA_TimeDelay(10);
	GPIO_DRV_ClearPinOutput(kHCSR04PinTrig);
	//int counter;

	//start timer

	uint32_t a = GPIO_DRV_ReadPinInput(kHCSR04PinEcho);

	OSA_TimeDelay(1000);

	uint32_t b = GPIO_DRV_ReadPinInput(kHCSR04PinEcho);

	/*
	while (GPIO_DRV_ReadPinInput(kHCSR04PinEcho) == 1)
	{
		OSA_TimeDelay(1);
		counter += 1;
		continue;
	}
	*/
	//stop timer

	/*
	int distance = counter * 0.01715;

	if (counter > 0)
	{
		SEGGER_RTT_printf(0, "%d\n", distance);
	} 
	else
	{
		SEGGER_RTT_printf(0, "The code did not work %d\n", 0);
	}

	return distance;
	*/
	SEGGER_RTT_printf(0, "%d, %d\n", a, b);
	return 1;

}