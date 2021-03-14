/******************** (C) COPYRIGHT 2016 ********************
 * 文件名  ：mpu6000.c
 * 描述    ：mpu6000 应用函数库
 *          
 * 实验平台：
 * 硬件连接：

***************************************************************/
#include <string.h>
#include "mpu6000_driver.h"
#include "spi1.h"


const struct gyro_reg_s reg = {
    .who_am_i       = 0x75,
    .rate_div       = 0x19,
    .lpf            = 0x1A,
    .prod_id        = 0x0C,
    .user_ctrl      = 0x6A,
    .fifo_en        = 0x23,
    .gyro_cfg       = 0x1B,
    .accel_cfg      = 0x1C,
    .motion_thr     = 0x1F,
    .motion_dur     = 0x20,
    .fifo_count_h   = 0x72,
    .fifo_r_w       = 0x74,
    .raw_gyro       = 0x43,
    .raw_accel      = 0x3B,
    .temp           = 0x41,
    .int_enable     = 0x38,
    .dmp_int_status = 0x39,
    .int_status     = 0x3A,
    .pwr_mgmt_1     = 0x6B,
    .pwr_mgmt_2     = 0x6C,
    .int_pin_cfg    = 0x37,
    .mem_r_w        = 0x6F,
    .accel_offs     = 0x06,
    .i2c_mst        = 0x24,
    .bank_sel       = 0x6D,
    .mem_start_addr = 0x6E,
    .prgm_start_h   = 0x70
};

const struct hw_s hw = {
    .addr           = 0x68,
    .max_fifo       = 1024,
    .num_reg        = 118,
    .temp_sens      = 340,
    .temp_offset    = -521,
    .bank_size      = 256
};

const struct test_s test = {
    .gyro_sens      = 32768/250,
    .accel_sens     = 32768/16,
    .reg_rate_div   = 0,    /* 1kHz. */
    .reg_lpf        = 1,    /* 188Hz. */
    .reg_gyro_fsr   = 0,    /* 250dps. */
    .reg_accel_fsr  = 0x18, /* 16g. */
    .wait_ms        = 50,
    .packet_thresh  = 5,    /* 5% */
    .min_dps        = 10.f,
    .max_dps        = 105.f,
    .max_gyro_var   = 0.14f,
    .min_g          = 0.3f,
    .max_g          = 0.95f,
    .max_accel_var  = 0.14f
};

static struct gyro_state_s st = {
    .reg = &reg,
    .hw = &hw,
    .test = &test
};



//-----------------------------------
void delay_mpu6000_ms(uint16_t nms)
{
	uint16_t i=0;
	uint16_t j=0;
	for(i=0; i<nms; i++)
		for(j=0; j<7000; j++)
		{
			;
		}
}
#define delay_ms(a)		delay_mpu6000_ms(a)
//------------------------------------
u8 i2c_write(u8 devicd, u8 addr, u8 length, u8 *data)
{
	SPI1_6000_WriteReg_NByte(addr, data, length);
	return 0;
}
//------------------------------------
u8 i2c_read(u8 devicd, u8 addr, u8 length, u8 *data)
{
	SPI1_6000_ReadReg_NByte(addr, data, length);
	return 0;
}
/**
 *  @brief      Get the gyro full-scale range.
 *  @param[out] fsr Current full-scale range.
 *  @return     0 if successful.
 */
int mpu_get_gyro_fsr(unsigned short *fsr)
{
    switch (st.chip_cfg.gyro_fsr) {
    case INV_FSR_250DPS:
        fsr[0] = 250;
        break;
    case INV_FSR_500DPS:
        fsr[0] = 500;
        break;
    case INV_FSR_1000DPS:
        fsr[0] = 1000;
        break;
    case INV_FSR_2000DPS:
        fsr[0] = 2000;
        break;
    default:
        fsr[0] = 0;
        break;
    }
    return 0;
}

/**
 *  @brief      Set the gyro full-scale range.
 *  @param[in]  fsr Desired full-scale range.
 *  @return     0 if successful.
 */
