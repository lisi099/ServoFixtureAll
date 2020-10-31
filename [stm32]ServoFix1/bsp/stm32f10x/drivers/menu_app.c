/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#include "menu_app.h"
#include <rtthread.h>
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "servo_serial.h"
#include "string.h"
#include "factory_data.h"

#define BLINK_TIME 	10
#define SIZE_OF_ITEM(x) (sizeof(x) / sizeof(struct Item))
extern struct rt_messagequeue key_mq;

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
extern struct PAGE Servo_Version_Page1;
extern struct PAGE Broadband_servo_Page;
extern struct PAGE Narrowband_servo_Page;
extern struct PAGE Lcd_Upgrade_Page;
//--4
extern struct PAGE Servo_Write_Memory_Page;
extern struct PAGE Servo_Read_Memory_Page;

extern struct Servo_Data_Stru_ servoDataStru;
struct Servo_Data_Stru_ servo_load_data;

uint8_t Item_Num_[2] = {0, 0};
extern Usart_State usart_state;

/************************************************1******************************************************/
//----1
struct Item Main_item[] =
{
    (char*)"Settings",       		&Setting_Page,  	0, 0,   0, SHOW_NULL, 0, 0,
    (char*)"Servo Test",			&Position_Page,     0, 0,   0, SHOW_NULL, 0, 0,
    (char*)"Information",			&Info_Page,     	0, 0,   0, SHOW_NULL, 0, 0,
};
struct PAGE mainPage = {0, Menu_One_CallBack, Main_item, SIZE_OF_ITEM(Main_item)};
/************************************************2*********************************************************/
//----2
struct Item Setting_item[] =
{
    (char*)" ",									0,  0,  1,   1, SHOW_STRING_VER, 0, 65535,
    (char*)"2.Max Power",						0,  0,  14,  1, SHOW_NUM, 1, 10,
    (char*)"3.Boost",							0,  0,  14,  1, SHOW_NUM, 1, 10,
    (char*)"4.Dead Band",						0,  0,  14,  1, SHOW_NUM, 1, 10,
    (char*)"5.Force",							0,  0,  14,  1, SHOW_NUM, 1, 10,
    (char*)"6.Tension",							0,  0,  14,  1, SHOW_NUM, 1, 3,
    (char*)"7.Brake",							0,  0,  14,  1, SHOW_NUM, 1, 10,
    (char*)"8.Center        L1",	&Servo_Center_Page,  0,  14,  1, SHOW_NULL, 1, 10,
    (char*)"9.Soft Start",						0,  0,  14,  1, SHOW_STRING, 0, 1,
    (char*)"10.Write Data",			&Data_Save_Page,  0,  0,  3, SHOW_NULL, 0, 0,
    (char*)"11.Read Data",			&Data_Read_Page,  0,  0,  3, SHOW_NULL, 0, 0,
    (char*)"12.Default",			&Reset_Data_Read_Page,  0,  0,  3, SHOW_NULL, 0, 0,
};

struct PAGE Setting_Page = {&mainPage, Menu_Two_CallBack, Setting_item, SIZE_OF_ITEM(Setting_item)};

//----2
struct Item Position_item[] =
{
    (char*)"Broad Band",						&Broadband_servo_Page,  0,  0,  1, SHOW_NULL, 0, 0,
    (char*)"Narrow Band",						&Narrowband_servo_Page,  0,  0,  1, SHOW_NULL, 0, 0,
};
struct PAGE Position_Page = {&mainPage, Menu_Two_CallBack, Position_item, SIZE_OF_ITEM(Position_item)};

//----2
struct Item Info_item[] =
{
    (char*)"Servo Version",			&Servo_Version_Page1,  0,  0,  1, SHOW_NULL, 0, 0,
    (char*)"Lcd Settings",        	&Lcd_Page,  		0, 0,   0, SHOW_NULL, 0, 0,
//	(char*)"DATA FACTOCY SET",      			&Set_Factory_Page,  0, 0,   0, SHOW_NULL,0,0,
};
struct PAGE Info_Page = {&mainPage, Menu_Two_CallBack, Info_item, SIZE_OF_ITEM(Info_item)};

