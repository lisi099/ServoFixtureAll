/******************** (C) COPYRIGHT 2016 ********************
 * 文件名  ：flash.c
 * 描述    ：flash 应用函数库
 *          
 * 实验平台：
 * 硬件连接：

***************************************************************/
#ifndef FLASH_H
#define	FLASH_H

#include "stm32f10x.h"

uint8_t erase_flash(void);
uint8_t flash_write_one_word(uint32_t WriteAddress,uint32_t WriteData);
int flash_read_n_byte(uint32_t ReadAddress, uint8_t *ReadBuf, int32_t ReadNum);
uint8_t erase_flash_part(const uint32_t flash_start, const uint32_t flash_end);


#endif 
