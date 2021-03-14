/******************** (C) COPYRIGHT 2016********************
 * 文件名  ：main.c
 * 描述    ：         
 * 库版本  ：ST3.5.0
*********************************************************/
#include "stm32f10x.h"
#include "includes.h"
#include "system_tick.h"
#include "led.h"
#include "iap.h"
#include "flash.h"
#include "board_info.h"
#include "usart1.h"
#include "crc16.h"
#include "stm32_crc32.h"
void delay_about_ms(uint16_t ms)
{
	uint16_t i,j;
	for(i=0; i<ms; i++)
	for(j=0; j<7000; j++)
		;
}


/*************************************************************
  Function   :
  Description:
  Input      : none
  return     : none
*************************************************************/
int main(void)
{
	uint8_t flash_state[4]={0x90, 0x90, 0x90, 0x90};
	delay_about_ms(100);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,ENABLE);
	set_board_systick();
	led_init();
	usart1_init();
	//test_prog();
	flash_read_n_byte(FLASH_INFO_ADDR, flash_state, 4);
	if(flash_state[0] == 0x08 || flash_state[0] == 0xFF) //不需要进行升级
	{	
		IAP_JumpToApplication(); 
	}
	while(1)
	{
		if(iap_data_receieve())
		{
			iap_update();
		}
		if(get_update_status())
		{
			 delay_about_ms(500);
			 IAP_JumpToApplication(); 
		}
		led1_blink(100);
	}
}	


/******************* (C) COPYRIGHT 2016 *****END OF FILE****/
