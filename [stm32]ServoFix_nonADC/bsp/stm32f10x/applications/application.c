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


volatile uint8_t servo_detect_flag_ = 0;
volatile uint8_t reconnect_taiwan_servo_flag_ = 0;
rt_mutex_t dynamic_mutex = RT_NULL;
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

volatile uint8_t init_servo_ = 0;
extern volatile uint8_t is_tai_servo_;
extern uint8_t connect_detect(void);
extern void Copy_Data_To_Show(void);
//-----------------------菜单处理-----------------------
void start_page(void)
{
    uint8_t rec_buff[2];
    uint8_t state = 3;

    Lcd_Clr_Scr();
    put_chars_middle(0, "Welcome to");
    put_chars_middle(1, "Power HD");
    rt_thread_delay(RT_TICK_PER_SECOND * 2);
//    put_chars(1, 0, "                ");

    Lcd_Clr_Scr();
    put_chars(0, 0, "Please Connect");
    put_chars(1, 0, "Servo");

    while(servo_detect_flag_ == 0)
    {
        rt_thread_delay(100);

    }
    rt_thread_delay(100);
    while(1)
    {
        if(is_tai_servo_)
        {
            Lcd_Clr_Scr();
            put_chars_middle(0, "SERVO");
            put_chars_middle(1, "Connect OK");
            rt_thread_delay(RT_TICK_PER_SECOND / 2);

            Lcd_Clr_Scr();
            put_chars(0, 0, "<Reading>....");
            Copy_Data_To_Show();
            rt_thread_delay(RT_TICK_PER_SECOND * 2);
            SetMainPage(&Setting_Page);
            ShowPage_Num(pPage, 0);
            init_servo_ = 1;
            break;
        }
        else if(connect_detect())
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
            init_servo_ = 1;
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
#include "pc_cmd.h"
#include "tai_servo.h"

//-----------------------USB与舵机通讯线程----------------------
extern volatile uint8_t connect_servo_state_;
extern void exit_pc_page(void);
static void usb_usart_thread(void* parameter)
{
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
        rt_thread_delay(500);
    }

    servo_detect_flag_ = 1;
    usart1_fifo_rx_init();
    if(!is_tai_servo_)
    {
        usart2_init_rx(19200);
    }
    rt_thread_delay(RT_TICK_PER_SECOND);
    while(1)
    {
        usart1_length_13_data_process();
        process_pc_data();
        process_test_data();
        rt_thread_delay(1);
    }
}


static void servo_detect_thread(void* parameter)
{
    static uint8_t connect_count = 0;
    rt_thread_delay(RT_TICK_PER_SECOND * 5);
    while(1)
    {
        rt_thread_delay(1000);
        if(init_servo_ == 0)
        {
            continue;
        }

        if(is_tai_servo_)
        {
            if(!connect_taiwan())
            {
                connect_count++;
            }
            if(connect_count <= 3)
            {
                continue;
            }
            connect_count = 0;
            if(connect_servo_state_ == 1)
            {
                response_disconnect();
                connect_servo_state_ = 0;
                exit_pc_page();
            }
            reconnect_taiwan_servo_flag_ = 1;
            while(reconnect_taiwan_servo_flag_)
            {
                rt_thread_delay(300);
            }
            Lcd_Clr_Scr();
            put_chars_middle(0, "SERVO");
            put_chars_middle(1, "Connect OK");
            rt_thread_delay(RT_TICK_PER_SECOND / 2);

            Lcd_Clr_Scr();
            put_chars(0, 0, "<Reading>....");
            Copy_Data_To_Show();
            rt_thread_delay(RT_TICK_PER_SECOND * 2);
            SetMainPage(&Setting_Page);
            ShowPage_Num(pPage, 0);
            init_servo_ = 1;

        }// taiwan servo
        else
        {
            if(get_servo_state_count() == 0) //servo out
            {
                if(connect_servo_state_ == 1)
                {
                    response_disconnect();
                    connect_servo_state_ = 0;
                    exit_pc_page();
                }
            }
            else if(get_servo_state_count() == 1) //servo in
            {
                if(init_servo_ == 1)
                {
                    if(connect_servo_state_ == 0)
                    {
                        rt_thread_delay(RT_TICK_PER_SECOND / 2);
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
                    }
                }
            }
        }//end of anzhe servo
    }
}

static void reconnect_taiwan_servo(void* parameter)
{
    rt_thread_delay(RT_TICK_PER_SECOND * 5);
    while(1)
    {
        if(reconnect_taiwan_servo_flag_ == 1)
        {
            if(is_taiwan_servo())
            {
                reconnect_taiwan_servo_flag_ = 0;
            }
        }
        rt_thread_delay(500);
    }
}

//-----------------------串口收发切换线程------------------------
static void usart_sw_thread_entry(void* parameter)
{
    rt_thread_delay(RT_TICK_PER_SECOND);

    while(1)
    {
        rt_mutex_take(dynamic_mutex, RT_WAITING_FOREVER);
        if(Txd2_Flag == 0 && usart2_mode == 0)
        {
            rt_thread_delay(2);
            usart2_init_rx(bd_set_);
        }
        rt_mutex_release(dynamic_mutex);
        rt_thread_delay(2);
    }
}

//------------------------初始化线程-----------------------
int rt_application_init(void)
{

    rt_mq_init(&key_mq, "key_mqt", &key_msg_pool[0], 2, sizeof(key_msg_pool), RT_IPC_FLAG_FIFO);
    rt_mq_init(&usart1_r_mq, "usart1_r_mq", &usart1_r_msg_pool[0], 1, sizeof(usart1_r_msg_pool), RT_IPC_FLAG_FIFO);
    rt_mq_init(&usart2_r_mq, "usart2_r_mq", &usart2_r_msg_pool[0], 1, sizeof(usart2_r_msg_pool), RT_IPC_FLAG_FIFO);

    dynamic_mutex = rt_mutex_create("dmutex", RT_IPC_FLAG_FIFO);

    rt_thread_t tid1 = RT_NULL;

    tid1 = rt_thread_create("reconnect", reconnect_taiwan_servo, RT_NULL, 512, 15, 5);
    if(tid1 != RT_NULL) rt_thread_startup(tid1);

    tid1 = rt_thread_create("key_scan", key_scan_thread, RT_NULL, 512, 19, 5);
    if(tid1 != RT_NULL) rt_thread_startup(tid1);

    tid1 = rt_thread_create("menu_process", menu_process_thread, RT_NULL, 512, 20, 5);
    if(tid1 != RT_NULL) rt_thread_startup(tid1);

    tid1 = rt_thread_create("usb_usart", usb_usart_thread, RT_NULL, 2048, 14, 10);
    if(tid1 != RT_NULL) rt_thread_startup(tid1);

    tid1 = rt_thread_create("usart_sw", usart_sw_thread_entry, RT_NULL, 1024, 16, 10);
    if(tid1 != RT_NULL) rt_thread_startup(tid1);

    tid1 = rt_thread_create("servo_detect", servo_detect_thread, RT_NULL, 1024, 21, 3);
    if(tid1 != RT_NULL) rt_thread_startup(tid1);

    return 0;
}

/*@}*/
