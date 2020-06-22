
#include "key_driver.h"
#include "key_check.h"
#include <rtthread.h>
#include "menu_com_protocol.h"
#include "lcd_disp.h"

void thread_disp_key_check(void* parameter)
{
	uint8_t key;
	PressState key_state;
	uint8_t msg_key[2] ={0};
	
	key_init();
	while(1)
	{
		for(key =0; key <KEY_NUM; key++)
		{
			key_state = key_scan(key);
			(void)(key_state);
			if(key_state == S_CLICK){
				msg_key[0] = key;
				msg_key[1] = 0;
			}
			else if(key_state == L_CLICK){
				msg_key[0] = key;
				msg_key[1] = 1;
//				rt_mq_send(&key_mq, msg_key, 2);
			}
		}
		rt_thread_delay( RT_TICK_PER_SECOND/50);
	}
}

void thread_menu_check(void* parameter)
{
	menu_disp_protocol(0, MENU_DISP_LOGO);
	while(1)
	{
		
	}
}


//-----------------------end-----------------------------
