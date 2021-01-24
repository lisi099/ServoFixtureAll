/******************** (C) COPYRIGHT 2016 ********************
 * 文件名  ：led.c
 * 描述    ：led 应用函数库
 *          
 * 实验平台：
 * 硬件连接：

***************************************************************/
#ifndef LED_H
#define	LED_H

#include "stm32f10x.h"

#define LED1(a)	if (a)	\
				GPIO_SetBits(GPIOB,GPIO_Pin_5);\
				else		\
				GPIO_ResetBits(GPIOB,GPIO_Pin_5)


void 	led_init(void);
void  led1_blink(int32_t freq);				

#endif 
