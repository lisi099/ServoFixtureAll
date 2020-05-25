/******************** (C) COPYRIGHT 2016********************
 * 文件名  ：main.c
 * 描述    ：         
 * 库版本  ：ST3.5.0
*********************************************************/
#include "stm32f10x.h"
#include "system_tick.h"
#include <string.h>
#include "1602_iic_sw.h"
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
//u8 text_write[20] 	= "AAAAAAAAAAAAAAAAAA";
//u8 text_read[20] 	= "000000000";
//u8  check = 100;

int main(void)
{
	delay_about_ms(100);
	set_board_systick();
	delay_about_ms(50);
//	init_1602();
	test_lcd();
	while(1)
	{
		delay_about_ms(500);
	}

}	
/******************* (C) COPYRIGHT 2016 *****END OF FILE****/