int mpu_set_gyro_fsr(unsigned short fsr)
{
    unsigned char data;
    if (!(st.chip_cfg.sensors))
        return -1;
		
    switch (fsr) 
		{
    case 250:
        data = INV_FSR_250DPS << 3;
        break;
    case 500:
        data = INV_FSR_500DPS << 3;
        break;
    case 1000:
        data = INV_FSR_1000DPS << 3;
        break;
    case 2000:
        data = INV_FSR_2000DPS << 3;
        break;
    default:
        return -1;
    }
    if (st.chip_cfg.gyro_fsr == (data >> 3))
        return 0;
    if(i2c_write(st.hw->addr, st.reg->gyro_cfg, 1, &data))
				return -1;
			
    st.chip_cfg.gyro_fsr = data >> 3;
    return 0;
}
/**
 *  @brief      Get the accel full-scale range.
 *  @param[out] fsr Current full-scale range.
 *  @return     0 if successful.
 */
int mpu_get_accel_fsr(unsigned char *fsr)
{
    switch (st.chip_cfg.accel_fsr) {
    case INV_FSR_2G:
        fsr[0] = 2;
        break;
    case INV_FSR_4G:
        fsr[0] = 4;
        break;
    case INV_FSR_8G:
        fsr[0] = 8;
        break;
    case INV_FSR_16G:
        fsr[0] = 16;
        break;
    default:
        return -1;
    }
    if (st.chip_cfg.accel_half)
        fsr[0] <<= 1;
    return 0;
}

/**
 *  @brief      Set the accel full-scale range.
 *  @param[in]  fsr Desired full-scale range.
 *  @return     0 if successful.
 */
int mpu_set_accel_fsr(unsigned char fsr)
{
    unsigned char data;

    if (!(st.chip_cfg.sensors))
        return -1;

    switch (fsr) {
    case 2:
        data = INV_FSR_2G << 3;
        break;
    case 4:
        data = INV_FSR_4G << 3;
        break;
    case 8:
        data = INV_FSR_8G << 3;
        break;
    case 16:
        data = INV_FSR_16G << 3;
        break;
    default:
        return -1;
    }

    if (st.chip_cfg.accel_fsr == (data >> 3))
        return 0;
    if (i2c_write(st.hw->addr, st.reg->accel_cfg, 1, &data))
        return -1;
    st.chip_cfg.accel_fsr = data >> 3;
    return 0;
}
/**
 *  @brief      Get the current DLPF setting.
 *  @param[out] lpf Current LPF setting.
 *  0 if successful.
 */
int mpu_get_lpf(unsigned short *lpf)
{
    switch (st.chip_cfg.lpf) {
    case INV_FILTER_188HZ:
        lpf[0] = 188;
        break;
    case INV_FILTER_98HZ:
        lpf[0] = 98;
        break;
    case INV_FILTER_42HZ:
        lpf[0] = 42;
        break;
    case INV_FILTER_20HZ:
        lpf[0] = 20;
        break;
    case INV_FILTER_10HZ:
        lpf[0] = 10;
        break;
    case INV_FILTER_5HZ:
        lpf[0] = 5;
        break;
    case INV_FILTER_256HZ_NOLPF2:
    case INV_FILTER_2100HZ_NOLPF:
    default:
        lpf[0] = 0;
        break;
    }
    return 0;
}

/**
 *  @brief      Set digital low pass filter.
 *  The following LPF settings are supported: 188, 98, 42, 20, 10, 5.
 *  @param[in]  lpf Desired LPF setting.
 *  @return     0 if successful.
 */
