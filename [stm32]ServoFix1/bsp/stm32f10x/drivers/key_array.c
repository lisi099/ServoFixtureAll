/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/

#include "key.h"
#include <rtthread.h>

#define DELAY_MS(x)		rt_thread_delay(x)
#define GET_SYS_TIME	rt_tick_get() 

static uint8_t  wait_on[KEY_NUM] = { 0 };

#define KEY_UP_PIN                    	(GPIO_Pin_5)
#define KEY_DOWN_PIN                  	(GPIO_Pin_6)
#define KEY_OK_PIN                    	(GPIO_Pin_4)
#define KEY_RE_PIN                    	(GPIO_Pin_7)

#define read_up_button()				(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5))
#define read_down_button()				(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6))
#define read_ok_button()				(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4))
#define read_re_button()				(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7))


void key_new_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
    GPIO_InitStructure.GPIO_Pin   = KEY_UP_PIN | KEY_DOWN_PIN | KEY_OK_PIN | KEY_RE_PIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}


uint8_t read_button(uint8_t key)
{
	switch(key){
		case 0:
			return read_up_button();
		case 1:
			return read_down_button();
		case 2:
			return read_ok_button();
		case 3:
			return read_re_button();
		default:
			break;
	}
	return 0;
}
//	WAIT_TRIG = 0,
//	TRIGGER,
//	ANTI_SHAKE,
//	CLICK,
//	WAILT_T,
//	TRIGGER_2,
//	ANTI_SHAKE_2,
//	DLICK
//-----------------------------------
PressState key_run(uint8_t key)
{
	uint32_t key_trig_time0, key_trig_time1, key_trig_time2;
	PressState	key_press = NO_CLICK;
	KeyState	key_sta = WAIT_TRIG;
	while(1)
	{
		switch(key_sta){
			case WAIT_TRIG:
				if(wait_on[key]){
					DELAY_MS(2000);
					wait_on[key] = 0;
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
				if(!read_button(key)){
					key_sta = WAILT_T;
					key_trig_time1 = GET_SYS_TIME;
				}
				else{
					DELAY_MS(30);
					if(GET_SYS_TIME - key_trig_time0 >2000){
						key_press = L_CLICK;
						wait_on[key] = 1;
						return key_press;
					}
				}
				break;
			case WAILT_T:
				DELAY_MS(200);
				if(read_button(key)){
					key_sta = TRIGGER_2;
					key_trig_time2 = GET_SYS_TIME;
				}
				else{
					if(key_trig_time1 - key_trig_time0 <2000){
						key_press = S_CLICK;
					}
					else{
						key_press = L_CLICK;
					}
					return key_press;
				}
				break;
			case TRIGGER_2:
				DELAY_MS(30);
				key_sta = ANTI_SHAKE_2;
				break;
			case ANTI_SHAKE_2:
				if(read_button(key)){
					key_sta = DLICK;
				}
				else{
					return key_press; 
				}
				break;
			case DLICK:
				if(!read_button(key)){
					if(GET_SYS_TIME - key_trig_time2 <2000){
						key_press = D_CLICK;
					}
					else{
						key_press = L_CLICK;
					}
					return key_press;
				}
				else{
					DELAY_MS(50);
				}
				break;
			default:
				key_sta = WAIT_TRIG;
				return key_press;
		}
	}
}

PressState key_run1(uint8_t key)
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

void key_thread_run(void* parameter)
{
	uint8_t key;
	PressState key_state;
	key_new_init();
	while(1)
	{
		for(key =0; key <KEY_NUM; key++)
		{
			key_state = key_run(key);
			(void)(key_state);
			//do something
		}
		DELAY_MS(50);
	}

}

