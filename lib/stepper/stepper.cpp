#include "stepper.h"
#include "uart.h"
#include "pinmap.h"
#include "config.h"


static float Rsense = 0.1;
static constexpr uint8_t TMC_READ = 0x00, TMC_WRITE = 0x80;
static constexpr uint8_t TMC2208_SYNC = 0x05, TMC2208_SLAVE_ADDR = 0x00;
static constexpr uint8_t abort_window = 5;
static constexpr uint8_t max_retries = 2;
static float holdMultiplier = 0.5;
static bool CRCerror = false;
static uint16_t bytesWritten = 0;
static constexpr uint8_t replyDelay = 2;

volatile GCONF_t GCONF_register;
volatile PWMCONF_t PWMCONF_register;
volatile TPOWERDOWN_t TPOWERDOWN_register;
volatile IHOLD_IRUN_t IHOLD_IRUN_register;
volatile CHOPCONF_t CHOPCONF_register;
volatile SLAVECONF_t SLAVECONF_register;
volatile TPWMTHRS_t TPWMTHRS_register;
volatile VACTUAL_t VACTUAL_register;
volatile long position = 0;
volatile long targetPoision = 0;
volatile uint16_t microsteps = 32;
uint8_t serial_available();
uint8_t serial_read();
uint8_t serial_write(const uint8_t data);

void stepper_write(uint8_t addr, uint32_t regVal);
uint32_t stepper_read(uint8_t addr);
uint8_t stepper_calcCRC(uint8_t datagram[], uint8_t len);
uint64_t stepper_sendDatagram(uint8_t datagram[], const uint8_t len, uint16_t timeout);
void stepper_init()
{
    GPIOA_ModeCfg(PIN_STEPPER_DIR | PIN_STEPPER_STEP | PIN_FOCUS | PIN_SHOT, GPIO_ModeOut_PP_20mA);
    GPIOB_ModeCfg(PIN_LED | PIN_STEPPER_EN, GPIO_ModeOut_PP_20mA);
    GPIOB_SetBits(PIN_STEPPER_EN);
    GPIOB_ResetBits(PIN_LED);
    GPIOA_ResetBits(PIN_FOCUS);
    GPIOA_ResetBits(PIN_SHOT);

    uart_init();
    TPOWERDOWN_register.sr = 20;
    CHOPCONF_register.sr = 0x10000053;
    PWMCONF_register.sr = 0xC10D0024;
    GCONF_register.i_scale_analog = 1;
    GCONF_register.en_spreadcycle = 0;  // OTP
    GCONF_register.multistep_filt = 1;  // OTP
    GCONF_register.pdn_disable = 1;
    GCONF_register.mstep_reg_select = 1;
    IHOLD_IRUN_register.iholddelay = 1; // OTP
    GCONF_register.internal_rsense = 0; // OTP
    CHOPCONF_register.dedge = 1;
    CHOPCONF_register.toff = 2;
    stepper_microsteps(32);
    stepper_rms_current(1000);
    stepper_push();
}
void stepper_rms_current(uint16_t mA)
{
    uint8_t CS = 32.0 * 1.41421 * mA / 1000.0 * (Rsense + 0.02) / 0.325 - 1;
    // If Current Scale is too low, turn on high sensitivity R_sense and calculate again
    if (CS < 16)
    {
        CHOPCONF_register.vsense = true;
        CS = 32.0 * 1.41421 * mA / 1000.0 * (Rsense + 0.02) / 0.180 - 1;
    }
    else
    { // If CS >= 16, turn off high_sense_r
        CHOPCONF_register.vsense = false;
    }

    if (CS > 31)
    {
        CS = 31;
    }
    IHOLD_IRUN_register.irun = CS;
    IHOLD_IRUN_register.ihold = CS * holdMultiplier;
    // val_mA = mA;
}

uint8_t stepper_microsteps(uint16_t ms)
{
    microsteps = ms;
    switch (ms)
    {
    case 256:
        CHOPCONF_register.mres = 0;
        break;
    case 128:
        CHOPCONF_register.mres = 1;
        break;
    case 64:
        CHOPCONF_register.mres = 2;
        break;
    case 32:
        CHOPCONF_register.mres = 3;
        break;
    case 16:
        CHOPCONF_register.mres = 4;
        break;
    case 8:
        CHOPCONF_register.mres = 5;
        break;
    case 4:
        CHOPCONF_register.mres = 6;
        break;
    case 2:
        CHOPCONF_register.mres = 7;
        break;
    case 0:
        CHOPCONF_register.mres = 8;
        break;
    default:
        break;
    }
    return CHOPCONF_register.mres;
}
uint16_t stepper_get_microsteps()
{
    return microsteps;
}

int32_t stepper_get_position()
{
    return position;
}
uint32_t stepper_push()
{
    stepper_write(GCONF_register.address, GCONF_register.sr);
    stepper_write(IHOLD_IRUN_register.address, IHOLD_IRUN_register.sr);
    stepper_write(SLAVECONF_register.address, SLAVECONF_register.sr);
    stepper_write(TPOWERDOWN_register.address, TPOWERDOWN_register.sr);
    stepper_write(TPWMTHRS_register.address, TPWMTHRS_register.sr);
    stepper_write(VACTUAL_register.address, VACTUAL_register.sr);
    stepper_write(CHOPCONF_register.address, CHOPCONF_register.sr);
    stepper_write(PWMCONF_register.address, PWMCONF_register.sr);
    return CHOPCONF_register.sr;
}

