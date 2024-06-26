#pragma once
#include <stdint.h>
void uart_init();
void uart_send(uint8_t *msg, int len);