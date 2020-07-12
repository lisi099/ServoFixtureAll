/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#include "menu_app.h"
#include <rtthread.h>
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "servo_serial.h"
#include "string.h"

//--0

extern struct PAGE *pPage;

//--1
extern struct PAGE mainPage;
//--2
extern struct PAGE Setting_Page;
extern struct PAGE Position_Page;
extern struct PAGE Info_Page;
extern struct PAGE Lcd_Page;
//--3
extern struct PAGE Servo_Version_Page;
extern struct PAGE Data_Save_Page;
extern struct PAGE Data_Read_Page;
extern struct PAGE Servo_Bd_Set_Page;
extern struct PAGE Host_Bd_Set_Page;
extern struct PAGE Reset_Data_Read_Page;
extern struct PAGE Servo_Version_Page1;
extern struct PAGE Lcd_Version_Page;
extern struct PAGE Broadband_servo_Page;
extern struct PAGE Narrowband_servo_Page;

//--4
extern struct PAGE Servo_Write_Memory_Page;
extern struct PAGE Servo_Read_Memory_Page;

extern struct Servo_Data_Stru_ servoDataStru;
struct Servo_Data_Stru_ servo_load_data;
uint8_t Item_Num_[2] ={0, 0};
/************************************************MAIN******************************************************/
//----1
struct Item Main_item[]={
                                    (char*)"1.SERVOSETTING",       	&Setting_Page,  	0, 0,   0, SHOW_BOOL,
									(char*)"2.SENDPOSITION",		&Position_Page,     0, 0,   0, SHOW_BOOL,
									(char*)"3.INFORMATION",			&Info_Page,     	0, 0,   0, SHOW_BOOL,
                                    (char*)"4.LCDSETTINGS",        	&Lcd_Page,  		0, 0,   0, SHOW_BOOL,
									(char*)"5.**************",      0,  		0, 0,   0, SHOW_BOOL,
};
struct PAGE mainPage={0, Menu_One_CallBack, Main_item, sizeof(Main_item)/sizeof(struct Item), DISPLAY_MODE_1_COLUMN};
/************************************************SET*********************************************************/
//----2
struct Item Setting_item[]={            (char*)"00.SERVO VERSION",				&Servo_Version_Page,  0,  0,  1, SHOW_U16,
										(char*)"01.MAXPOW",						0,  0,  11,  1, SHOW_U16,
										(char*)"02.BOOST",						0,  0,  11,  1, SHOW_U8,
                                        (char*)"03.DBAND",						0,  0,  11,  2, SHOW_U8,
										(char*)"04.FORCE",						0,  0,  11,  2, SHOW_U8,	
										(char*)"05.STERETC",					0,  0,  12,  2, SHOW_U8,	
                                        (char*)"06.BRAKE",						0,  0,  11,  3, SHOW_U8,
										(char*)"07.SFTSTA",						0,  0,  11,  3, SHOW_BOOL,
										(char*)"08.SET DAT SAVE",				&Data_Save_Page,  0,  0,  3, SHOW_BOOL,
										(char*)"09.SET DAT READ",				&Data_Read_Page,  0,  0,  3, SHOW_BOOL,
										(char*)"10.RES DAT READ",				&Reset_Data_Read_Page,  0,  0,  3, SHOW_BOOL,
										(char*)"11.*************",      0,  		0, 0,   0, SHOW_BOOL,
                                    };

struct PAGE Setting_Page={&mainPage,Menu_Two_CallBack,Setting_item,sizeof(Setting_item)/sizeof(struct Item),DISPLAY_MODE_1_COLUMN};
 
//----2
struct Item Position_item[]={           (char*)"1.BROADBANDSERVO",						&Broadband_servo_Page,  0,  0,  1, SHOW_U16,
										(char*)"2.NARROWBANDSERVO",						&Narrowband_servo_Page,  0,  0,  1, SHOW_U16,
                                    };
struct PAGE Position_Page={&mainPage,Menu_Two_CallBack,Position_item,sizeof(Position_item)/sizeof(struct Item),DISPLAY_MODE_1_COLUMN};
 
//----2
struct Item Info_item[]={ 				(char*)"1.LCD VERSION",							&Lcd_Version_Page,  0,  0,  1, SHOW_U16,
										(char*)"2.SERVO VERSION",						&Servo_Version_Page1,  0,  0,  1, SHOW_U16,
                                    };
