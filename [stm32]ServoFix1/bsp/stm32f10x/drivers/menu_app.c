/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#include "menu_app.h"
#include <rtthread.h>
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "servo_serial.h"
#include "string.h"

extern  struct PAGE mainPage;
extern  struct PAGE Setting_Page;
extern  struct PAGE Position_Page;
extern  struct PAGE Info_Page;
extern  struct PAGE Lcd_Page;

void mainPageCallBack(u8 key);
void Setting_CallBack(u8 key);
void Common_CallBack(u8 key);

extern struct PAGE *pPage;
extern struct Servo_Data_Stru_ servoDataStru;
extern Usart_State usart_state;
struct Servo_Data_Stru_ servo_load_data;
uint8_t Item_Num_[2] ={0, 0};
/************************************************MAIN******************************************************/
//----1
 struct Item Main_item[]={
                                    (char*)"1.SERVOSETTING",       	&Setting_Page,  	0, 0,   0, SHOW_BOOL,
									(char*)"2.SENDPOSITION",		&Position_Page,     0, 0,   0, SHOW_BOOL,
									(char*)"3.INFORMATION",			&Info_Page,     	0, 0,   0, SHOW_BOOL,
                                    (char*)"4.LCDSETTINGS",        	&Lcd_Page,  		0, 0,   0, SHOW_BOOL,
};
 struct PAGE mainPage={0, mainPageCallBack, Main_item, sizeof(Main_item)/sizeof(struct Item), DISPLAY_MODE_1_COLUMN};
/************************************************SET*********************************************************/
 //----2
 struct Item Setting_item[]={           (char*)"01.MAXPOW",						0,  0,  0,  1, SHOW_U16,
										(char*)"02.BOOST",						0,  0,  0,  1, SHOW_U16,
                                        (char*)"03.DBAND",						0,  0,  0,  2, SHOW_U16,
										(char*)"04.FORCE",						0,  0,  0,  2, SHOW_U16,	
										(char*)"05.STERETC",					0,  0,  0,  2, SHOW_U16,	
                                        (char*)"06.BRAKE",						0,  0,  0,  3, SHOW_U16,
										(char*)"07.SFTSTA",						0,  0,  0,  3, SHOW_U16,
										(char*)"08.SET DAT SAVE",				0,  0,  0,  3, SHOW_BOOL,
										(char*)"09.SET DAT READ",				0,  0,  0,  3, SHOW_BOOL,
										(char*)"10.RES DAT READ",				0,  0,  0,  3, SHOW_BOOL,
                                    };

 struct PAGE Setting_Page={&mainPage,Setting_CallBack,Setting_item,sizeof(Setting_item)/sizeof(struct Item),DISPLAY_MODE_1_COLUMN};
//----2
 struct Item Position_item[]={          (char*)"1.BROADBANDSERVO",						0,  0,  0,  1, SHOW_U16,
										(char*)"2.NARROWBANDSERVO",						0,  0,  0,  1, SHOW_U16,
                                    };
 struct PAGE Position_Page={&mainPage,Common_CallBack,Position_item,sizeof(Position_item)/sizeof(struct Item),DISPLAY_MODE_1_COLUMN};
//----2
 struct Item Info_item[]={ 				(char*)"1.LCD VER:",						0,  0,  0,  1, SHOW_U16,
										(char*)"2.SERVO VER:",						0,  0,  0,  1, SHOW_U16,
                                    };
struct PAGE Info_Page={&mainPage,Common_CallBack,Info_item,sizeof(Info_item)/sizeof(struct Item),DISPLAY_MODE_1_COLUMN};
//----2
 struct Item Lcd_item[]={ 				(char*)"1.SERVO BDRT SET",						0,  0,  0,  1, SHOW_U16,
										(char*)"2.HOST BDRT SET",						0,  0,  0,  1, SHOW_U16,
										(char*)"3.LCD UPGRADE",							0,  0,  0,  1, SHOW_U16,
                                    };
struct PAGE Lcd_Page={&mainPage,Common_CallBack,Lcd_item,sizeof(Lcd_item)/sizeof(struct Item),DISPLAY_MODE_1_COLUMN};
/************************************************SET*********************************************************/
//----3
 
/*************************************************FUNCTIONS**************************************************************/
 struct Servo_Data_Stru_  para_01= {2200,1500,800,3448,2048,648,2,0,0,1,0,1,380,1,16,248,1,1500,10,1,1,10,0,100,0,1020,1020,1500,1500,1500,1,0,3,1,8,16,32,1,1};
 struct Servo_Data_Stru_  para_02= {2200,1500,800,3448,2048,648,2,0,0,1,0,1,380,1,16,248,1,1500,10,1,1,10,0,100,0,1020,1020,1500,1500,1500,1,0,3,1,8,16,32,1,1};
 struct Servo_Data_Stru_  para_03= {2200,1500,800,3448,2048,648,2,0,0,1,0,1,380,1,16,248,1,1500,10,1,1,10,0,100,0,1020,1020,1500,1500,1500,1,0,3,1,8,16,32,1,1};
/*----------------------------------
回调函数
----------------------------------*/
void mainPageCallBack(u8 key)
{
	switch (key)
	{
		case KEY_UP:	
		case KEY_Down:
		case KEY_UP_L:
		case KEY_Down_L:			
			KeySelItem(key);
			Item_Num_[0] = Menu_GetSelItem();
			break;
		case KEY_Return:
			ShowPage(&mainPage);
			break;
		case KEY_Ok:
            Lcd_Clr_Scr();
            ShowItemPage();
            Item_Num_[1] = 0;
			break;
	}
}
/*----------------------------------
回调函数
----------------------------------*/
void Setting_CallBack(u8 key)
{
    switch (key)
    {
        case KEY_UP:
        case KEY_Down:
        case KEY_UP_L:
        case KEY_Down_L:
            KeySelItem(key);
            Item_Num_[1] = Menu_GetSelItem();
            break;
        case KEY_Return:
            usart_state = USB_SERIAL_DISABLE;
            ShowParentPage_Num(Item_Num_[0]);
            break;
        case KEY_Ok:
            ShowItemPage();
            break;
    }
}
/*----------------------------------
回调函数
----------------------------------*/
void Common_CallBack(u8 key)
{
    switch (key)
    {
        case KEY_UP:
        case KEY_Down:
        case KEY_UP_L:
        case KEY_Down_L:
            KeySelItem(key);
            Item_Num_[1] = Menu_GetSelItem();
            break;
        case KEY_Return:
//            usart_state = USB_SERIAL_DISABLE;
            ShowParentPage();
            break;
        case KEY_Ok:
            ShowItemPage();
            break;
    }
}


//-------------------------------------end line----------------------------------------
