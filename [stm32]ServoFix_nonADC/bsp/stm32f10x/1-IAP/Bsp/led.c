/******************** (C) COPYRIGHT 2016 ********************
 * 文件名  ：led.c
 * 描述    ：led 应用函数库
 *          
 * 实验平台：
 * 硬件连接：

***************************************************************/

#include "led.h"
#include "system_tick.h"

void led_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);	 // turn off all led
}

/*************************************************************
  Function   :
  Description:
  Input      : none
  return     : none
*************************************************************/
void  led1_blink(int32_t freq)
{
	static uint32_t led1_ticks = 0;
	static uint8_t 	count = 0;
	if(get_system_ms() - led1_ticks > freq)
	{
		led1_ticks = get_system_ms();
		count++;
		if(count%2)
			LED1(0);
		else
			LED1(1);
	}
}
/******************* (C) COPYRIGHT 2016*****END OF FILE****/
