#include "pinmap.h"
#include "config.h"
#include "HAL.h"
#include "gattprofile.h"
#include "peripheral.h"
#include "app_usb.h"
#include "uart.h"
#include "stepper.h"
#include "task.h"
#include "stdio.h"
#include <string.h>
#include <stdlib.h>
__attribute__((aligned(4))) uint32_t MEM_BUF[BLE_MEMHEAP_SIZE / 4];
volatile uint32_t count = 0;
__HIGH_CODE
__attribute__((noinline)) 
void Main_Circulation()
{
    long ms = TMOS_GetSystemClock();
    while (1)
    {
        if(TMOS_GetSystemClock()-ms>160)
        {
            GPIOB_InverseBits(PIN_LED);
            ms = TMOS_GetSystemClock(); 
        }
        TMOS_SystemProcess();
    }
}

int main(void)
{
#if (defined(DCDC_ENABLE)) && (DCDC_ENABLE == TRUE)
    PWR_DCDCCfg(ENABLE);
#endif
    SetSysClock(CLK_SOURCE_PLL_60MHz);
    CH58X_BLEInit();
    HAL_Init();
    GAPRole_PeripheralInit();
    Peripheral_Init();
    app_usb_init();

    GPIOA_ModeCfg(PIN_STEPPER_DIR | PIN_STEPPER_STEP, GPIO_ModeOut_PP_20mA);
    GPIOB_ModeCfg(PIN_LED | PIN_STEPPER_EN, GPIO_ModeOut_PP_20mA);
    GPIOB_SetBits(PIN_STEPPER_EN);
    GPIOB_ResetBits(PIN_LED);

    DelayMs(1000);
    uart_init();
    stepper_begin();
    GPIOB_SetBits(PIN_LED);
    stepper_push();
    // GPIOB_ResetBits(PIN_STEPPER_EN);
    // GPIOA_SetBits(PIN_STEPPER_DIR);
    // for (int i = 0; i < 10000; i++)
    // {
    //     GPIOA_SetBits(PIN_STEPPER_STEP);
    //     DelayUs(50);
    //     GPIOA_ResetBits(PIN_STEPPER_STEP);
    //     DelayUs(50);
    // }
    // GPIOA_ResetBits(PIN_STEPPER_DIR);
    // for (int i = 0; i < 10000; i++)
    // {
    //     GPIOA_SetBits(PIN_STEPPER_STEP);
    //     DelayUs(50);
    //     GPIOA_ResetBits(PIN_STEPPER_STEP);
    //     DelayUs(50);
    // }
    GPIOB_SetBits(PIN_STEPPER_EN);
    // task_init();
    // TMR0_TimerInit(FREQ_SYS / 10); 
    // TMR0_ITCfg(ENABLE, TMR0_3_IT_CYC_END); 
    // PFIC_EnableIRQ(TMR0_IRQn);

    // TMR2_TimerInit(FREQ_SYS); 
    // PFIC_EnableIRQ(TMR2_IRQn);
    // TMR2_ITCfg(ENABLE, TMR0_3_IT_CYC_END);
    // 
    // int s = 0;
    // GPIOB_ResetBits(PIN_LED);
    Main_Circulation();
}
/******************************** endfile @ main ******************************/