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

//----------------------消息定义-------------------------
struct rt_messagequeue key_mq;
char key_msg_pool[10];

struct rt_messagequeue usart1_r_mq;
static char usart1_r_msg_pool[60];

struct rt_messagequeue usart2_r_mq;
static char usart2_r_msg_pool[60];

//-----------------------按键处理线程-----------------------
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

//-----------------------菜单处理-----------------------
void start_page(void)
{
    uint8_t rec_buff[2];
    uint8_t state = 3;

    Lcd_Clr_Scr();
    put_chars_middle(0, "Welcome to");
    put_chars_middle(1, "Power HD");
    rt_thread_delay(RT_TICK_PER_SECOND * 2);
    put_chars(1, 0, "                ");

    while(1)
    {
        if(get_servo_state())
        {
            Lcd_Clr_Scr();
            put_chars_middle(0, "SERVO");
            put_chars_middle(1, "Connect OK");
            rt_thread_delay(RT_TICK_PER_SECOND / 2);

            Lcd_Clr_Scr();
            put_chars(0, 0, "<Reading>....");

            if(menu_combine_fb_work_parm())
            {
                if(!is_need_update())
                {
                    put_chars(1, 0, ">Success");
                }
                else
                {
                    put_chars(1, 0, ">Pls Update LCD");
                    rt_thread_delay(RT_TICK_PER_SECOND * 2);
                }
            }
            else
            {
                put_chars(1, 0, ">Failed");
                rt_thread_delay(RT_TICK_PER_SECOND);
                continue;
            }

            rt_thread_delay(RT_TICK_PER_SECOND * 2);
            SetMainPage(&Setting_Page);
            ShowPage_Num(pPage, 0);
            break;
        }
        else
        {
            if(state != 0)
            {
                state = 0;
                Lcd_Clr_Scr();
                put_chars(0, 0, "Please Connect");
                put_chars(1, 0, "Servo");
            }

            if(rt_mq_recv(&key_mq, &rec_buff, 2, RT_WAITING_NO) == RT_EOK)
            {
                if(rec_buff[0] == 2)
                {
                    SetMainPage(&mainPage);
                    ShowPage_Num(pPage, 0);
                    break;
                }
            }
        }

        rt_thread_delay(RT_TICK_PER_SECOND / 2);
    }
}

static void menu_process_thread(void* parameter)
{
    uint8_t rec_buff[2];
    uint8_t tempKey = KEY_NONE;
    adc_configration();
    lcd_init();

    start_page();

    while(1)
    {
        tempKey = KEY_NONE;

        if(rt_mq_recv(&key_mq, &rec_buff, 2, RT_WAITING_NO) == RT_EOK)
        {
            switch(rec_buff[0])
            {
                case 0:
                    if(rec_buff[1] == 0)
                    {
                        tempKey = KEY_UP;
                    }
                    else
                    {
                        tempKey = KEY_UP_L;
                    }

                    break;

                case 1:
                    if(rec_buff[1] == 0)
                    {
                        tempKey = KEY_Down;
                    }
                    else
                    {
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

            if(tempKey != KEY_NONE)
            {
                pPage->Function(tempKey);
            }
        }

        rt_thread_delay(RT_TICK_PER_SECOND / 50);
    }
}

//-----------------------USB与舵机通讯线程----------------------
static void usb_usart_thread(void* parameter)
{
    uint8_t data;
    uint8_t data_send[12];
    uint8_t size;

    usart1_fifo_rx_init();
    usart1_init(19200);
    usart2_init_rx(19200);
    rt_thread_delay(RT_TICK_PER_SECOND);

    while(1)
    {
        usart1_length_13_data_process();
        //
        size = 0;

        while(1)
        {
            if(rt_mq_recv(&usart1_r_mq, &data, 1, RT_WAITING_NO) == RT_EOK)
            {
                data_send[size] = data;
                size++;

                if(size == 12)
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }

        if(size > 0)
        {
            while(1)
            {
                if(Txd2_Flag == 0)
                {
                    break;
                }

                rt_thread_delay(1);
            }

            usart2_send_buff(data_send, size);
        }

        //
        size = 0;

        while(1)
        {
            if(rt_mq_recv(&usart2_r_mq, &data, 1, RT_WAITING_NO) == RT_EOK)
            {
                data_send[size] = data;
                size++;

                if(size == 12)
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }

        if(size > 0)
        {
            while(1)
            {
                if(usart1_get_tx_flag() == 0)
                {
                    break;
                }

                rt_thread_delay(1);
            }

            usart1_send_buff(data_send, size);
        }

        rt_thread_delay(1);
    }
}

//-----------------------串口收发切换线程------------------------
static void usart_sw_thread_entry(void* parameter)
{
    rt_thread_delay(RT_TICK_PER_SECOND);

    while(1)
    {
        if(Txd2_Flag == 0 && usart2_mode == 0)
        {
            rt_thread_delay(2);
            usart2_init_rx(bd_set_);
        }

        rt_thread_delay(1);
    }
}

//------------------------初始化线程-----------------------
int rt_application_init(void)
{
    rt_mq_init(&key_mq, "key_mqt", &key_msg_pool[0], 2, sizeof(key_msg_pool), RT_IPC_FLAG_FIFO);
    rt_mq_init(&usart1_r_mq, "usart1_r_mq", &usart1_r_msg_pool[0], 1, sizeof(usart1_r_msg_pool), RT_IPC_FLAG_FIFO);
    rt_mq_init(&usart2_r_mq, "usart2_r_mq", &usart2_r_msg_pool[0], 1, sizeof(usart2_r_msg_pool), RT_IPC_FLAG_FIFO);

    rt_thread_t tid1 = RT_NULL;

    tid1 = rt_thread_create("key_scan", key_scan_thread, RT_NULL, 512, 19, 5);

    if(tid1 != RT_NULL) rt_thread_startup(tid1);

    tid1 = rt_thread_create("menu_process", menu_process_thread, RT_NULL, 512, 20, 5);

    if(tid1 != RT_NULL) rt_thread_startup(tid1);

    tid1 = rt_thread_create("usb_usart", usb_usart_thread, RT_NULL, 2048, 17, 10);

    if(tid1 != RT_NULL) rt_thread_startup(tid1);

    tid1 = rt_thread_create("usart_sw", usart_sw_thread_entry, RT_NULL, 1024, 16, 10);

    if(tid1 != RT_NULL) rt_thread_startup(tid1);

    return 0;
}

/*@}*/
