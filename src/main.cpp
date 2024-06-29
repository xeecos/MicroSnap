#include "pinmap.h"
#include "config.h"
#include "HAL.h"
#include "gattprofile.h"
#include "peripheral.h"
#include "app_usb.h"
#include "uart.h"
#include "stepper.h"
#include "task.h"
__attribute__((aligned(4))) uint32_t MEM_BUF[BLE_MEMHEAP_SIZE / 4];

__HIGH_CODE
__attribute__((noinline)) void Main_Circulation()
{
    long ms = TMOS_GetSystemClock();
    long ledtime = ms;
    while (1)
    {
        if (TMOS_GetSystemClock() - ms > 0)
        {
            ms = TMOS_GetSystemClock();
            task_running();
        }
        if (TMOS_GetSystemClock() - ledtime > 160)
        {
            ledtime = TMOS_GetSystemClock();
            GPIOB_InverseBits(PIN_LED);
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
    stepper_init();
    task_init();
    Main_Circulation();
}
/******************************** endfile @ main ******************************/