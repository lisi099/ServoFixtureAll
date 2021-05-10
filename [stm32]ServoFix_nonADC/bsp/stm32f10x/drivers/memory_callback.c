/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */
#include "memory_callback.h"
#include "tai_pwm.h"
#include "servo_serial.h"
#include "key.h"
#include "menu.h"
#include "menu_app.h"
#include "factory_data.h"
#include "keep_callback.h"
#include "data_base_flash.h"

extern volatile uint8_t is_tai_servo_;
extern struct Servo_Data_Stru_ servoDataStru;

/***********************************************/
void Servo_Write_Memory_CallBack(u8 key)
{
    static uint8_t num = 0;
    char buf[16] = "  SERVO-DATA-00 ";
    uint16_t data_version;

    struct Servo_Data_Stru_ data;
    uint8_t buf_2[2];
    uint8_t distribtor, costormer;
    uint8_t is_taiwan_distribtor;

    uint8_t data_taiwan[128];
    switch(key)
    {
    case KEY_UP:
        num ++;
        if(num > 19)
        {
            num = 0;
        }
        break;
    case KEY_Down:
        if(num == 0)
        {
            num = 19;
        }
        else
        {
            num --;
        }
        break;

    case KEY_UP_L:
        num ++;
        if(num > 19)
        {
            num = 0;
        }
        break;
    case KEY_Down_L:
        if(num == 0)
        {
            num = 19;
        }
        else
        {
            num --;
        }
        break;
    case KEY_Return:
        ShowParentPage_Num(1);
        return;
    case KEY_Ok:
        Lcd_Clr_Scr();
        Copy_Data_To_Stru();
        if(is_tai_servo_ == 0)
        {
            data_version = servoDataStru.work_p12;
            buf_2[0] = servoDataStru.work_p12 / 10 % 10;
            buf_2[1] = servoDataStru.work_p12 / 1 % 10;
            data_version -= (buf_2[0] * 10 + buf_2[1]);
            data_version += (num + 1);
            LCD_Write_Str(0, 0, (char*)"<Writting>...");
            save_servo_data_in_flash(num + USER_OUT_SPACE, data_version);
        }
        else
        {
            data_version = (num + 1);
            LCD_Write_Str(0, 0, (char*)"<Writting>...");
            save_servo_data_in_flash(num + USER_OUT_SPACE, data_version);
        }
        rt_thread_delay(RT_TICK_PER_SECOND);

        if(keep(S_SUCCESS) == F_RETURN)
        {
            ShowParentPage_Num(1);
        }
        else
        {
            SetMainPage(&Setting_Page);
            ShowPage_Num(pPage, 0);
        }

        return;
    }
    read_servo_data_in_flash_(num, &data);

    read_servo_data_in_flash_taiwan(num, data_taiwan);
    is_taiwan_distribtor = is_version_taiwan(data_taiwan);
    if(is_taiwan_distribtor)
    {
        distribtor = (data_taiwan[96] - '0') * 10 + (data_taiwan[97] - '0');
    }
    else
    {
        buf_2[0] = data.work_p12 / 1000 % 10;
        buf_2[1] = data.work_p12 / 100 % 10;
        distribtor = buf_2[0] * 10 + buf_2[1];
    }

    if(find_version(distribtor) != 100)
    {
        buf_2[0] = data.work_p12  / 10000 % 10 + 1;
        if(is_taiwan_distribtor)
        {
            sprintf(buf, "v1.%d ", data_taiwan[95] - '0' + 1);
        }
        else
        {
            sprintf(buf, "v1.%d ", buf_2[0]);
        }
        sprintf(&buf[5], "%s-", get_ver_char(find_version(distribtor)));

        if(is_taiwan_distribtor)
        {
            costormer = (data_taiwan[98] - '0') * 10 + (data_taiwan[99] - '0');
        }
        else
        {
            buf_2[0] = data.work_p12 / 10 % 10;
            buf_2[1] = data.work_p12 / 1 % 10;
            costormer = buf_2[0] * 10 + buf_2[1];
        }

        if(costormer >= 21)
        {
            sprintf(&buf[13], "00");
        }
        else if(costormer >= 10)
        {
            sprintf(&buf[13], "%d", costormer);
        }
        else
        {
            sprintf(&buf[13], "0%d", costormer);
        }
    }

    Lcd_Clr_Scr();
    if(find_version(distribtor) == 100)
    {
        buf[13] += (num + 1) / 10 % 10;
        buf[14] += (num + 1) / 1 % 10;
    }

    put_chars_middle(0, " Write Memory");
    LCD_Write_Str(1, 0, (char*)buf);
}

