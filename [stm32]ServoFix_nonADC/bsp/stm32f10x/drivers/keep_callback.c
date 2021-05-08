/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */
#include "keep_callback.h"
#include "tai_pwm.h"
#include "servo_serial.h"
#include "key.h"
#include "menu.h"
#include "menu_app.h"
#include "factory_data.h"

#define BLINK_TIME 	10

extern struct rt_messagequeue key_mq;

void keep_string(uint8_t state)
{
    if(state == S_FAILED)
    {
        LCD_Write_Str(1, 0, (char*)">Failed!");
    }
    else if(state == S_SUCCESS)
    {
        LCD_Write_Str(1, 0, (char*)">Success!");
    }
    else if(state == S_CONFORM)
    {
        LCD_Write_Str(0, 0, (char*)">Confirm?");
    }
    else
    {
        return;
    }
}

uint8_t keep(uint8_t state)
{
    uint8_t rec_buff[2];
    uint8_t i;
    uint16_t time = 0;
    uint8_t sh = 0;

    keep_string(state);

    //防止执行过程中按键误按触发
    for(i = 0; i < 5; i++)
    {
        rt_mq_recv(&key_mq, &rec_buff, 2, RT_WAITING_NO);
    }

    //等待用户操作
    while(1)
    {
        if(rt_mq_recv(&key_mq, &rec_buff, 2, RT_WAITING_NO) == RT_EOK)
        {
            if(rec_buff[0] == 2)
            {
                return F_RETURN; //return
            }
            else if(rec_buff[0] == 3)
            {
                return F_OK; //ok
            }
        }

        rt_thread_delay(RT_TICK_PER_SECOND / 10);
        time ++;

        if(time >= BLINK_TIME)
        {
            time = 0;
        }

        if(time < BLINK_TIME / 2)
        {
            if(sh == 1)
            {
                keep_string(state);
                sh = 0;
            }
        }
        else
        {
            if(sh == 0)
            {
                LCD_Write_Str(1, 0, (char*)"         ");
                sh = 1;
            }
        }
    }
}

//-------------------------end-------------------------------
