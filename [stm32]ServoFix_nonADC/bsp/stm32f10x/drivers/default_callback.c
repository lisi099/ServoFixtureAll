/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */
#include "default_callback.h"
#include "tai_pwm.h"
#include "servo_serial.h"
#include "key.h"
#include "menu.h"
#include "menu_app.h"
#include "factory_data.h"

extern volatile uint8_t is_tai_servo_;
extern uint8_t Item_Num_[2];
extern uint8_t keep(uint8_t state);
extern volatile uint16_t current_servo_version_;

void Reset_Data_Read_Page_CallBack(u8 key)
{
    static uint8_t num = 0;
    static uint8_t fisrt = 0;
    int total_num  = get_total_num() - 1;
    char buf_title[17];
    uint8_t ch_value;

    uint16_t distribtor;
    uint8_t buf_2[2];
    buf_2[0] = current_servo_version_ / 1000 % 10;
    buf_2[1] = current_servo_version_ / 100 % 10;
    distribtor = buf_2[0] * 10 + buf_2[1];

    if(fisrt == 0)
    {
        Lcd_Clr_Scr();

        if(keep(S_CONFORM) == F_RETURN)
        {
            ShowParentPage_Num(Item_Num_[1]);
            fisrt = 0;
            return;
        }
    }

    fisrt = 1;
    Lcd_Clr_Scr();

    if(find_version(distribtor) != 100)
    {
        num = find_version(distribtor);
        ch_value = 1;
    }
    else
    {
        ch_value = 0;
    }

    switch(key)
    {
    case KEY_UP:
        if(ch_value == 0)
        {
            num ++;
            if(num > total_num)
            {
                num = 0;
            }
        }

        break;

    case KEY_Down:
        if(ch_value == 0)
        {
            num --;

            if(num > total_num)
            {
                num = total_num;
            }
        }

        break;

    case KEY_UP_L:
        if(ch_value == 0)
        {
            num ++;

            if(num > total_num)
            {
                num = 0;
            }
        }

        break;

    case KEY_Down_L:
        if(ch_value == 0)
        {
            num --;

            if(num > total_num)
            {
                num = total_num;
            }
        }

        break;

    case KEY_Return:
        ShowParentPage_Num(Item_Num_[1]);
        fisrt = 0;
        return;

    case KEY_Ok:

        if(get_ver_num(num) != distribtor)
        {
//			if(0){
            Lcd_Clr_Scr();
            LCD_Write_Str(0, 0, (char*)"<Restoring>...");
            keep(S_FAILED);
            break;
        }
        else
        {
//		num = 1;
            Lcd_Clr_Scr();
            LCD_Write_Str(0, 0, (char*)"<Restoring>...");
            read_servo_data_in_flash(num + 20);

            menu_combine_prom_work_parm();
            if(menu_combine_verify_work_parm())
            {
                if(keep(S_SUCCESS) == F_RETURN)
                {
                    ShowParentPage_Num(0);
                }
                else
                {
                    ShowParentPage_Num(0);
                }
            }
            else
            {
                keep(S_FAILED);
                ShowParentPage_Num(0);
            }

            fisrt = 0;
            return;
        }
    }

    Lcd_Clr_Scr();
    char* buf = get_ver_char(num);
    sprintf(buf_title, "Factory Reset");
    LCD_Write_Str(0, 0, buf_title);
    LCD_Write_Str(1, 0, buf);
}


//-------------------------end-------------------------------
