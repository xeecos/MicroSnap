#include "task.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "peripheral.h"
#include "app_usb.h"
#include "firmata.h"
#include "util.h"

void task_init()
{
    firmata_init();
    TMR0_TimerInit(60000);     // 1ms
    TMR0_ITCfg(ENABLE, TMR0_3_IT_CYC_END);
    PFIC_EnableIRQ(TMR0_IRQn); 
}
__INTERRUPT __HIGH_CODE void TMR0_IRQHandler(void)  {                 //timer0 interrupt handler
    if(TMR0_GetITFlag(TMR0_3_IT_CYC_END)){    //check flag
        TMR0_ClearITFlag(TMR0_3_IT_CYC_END);  //clear flag
        task_running();
    }
}
void task_add(int snaps, int steps, int dir, int speed, int waitingTime, int snapDuring, int endType)
{

}
void task_start()
{

}
void task_running()
{

}

void task_parse(uint8_t c)
{
    firmata_parse(c);
    // app_usb_notify(data, data_len);
    // USBSendData(data, data_len);
}

void task_command(uint8_t *cmd, int len)
{
    if(len>0)
    {
        switch(cmd[0])
        {
            case 0x05:
            {
                if(cmd[1]==0x1)
                {

                }
            }
            break;
            case 0x06:
            {
                if(cmd[1]==0x1)
                {
                    //forward
                    short_t s;
                    for(int i=0;i<2;i++)s.bytes[i] = cmd[2+i];
                    uint16_t steps = s.val;
                    float_t f;
                    for(int i=0;i<4;i++)f.bytes[i] = cmd[4+i];
                    float speed = f.val;
                    char *res = (char *)malloc(64);
                    memset(res, 0, 64);
                    sprintf(res, "steps:%d, speed:%.2f\n", steps, speed);
                    USBSendData((uint8_t*)res, strlen(res));
                    free(res);
                }
                else if(cmd[1]==0x2)
                {
                    //backward
                    short_t s;
                    for(int i=0;i<2;i++)s.bytes[i] = cmd[2+i];
                    uint16_t steps = s.val;
                    float_t f;
                    for(int i=0;i<4;i++)f.bytes[i] = cmd[4+i];
                    float speed = f.val;
                    char *res = (char *)malloc(64);
                    memset(res, 0, 64);
                    sprintf(res, "steps:%d, speed:%.2f\n", steps, speed);
                    USBSendData((uint8_t*)res, strlen(res));
                    free(res);
                }
                else if(cmd[1]==0x3)
                {
                    //setting
                    short_t s;
                    for(int i=0;i<2;i++)s.bytes[i] = cmd[2+i];
                    uint16_t microsteps = s.val;
                    for(int i=0;i<2;i++)s.bytes[i] = cmd[4+i];
                    uint16_t current = s.val;
                    char *res = (char *)malloc(64);
                    memset(res, 0, 64);
                    sprintf(res, "microsteps:%d, current:%d\n", microsteps, current);
                    USBSendData((uint8_t*)res, strlen(res));
                    free(res);
                }
            }
            break;
        }
    }
}