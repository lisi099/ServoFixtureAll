/**************************************************************************************
License:
Copyright (C) 2011-2012 InvenSense Corporation, All Rights Reserved.
See included License.txt for License information.

*  @addtogroup  DRIVERS Sensor Driver Layer
*  @brief       Hardware drivers to communicate with sensors via I2C.
*
*  @{
*      @file       
*      @brief      DMP image and interface functions.
*      @details    All functions are preceded by the dmp_ prefix to
*                  differentiate among MPL and general driver function calls.
**************************************************************************************/

int mpu_write_mem(unsigned short mem_addr, unsigned short length, unsigned char *data);
int mpu_read_mem(unsigned short mem_addr, unsigned short length,  unsigned char *data);
int mpu_get_accel_sens(unsigned short *sens);
int mpu_get_accel_fsr(unsigned char *fsr);
int mpu_read_fifo_stream(unsigned short length, unsigned char *data, unsigned char *more);
int mpu_reset_fifo(void);
//
u8 AnBT_DMP_MPU6050_DEV_CFG(void);
u8 dmp_init_all(void);		
void dmp_read_all_data(void);


//----------------------------------end of file --------------------------


