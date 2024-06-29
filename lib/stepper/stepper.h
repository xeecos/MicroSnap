#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
struct IHOLD_IRUN_t
{
    constexpr static uint8_t address = 0x10;
    union
    {
        uint32_t sr : 20;
        struct
        {
            uint8_t ihold : 5, : 3,
                irun : 5, : 3,
                iholddelay : 4;
        };
    };
};

struct GSTAT_t
{
    constexpr static uint8_t address = 0x01;
    union
    {
        uint8_t sr : 3;
        struct
        {
            bool reset : 1,
                drv_err : 1,
                uv_cp : 1;
        };
    };
};

struct TPOWERDOWN_t
{
    constexpr static uint8_t address = 0x11;
    uint8_t sr : 8;
};

struct TPWMTHRS_t
{
    constexpr static uint8_t address = 0x13;
    uint32_t sr : 20;
};

struct THIGH_t
{
    constexpr static uint8_t address = 0x15;
    uint32_t sr : 20;
};

struct XDIRECT_t
{
    constexpr static uint8_t address = 0x2D;
    union
    {
        uint32_t sr : 25;
        struct
        {
            int16_t coil_A : 9;
            int8_t : 7;
            int16_t coil_B : 9;
        };
    };
};

struct VDCMIN_t
{
    constexpr static uint8_t address = 0x33;
    uint32_t sr : 23;
};

struct CHOPCONF_t
{
    constexpr static uint8_t address = 0x6C;
    union
    {
        uint32_t sr;
        struct
        {
            uint8_t toff : 4,
                hstrt : 3,
                hend : 4, : 4,
                tbl : 2;
            bool vsense : 1;
            uint8_t : 6,
                mres : 4;
            bool intpol : 1,
                dedge : 1,
                diss2g : 1,
                diss2vs : 1;
        };
    };
};

struct COOLCONF_t
{
    constexpr static uint8_t address = 0x6D;
    union
    {
        uint32_t sr : 25;
        struct
        {
            uint8_t semin : 4, : 1,
                seup : 2, : 1,
                semax : 4, : 1,
                sedn : 2;
            bool seimin : 1;
            int8_t sgt : 7, : 1;
            bool sfilt : 1;
        };
    };
};

struct DCCTRL_t
{
    constexpr static uint8_t address = 0x6E;
    union
    {
        uint32_t sr : 24;
        struct
        {
            uint16_t dc_time : 10, : 6;
            uint8_t dc_sg : 8;
        };
    };
};

struct SLAVECONF_t
{
    constexpr static uint8_t address = 0x03;
    union
    {
        uint16_t sr : 12;
        struct
        {
            uint8_t slaveaddr : 8;
            uint8_t senddelay : 4;
        };
    };
};

struct PWM_AUTO_t
{
    constexpr static uint8_t address = 0x72;
    union
    {
        uint32_t sr : 24;
        struct
        {
            uint8_t pwm_ofs_auto : 8, : 8,
                pwm_grad_auto : 8;
        };
    };
};
struct GCONF_t
{
    constexpr static uint8_t address = 0x00;
    union
    {
        uint16_t sr : 10;
        struct
        {
            bool i_scale_analog : 1,
                internal_rsense : 1,
                en_spreadcycle : 1,
                shaft : 1,
                index_otpw : 1,
                index_step : 1,
                pdn_disable : 1,
                mstep_reg_select : 1,
                multistep_filt : 1,
                test_mode : 1;
        };
    };
};

struct IOIN_t
{
    constexpr static uint8_t address = 0x06;
    union
    {
        uint32_t sr;
        struct
        {
            bool enn : 1, : 1,
                ms1 : 1,
                ms2 : 1,
                diag : 1, : 1,
                pdn_uart : 1,
                step : 1,
                sel_a : 1,
                dir : 1;
            uint16_t : 14;
            uint8_t version : 8;
        };
    };
};

struct FACTORY_CONF_t
{
    constexpr static uint8_t address = 0x07;
    union
    {
        uint16_t sr;
        struct
        {
            uint8_t fclktrim : 5, : 3,
                ottrim : 2;
        };
    };
};

struct VACTUAL_t
{
    constexpr static uint8_t address = 0x22;
    uint32_t sr;
};

struct MSCURACT_t
{
    constexpr static uint8_t address = 0x6B;
    union
    {
        uint32_t sr : 25;
        struct
        {
            int16_t cur_a : 9, : 7,
                cur_b : 9;
        };
    };
};

struct PWMCONF_t
{
    constexpr static uint8_t address = 0x70;
    union
    {
        uint32_t sr;
        struct
        {
            uint8_t pwm_ofs : 8,
                pwm_grad : 8,
                pwm_freq : 2;
            bool pwm_autoscale : 1,
                pwm_autograd : 1;
            uint8_t freewheel : 2, : 2,
                pwm_reg : 4,
                pwm_lim : 4;
        };
    };
};

struct DRV_STATUS_t
{
    constexpr static uint8_t address = 0x6F;
    union
    {
        uint32_t sr;
        struct
        {
            bool otpw : 1,
                ot : 1,
                s2ga : 1,
                s2gb : 1,
                s2vsa : 1,
                s2vsb : 1,
                ola : 1,
                olb : 1,
                t120 : 1,
                t143 : 1,
                t150 : 1,
                t157 : 1;
            uint8_t : 4,
                cs_actual : 5, : 3, : 6;
            bool stealth : 1,
                stst : 1;
        };
    };
};

struct PWM_SCALE_t
{
    constexpr static uint8_t address = 0x71;
    union
    {
        uint32_t sr;
        struct
        {
            uint8_t pwm_scale_sum : 8, : 8;
            int16_t pwm_scale_auto : 9;
        };
    };
};

void stepper_init();
void stepper_rms_current(uint16_t mA);
void stepper_microsteps(uint16_t ms);
void stepper_push();
void stepper_move(int steps);
void stepper_moveto(int pos);
void stepper_set_position(int pos);
void stepper_running();
bool stepper_is_idle();
#ifdef __cplusplus
}
#endif