#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
void task_init();
void task_add(int snaps, int steps, int dir, int speed, int waitingTime, int snapDuring, int endType);
void task_start();
void task_running();
void task_parse(uint8_t c);
void task_command(uint8_t *cmd, uint8_t len);
#ifdef __cplusplus
}
#endif