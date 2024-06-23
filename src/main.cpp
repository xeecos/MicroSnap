#include "CH58x_common.h"
#include "config.h"
int main(void)
{
#if(defined(DCDC_ENABLE)) && (DCDC_ENABLE == TRUE)
    PWR_DCDCCfg(ENABLE);
#endif
    SetSysClock(CLK_SOURCE_PLL_60MHz);
    /* for blinky */
    GPIOA_ModeCfg(PIN_STEPPER_DIR|PIN_STEPPER_STEP, GPIO_ModeOut_PP_20mA);
    GPIOB_ModeCfg(PIN_LED|PIN_STEPPER_EN, GPIO_ModeOut_PP_20mA);
    GPIOB_SetBits(PIN_LED);
    GPIOB_ResetBits(PIN_STEPPER_EN);
    GPIOA_SetBits(PIN_STEPPER_DIR);
    for(int i = 0; i < 1000; i++)
    {
        GPIOA_SetBits(PIN_STEPPER_STEP);
        DelayUs(1);
        GPIOA_ResetBits(PIN_STEPPER_STEP);
        DelayUs(400);
    }
    GPIOA_ResetBits(PIN_STEPPER_DIR);
    for(int i = 0; i < 1000; i++)
    {
        GPIOA_SetBits(PIN_STEPPER_STEP);
        DelayUs(1);
        GPIOA_ResetBits(PIN_STEPPER_STEP);
        DelayUs(400);
    }
    GPIOB_SetBits(PIN_STEPPER_EN);
    while (1)
    {
        GPIOB_SetBits(PIN_LED);
        DelayMs(1000);
        GPIOB_ResetBits(PIN_LED);
        DelayMs(1000);
    }
}


/******************************** endfile @ main ******************************/