#include "uart.h"
#include "app_usb.h"
#include "config.h"
void uart_init() {
    GPIOPinRemap( DISABLE, RB_PIN_UART3 );
    GPIOA_SetBits(bTXD3);
    GPIOA_ModeCfg(bTXD3, GPIO_ModeOut_PP_5mA);
    GPIOA_SetBits(bRXD3);
    GPIOA_ModeCfg(bRXD3, GPIO_ModeIN_PU);
    UART3_DefInit();
    UART3_BaudRateCfg(115200);
}
 
void uart_send(uint8_t*msg,int len)
{
    for (int i = 0; i < len;i++)
    {
        while(R8_UART3_TFC == UART_FIFO_SIZE);
        R8_UART3_THR = *msg++;
    }
}
uint8_t uart_available()
{
    uint8_t c = R8_UART3_RFC;
    return c;
}
uint8_t uart_read()
{
    uint8_t c = R8_UART3_RBR;
    return c;
}