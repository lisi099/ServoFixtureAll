/******************** (C) COPYRIGHT 2016 ********************
 * �ļ���  ��spi1.c
 * ����    ��spi1 Ӧ�ú�����
 *          
 * ʵ��ƽ̨��
 * Ӳ�����ӣ�

***************************************************************/
#ifndef SPI1_H
#define	SPI1_H

#include "stm32f10x.h"

void spi1_init(void);		
u8 SPI1_6000_Who_Am_I(void);
void SPI1_6000_ReadReg_NByte(u8 reg, u8 *data, u8 size);
void SPI1_6000_WriteReg_NByte(u8 reg, u8 *data, u8 size);
void SPI1_5983_ReadReg_NByte(u8 reg, u8 *data, u8 size);
void SPI1_5983_WriteReg_NByte(u8 reg, u8 *data, u8 size);
void SPI1_5611_ReadReg_NByte(u8 reg, u8 *data, u8 size);
void SPI1_5611_WriteReg_NByte(u8 reg, u8 *data, u8 size);


#endif 
