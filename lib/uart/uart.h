#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
void uart_init();
void uart_send(uint8_t *msg, int len);
uint8_t uart_available();
uint8_t uart_read();
#ifdef __cplusplus
}
#endif