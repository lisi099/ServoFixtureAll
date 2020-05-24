#ifndef __1602_IIC_SW_H__
#define __1602_IIC_SW_H__

#include "stm32f10x.h"
#include "moniiic.h"
#include "string.h"
#include "stdio.h"

#define ADDR	0x78

//High bit to R,Loe bit to W
#define ADDRREAD	((ADDR<<1) | 0x01)
#define ADDRWRITE	(ADDR<<1)

#define BACKLIGHT_ON	0x08
#define BACKLIGHT_OFF	0x00 
#define LCD_CLC	0x00

#define CMD   0
#define DATA	1

void init_1602(void);
void lcdPrint(const char* charactor);
void disOneChar(unsigned char X, unsigned char Y, char DData);
void writeOneChar(const u8 cmdOrData, const u8 Data);
void setCursor(u8 colmn, u8 line);
void i2cAction(u8 status);

#endif
