#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
void firmata_init();
void firmata_start();
void firmata_end();
void firmata_write(uint8_t b);
void firmata_parse(uint8_t b);
void firmate_data(uint8_t*data,int len);
#ifdef __cplusplus
}
#endif