//----2
struct Item Lcd_item[] =
{
    (char*)"Servo Bd Set",						&Servo_Bd_Set_Page,  0,  0,  1, SHOW_NUM, 0, 0,
    (char*)"Host Bd Set",							&Host_Bd_Set_Page,   0,  0,  1, SHOW_NUM, 0, 0,
    (char*)"Lcd Upgrade",							&Lcd_Upgrade_Page,  0,  0,  1, SHOW_NUM, 0, 0,
};
struct PAGE Lcd_Page = {&Info_Page, Menu_Three_CallBack, Lcd_item, SIZE_OF_ITEM(Lcd_item)};

//struct PAGE Set_Factory_Page = {&Info_Page, Servo_Set_Factory_CallBack, 0, 0};
/*******************************************************3**************************************************/
//----3
struct PAGE Servo_Center_Page = {&Setting_Page, Servo_Center_Page_CallBack, 0, 0};
//----3
struct Item Data_Save_item[] =
{
    (char*)"To Servo",							0,  0,  0,  1, SHOW_NUM, 0, 0,
    (char*)"To Memory",						&Servo_Write_Memory_Page,  0,  0,  1, SHOW_NUM, 0, 0,
};
struct PAGE Data_Save_Page = {&Setting_Page, Menu_Three_CallBack, Data_Save_item, SIZE_OF_ITEM(Data_Save_item)};
//----3
struct Item Data_Read_item[] =
{
    (char*)"From Servo",							0,  0,  0,  1, SHOW_NUM, 0, 0,
    (char*)"From Memory",							&Servo_Read_Memory_Page,  0,  0,  1, SHOW_NUM, 0, 0,
};
struct PAGE Data_Read_Page = {&Setting_Page, Menu_Three_CallBack, Data_Read_item, SIZE_OF_ITEM(Data_Read_item)};
//----3
struct PAGE Reset_Data_Read_Page = {&Setting_Page, Reset_Data_Read_Page_CallBack, 0, 0};
//----3
struct Item Servo_Bd_Set_item[] =
{
    (char*)"19200",							0,  0,  0,  1, SHOW_NUM, 0, 0,
    (char*)"9600",							0,  0,  0,  1, SHOW_NUM, 0, 0,
    (char*)"38400",							0,  0,  0,  1, SHOW_NUM, 0, 0,
    (char*)"115200",						0,  0,  0,  1, SHOW_NUM, 0, 0,
};
struct PAGE Servo_Bd_Set_Page = {&Lcd_Page, Servo_Bd_Set_CallBack, Servo_Bd_Set_item, SIZE_OF_ITEM(Servo_Bd_Set_item)};
//----3
struct Item Host_Bd_Set_item[] =
{
    (char*)"19200",							0,  0,  0,  1, SHOW_NUM, 0, 0,
    (char*)"9600",							0,  0,  0,  1, SHOW_NUM, 0, 0,
    (char*)"38400",							0,  0,  0,  1, SHOW_NUM, 0, 0,
    (char*)"115200",						0,  0,  0,  1, SHOW_NUM, 0, 0,
};
struct PAGE Host_Bd_Set_Page = {&Lcd_Page, Lcd_Bd_Set_CallBack, Host_Bd_Set_item, SIZE_OF_ITEM(Host_Bd_Set_item)};

struct PAGE Lcd_Upgrade_Page = {&Lcd_Page, Lcd_Upgrade_CallBack, 0, 0};
//----3
struct PAGE Servo_Version_Page1 = {&Info_Page, Servo_Version_Page_CallBack, 0, 0};


