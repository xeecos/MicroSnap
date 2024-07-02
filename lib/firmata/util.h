#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

union float_t
{
    float val;
    uint8_t bytes[4];
};
union short_t
{
    int16_t val;
    uint8_t bytes[2];
};
union long_t
{
    int32_t val;
    uint8_t bytes[4];
};
#ifdef __cplusplus
}
#endif