/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */
#include "servo_rw_callback.h"
#include "tai_pwm.h"
#include "servo_serial.h"
#include "key.h"
#include "menu.h"
#include "menu_app.h"
#include "factory_data.h"
#include "keep_callback.h"

extern volatile uint8_t is_tai_servo_;
extern volatile uint16_t current_servo_version_;
extern struct Servo_Data_Stru_ servoDataStru;
extern uint8_t Item_Num_[2];
extern struct PAGE Data_Save_Page;
extern struct PAGE Data_Read_Page;

uint16_t get_thousands(uint16_t data)
{
    return (data / 1000 % 10);
}

uint16_t get_hundreds(uint16_t data)
{
    return (data / 100 % 10);
}

uint8_t servo_write_callback(void)
{
    uint16_t distribtor;
    uint8_t result = 0;

    if(is_tai_servo_)
    {
        distribtor = get_thousands(current_servo_version_) * 10 + get_hundreds(current_servo_version_);
    }
    else
    {
        distribtor = get_thousands(servoDataStru.work_p12) * 10 + get_hundreds(servoDataStru.work_p12);
    }

    if(find_version(distribtor) == 100)
    {
        Lcd_Clr_Scr();
        LCD_Write_Str(0, 0, (char*)"<Writting>...");
        keep(S_FAILED);
        SetMainPage(&Data_Save_Page);
        ShowPage_Num(pPage, 0);
    }
    else
    {
        Lcd_Clr_Scr();
        LCD_Write_Str(0, 0, (char*)"<Writting>...");
        Copy_Data_To_Stru();
        menu_combine_prom_work_parm();
        res = menu_combine_verify_work_parm();
        if(result)
        {
            if(keep(S_SUCCESS) == F_RETURN)
            {
                ShowParentPage_Num(Item_Num_[1]);
            }
            else
            {
                SetMainPage(&Setting_Page);
                ShowPage_Num(pPage, 0);
            }
        }
        else
        {
            keep(S_FAILED);
            SetMainPage(&Data_Save_Page);
            ShowPage_Num(pPage, 0);
        }
    }
    return result;
}

uint8_t servo_read_callback(void)
{
    uint8_t result = 0;

    Lcd_Clr_Scr();
    LCD_Write_Str(0, 0, (char*)"<Reading>...");

    if(menu_combine_fb_work_parm())
    {
        if(keep(S_SUCCESS) == F_RETURN)
        {
            ShowParentPage_Num(Item_Num_[1]);
        }
        else
        {
            SetMainPage(&Setting_Page);
            ShowPage_Num(pPage, 0);
        }
    }
    else
    {
        keep(S_FAILED);
        SetMainPage(&Data_Read_Page);
        ShowPage_Num(pPage, 0);
    }
    return result;
}

//-------------------------end-------------------------------
