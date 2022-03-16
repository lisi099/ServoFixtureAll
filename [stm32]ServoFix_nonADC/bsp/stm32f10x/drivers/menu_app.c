/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#include "menu_app.h"
#include <rtthread.h>
#include "usart1.h"
#include "usart2.h"
#include "servo_serial.h"
#include "string.h"
#include "factory_data.h"
#include "tai_servo.h"
#include "tai_pwm.h"
#include "servo_test_callback.h"
#include "default_callback.h"
#include "keep_callback.h"
#include "memory_callback.h"
#include "servo_rw_callback.h"

#define SIZE_OF_ITEM(x) (sizeof(x) / sizeof(struct Item))
	
extern struct rt_messagequeue key_mq;
extern volatile uint8_t is_tai_servo_;
extern void copy_read_data(void);
extern struct Servo_Data_Stru_ servoDataStru;

struct Servo_Data_Stru_ servo_load_data;
volatile uint16_t current_servo_version_ = 0;
uint8_t Item_Num_[2] = {0, 0};
static int8_t l_num = 0, r_num = 0;

//--0
extern struct PAGE* pPage;
//--1
extern struct PAGE mainPage;
//--2
extern struct PAGE Setting_Page;
extern struct PAGE Position_Page;
extern struct PAGE Info_Page;
extern struct PAGE Lcd_Page;
extern struct PAGE Set_Factory_Page;
//--3
extern struct PAGE Servo_Center_Page;
extern struct PAGE Data_Save_Page;
extern struct PAGE Data_Read_Page;
extern struct PAGE Servo_Bd_Set_Page;
extern struct PAGE Host_Bd_Set_Page;
extern struct PAGE Reset_Data_Read_Page;
extern struct PAGE Lcd_Version_Page;
extern struct PAGE Broadband_servo_Page;
extern struct PAGE Narrowband_servo_Page;
extern struct PAGE Lcd_Upgrade_Page;
//--4
extern struct PAGE Servo_Write_Memory_Page;
extern struct PAGE Servo_Read_Memory_Page;

extern struct PAGE Pc_Page;
/************************************************1******************************************************/
//----1
struct Item Main_item[] =
{
    (char*)"SETTING",       		&Setting_Page,  	0, 0,   0, SHOW_NULL, 0, 0,
    (char*)"SERVO TEST",			&Position_Page,     0, 0,   0, SHOW_NULL, 0, 0,
    (char*)"INFORMATION",			&Lcd_Version_Page,     	0, 0,   0, SHOW_NULL, 0, 0,
};
struct PAGE mainPage = {0, Menu_One_CallBack, Main_item, SIZE_OF_ITEM(Main_item)};
/************************************************2*********************************************************/
//----2
struct Item Setting_item[] =
{
    (char*)" ",									0,  0,  1,   1, SHOW_STRING_VER, 0, 65535,
    (char*)"1.MP",						0,  0,  14,  1, SHOW_NUM, 1, 10,
    (char*)"2.BT",							0,  0,  14,  1, SHOW_NUM, 1, 10,
    (char*)"3.DB",						0,  0,  14,  1, SHOW_NUM, 1, 10,
    (char*)"4.T",							0,  0,  14,  1, SHOW_NUM, 1, 3,
    (char*)"5.F",							0,  0,  14,  1, SHOW_NUM, 1, 10,
    (char*)"6.B",							0,  0,  14,  1, SHOW_NUM, 1, 10,
    (char*)"7.CENTER",				&Servo_Center_Page,  0,  14,  1, SHOW_NULL, 1, 10,
    (char*)"8.SOFT START",						0,  0,  14,  1, SHOW_STRING, 0, 1,
    (char*)"9.WRITE DATA",			&Data_Save_Page,  0,  0,  3, SHOW_NULL, 0, 0,
    (char*)"10.READ DATA",			&Data_Read_Page,  0,  0,  3, SHOW_NULL, 0, 0,
    (char*)"11.DEFAULT",			&Reset_Data_Read_Page,  0,  0,  3, SHOW_NULL, 0, 0,
};

struct PAGE Setting_Page = {&mainPage, Menu_Two_CallBack, Setting_item, SIZE_OF_ITEM(Setting_item)};