int mpu_set_lpf(unsigned short lpf)
{
    unsigned char data;

    if (!(st.chip_cfg.sensors))
        return -1;

    if (lpf >= 188)
        data = INV_FILTER_188HZ;
    else if (lpf >= 98)
        data = INV_FILTER_98HZ;
    else if (lpf >= 42)
        data = INV_FILTER_42HZ;
    else if (lpf >= 20)
        data = INV_FILTER_20HZ;
    else if (lpf >= 10)
        data = INV_FILTER_10HZ;
    else
        data = INV_FILTER_5HZ;

    if (st.chip_cfg.lpf == data)
        return 0;
    if (i2c_write(st.hw->addr, st.reg->lpf, 1, &data))
        return -1;
    st.chip_cfg.lpf = data;
    return 0;
}
/**
 *  @brief      Enable latched interrupts.
 *  Any MPU register will clear the interrupt.
 *  @param[in]  enable  1 to enable, 0 to disable.
 *  @return     0 if successful.
 */
int mpu_set_int_latched(unsigned char enable)
{
    unsigned char tmp;
    if (st.chip_cfg.latched_int == enable)
        return 0;

    if (enable)
        tmp = BIT_LATCH_EN | BIT_ANY_RD_CLR;
    else
        tmp = 0;
    if (st.chip_cfg.bypass_mode)
        tmp |= BIT_BYPASS_EN;
    if (st.chip_cfg.active_low_int)
        tmp |= BIT_ACTL;
    if (i2c_write(st.hw->addr, st.reg->int_pin_cfg, 1, &tmp))
        return -1;
    st.chip_cfg.latched_int = enable;
    return 0;
}
/**
 *  @brief      Enable/disable data ready interrupt.
 *  If the DMP is on, the DMP interrupt is enabled. Otherwise, the data ready
 *  interrupt is used.
 *  @param[in]  enable      1 to enable interrupt.
 *  @return     0 if successful.
 */
static int set_int_enable(unsigned char enable)
{
    unsigned char tmp;

    if (st.chip_cfg.dmp_on) {
        if (enable)
            tmp = BIT_DMP_INT_EN;
        else
            tmp = 0x00;
        if (i2c_write(st.hw->addr, st.reg->int_enable, 1, &tmp))
            return -1;
        st.chip_cfg.int_enable = tmp;
    } else {
        if (!st.chip_cfg.sensors)
            return -1;
        if (enable && st.chip_cfg.int_enable)
            return 0;
        if (enable)
            tmp = BIT_DATA_RDY_EN;
        else
            tmp = 0x00;
        if (i2c_write(st.hw->addr, st.reg->int_enable, 1, &tmp))
            return -1;
        st.chip_cfg.int_enable = tmp;
    }
    return 0;
}
/**
 *  @brief  Reset FIFO read/write pointers.
 *  @return 0 if successful.
 */
int mpu_reset_fifo(void)
{
    unsigned char data;

    if (!(st.chip_cfg.sensors))
        return -1;

    data = 0;
    if (i2c_write(st.hw->addr, st.reg->int_enable, 1, &data))
        return -1;
    if (i2c_write(st.hw->addr, st.reg->fifo_en, 1, &data))
        return -1;
    if (i2c_write(st.hw->addr, st.reg->user_ctrl, 1, &data))
        return -1;

    if (st.chip_cfg.dmp_on) {
        data = BIT_FIFO_RST | BIT_DMP_RST;
        if (i2c_write(st.hw->addr, st.reg->user_ctrl, 1, &data))
            return -1;
        delay_ms(50);
        data = BIT_DMP_EN | BIT_FIFO_EN;
        if (st.chip_cfg.sensors & INV_XYZ_COMPASS)
            data |= BIT_AUX_IF_EN;
        if (i2c_write(st.hw->addr, st.reg->user_ctrl, 1, &data))
            return -1;
        if (st.chip_cfg.int_enable)
            data = BIT_DMP_INT_EN;
        else
            data = 0;
        if (i2c_write(st.hw->addr, st.reg->int_enable, 1, &data))
            return -1;
        data = 0;
        if (i2c_write(st.hw->addr, st.reg->fifo_en, 1, &data))
            return -1;
    } else {
        data = BIT_FIFO_RST;
        if (i2c_write(st.hw->addr, st.reg->user_ctrl, 1, &data))
            return -1;
        if (st.chip_cfg.bypass_mode || !(st.chip_cfg.sensors & INV_XYZ_COMPASS))
            data = BIT_FIFO_EN;
        else
            data = BIT_FIFO_EN | BIT_AUX_IF_EN;
        if (i2c_write(st.hw->addr, st.reg->user_ctrl, 1, &data))
            return -1;
        delay_ms(50);
        if (st.chip_cfg.int_enable)
            data = BIT_DATA_RDY_EN;
        else
            data = 0;
        if (i2c_write(st.hw->addr, st.reg->int_enable, 1, &data))
            return -1;
        if (i2c_write(st.hw->addr, st.reg->fifo_en, 1, &st.chip_cfg.fifo_enable))
            return -1;
    }
    return 0;
}
/**
 *  @brief      Select which sensors are pushed to FIFO.
 *  @e sensors can contain a combination of the following flags:
 *  \n INV_X_GYRO, INV_Y_GYRO, INV_Z_GYRO
 *  \n INV_XYZ_GYRO
 *  \n INV_XYZ_ACCEL
 *  @param[in]  sensors Mask of sensors to push to FIFO.
 *  @return     0 if successful.
 */
