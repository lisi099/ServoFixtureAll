/******************** (C) COPYRIGHT 2016 ********************
 * �ļ���  ��led.c
 * ����    ��led Ӧ�ú�����
 *          
 * ʵ��ƽ̨��
 * Ӳ�����ӣ�

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