uint8_t stepper_calcCRC(uint8_t datagram[], uint8_t len)
{
    uint8_t crc = 0;
    for (uint8_t i = 0; i < len; i++)
    {
        uint8_t currentByte = datagram[i];
        for (uint8_t j = 0; j < 8; j++)
        {
            if ((crc >> 7) ^ (currentByte & 0x01))
            {
                crc = (crc << 1) ^ 0x07;
            }
            else
            {
                crc = (crc << 1);
            }
            crc &= 0xff;
            currentByte = currentByte >> 1;
        }
    }
    return crc;
}

uint8_t serial_available()
{
    return uart_available();
}
uint8_t serial_read()
{
    return uart_read();
}
uint8_t serial_write(const uint8_t data)
{
    int out = 0;
    uint8_t c = data;
    uart_send(&c, 1);
    return out;
}

void stepper_write(uint8_t addr, uint32_t regVal)
{
    uint8_t len = 7;
    addr |= TMC_WRITE;
    uint8_t datagram[] = {TMC2208_SYNC, TMC2208_SLAVE_ADDR, addr, (uint8_t)(regVal >> 24), (uint8_t)(regVal >> 16), (uint8_t)(regVal >> 8), (uint8_t)(regVal >> 0), 0x00};

    datagram[len] = stepper_calcCRC(datagram, len);

    for (uint8_t i = 0; i <= len; i++)
    {
        bytesWritten += serial_write(datagram[i]);
    }

    DelayMs(2);
}
uint32_t stepper_read(uint8_t addr)
{
    constexpr uint8_t len = 3;
    addr |= TMC_READ;
    uint8_t datagram[] = {TMC2208_SYNC, TMC2208_SLAVE_ADDR, addr, 0x00};
    datagram[len] = stepper_calcCRC(datagram, len);
    uint64_t out = 0x00000000UL;

    for (uint8_t i = 0; i < max_retries; i++)
    {
        out = stepper_sendDatagram(datagram, len, abort_window);

        DelayMs(2);

        CRCerror = false;
        uint8_t out_datagram[] = {
            static_cast<uint8_t>(out >> 56),
            static_cast<uint8_t>(out >> 48),
            static_cast<uint8_t>(out >> 40),
            static_cast<uint8_t>(out >> 32),
            static_cast<uint8_t>(out >> 24),
            static_cast<uint8_t>(out >> 16),
            static_cast<uint8_t>(out >> 8),
            static_cast<uint8_t>(out >> 0)};
        uint8_t crc = stepper_calcCRC(out_datagram, 7);
        if ((crc != static_cast<uint8_t>(out)) || crc == 0)
        {
            CRCerror = true;
            out = 0;
        }
        else
        {
            break;
        }
    }

    return out >> 8;
}

uint64_t stepper_sendDatagram(uint8_t datagram[], const uint8_t len, uint16_t timeout)
{
    while (serial_available() > 0)
        serial_read(); // Flush

    for (int i = 0; i <= len; i++)
        serial_write(datagram[i]);

    DelayMs(replyDelay);

    // scan for the rx frame and read it
    uint32_t ms = SYS_GetSysTickCnt();
    uint32_t sync_target = (static_cast<uint32_t>(datagram[0]) << 16) | 0xFF00 | datagram[2];
    uint32_t sync = 0;

    do
    {
        uint32_t ms2 = SYS_GetSysTickCnt();
        if (ms2 != ms)
        {
            // 1ms tick
            ms = ms2;
            timeout--;
        }
        if (!timeout)
            return 0;

        int16_t res = serial_read();
        if (res < 0)
            continue;

        sync <<= 8;
        sync |= res & 0xFF;
        sync &= 0xFFFFFF;

    } while (sync != sync_target);

    uint64_t out = sync;
    ms = SYS_GetSysTickCnt();
    timeout = abort_window;

    for (uint8_t i = 0; i < 5;)
    {
        uint32_t ms2 = SYS_GetSysTickCnt();
        if (ms2 != ms)
        {
            // 1ms tick
            ms = ms2;
            timeout--;
        }
        if (!timeout)
            return 0;

        int16_t res = serial_read();
        if (res < 0)
            continue;

        out <<= 8;
        out |= res & 0xFF;

        i++;
    }

    while (serial_available() > 0)
        serial_read(); // Flush

    return out;
}
void stepper_running()
{
    long dPos = targetPoision - position;
    if (dPos>0)
    {
        GPIOB_ResetBits(PIN_STEPPER_EN);
        GPIOA_SetBits(PIN_STEPPER_DIR);
        GPIOA_InverseBits(PIN_STEPPER_STEP);
        position++;
    }
    else if(dPos<0)
    {
        GPIOB_ResetBits(PIN_STEPPER_EN);
        GPIOA_ResetBits(PIN_STEPPER_DIR);
        GPIOA_InverseBits(PIN_STEPPER_STEP);
        position--;
    }
    else
    {
        GPIOB_SetBits(PIN_STEPPER_EN);
    }
}
void stepper_move(int steps)
{
    targetPoision += steps;
}
void stepper_moveto(int pos)
{
    targetPoision = pos;
}
void stepper_set_position(int pos)
{
    position = pos;
    targetPoision = pos;
}
bool stepper_is_idle()
{
    return targetPoision == position;
}