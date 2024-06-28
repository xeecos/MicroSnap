const Format = require("./format");

class Firmata
{
    constructor()
    {
        this._cmd = [];
        this._data = [];
    }
    start ()
    {
        this._data = [0xf0];
        for (let i = 0; i < 1024; i++)
        {
            this._data.push(0);
        }
        this.mSendIdx = 1;
    }
    addByte (b)
    {
        let bits = this.mSendIdx % 7;
        this._data[this.mSendIdx] += b >> bits;
        this.mSendIdx++;
        this._data[this.mSendIdx] = (b << (7 - bits)) & 0x7f;
        if (bits == 6)
        {
            this.mSendIdx++;
        }
    }
    end ()
    {
        this.mSendIdx++;
        this._data[this.mSendIdx] = 0xf7;
        this.mSendIdx++;
    }
    snapTrigger (time, hdr)
    {
        this._cmd = [2, 0];
        this._cmd = this._cmd.concat(Format.short2bytes(time));
        return this.data;
    }
    snapRelease ()
    {
        this._cmd = [2, 1];
        return this.data;
    }
    startTimer (delayTime, exposureTime, duringTime, frames)
    {
        this._cmd = [4];
        this._cmd = this._cmd.concat(Format.short2bytes(delayTime));
        this._cmd = this._cmd.concat(Format.short2bytes(exposureTime));
        this._cmd = this._cmd.concat(Format.short2bytes(duringTime));
        this._cmd = this._cmd.concat(Format.short2bytes(frames));

        return this.data;
    }
    startMultiTimers (tasks)
    {
        this._cmd = [4];
        for (let i = 0; i < tasks.length; i++)
        {
            this._cmd = this._cmd.concat(Format.short2bytes(tasks[i].delayTime));
            this._cmd = this._cmd.concat(Format.short2bytes(tasks[i].exposureTime));
            this._cmd = this._cmd.concat(Format.short2bytes(tasks[i].duringTime));
            this._cmd = this._cmd.concat(Format.short2bytes(tasks[i].frames));
        }
        return this.data;
    }
    stopTimer ()
    {
        this._cmd = [3, 0];
        return this.data;
    }
    motion (frames, steps, time1, time2, time3, end)
    {
        this._cmd = [5, 1];
        this._cmd = this._cmd.concat(Format.short2bytes(frames)); // 拍摄帧数
        this._cmd = this._cmd.concat(Format.short2bytes(steps)); // 步进电机步数
        this._cmd = this._cmd.concat(Format.short2bytes(time1)); //等待时间后开始，毫秒，最大60秒
        this._cmd = this._cmd.concat(Format.short2bytes(time2)); //运动停止等待时间，毫秒，最大60秒
        this._cmd = this._cmd.concat(Format.short2bytes(time3)); //拍照结束等待时间，毫秒，最大60秒
        this._cmd = this._cmd.concat(Format.short2bytes(end)); //结束后动作

        return this.data;
    }
    forward (steps, speed)
    {
        this._cmd = [6, 1];
        this._cmd = this._cmd.concat(Format.short2bytes(steps)); // 步进电机步数
        this._cmd = this._cmd.concat(Format.float2bytes(speed)); // 步进电机速度，mm/s
        return this.data;
    }
    backward (steps, speed)
    {
        this._cmd = [6, 2];
        this._cmd = this._cmd.concat(Format.short2bytes(steps)); // 步进电机步数
        this._cmd = this._cmd.concat(Format.float2bytes(speed)); // 步进电机速度，mm/s
        return this.data;
    }
    stepper (microsteps, current)
    {
        this._cmd = [6, 3];
        this._cmd = this._cmd.concat(Format.short2bytes(microsteps)); // 步进电机细分
        this._cmd = this._cmd.concat(Format.short2bytes(current));  // 步进电机电流，mA
        return this.data;
    }
    get data ()
    {
        this.start();
        for (let i = 0; i < this._cmd.length; i++)
        {
            this.addByte(this._cmd[i]);
        }
        this.end();
        let data = [];
        for (let i = 0; i < this.mSendIdx; i++)
        {
            data.push(this._data[i]);
        }
        return data;
    }
    parse (b)
    {
        if (b == 0xf0)
        {
            this.mRecvIdx = 0;
            this.mRecvBuf = [];
        }
        else if (b == 0xf7)
        {
            for (let i = 1; i < this.mRecvIdx; i++)
            {
                let bits = i % 7;
                if (bits > 0)
                {
                    let c = (((this.mRecvBuf[i - 1]) << bits) & 0xff) + ((this.mRecvBuf[i]) >> (7 - bits));
                    console.log(c);
                }
            }
        }
        else
        {
            this.mRecvBuf[this.mRecvIdx] = b & 0x7f;
            this.mRecvIdx++;
        }
    }
}
module.exports = new Firmata();