int mpu_configure_fifo(unsigned char sensors)
{
    unsigned char prev;
    int result = 0;

    /* Compass data isn't going into the FIFO. Stop trying. */
    sensors &= ~INV_XYZ_COMPASS;

    if (st.chip_cfg.dmp_on)
        return 0;
    else {
        if (!(st.chip_cfg.sensors))
            return -1;
        prev = st.chip_cfg.fifo_enable;
        st.chip_cfg.fifo_enable = sensors & st.chip_cfg.sensors;
        if (st.chip_cfg.fifo_enable != sensors)
            /* You're not getting what you asked for. Some sensors are
             * asleep.
             */
            result = -1;
        else
            result = 0;
        if (sensors || st.chip_cfg.lp_accel_mode)
            set_int_enable(1);
        else
            set_int_enable(0);
        if (sensors) {
            if (mpu_reset_fifo()) {
                st.chip_cfg.fifo_enable = prev;
                return -1;
            }
        }
    }

    return result;
}
/**
 *  @brief      Enter low-power accel-only mode.
 *  In low-power accel mode, the chip goes to sleep and only wakes up to sample
 *  the accelerometer at one of the following frequencies:
 *  \n MPU6050: 1.25Hz, 5Hz, 20Hz, 40Hz
 *  \n MPU6500: 1.25Hz, 2.5Hz, 5Hz, 10Hz, 20Hz, 40Hz, 80Hz, 160Hz, 320Hz, 640Hz
 *  \n If the requested rate is not one listed above, the device will be set to
 *  the next highest rate. Requesting a rate above the maximum supported
 *  frequency will result in an error.
 *  \n To select a fractional wake-up frequency, round down the value passed to
 *  @e rate.
 *  @param[in]  rate        Minimum sampling rate, or zero to disable LP
 *                          accel mode.
 *  @return     0 if successful.
 */
int mpu_lp_accel_mode(unsigned short rate)
{
    unsigned char tmp[2];

    if (rate > 40)
        return -1;

    if (!rate) {
        mpu_set_int_latched(0);
        tmp[0] = 0;
        tmp[1] = BIT_STBY_XYZG;
        if (i2c_write(st.hw->addr, st.reg->pwr_mgmt_1, 2, tmp))
            return -1;
        st.chip_cfg.lp_accel_mode = 0;
        return 0;
    }
    /* For LP accel, we automatically configure the hardware to produce latched
     * interrupts. In LP accel mode, the hardware cycles into sleep mode before
     * it gets a chance to deassert the interrupt pin; therefore, we shift this
     * responsibility over to the MCU.
     *
     * Any register read will clear the interrupt.
     */
    mpu_set_int_latched(1);
    tmp[0] = BIT_LPA_CYCLE;
    if (rate == 1) {
        tmp[1] = INV_LPA_1_25HZ;
        mpu_set_lpf(5);
    } else if (rate <= 5) {
        tmp[1] = INV_LPA_5HZ;
        mpu_set_lpf(5);
    } else if (rate <= 20) {
        tmp[1] = INV_LPA_20HZ;
        mpu_set_lpf(10);
    } else {
        tmp[1] = INV_LPA_40HZ;
        mpu_set_lpf(20);
    }
    tmp[1] = (tmp[1] << 6) | BIT_STBY_XYZG;
    if (i2c_write(st.hw->addr, st.reg->pwr_mgmt_1, 2, tmp))
        return -1;
    st.chip_cfg.sensors = INV_XYZ_ACCEL;
    st.chip_cfg.clk_src = 0;
    st.chip_cfg.lp_accel_mode = 1;
    mpu_configure_fifo(0);

    return 0;
}
/**
 *  @brief      Get sampling rate.
 *  @param[out] rate    Current sampling rate (Hz).
 *  @return     0 if successful.
 */
