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
#include "tai_servo.h"
#include "pc_cmd.h"
#include "tai_servo.h"

//----------------------全局变量---------------------------
volatile uint8_t servo_detect_flag_ = 0;
volatile uint8_t reconnect_taiwan_servo_flag_ = 0;
rt_mutex_t servo_mutex = RT_NULL;
rt_mutex_t lcd_mutex = RT_NULL;

//----------------------消息定义---------------------------
struct rt_messagequeue key_mq;
char key_msg_pool[10];

/*-----------------------按键处理线程-----------------------*/
static void key_scan_thread(void* parameter)
{
    uint8_t key;
    PressState key_state;
    uint8_t msg_key[2] = {0};

    key_new_init();

    while(1)
    {
        for(key = 0; key < KEY_NUM; key++)
        {
            key_state = key_run1(key);
            (void)(key_state);

            if(key_state == S_CLICK)
            {
                msg_key[0] = key;
                msg_key[1] = 0;
                rt_mq_send(&key_mq, msg_key, 2);
            }
            else if(key_state == L_CLICK)
            {
                msg_key[0] = key;
                msg_key[1] = 1;
                rt_mq_send(&key_mq, msg_key, 2);
            }
        }

        rt_thread_delay(RT_TICK_PER_SECOND / 50);
    }
}

/*-----------------------菜单处理-----------------------*/
extern volatile uint8_t is_tai_servo_;
extern uint8_t connect_detect(void);
extern void Copy_Data_To_Show(void);

static void menu_process_thread(void* parameter)
{
    uint8_t rec_buff[2];
    uint8_t tempKey = KEY_NONE;
		uint8_t i ;
    while(1)
    {
        tempKey = KEY_NONE;

        if(rt_mq_recv(&key_mq, &rec_buff, 2, RT_WAITING_NO) == RT_EOK)
        {
            switch(rec_buff[0])
            {
            case 0:
                if(rec_buff[1] == 0) tempKey = KEY_UP;
                else tempKey = KEY_UP_L;
                break;
            case 1:
                if(rec_buff[1] == 0) tempKey = KEY_Down;
                else tempKey = KEY_Down_L;
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
            if(tempKey != KEY_NONE){
							rt_mutex_take(servo_mutex, RT_WAITING_FOREVER);
							pPage->Function(tempKey);
							rt_mutex_release(servo_mutex);
							for(i = 0; i < 5; i++)
							{
									rt_mq_recv(&key_mq, &rec_buff, 2, RT_WAITING_NO);
							}
							//
						}
        }
				
        rt_thread_delay(RT_TICK_PER_SECOND / 50);
    }
}

//-----------------------USB串口数据处理-------------
static void usb_usart_thread(void* parameter)
{
		usart1_init(115200);
    usart1_fifo_rx_init();
    rt_thread_delay(RT_TICK_PER_SECOND);
    while(1)
    {
        usart1_length_13_data_process();
        process_pc_data();
        process_test_data();
        rt_thread_delay(1);
    }
}
//-----------------------断开判断-------------
//static void reconnect_taiwan_servo(void* parameter)
//{
//    rt_thread_delay(RT_TICK_PER_SECOND * 5);
//    while(1)
//    {
//        if(reconnect_taiwan_servo_flag_ == 1)
//        {
//            if(is_taiwan_servo())
//            {
//                reconnect_taiwan_servo_flag_ = 0;
//            }
//        }
//        rt_thread_delay(500);
//    }
//}

//-----------------------串口收发切换线程------------------------
static void usart_sw_thread_entry(void* parameter)
{
    rt_thread_delay(RT_TICK_PER_SECOND);

    while(1)
    {
        rt_mutex_take(servo_mutex, RT_WAITING_FOREVER);
        if(Txd2_Flag == TX_FINISH && usart2_mode == TX_MODE)
        {
            rt_thread_delay(2);
            usart2_init_rx(bd_set_);
        }
        rt_mutex_release(servo_mutex);
				
        rt_thread_delay(2);
    }
}

/*------------------------------运行线程----------------------------*/
static void running(void* parameter)
{
		rt_thread_t tid1 = RT_NULL;
	
		/*类型识别*/
		lcd_init();
		Lcd_Clr_Scr();
    put_chars_middle(0, "Welcome to");
    put_chars_middle(1, "Power HD");
    rt_thread_delay(RT_TICK_PER_SECOND * 2);

    Lcd_Clr_Scr();
    put_chars(0, 0, "Please Connect");
    put_chars(1, 0, "Servo");
	
		while(1)
    {
        if(is_taiwan_servo())
        {
            break;
        }
        else if(connect_detect())
        {
            break;
        }
				rt_thread_delay(200);
    }
		
		Lcd_Clr_Scr();
		put_chars_middle(0, "SERVO");
		put_chars_middle(1, "Connect OK");
		rt_thread_delay(RT_TICK_PER_SECOND / 2);
		Lcd_Clr_Scr();
		put_chars(0, 0, "<Reading>....");
		
		if(is_tai_servo_)
		{
				Copy_Data_To_Show();
		}
		else
		{
				if(menu_combine_fb_work_parm())
				{
						if(!is_need_update())
						{
								put_chars(1, 0, ">Success");
						}
						else
						{
								put_chars(1, 0, ">Pls Update LCD");
								while(1)
								{
									rt_thread_delay(RT_TICK_PER_SECOND * 2);
								}
						}
				}
				else{
						put_chars(1, 0, ">Pls Read Erro!");
						while(1)
						{
							rt_thread_delay(RT_TICK_PER_SECOND * 2);
						}
				}
		}
		
		rt_thread_delay(RT_TICK_PER_SECOND * 2);
		SetMainPage(&Setting_Page);
		ShowPage_Num(pPage, 0);
		
		
//    tid1 = rt_thread_create("reconnect", reconnect_taiwan_servo, RT_NULL, 512, 15, 5);
//    if(tid1 != RT_NULL) rt_thread_startup(tid1);

    tid1 = rt_thread_create("key_scan", key_scan_thread, RT_NULL, 512, 19, 5);
    if(tid1 != RT_NULL) rt_thread_startup(tid1);

    tid1 = rt_thread_create("menu_process", menu_process_thread, RT_NULL, 1024, 20, 5);
    if(tid1 != RT_NULL) rt_thread_startup(tid1);

    tid1 = rt_thread_create("usb_usart", usb_usart_thread, RT_NULL, 2048, 14, 10);
    if(tid1 != RT_NULL) rt_thread_startup(tid1);

    tid1 = rt_thread_create("usart_sw", usart_sw_thread_entry, RT_NULL, 1024, 16, 10);
    if(tid1 != RT_NULL) rt_thread_startup(tid1);
}


/*----------------------------初始化线程--------------------------*/
int rt_application_init(void)
{
		/*按键消息队列*/
    rt_mq_init(&key_mq, "key_mqt", &key_msg_pool[0], 2, sizeof(key_msg_pool), RT_IPC_FLAG_FIFO);
		
		/*串口占用互斥量*/
    servo_mutex = rt_mutex_create("servo_mutex", RT_IPC_FLAG_FIFO);
		lcd_mutex = rt_mutex_create("lcd_mutex", RT_IPC_FLAG_FIFO);

    rt_thread_t tid1 = RT_NULL;
	
		tid1 = rt_thread_create("running", running, RT_NULL, 512, 10, 100);
    if(tid1 != RT_NULL) rt_thread_startup(tid1);

    return 0;
}

/*@}*/
