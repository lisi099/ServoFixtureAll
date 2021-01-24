/******************** (C) COPYRIGHT 2016********************
 * 文件名  ：main.c
 * 描述    ：         
 * 库版本  ：ST3.5.0
*********************************************************/
#include "stm32f10x.h"
#include "usart1.h"
#include "led.h"
#include "system_tick.h"
#include "board_info.h"
#include "usart1_fifo.h"
#include "usart1_package.h"

#include "anbt_dmp_fun.h"
#include "anbt_i2c.h"
#include "anbt_dmp_mpu6050.h"
#include "anbt_dmp_driver.h"

/*	USART1_TX		USART1_RX		
		PA9					PA10				
		IAP					IAP				
*/
/*************************************************************
  Function   :
  Description:
  Input      : 
  return     : 
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
	//NVIC_SetVectorTable(FLASH_START_ADDR, 0x4000);
	delay_about_ms(100);
	set_board_systick();
	led_init();
	dmp_i2c_init_all();	//--i2c初始化
	dmp_init_all();	    //--mpu6050_dpm初始化
  
	usart1_init();
	usart1_fifo_rx_init();
	usart1_fifo_tx_init();
	delay_about_ms(100);
	
	while(1)
	{
		led1_blink(500);
		usart1_length_13_data_process();
		delay_about_ms(5);
		dmp_read_all_data();
		if(get_reset_mcu_flag() == 1){
			delay_about_ms(50);
			NVIC_SystemReset();
		}
	}
}	


/******************* (C) COPYRIGHT 2016 *****END OF FILE****/