//----2
struct Item Position_item[] =
{
    (char*)"BROAD BAND",						&Broadband_servo_Page,  0,  0,  1, SHOW_NULL, 0, 0,
    (char*)"NARROW BAND",						&Narrowband_servo_Page,  0,  0,  1, SHOW_NULL, 0, 0,
};
struct PAGE Position_Page = {&mainPage, Menu_Two_CallBack, Position_item, SIZE_OF_ITEM(Position_item)};

//----2

struct PAGE Pc_Page = {0, Pc_Page_CallBack, 0, 0};
/*******************************************************3**************************************************/
//----3
struct PAGE Servo_Center_Page = {&Setting_Page, Servo_Center_Page_CallBack, 0, 0};
//----3
struct Item Data_Save_item[] =
{
    (char*)"TO SERVO",							0,  0,  0,  1, SHOW_NUM, 0, 0,
    (char*)"To MEMORY",						&Servo_Write_Memory_Page,  0,  0,  1, SHOW_NUM, 0, 0,
};
struct PAGE Data_Save_Page = {&Setting_Page, Menu_Three_CallBack, Data_Save_item, SIZE_OF_ITEM(Data_Save_item)};
//----3
struct Item Data_Read_item[] =
{
    (char*)"FROM SERVO",							0,  0,  0,  1, SHOW_NUM, 0, 0,
    (char*)"FROM MEMORY",							&Servo_Read_Memory_Page,  0,  0,  1, SHOW_NUM, 0, 0,
};
struct PAGE Data_Read_Page = {&Setting_Page, Menu_Three_CallBack, Data_Read_item, SIZE_OF_ITEM(Data_Read_item)};
//----3
struct PAGE Reset_Data_Read_Page = {&Setting_Page, Reset_Data_Read_Page_CallBack, 0, 0};
//----3
struct PAGE Lcd_Version_Page = {&mainPage, Servo_Version_Page_CallBack, 0, 0};
/************************************************4*********************************************************/
struct Item Brodband_item[] =
{
    (char*)"PWM:500us",						&Broadband_servo_Page,  0,  0,  1, SHOW_NUM, 0, 0,
    (char*)"PWM:900us",						&Broadband_servo_Page,  0,  0,  1, SHOW_NUM, 0, 0,
    (char*)"PWM:1500us",					&Broadband_servo_Page,  0,  0,  1, SHOW_NUM, 0, 0,
    (char*)"PWM:2100us",					&Broadband_servo_Page,  0,  0,  1, SHOW_NUM, 0, 0,
    (char*)"PWM:2500us",					&Broadband_servo_Page,  0,  0,  1, SHOW_NUM, 0, 0,
};
struct PAGE Broadband_servo_Page = {&Position_Page, Broadband_Page_CallBack, Brodband_item, SIZE_OF_ITEM(Brodband_item)};
/************************************************4*********************************************************/
struct Item Narrowband_item[] =
{
    (char*)"PWM:500us",						&Narrowband_servo_Page,  0,  0,  1, SHOW_NUM, 0, 0,
    (char*)"PWM:750us",						&Narrowband_servo_Page,  0,  0,  1, SHOW_NUM, 0, 0,
    (char*)"PWM:1000us",					&Narrowband_servo_Page,  0,  0,  1, SHOW_NUM, 0, 0,
};
struct PAGE Narrowband_servo_Page = {&Position_Page, Narrowband_Page_CallBack, Narrowband_item, SIZE_OF_ITEM(Narrowband_item)};
/************************************************4*********************************************************/
//----4
struct PAGE Servo_Write_Memory_Page = {&Data_Save_Page, Servo_Write_Memory_CallBack, 0, 0};
//----4
struct PAGE Servo_Read_Memory_Page = {&Data_Read_Page, Servo_Read_Memory_CallBack, 0, 0};
/*************************************************FUNCTIONS**************************************************************/

/*---------------------------------------------------------------
回调函数
----------------------------------------------------------------*/
void Pc_Page_CallBack(u8 key)
{
    Lcd_Clr_Scr();
    LCD_Write_Str(0, 0, (char*)"<PC MODE>...");
}

void enter_pc_page(void)
{
    pPage = &Pc_Page;
    Pc_Page_CallBack(0);
}

