#include "task.h"
#include "config.h"
#include "peripheral.h"
#include "firmata.h"

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

}