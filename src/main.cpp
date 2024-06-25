#include "pinmap.h"
#include "config.h"
#include "HAL.h"
#include "gattprofile.h"
#include "peripheral.h"
#include "app_usb.h"
#include "uart.h"
#include "TMCStepper.h"
TMC2208Stepper stepper(0.1);

__attribute__((aligned(4))) uint32_t MEM_BUF[BLE_MEMHEAP_SIZE / 4];

#if(defined(BLE_MAC)) && (BLE_MAC == TRUE)
const uint8_t MacAddr[6] = {0x84, 0xC2, 0xE4, 0x03, 0x02, 0x02};
#endif
int main(void)
{
#if(defined(DCDC_ENABLE)) && (DCDC_ENABLE == TRUE)
    PWR_DCDCCfg(ENABLE);
#endif
    SetSysClock(CLK_SOURCE_PLL_60MHz);
    SysTick_Config(60000);
    CH58X_BLEInit();
    HAL_Init();
    GAPRole_PeripheralInit();
    Peripheral_Init();
    app_usb_init();
    
    GPIOA_ModeCfg(PIN_STEPPER_DIR|PIN_STEPPER_STEP, GPIO_ModeOut_PP_20mA);
    GPIOB_ModeCfg(PIN_LED|PIN_STEPPER_EN, GPIO_ModeOut_PP_20mA);
    GPIOB_SetBits(PIN_LED);

    // uart_init();
    stepper.begin();
    // while (stepper.test_connection())
    // {
    //     DelayMs(1000);
    // }
    stepper.internal_Rsense(false);
    stepper.dedge(true);
    stepper.microsteps(256);
    stepper.rms_current(1000);
    stepper.toff(2);
    stepper.push();
    // GPIOB_ResetBits(PIN_STEPPER_EN);
    // GPIOA_SetBits(PIN_STEPPER_DIR);
    // for(int i = 0; i < 1000; i++)
    // {
    //     GPIOA_SetBits(PIN_STEPPER_STEP);
    //     DelayUs(400);
    //     GPIOA_ResetBits(PIN_STEPPER_STEP);
    //     DelayUs(400);
    // }
    // GPIOA_ResetBits(PIN_STEPPER_DIR);
    // for(int i = 0; i < 1000; i++)
    // {
    //     GPIOA_SetBits(PIN_STEPPER_STEP);
    //     DelayUs(400);
    //     GPIOA_ResetBits(PIN_STEPPER_STEP);
    //     DelayUs(400);
    // }
    GPIOB_SetBits(PIN_STEPPER_EN);
    while(1)
    {
        GPIOB_SetBits(PIN_LED);
        DelayMs(100);
        GPIOB_ResetBits(PIN_LED);
        DelayMs(100);
        // stepper.test_connection();
        
        // 
        TMOS_SystemProcess(); 
    }
}


/******************************** endfile @ main ******************************/