struct PAGE Info_Page={&mainPage,Menu_Two_CallBack,Info_item,sizeof(Info_item)/sizeof(struct Item),DISPLAY_MODE_1_COLUMN};

//----2
 struct Item Lcd_item[]={ 				(char*)"1.SERVO BDRT SET",						&Servo_Bd_Set_Page,  0,  0,  1, SHOW_U16,
										(char*)"2.HOST BDRT SET",						&Host_Bd_Set_Page,   0,  0,  1, SHOW_U16,
										(char*)"3.LCD UPGRADE",							0,  0,  0,  1, SHOW_U16,
                                    };
struct PAGE Lcd_Page={&mainPage,Menu_Two_CallBack,Lcd_item,sizeof(Lcd_item)/sizeof(struct Item),DISPLAY_MODE_1_COLUMN};
/*********************************************************************************************************/
//----3
struct PAGE Servo_Version_Page ={&Setting_Page, Servo_Version_Page_CallBack, 0, 0,DISPLAY_MODE_1_COLUMN};
//----3
struct Item Data_Save_item[]={ 			(char*)"1.Write Servo",							0,  0,  0,  1, SHOW_U16,
										(char*)"2.Write Memory",						&Servo_Write_Memory_Page,  0,  0,  1, SHOW_U16,
                                    };
struct PAGE Data_Save_Page={&Setting_Page, Menu_Three_CallBack, Data_Save_item,sizeof(Data_Save_item)/sizeof(struct Item),DISPLAY_MODE_1_COLUMN};
//----3
struct Item Data_Read_item[]={ 			(char*)"1.Read Servo",							0,  0,  0,  1, SHOW_U16,
										(char*)"2.Read Memory",							&Servo_Read_Memory_Page,  0,  0,  1, SHOW_U16,
                                    };
struct PAGE Data_Read_Page={&Setting_Page, Menu_Three_CallBack, Data_Read_item,sizeof(Data_Read_item)/sizeof(struct Item), DISPLAY_MODE_1_COLUMN};
//----3
struct PAGE Reset_Data_Read_Page ={&Setting_Page, Reset_Data_Read_Page_CallBack, 0, 0,DISPLAY_MODE_1_COLUMN};
//----3
struct Item Servo_Bd_Set_item[]={ 		(char*)"1.19200",							0,  0,  0,  1, SHOW_U16,
										(char*)"2.9600",							0,  0,  0,  1, SHOW_U16,
										(char*)"3.38400",							0,  0,  0,  1, SHOW_U16,
										(char*)"4.115200",							0,  0,  0,  1, SHOW_U16,
                                    };
struct PAGE Servo_Bd_Set_Page={&Setting_Page, Menu_Three_CallBack, Servo_Bd_Set_item,sizeof(Servo_Bd_Set_item)/sizeof(struct Item),DISPLAY_MODE_1_COLUMN};
//----3
struct Item Host_Bd_Set_item[]={ 		(char*)"1.19200",							0,  0,  0,  1, SHOW_U16,
										(char*)"2.9600",							0,  0,  0,  1, SHOW_U16,
										(char*)"3.38400",							0,  0,  0,  1, SHOW_U16,
										(char*)"4.115200",							0,  0,  0,  1, SHOW_U16,
                                    };
