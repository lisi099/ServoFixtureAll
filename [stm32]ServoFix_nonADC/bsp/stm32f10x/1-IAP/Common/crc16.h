/******************** (C) COPYRIGHT 2016 ********************
 * 文件名  ：crc16.h
 * 描述    ：crc16 应用函数库
 *          
 * 实验平台：
 * 硬件连接：

***************************************************************/
#ifndef __CRC16_H
#define	__CRC16_H

#include "stm32f10x.h"

uint16_t Cal_CRC16(uint8_t *data, uint32_t size);

#endif 
