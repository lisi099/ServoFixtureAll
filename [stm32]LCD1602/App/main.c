/******************** (C) COPYRIGHT 2016********************
 * 文件名  ：main.c
 * 描述    ：         
 * 库版本  ：ST3.5.0
*********************************************************/
#include "stm32f10x.h"
#include "system_tick.h"
#include <string.h>
#include "1602_iic_sw.h"
#include "ServoAdc.h"
/*************************************************************
  Function   :
  Description:
  Input      : none
  return     : none
*************************************************************/
void delay_about_ms(uint16_t nms)
{
	uint16_t i=0;
	uint16_t j=0;
	for(i=0; i<nms; i++)
		for(j=0; j<7000; j++);
}
/*************************************************************
  Function   :
  Description:
  Input      : 
  return     : 
*************************************************************/

int main(void)
{
	delay_about_ms(100);
	set_board_systick();
	delay_about_ms(50);
	adc_configration();
//	test_lcd();
	while(1)
	{
		delay_about_ms(500);
	}

}	
/******************* (C) COPYRIGHT 2016 *****END OF FILE****/
