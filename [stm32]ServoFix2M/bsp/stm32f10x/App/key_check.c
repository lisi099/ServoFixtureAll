
#include "key_driver.h"
#include "key_check.h"
#include <rtthread.h>

#define DELAY_MS(x)		rt_thread_delay(x)
#define GET_SYS_TIME	rt_tick_get() 

static uint8_t  wait_on[KEY_NUM] = { 0 };

uint8_t read_button(uint8_t key)
{
	switch(key){
		case KEY_BUTTON_UP:
			return read_up_button();
		case KEY_BUTTON_DOWN:
			return read_down_button();
		case KEY_BUTTON_OK:
			return read_ok_button();
		case KEY_BUTTON_ESC:
			return read_re_button();
		default:
			break;
	}
	return 0;
}

PressState key_scan(uint8_t key)
{
	uint32_t key_trig_time0;
	PressState	key_press = NO_CLICK;
	KeyState	key_sta = WAIT_TRIG;
	while(1)
	{
		switch(key_sta){
			case WAIT_TRIG:
				if(wait_on[key]){
					DELAY_MS(250);
					if(read_button(key)){
						wait_on[key] = 1;
						key_press = L_CLICK;
						return key_press;
					}
					else{
						wait_on[key] = 0;
						return key_press;
					}
					
				}
				if(read_button(key)){
					key_sta = TRIGGER;
					key_trig_time0 = GET_SYS_TIME;
				}
				else{
					return key_press;
				}
				break;
			case TRIGGER:
				DELAY_MS(30);
				key_sta = ANTI_SHAKE;
				break;
			case ANTI_SHAKE:
				if(read_button(key)){
					key_sta = CLICK;
				}
				else{
					return key_press;
				}
				break;
			case CLICK:
				while(1){
					DELAY_MS(30);
					if(!read_button(key)){ //ÊÍ·Å
						key_press = S_CLICK;
						return key_press;
					}
					else{
						if(GET_SYS_TIME - key_trig_time0 >600){
							key_press = L_CLICK;
							wait_on[key] = 1;
							return key_press;
						}
					}
				}
			default:
				key_sta = WAIT_TRIG;
				return key_press;
		}
	}
}