int mpu_get_sample_rate(unsigned short *rate)
{
    if (st.chip_cfg.dmp_on)
        return -1;
    else
        rate[0] = st.chip_cfg.sample_rate;
    return 0;
}

/**
 *  @brief      Set sampling rate.
 *  Sampling rate must be between 4Hz and 1kHz.
 *  @param[in]  rate    Desired sampling rate (Hz).
 *  @return     0 if successful.
 */
int mpu_set_sample_rate(unsigned short rate)
{
    unsigned char data;

    if (!(st.chip_cfg.sensors))
        return -1;

    if (st.chip_cfg.dmp_on)
        return -1;
    else {
        if (st.chip_cfg.lp_accel_mode) {
            if (rate && (rate <= 40)) {
                /* Just stay in low-power accel mode. */
                mpu_lp_accel_mode(rate);
                return 0;
            }
            /* Requested rate exceeds the allowed frequencies in LP accel mode,
             * switch back to full-power mode.
             */
            mpu_lp_accel_mode(0);
        }
        if (rate < 4)
            rate = 4;
        else if (rate > 1000)
            rate = 1000;

        data = 1000 / rate - 1;
        if (i2c_write(st.hw->addr, st.reg->rate_div, 1, &data))
            return -1;

        st.chip_cfg.sample_rate = 1000 / (1 + data);

        /* Automatically set LPF to 1/2 sampling rate. */
        mpu_set_lpf(st.chip_cfg.sample_rate >> 1);
        return 0;
    }
}
/**
 *  @brief      Get compass sampling rate.
 *  @param[out] rate    Current compass sampling rate (Hz).
 *  @return     0 if successful.
 */
int mpu_get_compass_sample_rate(unsigned short *rate)
{
#ifdef AK89xx_SECONDARY
    rate[0] = st.chip_cfg.compass_sample_rate;
    return 0;
#else
    rate[0] = 0;
    return -1;
#endif
}

/**
 *  @brief      Set compass sampling rate.
 *  The compass on the auxiliary I2C bus is read by the MPU hardware at a
 *  maximum of 100Hz. The actual rate can be set to a fraction of the gyro
 *  sampling rate.
 *
 *  \n WARNING: The new rate may be different than what was requested. Call
 *  mpu_get_compass_sample_rate to check the actual setting.
 *  @param[in]  rate    Desired compass sampling rate (Hz).
 *  @return     0 if successful.
 */
int mpu_set_compass_sample_rate(unsigned short rate)
{
#ifdef AK89xx_SECONDARY
    unsigned char div;
    if (!rate || rate > st.chip_cfg.sample_rate || rate > MAX_COMPASS_SAMPLE_RATE)
        return -1;

    div = st.chip_cfg.sample_rate / rate - 1;
    if (i2c_write(st.hw->addr, st.reg->s4_ctrl, 1, &div))
        return -1;
    st.chip_cfg.compass_sample_rate = st.chip_cfg.sample_rate / (div + 1);
    return 0;
#else
    return -1;
#endif
}
/**
 *  @brief      Set device to bypass mode.
 *  @param[in]  bypass_on   1 to enable bypass mode.
 *  @return     0 if successful.
 */
