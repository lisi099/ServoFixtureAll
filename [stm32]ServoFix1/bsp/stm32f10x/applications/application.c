/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 */
 
#include <board.h>
#include <rtthread.h>
#include "1602_iic_sw.h"
#include "menu.h"
#include "menu_app.h"

#include "usart1.h"
#include "usart2.h"

#include "servo_serial.h"
#include "usart1_package.h"
#include "usart1_fifo.h"
#include "servo_adc.h"

//----------------------��Ϣ����-------------------------
static struct rt_messagequeue key_mq;
static char key_msg_pool[50];

struct rt_messagequeue usart1_r_mq;
static char usart1_r_msg_pool[120];

struct rt_messagequeue usart2_r_mq;
static char usart2_r_msg_pool[120];

//-----------------------���������߳�-----------------------
static void key_scan_thread(void* parameter)
{
	uint8_t key;
	PressState key_state;
	uint8_t msg_key[2] ={0};
	
	key_new_init();
	while(1)
	{
		for(key =0; key <KEY_NUM; key++)
		{
			key_state = key_run1(key);
			(void)(key_state);
			if(key_state == S_CLICK){
				msg_key[0] = key;
				msg_key[1] = 0;
				rt_mq_send(&key_mq, msg_key, 2);
			}
			else if(key_state == L_CLICK){
				msg_key[0] = key;
				msg_key[1] = 1;
				rt_mq_send(&key_mq, msg_key, 2);
			}
		}
		rt_thread_delay(RT_TICK_PER_SECOND/50);
	}
}

//-----------------------�˵�����-----------------------
void start_page(void)
{
	put_chars(0, 0, "     POWEHO     ");
	rt_thread_delay(RT_TICK_PER_SECOND*2);
	while(1)
	{
		if(get_servo_state()){
			put_chars(0, 0, "  CONNECT OK!   ");
			rt_thread_delay(RT_TICK_PER_SECOND/2);
			put_chars(0, 0, "  READDING....  ");
			if(menu_combine_fb_work_parm()){
				put_chars(0, 0, " READ PARAM OK! ");
			}
			else{
				put_chars(0, 0, "READ PARAM FAIL!");
			}
			rt_thread_delay(RT_TICK_PER_SECOND*2);
			break;
		}
		else{
			put_chars(0, 0, "PLS CONNECT SERV");
		}
		rt_thread_delay(RT_TICK_PER_SECOND/2);
	}
}

static void menu_process_thread(void* parameter)
{
	uint8_t rec_buff[2];
	uint8_t tempKey = KEY_NONE;
	adc_configration();
	lcd_init();
	
	start_page();
	SetMainPage(&mainPage);
    ShowPage(pPage);
    while (1)
    {
		tempKey = KEY_NONE;
		if(rt_mq_recv(&key_mq, &rec_buff, 2, RT_WAITING_NO)== RT_EOK){
			switch (rec_buff[0]){
				case 0:
					if(rec_buff[1] == 0){
						tempKey = KEY_UP;
					}
					else{
						tempKey = KEY_UP_L;
					}
					break;
				case 1:
					if(rec_buff[1] == 0){
						tempKey = KEY_Down;
					}
					else{
						tempKey = KEY_Down_L;
					}
					break;
				case 3:
					tempKey = KEY_Ok;
					break;
				case 2:
					tempKey = KEY_Return;
					break;
				default:
					break;
			}
			if(tempKey !=KEY_NONE){
				pPage->Function(tempKey);
			}
		}
		rt_thread_delay(RT_TICK_PER_SECOND/50);
    }
}

//-----------------------USB����ͨѶ�߳�----------------------
static void usb_usart_thread(void* parameter)
{
	uint8_t data;
	uint8_t data_send[12];
	uint8_t size;
	
	usart1_fifo_rx_init();
	usart1_init(9600);
	usart2_init_rx(9600);
	rt_thread_delay(RT_TICK_PER_SECOND);
    while (1)
    {
		usart1_length_13_data_process();
		//
		size = 0;
		while(1){
			if(rt_mq_recv(&usart1_r_mq, &data, 1, RT_WAITING_NO)== RT_EOK){
				data_send[size] = data;
				size++;
				if(size ==12){
					break;
				}
			}
			else{
				break;
			}
		}
		if(size >0){
			while(1){
				if(Txd2_Flag == 0){
					break;
				}
				rt_thread_delay(1);
			}
			usart2_send_buff(data_send, size);
		}
		//
		size = 0;
		while(1){
			if(rt_mq_recv(&usart2_r_mq, &data, 1, RT_WAITING_NO)== RT_EOK){
				data_send[size] = data;
				size++;
				if(size ==12){
					break;
				}
			}
			else{
				break;
			}
		}
		if(size >0){
			while(1){
				if(usart1_get_tx_flag() == 0){
					break;
				}
				rt_thread_delay(1);
			}
			usart1_send_buff(data_send, size);
		}
		
		
		rt_thread_delay(1);
    }
}

//-----------------------�����շ��л��߳�------------------------
static void usart_sw_thread_entry(void* parameter)
{
	rt_thread_delay(RT_TICK_PER_SECOND);
	while(1)
	{
		if(Txd2_Flag ==0 && usart2_mode ==0){
			rt_thread_delay(2);
			usart2_init_rx(bd_set);
		}
		rt_thread_delay(1);
	}
}

//------------------------��ʼ���߳�-----------------------
int rt_application_init(void)
{
	rt_mq_init(&key_mq, "key_mqt", &key_msg_pool[0], 2, sizeof(key_msg_pool), RT_IPC_FLAG_FIFO);
	rt_mq_init(&usart1_r_mq, "usart1_r_mq", &usart1_r_msg_pool[0], 1, sizeof(usart1_r_msg_pool), RT_IPC_FLAG_FIFO);
	rt_mq_init(&usart2_r_mq, "usart2_r_mq", &usart2_r_msg_pool[0], 1, sizeof(usart2_r_msg_pool), RT_IPC_FLAG_FIFO);
	
	rt_thread_t tid1 = RT_NULL;
	
	tid1 = rt_thread_create("key_scan", key_scan_thread, RT_NULL, 512, 19, 5);
	if (tid1 != RT_NULL) rt_thread_startup(tid1);
	
	tid1 = rt_thread_create("menu_process", menu_process_thread, RT_NULL, 512, 20, 5);
	if (tid1 != RT_NULL) rt_thread_startup(tid1);
	
	tid1 = rt_thread_create("usb_usart", usb_usart_thread, RT_NULL, 2048, 17, 10);
	if (tid1 != RT_NULL) rt_thread_startup(tid1);
	
	tid1 = rt_thread_create("usart_sw", usart_sw_thread_entry, RT_NULL, 1024, 16, 10);
	if (tid1 != RT_NULL) rt_thread_startup(tid1);
	
    return 0;
}

/*@}*/
