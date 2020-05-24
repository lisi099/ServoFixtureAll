/******************** (C) COPYRIGHT 2016********************
 * 文件名  ：main.c
 * 描述    ：         
 * 库版本  ：ST3.5.0
*********************************************************/
#include "stm32f10x.h"
#include "system_tick.h"
#include <string.h>
//#include "1602_iic_sw.h"
//#include "At24cXX.h"
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
u8  check = 100;
int main(void)
{
	delay_about_ms(100);
	set_board_systick();
	delay_about_ms(50);
//	init_1602();

	IIC_EEInit();
	
//	AT24CXX_Init();
//	AT24CXX_WriteOneByte(EE_TYPE,0X55);
//	check = AT24CXX_ReadOneByte(EE_TYPE);
//	AT24CXX_Write(50, text_write, sizeof(text_write));
	AT24CXX_Write(50, text_write, sizeof(text_write));
	
	while(1)
	{
		AT24CXX_Read(50, text_read, sizeof(text_read));
//		AT24CXX_Read(50, text_read, sizeof(text_read));
		delay_about_ms(500);
//		disOneChar(1,1, 'A');
	}

}	
/******************* (C) COPYRIGHT 2016 *****END OF FILE****/