int mpu_set_bypass(unsigned char bypass_on)
{
    unsigned char tmp;

    if (st.chip_cfg.bypass_mode == bypass_on)
        return 0;

    if (bypass_on) {
        if (i2c_read(st.hw->addr, st.reg->user_ctrl, 1, &tmp))
            return -1;
        tmp &= ~BIT_AUX_IF_EN;
        if (i2c_write(st.hw->addr, st.reg->user_ctrl, 1, &tmp))
            return -1;
        delay_ms(3);
        tmp = BIT_BYPASS_EN;
        if (st.chip_cfg.active_low_int)
            tmp |= BIT_ACTL;
        if (st.chip_cfg.latched_int)
            tmp |= BIT_LATCH_EN | BIT_ANY_RD_CLR;
        if (i2c_write(st.hw->addr, st.reg->int_pin_cfg, 1, &tmp))
            return -1;
    } else {
        /* Enable I2C master mode if compass is being used. */
        if (i2c_read(st.hw->addr, st.reg->user_ctrl, 1, &tmp))
            return -1;
        if (st.chip_cfg.sensors & INV_XYZ_COMPASS)
            tmp |= BIT_AUX_IF_EN;
        else
            tmp &= ~BIT_AUX_IF_EN;
        if (i2c_write(st.hw->addr, st.reg->user_ctrl, 1, &tmp))
            return -1;
        delay_ms(3);
        if (st.chip_cfg.active_low_int)
            tmp = BIT_ACTL;
        else
            tmp = 0;
        if (st.chip_cfg.latched_int)
            tmp |= BIT_LATCH_EN | BIT_ANY_RD_CLR;
        if (i2c_write(st.hw->addr, st.reg->int_pin_cfg, 1, &tmp))
            return -1;
    }
    st.chip_cfg.bypass_mode = bypass_on;
    return 0;
}
/**
 *  @brief      Turn specific sensors on/off.
 *  @e sensors can contain a combination of the following flags:
 *  \n INV_X_GYRO, INV_Y_GYRO, INV_Z_GYRO
 *  \n INV_XYZ_GYRO
 *  \n INV_XYZ_ACCEL
 *  \n INV_XYZ_COMPASS
 *  @param[in]  sensors    Mask of sensors to wake.
 *  @return     0 if successful.
 */
