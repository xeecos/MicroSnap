/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.1
 * Date               : 2022/05/10
 * Description        : FreeRTOS例程，使用硬件压栈，中断嵌套可选，中断函数不再使用修饰
 *                      __attribute__((interrupt("WCH-Interrupt-fast")))，
 *                      中断函数直接按照普通函数定义，只使用HIGHCODE修饰即可。
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

/******************************************************************************/
/* 头文件包含 */
#if defined(CH58X)
#include "CH58x_common.h"
#elif defined(CH59X)
#include "CH59x_common.h"
#endif

#define BLINKY_GPIO_PIN  GPIO_Pin_14


/*********************************************************************
 * @fn      main
 *
 * @brief   main function.
 *
 * @param   none
 *
 * @return  none
 */
int main(void)
{
#if(defined(DCDC_ENABLE)) && (DCDC_ENABLE == TRUE)
    PWR_DCDCCfg(ENABLE);
#endif
    SetSysClock(CLK_SOURCE_PLL_60MHz);
    /* for blinky */
    GPIOB_ModeCfg(BLINKY_GPIO_PIN, GPIO_ModeOut_PP_20mA);
    GPIOB_SetBits(BLINKY_GPIO_PIN);
    while (1)
    {
        GPIOB_SetBits(BLINKY_GPIO_PIN);
        DelayMs(1000);
        GPIOB_ResetBits(BLINKY_GPIO_PIN);
        DelayMs(1000);
    }
}


/******************************** endfile @ main ******************************/