struct Item Brodband_item[] =
{
    (char*)"PWM:500us",						&Broadband_servo_Page,  0,  0,  1, SHOW_NUM, 0, 0,
    (char*)"PWM:900us",						&Broadband_servo_Page,  0,  0,  1, SHOW_NUM, 0, 0,
    (char*)"PWM:1500us",					&Broadband_servo_Page,  0,  0,  1, SHOW_NUM, 0, 0,
    (char*)"PWM:2100us",					&Broadband_servo_Page,  0,  0,  1, SHOW_NUM, 0, 0,
    (char*)"PWM:2500us",					&Broadband_servo_Page,  0,  0,  1, SHOW_NUM, 0, 0,
};
struct PAGE Broadband_servo_Page = {&Position_Page, Narrowband_Page_CallBack, Brodband_item, SIZE_OF_ITEM(Brodband_item)};

struct Item Narrowband_item[] =
{
    (char*)"PWM:500us",						&Narrowband_servo_Page,  0,  0,  1, SHOW_NUM, 0, 0,
    (char*)"PWM:750us",						&Narrowband_servo_Page,  0,  0,  1, SHOW_NUM, 0, 0,
    (char*)"PWM:1000us",					&Narrowband_servo_Page,  0,  0,  1, SHOW_NUM, 0, 0,
};
struct PAGE Narrowband_servo_Page = {&Position_Page, Broadband_Page_CallBack, Narrowband_item, SIZE_OF_ITEM(Narrowband_item)};
/************************************************4*********************************************************/
//----4
struct PAGE Servo_Write_Memory_Page = {&Data_Save_Page, Servo_Write_Memory_CallBack, 0, 0};
//----4
struct PAGE Servo_Read_Memory_Page = {&Data_Read_Page, Servo_Read_Memory_CallBack, 0, 0};
/*************************************************FUNCTIONS**************************************************************/
// struct Servo_Data_Stru_  para_01= {2200,1500,800,3448,2048,648,2,0,0,1,0,1,380,1,16,248,1,1500,10,1,1,10,0,100,0,1020,1020,1500,1500,1500,1,0,3,1,8,16,32,1,1};
/*----------------------------------
回调函数
----------------------------------*/
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
//-------------------------------------------------------------------
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
            if(pPage == &Data_Save_Page && Menu_GetSelItem() == 0)
            {
                Lcd_Clr_Scr();
                LCD_Write_Str(0, 0, (char*)"<Writting>...");
                Copy_Data_To_Stru();
                menu_combine_prom_work_parm();

                if(menu_combine_verify_work_parm())
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

                break;
            }
            else if(pPage == &Data_Read_Page && Menu_GetSelItem() == 0)
            {
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

                break;
            }

            //----------------
            ShowItemPage_Num(0);
            break;
    }
}
//-------------------------------------------------------------------
void Lcd_Upgrade_CallBack(u8 key)
{
    Lcd_Clr_Scr();
    put_chars_middle(0, "LCD Upgrade");
    usart_state = USB_SERIAL_PROGRAM;
    usart1_init(115200);

    switch(key)
    {
        case KEY_Return:
            ShowParentPage_Num(2);
            break;
    }
}

void Servo_Bd_Set_CallBack(u8 key)
{
    uint32_t bd_set;

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
            break;

        case KEY_Ok:
            if(Menu_GetSelItem() == 0)
            {
                bd_set = 19200;
            }
            else if(Menu_GetSelItem() == 1)
            {
                bd_set = 9600;
            }
            else if(Menu_GetSelItem() == 2)
            {
                bd_set = 38400;
            }
            else if(Menu_GetSelItem() == 3)
            {
                bd_set = 115200;
            }

            usart2_init_rx(bd_set);
            LCD_Write_Str(0, 0, (char*)"<BAUD SET>...");
            rt_thread_delay(RT_TICK_PER_SECOND);
            keep(S_SUCCESS);
            ShowParentPage_Num(0);
            break;
    }
}

void Lcd_Bd_Set_CallBack(u8 key)
{
    uint32_t bd_set;

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
            break;

        case KEY_Ok:
            if(Menu_GetSelItem() == 0)
            {
                bd_set = 19200;
            }
            else if(Menu_GetSelItem() == 1)
            {
                bd_set = 9600;
            }
            else if(Menu_GetSelItem() == 2)
            {
                bd_set = 38400;
            }
            else if(Menu_GetSelItem() == 3)
            {
                bd_set = 115200;
            }

            usart1_init(bd_set);
            LCD_Write_Str(0, 0, (char*)"<BAUD SET>...");
            rt_thread_delay(RT_TICK_PER_SECOND);
            keep(S_SUCCESS);
            ShowParentPage_Num(1);
            break;
    }
}

