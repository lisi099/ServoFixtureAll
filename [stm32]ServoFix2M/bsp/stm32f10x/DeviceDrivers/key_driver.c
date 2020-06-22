/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#include "key_driver.h"
#include <rtthread.h>


void key_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
    GPIO_InitStructure.GPIO_Pin   = KEY_UP_PIN | KEY_DOWN_PIN | KEY_OK_PIN | KEY_RE_PIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}




