#include <stdint.h>

#include "fsl_spi_master_driver.h"
#include "fsl_port_hal.h"
#include "fsl_lptmr_driver.h"

#include "SEGGER_RTT.h"
#include "gpio_pins.h"
#include "warp.h"
#include "devHCSR04.h"

static void dummy_handler(void){};
static void (*tick_handler)(void) = &dummy_handler;

#define LPTMR_INSTANCE 0

static lptmr_state_t gLPTMRState;




enum
{
	kHCSR04PinEcho		= GPIO_MAKE_PIN(HW_GPIOB, 10),
	kHCSR04PinTrig		= GPIO_MAKE_PIN(HW_GPIOB, 11),
};



int
takeReading()
{
	SEGGER_RTT_printf(0, "%d about to start\n", 1);


	lptmr_user_config_t LptmrUserConfig =
    {
        .timerMode = kLptmrTimerModeTimeCounter, // Use LPTMR in Time Counter mode
        .freeRunningEnable = false, // When hit compare value, set counter back to zero
        .prescalerEnable = false, // bypass prescaler
        .prescalerClockSource = kClockLptmrSrcLpoClk, // use 1kHz Low Power Clock
        .isInterruptEnabled = false
    };

    SEGGER_RTT_printf(0, "%dst stage passed\n", 1);

    // Initialize LPTMR
    lptmr_status_t status = LPTMR_DRV_Init(LPTMR_INSTANCE,&LptmrUserConfig,&gLPTMRState);

    SEGGER_RTT_printf(0, "%dnd stage passed\n", 2);

    if (status != kStatus_LPTMR_Success)
    {
    	SEGGER_RTT_printf(0, "%dnitialiser did not work\n", 1);
    	return 0;
    }


    // Set the timer period for 250 milliseconds
    //LPTMR_DRV_SetTimerPeriodUs(LPTMR_INSTANCE,250000);

    // Specify the callback function when a LPTMR interrupt occurs
    //LPTMR_DRV_InstallCallback(LPTMR_INSTANCE,lptmr_isr_callback);

    status = LPTMR_DRV_Start(LPTMR_INSTANCE);

    SEGGER_RTT_printf(0, "%drd stage passed\n", 3);

    if (status != kStatus_LPTMR_Success)
    {
    	SEGGER_RTT_printf(0, "%d Starter did not work\n", 1);
    	return 0;
    }

    OSA_TimeDelay(1);

    SEGGER_RTT_printf(0, "%d time delay passed\n", 4);

    status = LPTMR_DRV_Stop(LPTMR_INSTANCE);
    if (status != kStatus_LPTMR_Success)
    {
    	SEGGER_RTT_printf(0, "%d Stopper did not work\n", 1);
    	return 0;
    }

    SEGGER_RTT_printf(0, "%d time stopped\n", 1);

	uint32_t time = LPTMR_DRV_GetCurrentTimeUs(LPTMR_INSTANCE);



    /*

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

	while (a == 0) 
	{
		a = GPIO_DRV_ReadPinInput(kHCSR04PinEcho);
	}

	// Start counting
    LPTMR_DRV_Start(LPTMR_INSTANCE);

	while (a == 1) 
	{
		a = GPIO_DRV_ReadPinInput(kHCSR04PinEcho);
	}

	//Stop Counting
	LPTMR_DRV_Stop(LPTMR_INSTANCE);

	uint32_t time = LPTMR_DRV_GetCurrentTimeUs(LPTMR_INSTANCE);


	//OSA_TimeDelay(1000);

	//uint32_t b = GPIO_DRV_ReadPinInput(kHCSR04PinEcho);

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

	SEGGER_RTT_printf(0, "%d\n", time);
	return 1;

}

