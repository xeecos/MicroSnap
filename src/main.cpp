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

__HIGH_CODE
__attribute__((noinline)) void Main_Circulation()
{
    long ms = TMOS_GetSystemClock();
    while (1)
    {
        if (TMOS_GetSystemClock() - ms > 1)
        {
            GPIOB_InverseBits(PIN_LED);
            ms = TMOS_GetSystemClock();
            stepper_running();
            task_running();
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
    uart_init();
    stepper_init();
    task_init();
    Main_Circulation();
}
/******************************** endfile @ main ******************************/