void exit_pc_page(void)
{
    SetMainPage(&Setting_Page);
    ShowPage_Num(pPage, 0);
}
//一级菜单回调
void Menu_One_CallBack(u8 key)
{
    switch(key)
    {
    case KEY_UP:
    case KEY_Down:
    case KEY_UP_L:
    case KEY_Down_L:
        KeySelItem(key);
        Item_Num_[0] = Menu_GetSelItem();
        break;
    case KEY_Return:
        ShowParentPage_Num(0);
        break;
    case KEY_Ok:
        ShowItemPage_Num(0);
        Item_Num_[1] = 0;
        break;
    }
}
//二级菜单回调
void Menu_Two_CallBack(u8 key)
{
    switch(key)
    {
    case KEY_UP:
    case KEY_Down:
    case KEY_UP_L:
    case KEY_Down_L:
        KeySelItem(key);
        Item_Num_[1] = Menu_GetSelItem();
        break;
    case KEY_Return:
        ShowParentPage_Num(Item_Num_[0]);
        break;
    case KEY_Ok:
        if(pPage == &Position_Page)
        {
            if(Menu_GetSelItem() == 0)
            {
                ShowItemPage_Num(2);
            }
            else
            {
                ShowItemPage_Num(1);
            }
        }
        else
        {
            ShowItemPage_Num(0);
        }
        break;
    }
}
//三级菜单回调
void Menu_Three_CallBack(u8 key)
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
        ShowParentPage_Num(Item_Num_[1]);
        break;

    case KEY_Ok:
        if(pPage == &Data_Save_Page && Menu_GetSelItem() == 0){
					servo_write_callback();
        }
        else if(pPage == &Data_Read_Page && Menu_GetSelItem() == 0){
					servo_read_callback();
        }
				else{
					ShowItemPage_Num(0);
				}
    }//end switch
}

void Servo_Version_Page_CallBack(u8 key)
{
    char str[16] = "PGC-LCD-6033";

    switch(key)
    {
    case KEY_Return:
        ShowParentPage_Num(0);
        return;
    case KEY_Ok:
        break;
    }
    Lcd_Clr_Scr();
    put_chars_middle(0, str);
}

extern volatile uint8_t 					pwm_continue_flag;
extern volatile uint16_t 					pwm_count;

void Servo_Center_Page_CallBack(u8 key)
{
    char oper_num[] = "L10 ---0--- R10";
    switch(key)
    {
    case KEY_Down:
        if(l_num == 0 && r_num == 0)
        {
            if(l_num < 10)
            {
                l_num++;
            }
        }
        else if(l_num != 0)
        {
            if(l_num < 10)
            {
                l_num++;
            }
        }
        else if(r_num != 0)
        {
            r_num--;
        }
        break;
    case KEY_UP:
        if(l_num == 0 && r_num == 0)
        {
            if(r_num < 10)
            {
                r_num++;
            }
        }
        else if(r_num != 0)
        {
            if(r_num < 10)
            {
                r_num++;
            }
        }
        else if(l_num != 0)
        {
            l_num--;
        }

        break;
    case KEY_Return:
		pwm_continue_flag = 0;
		rt_thread_delay(200);
        ShowParentPage_Num(Item_Num_[1]);
        return;
	default:
		break;
    }

    //show data
    if(l_num == 0 && r_num == 0)
    {
        oper_num[4] = '-';
        oper_num[10] = '-';
    }
    else if(r_num == 0)
    {
        oper_num[4] = '<';
        oper_num[10] = '-';
    }
    else
    {
        oper_num[4] = '-';
        oper_num[10] = '>';
    }

    //-------
    if(l_num < 10) //1,2
    {
        oper_num[1] = '0';
        oper_num[2] = '0' + l_num;
    }
    else
    {
        oper_num[1] = '1';
        oper_num[2] = '0';
    }

    if(r_num < 10)
    {
        oper_num[13] = '0';
        oper_num[14] = '0' + r_num;
    }
    else
    {
        oper_num[13] = '1';
        oper_num[14] = '0';
    }
	
    if(!is_tai_servo_)
    {
        menu_combine_center(-l_num + r_num);
    }
    else
    {
		int8_t center;
        get_senter(&center);
		pwm_count = 1500 + (-l_num + r_num -center) *15;
		pwm_continue_flag = 1;
//		Copy_Data_To_Stru();
//		menu_combine_prom_work_parm();
//		rt_thread_delay(120);	
//      produce_pwm_count(1500,60);//旋转
		
		rt_thread_delay(200);
    }
    Lcd_Clr_Scr();
    put_chars_middle(0, "Center");
    put_chars_middle(1, oper_num);
}


