/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */
#include "servo_test_callback.h"
#include "tai_pwm.h"
#include "servo_serial.h"
#include "key.h"
#include "Menu.h"

extern volatile uint8_t is_tai_servo_;

void Broadband_Page_CallBack(u8 key)
{
    switch(key)
    {
    case KEY_UP:
    case KEY_Down:
    case KEY_UP_L:
    case KEY_Down_L:
        KeySelItem(key);
        break;
    case KEY_Return:
        ShowParentPage_Num(0);
        return;
    case KEY_Ok:
        break;
    }

    switch(Menu_GetSelItem())
    {
    case 0:
        if(!is_tai_servo_)
        {
            menu_combine_position(500);
        }
        else
        {
            produce_pwm_count(500,10);
        }
        break;

    case 1:
        if(!is_tai_servo_)
        {
            menu_combine_position(900);
        }
        else
        {
            produce_pwm_count(900,10);
        }
        break;

    case 2:
        if(!is_tai_servo_)
        {
            menu_combine_position(1500);
        }
        else
        {
            produce_pwm_count(1500,10);
        }
        break;

    case 3:
        if(!is_tai_servo_)
        {
            menu_combine_position(2100);
        }
        else
        {
            produce_pwm_count(2100,10);
        }
        break;

    case 4:
        if(!is_tai_servo_)
        {
            menu_combine_position(2500);
        }
        else
        {
            produce_pwm_count(2500,10);
        }
        break;

    default:
        break;
    }
}

void Narrowband_Page_CallBack(u8 key)
{
    switch(key)
    {
    case KEY_UP:
    case KEY_Down:
    case KEY_UP_L:
    case KEY_Down_L:
        KeySelItem(key);
        break;

    case KEY_Return:
        ShowParentPage_Num(0);
        return;

    case KEY_Ok:
        break;
    }

    switch(Menu_GetSelItem())
    {
    case 0:
        if(!is_tai_servo_)
        {
            menu_combine_position(500);
        }
        else
        {
            produce_pwm(500);
        }
        break;

    case 1:
        if(!is_tai_servo_)
        {
            menu_combine_position(750);
        }
        else
        {
            produce_pwm(750);
        }
        break;

    case 2:
        if(!is_tai_servo_)
        {
            menu_combine_position(1000);
        }
        else
        {
            produce_pwm(1000);
        }
        break;

    default:
        break;
    }
}


//-------------------------end-------------------------------