struct PAGE Host_Bd_Set_Page={&Setting_Page, Menu_Three_CallBack, Host_Bd_Set_item,sizeof(Host_Bd_Set_item)/sizeof(struct Item),DISPLAY_MODE_1_COLUMN};
//----3
struct PAGE Servo_Version_Page1 ={&Info_Page, Servo_Version_Page_CallBack, 0, 0,DISPLAY_MODE_1_COLUMN};
struct PAGE Lcd_Version_Page ={&Info_Page, Lcd_Version_Page_CallBack, 0, 0,DISPLAY_MODE_1_COLUMN};
struct PAGE Broadband_servo_Page ={&Position_Page, Narrowband_Page_CallBack, 0, 0,DISPLAY_MODE_1_COLUMN};
struct PAGE Narrowband_servo_Page ={&Position_Page, Broadband_Page_CallBack, 0, 0,DISPLAY_MODE_1_COLUMN};
//----4
struct PAGE Servo_Write_Memory_Page ={&Data_Save_Page, Servo_Write_Memory_CallBack, 0, 0,DISPLAY_MODE_1_COLUMN};
//----4
struct PAGE Servo_Read_Memory_Page ={&Data_Read_Page, Servo_Read_Memory_CallBack, 0, 0,DISPLAY_MODE_1_COLUMN};
/*************************************************FUNCTIONS**************************************************************/
// struct Servo_Data_Stru_  para_01= {2200,1500,800,3448,2048,648,2,0,0,1,0,1,380,1,16,248,1,1500,10,1,1,10,0,100,0,1020,1020,1500,1500,1500,1,0,3,1,8,16,32,1,1};
// struct Servo_Data_Stru_  para_02= {2200,1500,800,3448,2048,648,2,0,0,1,0,1,380,1,16,248,1,1500,10,1,1,10,0,100,0,1020,1020,1500,1500,1500,1,0,3,1,8,16,32,1,1};
// struct Servo_Data_Stru_  para_03= {2200,1500,800,3448,2048,648,2,0,0,1,0,1,380,1,16,248,1,1500,10,1,1,10,0,100,0,1020,1020,1500,1500,1500,1,0,3,1,8,16,32,1,1};
/*----------------------------------
»Øµ÷º¯Êý
----------------------------------*/

void Menu_One_CallBack(u8 key)
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
			ShowParentPage();
			break;
		case KEY_Ok:
            ShowItemPage();
            Item_Num_[1] = 0;
			break;
	}
}

void Menu_Two_CallBack(u8 key)
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
			ShowParentPage_Num(Item_Num_[0]);
			break;
		case KEY_Ok:
            ShowItemPage();
			break;
	}
}

void Menu_Three_CallBack(u8 key)
{
	switch (key)
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
				Lcd_Clr_Scr();
				LCD_Write_Str(0,0,(char*)"Write Servo....");
				Copy_Data_To_Stru();
				if(menu_combine_verify_work_parm()){
					Lcd_Clr_Scr();
					LCD_Write_Str(0,0,(char*)"Write Servo Ok..");
					rt_thread_delay(RT_TICK_PER_SECOND);
					SetMainPage(&Data_Save_Page);
					ShowPage(pPage);
				}
				else{
					Lcd_Clr_Scr();
					LCD_Write_Str(0,0,(char*)"Write Servo Fail");
					rt_thread_delay(RT_TICK_PER_SECOND);
					SetMainPage(&Data_Save_Page);
					ShowPage(pPage);
				}
				break;
			}
			else if(pPage == &Data_Read_Page && Menu_GetSelItem() == 0){
				Lcd_Clr_Scr();
				LCD_Write_Str(0,0,(char*)"Read Servo....");
				if(menu_combine_fb_work_parm()){
					Lcd_Clr_Scr();
					LCD_Write_Str(0,0,(char*)"Read Servo Ok..");
					rt_thread_delay(RT_TICK_PER_SECOND);
					SetMainPage(&Data_Read_Page);
					ShowPage(pPage);
				}
				else{
					Lcd_Clr_Scr();
					LCD_Write_Str(0,0,(char*)"Read Servo Fail");
					rt_thread_delay(RT_TICK_PER_SECOND);
					SetMainPage(&Data_Read_Page);
					ShowPage(pPage);
				}
				break;
			}
            ShowItemPage();
			break;
	}
}
void Servo_Read_Memory_CallBack(u8 key)
{
	static uint8_t num = 0;
	char buf[] = "  UDATA_XXX00   ";
	switch (key)
	{
		case KEY_UP:
			num ++;
			if(num >19){
				num = 0;
			}
			break;
		case KEY_Down:
			num --;
			if(num >19){
				num = 19;
			}
			break;
		case KEY_UP_L:
			num ++;
			if(num >19){
				num = 0;
			}
			break;
		case KEY_Down_L:
			num --;
			if(num >19){
				num = 19;
			}
			break;
		case KEY_Return:
			ShowParentPage_Num(Item_Num_[1]);
			return;
		case KEY_Ok:
			Lcd_Clr_Scr();
			LCD_Write_Str(0,0,(char*)"   Read...   ");
			read_servo_data_in_flash(num);
			LCD_Write_Str(0,0,(char*)"   Read Ok   ");
			rt_thread_delay(RT_TICK_PER_SECOND);
			ShowParentPage();
			return;
	}
	Lcd_Clr_Scr();
	buf[11] += num / 10 %10;
	buf[12] += num / 1 %10;
	LCD_Write_Str(0,0,(char*)buf);
}