/***********************************************/
void Servo_Read_Memory_CallBack(u8 key)
{
    static uint8_t num = 0;
    char buf[16] = "  SERVO-DATA-00 ";
    struct Servo_Data_Stru_ data;
    uint8_t buf_2[2];
    uint8_t distribtor, costormer;

    uint8_t data_taiwan[128];
    uint8_t is_taiwan_distribtor;
    switch(key)
    {
    case KEY_UP:
        num ++;
        if(num > 19)
        {
            num = 0;
        }
        break;
    case KEY_Down:
        if(num == 0)
        {
            num = 19;
        }
        else
        {
            num --;
        }
        break;
    case KEY_UP_L:
        num ++;
        if(num > 19)
        {
            num = 0;
        }
        break;
    case KEY_Down_L:
        if(num == 0)
        {
            num = 19;
        }
        else
        {
            num --;
        }
        break;
    case KEY_Return:
        ShowParentPage_Num(1);
        return;
    case KEY_Ok:
        Lcd_Clr_Scr();
        LCD_Write_Str(0, 0, (char*)"<Reading>...");
        read_servo_data_in_flash(num + USER_OUT_SPACE);
        rt_thread_delay(RT_TICK_PER_SECOND);

        if(keep(S_SUCCESS) == F_RETURN)
        {
            ShowParentPage_Num(1);
        }
        else
        {
            SetMainPage(&Setting_Page);
            ShowPage_Num(pPage, 0);
        }
        return;
    }
    read_servo_data_in_flash_taiwan(num, data_taiwan);
    read_servo_data_in_flash_(num, &data);
    is_taiwan_distribtor = is_version_taiwan(data_taiwan);
    if(is_taiwan_distribtor)
    {
        distribtor = (data_taiwan[96] - '0') * 10 + (data_taiwan[97] - '0');
    }
    else
    {
        buf_2[0] = data.work_p12 / 1000 % 10;
        buf_2[1] = data.work_p12 / 100 % 10;
        distribtor = buf_2[0] * 10 + buf_2[1];
    }

    if(find_version(distribtor) != 100)
    {
        if(is_taiwan_distribtor)
        {
            sprintf(buf, "v1.%d ", data_taiwan[95] - '0' + 1);
        }
        else
        {
            buf_2[0] = data.work_p12  / 10000 % 10 + 1;
            sprintf(buf, "v1.%d ", buf_2[0]);
        }
        sprintf(&buf[5], "%s-", get_ver_char(find_version(distribtor)));

        if(is_taiwan_distribtor)
        {
            costormer = (data_taiwan[98] - '0') * 10 + (data_taiwan[99] - '0');
        }
        else
        {
            buf_2[0] = data.work_p12 / 10 % 10;
            buf_2[1] = data.work_p12 / 1 % 10;
            costormer = buf_2[0] * 10 + buf_2[1];
        }

        if(costormer >= 21)
        {
            sprintf(&buf[13], "00");
        }
        else if(costormer >= 10)
        {
            sprintf(&buf[13], "%d", costormer);
        }
        else
        {
            sprintf(&buf[13], "0%d", costormer);
        }
    }
    else
    {
        buf[13] += (num + 1) / 10 % 10;
        buf[14] += (num + 1) / 1 % 10;
    }

    Lcd_Clr_Scr();
    put_chars_middle(0, " Read Memory");
    LCD_Write_Str(1, 0, (char*)buf);
}



//-------------------------end-------------------------------
