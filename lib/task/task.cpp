#include "task.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "peripheral.h"
#include "app_usb.h"
#include "firmata.h"
#include "util.h"
#include "stepper.h"
#include "pinmap.h"

volatile Task_t current;
volatile Task_t task;
void task_init()
{
    firmata_init();
}

void task_add(int snaps, int steps, int delayTime, int waitingTime, int duringTime, int endType)
{
    task.snaps = snaps;
    task.steps = steps;
    task.delay = delayTime;
    task.waiting = waitingTime;
    task.during = duringTime;
    task.end = endType;
    stepper_set_position(0);
}
void task_start()
{
}
void task_running()
{
    if (task.delay > 0)
    {
        task.delay--;
    }
    else
    {
        if (stepper_is_idle() && task.snaps > 0 && current.during == 0)
        {
            task.snaps--;
            current.during = task.during;
            current.waiting = task.waiting;
        }
        else if (stepper_is_idle() && current.waiting > 0)
        {
            current.waiting--;
            if (current.waiting == 0)
            {
                GPIOA_SetBits(PIN_SHOT);
            }
        }
        else if (stepper_is_idle() && current.during > 0)
        {
            GPIOA_ResetBits(PIN_SHOT);
            current.during--;
        }
        else if (stepper_is_idle() && task.snaps == 0 && task.end > 0)
        {
            task.end = 0;
            stepper_moveto(0);
        }
    }
    stepper_running();
}

void task_parse(uint8_t c)
{
    firmata_parse(c);
    // app_usb_notify(data, data_len);
    // USBSendData(&c, 1);
}

void task_command(uint8_t *cmd, uint8_t len)
{
    if (len > 0)
    {
        switch (cmd[0])
        {
            case 0x02:
            {
                if (cmd[1] == 0x0)
                {
                    GPIOA_SetBits(PIN_SHOT);
                }
                else if (cmd[1] == 0x1)
                {
                    GPIOA_ResetBits(PIN_SHOT);
                }
            }
            break;
            case 0x05:
            {
                if (cmd[1] == 0x1)
                {
                    short_t s;
                    for (int i = 0; i < 2; i++)
                        s.bytes[i] = cmd[2 + i];
                    uint16_t frames = s.val;
                    for (int i = 0; i < 2; i++)
                        s.bytes[i] = cmd[4 + i];
                    uint16_t steps = s.val;
                    for (int i = 0; i < 2; i++)
                        s.bytes[i] = cmd[6 + i];
                    uint16_t time1 = s.val;
                    for (int i = 0; i < 2; i++)
                        s.bytes[i] = cmd[8 + i];
                    uint16_t time2 = s.val;
                    for (int i = 0; i < 2; i++)
                        s.bytes[i] = cmd[10 + i];
                    uint16_t time3 = s.val;
                    for (int i = 0; i < 2; i++)
                        s.bytes[i] = cmd[12 + i];
                    uint16_t end = s.val;
                    task_add(frames, steps, time1, time2, time3, end);
                }
            }
            break;
            case 0x06:
            {
                if (cmd[1] == 0x1)
                {
                    // forward
                    short_t s;
                    for (int i = 0; i < 2; i++)
                        s.bytes[i] = cmd[2 + i];
                    uint16_t steps = s.val;
                    // float_t f;
                    // for (int i = 0; i < 4; i++)
                    //     f.bytes[i] = cmd[4 + i];
                    // float speed = f.val;
                    stepper_move(steps);
                    // char *res = (char *)malloc(64);
                    // memset(res, 0, 64);
                    // sprintf(res, "forward:%d\n", steps);
                    // USBSendData((uint8_t*)res, strlen(res));
                    // free(res);
                }
                else if (cmd[1] == 0x2)
                {
                    // backward
                    short_t s;
                    for (int i = 0; i < 2; i++)
                        s.bytes[i] = cmd[2 + i];
                    uint16_t steps = s.val;
                    // float_t f;
                    // for (int i = 0; i < 4; i++)
                    //     f.bytes[i] = cmd[4 + i];
                    // float speed = f.val;
                    stepper_move(-steps);
                    // char *res = (char *)malloc(64);
                    // memset(res, 0, 64);
                    // sprintf(res, "backward:%d\n", steps);
                    // USBSendData((uint8_t*)res, strlen(res));
                    // free(res);
                }
                else if (cmd[1] == 0x3)
                {
                    // moveto
                    short_t s;
                    for (int i = 0; i < 2; i++)
                        s.bytes[i] = cmd[2 + i];
                    uint16_t steps = s.val;
                    stepper_moveto(steps);
                }
                else if (cmd[1] == 0x4)
                {
                    // set position
                    short_t s;
                    for (int i = 0; i < 2; i++)
                        s.bytes[i] = cmd[2 + i];
                    uint16_t steps = s.val;
                    stepper_set_position(steps);
                }
                else if (cmd[1] == 0x5)
                {
                    // setting
                    short_t s;
                    for (int i = 0; i < 2; i++)
                        s.bytes[i] = cmd[2 + i];
                    uint16_t microsteps = s.val;
                    for (int i = 0; i < 2; i++)
                        s.bytes[i] = cmd[4 + i];
                    uint16_t current = s.val;
                    GPIOB_ResetBits(PIN_STEPPER_EN);
                    stepper_microsteps(microsteps);
                    stepper_rms_current(current);
                    GPIOB_SetBits(PIN_STEPPER_EN);
                    // char *res = (char *)malloc(64);
                    // memset(res, 0, 64);
                    // sprintf(res, "stepper:%d %d\n", microsteps, current);
                    // USBSendData((uint8_t*)res, strlen(res));
                    // free(res);
                }
            }
            break;
            case 0x7:
            {
                if (cmd[1] == 0x1)
                {
                    // version
                    
                }
                else if (cmd[1] == 0x2)
                {
                    // status
                }
            }
        }
    }
}