int mpu_set_sensors(unsigned char sensors)
{
    unsigned char data;
#ifdef AK89xx_SECONDARY
    unsigned char user_ctrl;
#endif

    if (sensors & INV_XYZ_GYRO)
        data = INV_CLK_PLL;
    else if (sensors)
        data = 0;
    else
        data = BIT_SLEEP;
    if (i2c_write(st.hw->addr, st.reg->pwr_mgmt_1, 1, &data)) {
        st.chip_cfg.sensors = 0;
        return -1;
    }
    st.chip_cfg.clk_src = data & ~BIT_SLEEP;

    data = 0;
    if (!(sensors & INV_X_GYRO))
        data |= BIT_STBY_XG;
    if (!(sensors & INV_Y_GYRO))
        data |= BIT_STBY_YG;
    if (!(sensors & INV_Z_GYRO))
        data |= BIT_STBY_ZG;
    if (!(sensors & INV_XYZ_ACCEL))
        data |= BIT_STBY_XYZA;
    if (i2c_write(st.hw->addr, st.reg->pwr_mgmt_2, 1, &data)) {
        st.chip_cfg.sensors = 0;
        return -1;
    }

    if (sensors && (sensors != INV_XYZ_ACCEL))
        /* Latched interrupts only used in LP accel mode. */
        mpu_set_int_latched(0);

#ifdef AK89xx_SECONDARY
#ifdef AK89xx_BYPASS
    if (sensors & INV_XYZ_COMPASS)
        mpu_set_bypass(1);
    else
        mpu_set_bypass(0);
#else
    if (i2c_read(st.hw->addr, st.reg->user_ctrl, 1, &user_ctrl))
        return -1;
    /* Handle AKM power management. */
    if (sensors & INV_XYZ_COMPASS) {
        data = AKM_SINGLE_MEASUREMENT;
        user_ctrl |= BIT_AUX_IF_EN;
    } else {
        data = AKM_POWER_DOWN;
        user_ctrl &= ~BIT_AUX_IF_EN;
    }
    if (st.chip_cfg.dmp_on)
        user_ctrl |= BIT_DMP_EN;
    else
        user_ctrl &= ~BIT_DMP_EN;
    if (i2c_write(st.hw->addr, st.reg->s1_do, 1, &data))
        return -1;
    /* Enable/disable I2C master mode. */
    if (i2c_write(st.hw->addr, st.reg->user_ctrl, 1, &user_ctrl))
        return -1;
#endif
#endif

    st.chip_cfg.sensors = sensors;
    st.chip_cfg.lp_accel_mode = 0;
    delay_ms(50);
    return 0;
}
/**
 *  @brief      Initialize hardware.
 *  Initial configuration:\n
 *  Gyro FSR: +/- 2000DPS\n
 *  Accel FSR +/- 2G\n
 *  DLPF: 42Hz\n
 *  FIFO rate: 50Hz\n
 *  Clock source: Gyro PLL\n
 *  FIFO: Disabled.\n
 *  Data ready interrupt: Disabled, active low, unlatched.
 *  @param[in]  int_param   Platform-specific parameters to interrupt API.
 *  @return     0 if successful.
void SPI1_6000_ReadReg_NByte(u8 reg, u8 *data, u8 size);
void SPI1_6000_WriteReg_NByte(u8 reg, u8 *data, u8 size);
 */
int mpu_init(struct int_param_s *int_param)
{
    unsigned char data[6];

    /* Reset device. */
    data[0] = BIT_RESET;
    i2c_write(st.hw->addr, st.reg->pwr_mgmt_1, 1, data);
    delay_mpu6000_ms(100);

    /* Wake up chip. */
    data[0] = 0x00;
    i2c_write(st.hw->addr, st.reg->pwr_mgmt_1, 1, data);

    st.chip_cfg.accel_half = 0;

    /* Set to invalid values to ensure no I2C writes are skipped. */
    st.chip_cfg.sensors = 0xFF;
    st.chip_cfg.gyro_fsr = 0xFF;
    st.chip_cfg.accel_fsr = 0xFF;
    st.chip_cfg.lpf = 0xFF;
    st.chip_cfg.sample_rate = 0xFFFF;
    st.chip_cfg.fifo_enable = 0xFF;
    st.chip_cfg.bypass_mode = 0xFF;

    /* mpu_set_sensors always preserves this setting. */
    st.chip_cfg.clk_src = INV_CLK_PLL;
    /* Handled in next call to mpu_set_bypass. */
    st.chip_cfg.active_low_int = 1;
    st.chip_cfg.latched_int = 0;
    st.chip_cfg.int_motion_only = 0;
    st.chip_cfg.lp_accel_mode = 0;
    memset(&st.chip_cfg.cache, 0, sizeof(st.chip_cfg.cache));
    st.chip_cfg.dmp_on = 0;
    st.chip_cfg.dmp_loaded = 0;
    st.chip_cfg.dmp_sample_rate = 0;

    if (mpu_set_gyro_fsr(2000))
        return -1;
    if (mpu_set_accel_fsr(2))
        return -1;
    if (mpu_set_lpf(42))
        return -1;
    if (mpu_set_sample_rate(50))
        return -1;
    if (mpu_configure_fifo(0))
        return -1;

//    if (int_param)
//        reg_int_cb(int_param);

    /* Already disabled by setup_compass. */
    if (mpu_set_bypass(0))
        return -1;
    mpu_set_sensors(0);
    return 0;
}
/******************* (C) COPYRIGHT 2016*****END OF FILE****/