void Servo_Write_Memory_CallBack(u8 key)
{
	static uint8_t num = 0;
	char buf[] = "  UDATA_XXX00   ";
	switch (key)
	{
		case KEY_UP:
			num ++;
			if(num >19){
				num = 0;
			}
			break;
		case KEY_Down:
			num --;
			if(num >19){
				num = 19;
			}
			break;
		case KEY_UP_L:
			num ++;
			if(num >19){
				num = 0;
			}
			break;
		case KEY_Down_L:
			num --;
			if(num >19){
				num = 19;
			}
			break;
		case KEY_Return:
			ShowParentPage_Num(Item_Num_[1]);
			return;
		case KEY_Ok:
			Lcd_Clr_Scr();
			LCD_Write_Str(0,0,(char*)"   Writing...   ");
			save_servo_data_in_flash(num);
			LCD_Write_Str(0,0,(char*)"   Writing Ok   ");
			rt_thread_delay(RT_TICK_PER_SECOND);
			ShowParentPage();
			return;
	}
	Lcd_Clr_Scr();
	buf[11] += num / 10 %10;
	buf[12] += num / 1 %10;
	LCD_Write_Str(0,0,(char*)buf);
}

void Servo_Version_Page_CallBack(u8 key)
{
	char buf[5] ={'0', '0', '0', '0', '0'};
	char buf_show[16];
	uint8_t i;
	for(i=0; i<16; i++){
		buf_show[i] = ' ';
	}
	
	Lcd_Clr_Scr();
	LCD_Write_Str(0,0,(char*)" Servo Version ");
	if(servoDataStru.work_p12 >9999){
		buf[0] += servoDataStru.work_p12 / 10000 %10;
		buf[1] += servoDataStru.work_p12 / 1000 %10;
		buf[2] += servoDataStru.work_p12 / 100 %10;
		buf[3] += servoDataStru.work_p12 / 10 %10;
		buf[4] += servoDataStru.work_p12 / 1 %10;
	}
	else if(servoDataStru.work_p12 >999){
		buf[0] += servoDataStru.work_p12 / 1000 %10;
		if(servoDataStru.work_p12 %100 >10){
			buf[2] += servoDataStru.work_p12 / 100 %10;
			buf[3] += servoDataStru.work_p12 / 10 %10;
			buf[4] += servoDataStru.work_p12 / 1 %10;
		}
		else{
			buf[1] += servoDataStru.work_p12 / 100 %10;
			buf[2] += servoDataStru.work_p12 / 10 %10;
			buf[4] += servoDataStru.work_p12 / 1 %10;
		}
	}
	else if(servoDataStru.work_p12 >99){
		buf[0] += servoDataStru.work_p12 / 100 %10;
		buf[2] += servoDataStru.work_p12 / 10 %10;
		buf[4] += servoDataStru.work_p12 / 1 %10;
	}
	for(i=0; i<5; i++){
		buf_show[i+5] = buf[i];
	}
	LCD_Write_Str(1,0,buf_show);
	
	if (key == KEY_Return){
        ShowParentPage();
    }
}

void Lcd_Version_Page_CallBack(u8 key)
{
	char buf[5] ={'0', '0', '.', '0', '1'};
	char buf_show[16];
	uint8_t i;
	for(i=0; i<16; i++){
		buf_show[i] = ' ';
	}
	for(i=0; i<5; i++){
		buf_show[i+5] = buf[i];
	}
	Lcd_Clr_Scr();
	LCD_Write_Str(0,0,(char*)"   Lcd Version  ");
	LCD_Write_Str(1,0,buf_show);
	if (key == KEY_Return){
        ShowParentPage();
    }
}