void Servo_Write_Memory_CallBack(u8 key)
{
    static uint8_t num = 0;
    char buf[] = "  UDATA_XXX00   ";
    uint16_t data_version;
    uint16_t version;

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
            num --;

            if(num > 19)
            {
                num = 19;
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
            num --;

            if(num > 19)
            {
                num = 19;
            }

            break;

        case KEY_Return:
            ShowParentPage_Num(1);
            return;

        case KEY_Ok:
            Lcd_Clr_Scr();
            version = (uint16_t) servoDataStru.work_p12;
            data_version = version / 100 * 100 + num;

            Copy_Data_To_Stru();
            LCD_Write_Str(0, 0, (char*)"<Writting>...");
            save_servo_data_in_flash(num + USER_OUT_SPACE, data_version);
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

    Lcd_Clr_Scr();
    buf[11] += num / 10 % 10;
    buf[12] += num / 1 % 10;
    put_chars_middle(0, "Write Memory");
    LCD_Write_Str(1, 0, (char*)buf);
}


void Servo_Read_Memory_CallBack(u8 key)
{
    static uint8_t num = 0;
    char buf[] = "  UDATA_XXX00   ";

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
            num --;

            if(num > 19)
            {
                num = 19;
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
            num --;

            if(num > 19)
            {
                num = 19;
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

    Lcd_Clr_Scr();
    buf[11] += num / 10 % 10;
    buf[12] += num / 1 % 10;
    put_chars_middle(0, "Read Memory");
    LCD_Write_Str(1, 0, (char*)buf);
}

void Servo_Version_Page_CallBack(u8 key)
{
	int distribtor, costormer;
    uint8_t buf[2];
	char str[16] = { 0 };
	
	buf[0] = Setting_item[0].data / 10000 % 10 + 1;
	sprintf(str, "V1.%d ", buf[0]);

	buf[0] = Setting_item[0].data / 1000 % 10;
	buf[1] = Setting_item[0].data / 100 % 10;
	distribtor = buf[0] * 10 + buf[1];

	if(find_version(distribtor) == 100)
	{
	sprintf(&str[5], "%s-", "XXX-XXX");
	}
	else
	{
	sprintf(&str[5], "%s-", get_ver_char(find_version(distribtor)));
	}

	buf[0] = Setting_item[0].data / 10 % 10;
	buf[1] = Setting_item[0].data / 1 % 10;
	costormer = buf[0] * 10 + buf[1];

	if(costormer == 21)
	{
		sprintf(&str[13], "00");
	}
	else if(costormer >= 10)
	{
		sprintf(&str[13], "%d", costormer);
	}
	else
	{
		sprintf(&str[13], "0%d", costormer);
	}
	
	switch(key)
    {
        case KEY_UP:
            break;
        case KEY_Down:
            break;
        case KEY_UP_L:
            break;
        case KEY_Down_L:
            break;
        case KEY_Return:
            ShowParentPage_Num(0);
            return;
        case KEY_Ok:
            break;
    }
	Lcd_Clr_Scr();
	put_chars_middle(0, str);
}

void Servo_Center_Page_CallBack(u8 key)
{
    char oper_num[] = "L10 ---0--- R10";
    static uint8_t l_num = 0, r_num = 0;
    Lcd_Clr_Scr();

    switch(key)
    {
        case KEY_UP:
            r_num ++;

            if(r_num > 10)
            {
                r_num = 0;
            }

            l_num = 0;
            break;

        case KEY_Down:
            l_num ++;

            if(l_num > 10)
            {
                l_num = 0;
            }

            r_num = 0;
            break;
    }

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

    put_chars_middle(0, "Center");
    put_chars_middle(1, oper_num);

    switch(key)
    {
        case KEY_UP:
            menu_combine_center(r_num);
            break;

        case KEY_Down:
            menu_combine_center(-l_num);
            break;
    }

    menu_combine_position(1500);

    if(key == KEY_Return)
    {
        ShowParentPage_Num(Item_Num_[1]);
    }
}

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
            menu_combine_position(500);
            break;

        case 1:
            menu_combine_position(900);
            break;

        case 2:
            menu_combine_position(1500);
            break;

        case 3:
            menu_combine_position(2100);
            break;

        case 4:
            menu_combine_position(2500);
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
            menu_combine_position(500);
            break;

        case 1:
            menu_combine_position(750);
            break;

        case 2:
            menu_combine_position(1000);
            break;

        default:
            break;
    }
}

void Reset_Data_Read_Page_CallBack(u8 key)
{
    static uint8_t num = 0;
    static uint8_t fisrt = 0;
    int total_num  = get_total_num() - 1;
    char buf_title[17];

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
    
    switch(key)
    {
        case KEY_UP:
            num ++;

            if(num > total_num)
            {
                num = 0;
            }

            break;

        case KEY_Down:
            num --;

            if(num > total_num)
            {
                num = total_num;
            }

            break;

        case KEY_UP_L:
            num ++;

            if(num > total_num)
            {
                num = 0;
            }

            break;

        case KEY_Down_L:
            num --;

            if(num > total_num)
            {
                num = total_num;
            }

            break;

        case KEY_Return:
            ShowParentPage_Num(Item_Num_[1]);
            fisrt = 0;
            return;

        case KEY_Ok:
            Lcd_Clr_Scr();
            LCD_Write_Str(0, 0, (char*)"<Restoring>...");
            read_servo_data_in_flash(num + 20);
            rt_thread_delay(RT_TICK_PER_SECOND);
            keep(S_SUCCESS);
            ShowParentPage_Num(0);
            fisrt = 0;
            return;
    }

//    buf[9] += num / 10 % 10;
//    buf[10] += num / 1 % 10;
    Lcd_Clr_Scr();
		char *buf = get_ver_char(num);
//    put_chars_middle(0, (char*)"Factory Reset");
//    LCD_Write_Str(1, 0, (char*)buf);
    sprintf(buf_title, "Factory Reset %d", num);
    LCD_Write_Str(0, 0, buf_title);
    LCD_Write_Str(1, 0, buf);
}

void Copy_Data_To_Show(void)
{
    Setting_item[0].data = servoDataStru.work_p12;
//	Setting_item[0].data = 10121;
    Setting_item[1].data = (servoDataStru.set_p11 - 727.7f) / 72.2f;
    Setting_item[2].data = (servoDataStru.set_p15 - 4.3f) / 5.6f;
    Setting_item[3].data = servoDataStru.work_p6;
    Setting_item[4].data = servoDataStru.debug_p0;
    Setting_item[5].data = servoDataStru.debug_p5;
    Setting_item[6].data = (servoDataStru.debug_p2 + 3.2f) / 4.3f;
    Setting_item[7].data = servoDataStru.set_p14; //
}

void Copy_Data_To_Stru(void)
{
    servoDataStru.work_p12 = Setting_item[0].data; //version
    servoDataStru.set_p11 = Setting_item[1].data * 72.2 + 727.7f; //servo_max_pwm_set
    servoDataStru.set_p15 = Setting_item[2].data * 5.6f + 4.3f; //servo_work_base_voltage
    servoDataStru.work_p6 = Setting_item[3].data; //servo_zero_zone_set
    servoDataStru.debug_p0 = Setting_item[4].data; //servo_position_pid_parm_p_set
    servoDataStru.debug_p5 = Setting_item[5].data; //servo_speed_pid_parm_p_set
    servoDataStru.debug_p2 = Setting_item[6].data * 4.3f - 3.2f; //servo_speed_run_sample_k_set
    servoDataStru.set_p14 = Setting_item[7].data;//servo_init_flag_set
}

