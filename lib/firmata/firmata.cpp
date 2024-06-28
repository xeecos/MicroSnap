#include "firmata.h"
#include "task.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
uint8_t *mSendBuf;
uint8_t *mRecvBuf;
uint16_t mSendIdx;
uint16_t mRecvIdx;
void firmata_init()
{
    mSendBuf = (uint8_t *)malloc(256);
    mRecvBuf = (uint8_t *)malloc(256);
    mSendIdx = 0;
    mRecvIdx = 0;
}
void firmata_start()
{
    memset(mSendBuf, 0, 256);
    mSendBuf[0] = 0xf0;
    mSendIdx++;
}
void firmata_end()
{
    mSendIdx++;
    mSendBuf[mSendIdx] = 0xf7;
    mSendIdx++;
}
void firmata_write(uint8_t b)
{
    int bits = mSendIdx % 7;
    mSendBuf[mSendIdx] += b >> bits;
    mSendIdx++;
    mSendBuf[mSendIdx] = (b << (7 - bits)) & 0x7f;
    if (bits == 6)
    {
        mSendIdx++;
    }
}
void firmata_parse(uint8_t b)
{
    if (b == 0xf0)
    {
        mRecvIdx = 0;
    }
    else if (b == 0xf7)
    {
        uint8_t *cmd = (uint8_t *)malloc(256);
        int n = 0;
        for (int i = 1; i < mRecvIdx; i++)
        {
            int bits = i % 7;
            if (bits > 0)
            {
                uint8_t c = (((mRecvBuf[i - 1]) << bits) & 0xff) + ((mRecvBuf[i]) >> (7 - bits));
                // USBSerial.printf("%d\n",c);
                cmd[n++] = c;
            }
        }
        task_command(cmd, mRecvIdx - 2);
        free(cmd);
    }
    else
    {
        mRecvBuf[mRecvIdx] = b & 0x7f;
        mRecvIdx++;
    }
}
void firmate_data(uint8_t *data, int len)
{
    firmata_start();
    for (int i = 0; i < len; i++)
    {
        firmata_write(data[i]);
    }
    firmata_end();
}