#include <stdint.h>

#include "fsl_spi_master_driver.h"
#include "fsl_port_hal.h"
#include "fsl_lptmr_driver.h"
#include "fsl_hwtimer.h"

#include "SEGGER_RTT.h"
#include "gpio_pins.h"
#include "warp.h"
#include "devHCSR04.h"

/*
static void dummy_handler(void){};
static void (*tick_handler)(void) = &dummy_handler;

#define LPTMR_INSTANCE 0

static lptmr_state_t gLPTMRState;


void lptmr_isr_callback(void)
{
    // gLPTMR_counter++;
    (*tick_handler)();
    // printf("%d ",gLPTMR_counter);
}
*/


#define HWTIMER_LL_DEVIF kSystickDevif
#define HWTIMER_LL_ID 0
#define HWTIMER_PERIOD 1000000

extern const hwtimer_devif_t kSystickDevif;
extern const hwtimer_devif_t kPitDevif;



hwtimer_t hwtimer;

enum
{
	kHCSR04PinEcho		= GPIO_MAKE_PIN(HW_GPIOB, 10),
	kHCSR04PinTrig		= GPIO_MAKE_PIN(HW_GPIOB, 11),
};



int
takeReading()
{
	/*
	SEGGER_RTT_printf(0, "%d about to start\n", 1);

	


	lptmr_user_config_t LptmrUserConfig =
    {
        .timerMode = kLptmrTimerModeTimeCounter, // Use LPTMR in Time Counter mode
        .freeRunningEnable = false, // When hit compare value, set counter back to zero
        .prescalerEnable = false, // bypass prescaler
        //.prescalerClockSource = kClockLptmrSrcLpoClk, // use 1kHz Low Power Clock
        .prescalerClockSource = 0x00,
        .isInterruptEnabled = true
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
    LPTMR_DRV_SetTimerPeriodUs(LPTMR_INSTANCE,250000);

    LPTMR_DRV_InstallCallback(LPTMR_INSTANCE,lptmr_isr_callback);

    

    status = LPTMR_DRV_Start(LPTMR_INSTANCE);

    SEGGER_RTT_printf(0, "%drd stage passed\n", 3);

    if (status != kStatus_LPTMR_Success)
    {
    	SEGGER_RTT_printf(0, "%d Starter did not work\n", 1);
    	return 0;
    }

    //OSA_TimeDelay(1);

    uint32_t time = LPTMR_DRV_GetCurrentTimeUs(LPTMR_INSTANCE);

    while (time < 1)
    {
    	time = LPTMR_DRV_GetCurrentTimeUs(LPTMR_INSTANCE);
    }

   
    status = LPTMR_DRV_Stop(LPTMR_INSTANCE);
    if (status != kStatus_LPTMR_Success)
    {
    	SEGGER_RTT_printf(0, "%d Stopper did not work\n", 1);
    	return 0;
    }


    SEGGER_RTT_printf(0, "%d time stopped\n", 1);
    


	

	SEGGER_RTT_printf(0, "%dtime received\n", 1);



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



	if (kHwtimerSuccess != HWTIMER_SYS_Init(&hwtimer, &HWTIMER_LL_DEVIF, HWTIMER_LL_ID, 0, NULL))
	{
		SEGGER_RTT_printf(0,"\r\nError: hwtimer initialization.\r\n");
	}

	SEGGER_RTT_printf(0,"Initialise success\n");

	if (kHwtimerSuccess != HWTIMER_SYS_SetFreq(&hwtimer, kCoreClock, HWTIMER_PERIOD))
    {
         SEGGER_RTT_printf(0,"\r\nError: hwtimer set period.\r\n");
    }

    SEGGER_RTT_printf(0,"Frequency set\n");

    if (kHwtimerSuccess != HWTIMER_SYS_Start(&hwtimer))
    {
         SEGGER_RTT_printf(0,"\r\nError: hwtimer start.\r\n");
    }

    SEGGER_RTT_printf(0,"timer started\n");

    while (HWTIMER_SYS_GetTicks(&hwtimer) < 1)
    {
    	continue;
    }

    uint32_t time = HWTIMER_SYS_GetTicks(&hwtimer);

    SEGGER_RTT_printf(0,"time extracted\n");

    if (kHwtimerSuccess != HWTIMER_SYS_Stop(&hwtimer))
    {
         SEGGER_RTT_printf(0,"\r\nError: hwtimer stop.\r\n");
    }

    SEGGER_RTT_printf(0,"timer stopped\n");

    SEGGER_RTT_printf(0, "%d\n", time);
	return 1;
}

/*

void hal_tick_set_handler(void (*handler)(void)) { //this will get called every "hal_tick_get_tick_period_in_ms"

    if (handler == NULL)
    {
        tick_handler = &dummy_handler;
        return;
    }
    tick_handler = handler;
}

int hal_tick_get_tick_period_in_ms(void){
    return 250;
}
*/
