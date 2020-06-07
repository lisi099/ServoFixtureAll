/******************** (C) COPYRIGHT 2016********************
 * �ļ���  ��main.c
 * ����    ��         
 * ��汾  ��ST3.5.0
*********************************************************/
#include "stm32f10x.h"
#include "system_tick.h"
#include <string.h>
#include "1602_iic_sw.h"
#include "ServoAdc.h"
#include "At24c128.h"
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
u8 text_write[20] 	= "AAAAAAAAAAAAAAAAAA";
u8 text_read[20] 	= "000000000";
int main(void)
{
	delay_about_ms(100);
	set_board_systick();
	delay_about_ms(50);
	adc_configration();
	IIC_EEInit();
	AT24CXX_Write(0, text_write, 20);
	delay_about_ms(1000);
	AT24CXX_Read(0, text_read, 20);
	test_lcd();
	
	while(1)
	{
		delay_about_ms(500);
	}

}	
/******************* (C) COPYRIGHT 2016 *****END OF FILE****/
