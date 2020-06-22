/******************** (C) COPYRIGHT 2016 ********************
º

***************************************************************/
#ifndef _KEY_H_
#define _KEY_H_

#include "stm32f10x.h"


#define KEY_UP_PIN                    	(GPIO_Pin_5)
#define KEY_DOWN_PIN                  	(GPIO_Pin_6)
#define KEY_OK_PIN                    	(GPIO_Pin_4)
#define KEY_RE_PIN                    	(GPIO_Pin_7)

#define read_up_button()				(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5))
#define read_down_button()				(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6))
#define read_ok_button()				(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4))
#define read_re_button()				(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7))

void key_init(void);


#endif
