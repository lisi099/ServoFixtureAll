/******************** (C) COPYRIGHT 2016 ********************
 * 文件名  ：.h
 * 描述    ： 应用函数库
 *          
 * 实验平台：
 * 硬件连接：

***************************************************************/
#ifndef STM32_CRC32_H
#define	STM32_CRC32_H

#include "stm32f10x.h"

uint32_t stm32_crc32( uint32_t *pBuf, uint16_t nSize );

void test_prog(void);



#endif 
