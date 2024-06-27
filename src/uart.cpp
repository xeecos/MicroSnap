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
uint16_t user_rx_buffer_length_mask = 1024-1;
uint16_t user_rx_buffer_write_index = 0;
uint16_t user_tx_buffer_length_mask = 1024-1;
uint16_t user_tx_buffer_write_index = 0;
uint16_t user_tx_buffer_read_index = 0;
uint8_t uart_rx_buffer[1024];
uint8_t uart_tx_buffer[1024];
 
void uart_send(uint8_t*msg,int len)
{
    for (int i = 0; i < len;i++)
    {
        while(R8_UART3_TFC == UART_FIFO_SIZE);
        R8_UART3_THR = msg[i];
    }
}