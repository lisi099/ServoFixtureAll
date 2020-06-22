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
u8 text_write[64] 	= "0";
u8 text_read[64] 	= "0";

uint32_t address_ = 100;
uint8_t	 crc = 1;
int main(void)
{
	uint8_t i;
	delay_about_ms(100);
	set_board_systick();
	delay_about_ms(50);
	adc_configration();
	
	for(i=0; i<64; i++){
		text_write[i] = 'A';
	}
	IIC_EEInit();
	AT24CXX_Write(address_, text_write, 64);
	delay_about_ms(1000);
	AT24CXX_Read(address_, text_read, 64);
	for(i=0; i<64; i++){
		if(text_read[i] != text_write[i]){
			crc = 0;
			break;
		}
	}
	
	test_lcd();
	
	while(1)
	{
		delay_about_ms(500);
	}

}	
/******************* (C) COPYRIGHT 2016 *****END OF FILE****/
