/******************** (C) COPYRIGHT 2016 ********************
 * �ļ���  ��flash.c
 * ����    ��flash Ӧ�ú�����
 *          
 * ʵ��ƽ̨��
 * Ӳ�����ӣ�

***************************************************************/
#ifndef FLASH_H
#define	FLASH_H

#include "stm32f10x.h"

uint8_t erase_flash(void);
uint8_t flash_write_one_word(uint32_t WriteAddress,uint32_t WriteData);
int flash_read_n_byte(uint32_t ReadAddress, uint8_t *ReadBuf, int32_t ReadNum);
uint8_t erase_flash_part(const uint32_t flash_start, const uint32_t flash_end);


#endif 
