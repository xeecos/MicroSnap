#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
struct Task_t
{
    int snaps;
    int steps;
    int delay;
    int waiting;
    int during;
    int end;
};

void task_init();
void task_add(int snaps, int steps, int delayTime, int waitingTime, int duringTime, int endType);
void task_start();
void task_running();
void task_parse(uint8_t c);
void task_command(uint8_t *cmd, uint8_t len);
#ifdef __cplusplus
}
#endif