void Broadband_Page_CallBack(u8 key)
{
	char buf1[] ={"High"};
	char buf2[] ={"Middle"};
	char buf3[] ={"Low"};
	char buf_show[16];
	uint8_t i;
	static uint8_t seq = 0;
	
	for(i=0; i<16; i++){
		buf_show[i] = ' ';
	}
	
	switch (key)
	{
		case KEY_UP:
			seq ++;
			if(seq >= 3){
				seq = 0;
			}
			break;
		case KEY_Down:
			break;
		case KEY_Return:
			ShowParentPage();
			return;
	}
	
	if(seq == 1){ //middle
		for(i=0; i<6; i++){
			buf_show[i+5] = buf2[i];
		}
	}
	else if(seq ==2){ // 
		for(i=0; i<4; i++){
			buf_show[i+5] = buf1[i];
		}
	}
	else if(seq ==0){ //
		for(i=0; i<3; i++){
			buf_show[i+5] = buf3[i];
		}
	}
	Lcd_Clr_Scr();
	LCD_Write_Str(0,0,(char*)"Narrowband Servo");
	LCD_Write_Str(1,0,buf_show);
}

void Narrowband_Page_CallBack(u8 key)
{
	char buf1[] ={"High"};
	char buf2[] ={"Middle"};
	char buf3[] ={"Low"};
	char buf_show[16];
	uint8_t i;
	static uint8_t seq = 0;
	
	for(i=0; i<16; i++){
		buf_show[i] = ' ';
	}
	
	switch (key)
	{
		case KEY_UP:
			break;
		case KEY_Down:
			seq ++;
			if(seq >= 3){
				seq = 0;
			}
			break;
		case KEY_Return:
			ShowParentPage();
			return;
	}
	
	if(seq == 1){ //middle
		for(i=0; i<6; i++){
			buf_show[i+5] = buf2[i];
		}
	}
	else if(seq ==2){ // 
		for(i=0; i<4; i++){
			buf_show[i+5] = buf1[i];
		}
	}
	else if(seq ==0){ //
		for(i=0; i<3; i++){
			buf_show[i+5] = buf3[i];
		}
	}
	Lcd_Clr_Scr();
	LCD_Write_Str(0,0,(char*)"Narrowband Servo");
	LCD_Write_Str(1,0,buf_show);
	if (key == KEY_Return){
        ShowParentPage();
    }
}

void Reset_Data_Read_Page_CallBack(u8 key)
{
	static uint8_t num = 0;
	Lcd_Clr_Scr();
	
	char buf[] = "  FDATA_X00XX   ";
	switch (key)
	{
		case KEY_UP:
			num ++;
			if(num >99){
				num = 0;
			}
			break;
		case KEY_Down:
			num --;
			if(num >99){
				num = 99;
			}
			break;
		case KEY_UP_L:
			num ++;
			if(num >99){
				num = 0;
			}
			break;
		case KEY_Down_L:
			num --;
			if(num >99){
				num = 99;
			}
			break;
		case KEY_Return:
			ShowParentPage_Num(Item_Num_[1]);
			return;
		case KEY_Ok:
			Lcd_Clr_Scr();
			LCD_Write_Str(0,0,(char*)"     Reset...   ");
			read_servo_data_in_flash(num+20);
			LCD_Write_Str(0,0,(char*)"     Reset Ok   ");
			rt_thread_delay(RT_TICK_PER_SECOND);
			ShowParentPage();
			return;
	}
	buf[9] += num / 10 %10;
	buf[10] += num / 1 %10;
	Lcd_Clr_Scr();
	LCD_Write_Str(0,0,(char*)" Reset Factory  ");
	LCD_Write_Str(1,0,(char*)buf);
}

void Copy_Data_To_Show(void)
{
	Setting_item[1].data = servoDataStru.set_p11;
	Setting_item[2].data = servoDataStru.set_p15;
	Setting_item[3].data = servoDataStru.work_p6;
	Setting_item[4].data = servoDataStru.debug_p0;
	Setting_item[5].data = servoDataStru.debug_p5;
	Setting_item[6].data = servoDataStru.debug_p2;
	Setting_item[7].data = servoDataStru.work_p14;
}

void Copy_Data_To_Stru(void)
{
	servoDataStru.set_p11 = Setting_item[1].data;
	servoDataStru.set_p15 = Setting_item[2].data;
	servoDataStru.work_p6 = Setting_item[3].data;
	servoDataStru.debug_p0 = Setting_item[4].data;
	servoDataStru.debug_p5 = Setting_item[5].data;
	servoDataStru.debug_p2 = Setting_item[6].data ;
	servoDataStru.work_p14 = Setting_item[7].data;
}


//-------------------------------------end line----------------------------------------
