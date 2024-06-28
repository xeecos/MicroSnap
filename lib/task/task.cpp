#include "task.h"
#include "config.h"
#include "peripheral.h"
#include "firmata.h"

void task_init()
{
    firmata_init();
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