#pragma once
#include "app_usb.h"
#include "config.h"
void uart_init() {
    // if(remap){
    //     //重映射
    //     GPIOPinRemap( ENABLE, RB_PIN_UART3 );
    //     //TXD gpio初始化
    //     GPIOB_SetBits(bTXD3_);
    //     GPIOB_ModeCfg(bTXD3_, GPIO_ModeOut_PP_5mA);
    //     //RXD gpio初始化
    //     GPIOB_SetBits(bRXD3_);
    //     GPIOB_ModeCfg(bRXD3_, GPIO_ModeIN_PU);
    // } else {
        //不使能重映射
        GPIOPinRemap( DISABLE, RB_PIN_UART3 );
        //TXD gpio初始化
        GPIOA_SetBits(bTXD3);
        GPIOA_ModeCfg(bTXD3, GPIO_ModeOut_PP_5mA);
        //RXD gpio初始化
        GPIOA_SetBits(bRXD3);
        GPIOA_ModeCfg(bRXD3, GPIO_ModeIN_PU);
    // }
    //uart3 init
    UART3_DefInit();
    UART3_BaudRateCfg(115200);
 
    //enable interupt
    // UART3_INTCfg(ENABLE, RB_IER_RECV_RDY | RB_IER_LINE_STAT);
    // PFIC_EnableIRQ(UART3_IRQn);
}
uint16_t user_rx_buffer_length_mask = 1024-1;
uint16_t user_rx_buffer_write_index = 0;
uint16_t user_tx_buffer_length_mask = 1024-1;
uint16_t user_tx_buffer_write_index = 0;
uint16_t user_tx_buffer_read_index = 0;
uint8_t uart_rx_buffer[1024];
uint8_t uart_tx_buffer[1024];
 
void UART3_IRQHandler(void){
    switch( UART3_GetITFlag() ){
        case UART_II_LINE_STAT:        // 线路状态错误
            UART3_GetLinSTA();
            break;
        case UART_II_RECV_RDY:
        case UART_II_RECV_TOUT:
            while(R8_UART3_RFC) {
                // uart_rx_buffer[user_rx_buffer_write_index & user_rx_buffer_length_mask] = R8_UART3_RBR;
                // user_rx_buffer_write_index += 1;
                uint8_t c = R8_UART3_RBR;
                USBSendData(&c, 1);
            }
            break;
        case UART_II_THR_EMPTY: // 发送缓存区空，可继续发送
            break;
        case UART_II_MODEM_CHG: // 只支持串口0
            break;
        default:
            break;
     }
}
void uart_send(uint8_t*msg,int len)
{
    int i = 0;
    while(R8_UART3_TFC < UART_FIFO_SIZE)
    {
        //判断发送软件缓冲区,是否空,如果不空,就一个一个读出来,填到硬件fifo里
        if(i<len) {
            //把软件缓冲区的数据填到uart的硬件发送fifo里
            R8_UART1_THR = msg[i];
            i++;
        }else{
            break;
        }
    }
}