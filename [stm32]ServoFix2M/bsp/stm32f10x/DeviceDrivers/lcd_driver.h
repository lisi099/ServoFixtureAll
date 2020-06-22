/******************** (C) COPYRIGHT 2016 ********************

***************************************************************/
#ifndef _LCD_DRIVER_
#define _LCD_DRIVER_

#include "stm32f10x.h"
#include "pin_driver.h"

#define 	I2C_SCL     GPIO_Pin_6
#define 	I2C_SDA     GPIO_Pin_7
#define 	I2C_RST     GPIO_Pin_5

#define  	IIC_SCL    PBout(6)   
#define  	IIC_SDA    PBout(7)
#define  	READ_SDA   PBin(7)

#define  	scl    		PBout(6)   
#define  	sda    		PBout(7)
#define 	res			PBout(5)


void lcd_init(void);
void put_chars(unsigned char row, unsigned char col, char *p);

#endif