int16_t round_f(float data)
{
    float data_f;
    int16_t d = (int16_t)data;
    data_f = d + 0.5f;

    if(data > data_f) return d + 1;
    else return d;
}

void Copy_Data_To_Show(void)
{
    if(is_tai_servo_ == 0)
    {
        int16_t center = (servoDataStru.work_p3 - 3100) / 28;
        if(center < 0)
        {
            l_num = -center;
            r_num = 0;

        }
        else if(center > 0)
        {
            r_num = center;
            l_num = 0;
        }
        else
        {
            l_num = 0;
            r_num = 0;
        }
        Setting_item[0].data = servoDataStru.work_p12;
        Setting_item[1].data = round_f((servoDataStru.set_p11 - 727.7f) / 72.2f);
        Setting_item[2].data = round_f((servoDataStru.set_p15 - 4.3f) / 5.6f);
        Setting_item[3].data = servoDataStru.work_p6;
        Setting_item[4].data = servoDataStru.debug_p5;
        Setting_item[5].data = servoDataStru.debug_p0;
        Setting_item[6].data = round_f((servoDataStru.debug_p2 + 3.2f) / 4.3f);
        Setting_item[8].data = servoDataStru.set_p0; //
    }
    else
    {
        int8_t center;
        get_senter(&center);
        if(center < 0)
        {
            l_num = -center;
            r_num = 0;
        }
        else if(center > 0)
        {
            r_num = center;
            l_num = 0;
        }
        else
        {
            l_num = 0;
            r_num = 0;
        }
        int8_t get_value;
        Setting_item[0].data = get_version();

        get_maxpower(&get_value);
        Setting_item[1].data = get_value;//1.max power

        get_boost(&get_value);
        Setting_item[2].data = get_value; //2.Boost

        get_deadband(&get_value);
        Setting_item[3].data = get_value;//3.Dead Band

        get_tension(&get_value);
        Setting_item[4].data = get_value;//4.Tension

        get_force(&get_value);
        Setting_item[5].data = get_value;//5.Force

        get_brake(&get_value);
        Setting_item[6].data = get_value;//6.Brake

        get_soft_start(&get_value);
        Setting_item[8].data = get_value; //8.Soft Start
    }
}

void Copy_Data_To_Stru(void)
{
    if(is_tai_servo_ == 0)
    {
        servoDataStru.work_p3 = 3100 + (-l_num + r_num) * 28;
        servoDataStru.work_p4 = 2048 + (-l_num + r_num) * 28;
        servoDataStru.work_p5 = 996 + (-l_num + r_num) * 28;

        servoDataStru.work_p12 = Setting_item[0].data; //version
        servoDataStru.set_p11 = round_f(Setting_item[1].data * 72.2 + 727.7f); //servo_max_pwm_set
        servoDataStru.set_p15 = round_f(Setting_item[2].data * 5.6f + 4.3f); //servo_work_base_voltage
        servoDataStru.work_p6 = Setting_item[3].data; //servo_zero_zone_set
        servoDataStru.debug_p5 = Setting_item[4].data; //servo_position_pid_parm_p_set
        servoDataStru.debug_p0 = Setting_item[5].data; //servo_speed_pid_parm_p_set
        servoDataStru.debug_p2 = round_f(Setting_item[6].data * 4.3f - 3.2f); //servo_speed_run_sample_k_set
        servoDataStru.set_p0 = Setting_item[8].data;//servo_init_flag_set
    }
    else
    {
        copy_read_data();
        set_maxpower(Setting_item[1].data);
        set_boost(Setting_item[2].data);
        set_deadband(Setting_item[3].data);
        set_tension(Setting_item[4].data);
        set_force(Setting_item[5].data);
        set_brake(Setting_item[6].data);
        set_senter(-l_num + r_num);
        set_soft_start(Setting_item[8].data);
        servoDataStru.work_p12 = current_servo_version_;
    }
}


//------------------------------------------------end-------------------------------------------
