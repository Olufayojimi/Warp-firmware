#include <stdint.h>
#include <stdio.h>

#include "fsl_spi_master_driver.h"
#include "fsl_port_hal.h"
#include "fsl_debug_console.h"
#include "fsl_clock_manager.h"

#include "SEGGER_RTT.h"
#include "gpio_pins.h"
#include "warp.h"
#include "devHCSR04.h"


// Declare the pins that will control the HC-SR04 sensor
enum
{
	kHCSR04PinEcho		= GPIO_MAKE_PIN(HW_GPIOB, 10),
	kHCSR04PinTrig		= GPIO_MAKE_PIN(HW_GPIOB, 11),
};


int
takeReading(int option)
{
	
	//Set the direction of the pins
	GPIO_DRV_SetPinDir(kHCSR04PinEcho, kGpioDigitalInput);
	GPIO_DRV_SetPinDir(kHCSR04PinTrig, kGpioDigitalOutput);

    int total = 0;
    int loop = 0;


    // Set the number of iterations depending on if it is for calibration or continuous reading
    if (option == 0)
    {
    	loop = 200;
    }
    else
    {
    	loop = 20;
    }

    for (int i=0; i < loop; i++)
    {

    	GPIO_DRV_ClearPinOutput(kHCSR04PinTrig);
		OSA_TimeDelay(1);


		// Set the trigger pin high for 10 milliseconds
		GPIO_DRV_SetPinOutput(kHCSR04PinTrig);
		OSA_TimeDelay(10);
		GPIO_DRV_ClearPinOutput(kHCSR04PinTrig);

		uint32_t a = GPIO_DRV_ReadPinInput(kHCSR04PinEcho);

		// Start counting when a goes high
		while (a == 0) 
		{
			a = GPIO_DRV_ReadPinInput(kHCSR04PinEcho);
		}

    	int counter = 0;
    	
    	while (a == 1) 
		{
			a = GPIO_DRV_ReadPinInput(kHCSR04PinEcho);
			counter += 1;
			
		}

		total += counter;
    }

    int average = total/loop;
    SEGGER_RTT_printf(0,"%d\n", average);

    return average;
}

