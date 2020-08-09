/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#include "menu_app.h"
#include <rtthread.h>
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "servo_serial.h"
#include "string.h"

#define BLINK_TIME 10

extern struct rt_messagequeue key_mq;

//--0
extern struct PAGE *pPage;
//--1
extern struct PAGE mainPage;
//--2
extern struct PAGE Setting_Page;
extern struct PAGE Position_Page;
extern struct PAGE Info_Page;
extern struct PAGE Lcd_Page;
extern struct PAGE Set_Factory_Page;
//--3
extern struct PAGE Servo_Version_Page;
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
uint8_t Item_Num_[2] ={0, 0};
extern Usart_State usart_state;
/************************************************MAIN******************************************************/
//----1
struct Item Main_item[]={
                                    (char*)"SETTINGS",       		&Setting_Page,  	0, 0,   0, SHOW_NULL,0,0,
									(char*)"SERVO TEST",			&Position_Page,     0, 0,   0, SHOW_NULL,0,0,
									(char*)"INFORMATION",			&Info_Page,     	0, 0,   0, SHOW_NULL,0,0,
};
struct PAGE mainPage={0, Menu_One_CallBack, Main_item, sizeof(Main_item)/sizeof(struct Item)};
/************************************************SET*********************************************************/
//----2
struct Item Setting_item[]={            (char*)"1.VER.",						&Servo_Version_Page,  0,  0,  1, SHOW_NUM,0,0,
										(char*)"2.MAX POWER",					0,  0,  12,  1, SHOW_NUM,500,1500,
										(char*)"3.BOOST",						0,  0,  13,  1, SHOW_NUM,0,100,
                                        (char*)"4.DEAD BAND",					0,  0,  14,  2, SHOW_NUM,2,10,
										(char*)"5.FORCE",						0,  0,  14,  2, SHOW_NUM,1,10,	
										(char*)"6.SENSITIVITY",					0,  0,  14,  2, SHOW_NUM,1,10,	
                                        (char*)"7.BRAKE",						0,  0,  13,  3, SHOW_NUM,1,100,
										(char*)"8.SOFT START",					0,  0,  14,  3, SHOW_STRING,0,1,
										(char*)"9.WRITE DATA",					&Data_Save_Page,  0,  0,  3, SHOW_NULL,0,0,
										(char*)"10.READ DATA",					&Data_Read_Page,  0,  0,  3, SHOW_NULL,0,0,
										(char*)"11.FACTORY RESET",				&Reset_Data_Read_Page,  0,  0,  3, SHOW_NULL,0,0,
                                    };

struct PAGE Setting_Page={&mainPage,Menu_Two_CallBack,Setting_item,sizeof(Setting_item)/sizeof(struct Item)};
 
//----2
struct Item Position_item[]={           (char*)"BROAD BAND",							&Broadband_servo_Page,  0,  0,  1, SHOW_NUM,0,0,
										(char*)"NARROW BAND",						&Narrowband_servo_Page,  0,  0,  1, SHOW_NUM,0,0,
                                    };
struct PAGE Position_Page={&mainPage,Menu_Two_CallBack,Position_item,sizeof(Position_item)/sizeof(struct Item)};
 
//----2
struct Item Info_item[]={
										(char*)"SERVO VERSION",					&Servo_Version_Page1,  0,  0,  1, SHOW_NUM,0,0,
										(char*)"LCD SETTINGS",        			&Lcd_Page,  		0, 0,   0, SHOW_NULL,0,0,
//										(char*)"DATA FACTOCY SET",      			&Set_Factory_Page,  0, 0,   0, SHOW_NULL,0,0,
                                    };
struct PAGE Info_Page={&mainPage,Menu_Two_CallBack,Info_item,sizeof(Info_item)/sizeof(struct Item)};

//----2
 struct Item Lcd_item[]={ 				(char*)"SERVO BDRT SET",						&Servo_Bd_Set_Page,  0,  0,  1, SHOW_NUM,0,0,
										(char*)"HOST BDRT SET",							&Host_Bd_Set_Page,   0,  0,  1, SHOW_NUM,0,0,
										(char*)"LCD UPGRADE",							&Lcd_Upgrade_Page,  0,  0,  1, SHOW_NUM,0,0,
                                    };
struct PAGE Lcd_Page={&Info_Page, Menu_Three_CallBack,Lcd_item,sizeof(Lcd_item)/sizeof(struct Item)};

struct PAGE Set_Factory_Page ={&Info_Page, Servo_Set_Factory_CallBack, 0, 0};
/*********************************************************************************************************/
//----3
struct PAGE Servo_Version_Page ={&Setting_Page, Servo_Version_Page_CallBack, 0, 0};
//----3
struct Item Data_Save_item[]={ 			(char*)"TO SERVO",							0,  0,  0,  1, SHOW_NUM,0,0,
										(char*)"TO MEMORY",						&Servo_Write_Memory_Page,  0,  0,  1, SHOW_NUM,0,0,
                                    };
struct PAGE Data_Save_Page={&Setting_Page, Menu_Three_CallBack, Data_Save_item,sizeof(Data_Save_item)/sizeof(struct Item)};
//----3
struct Item Data_Read_item[]={ 			(char*)"FROM SERVO",							0,  0,  0,  1, SHOW_NUM,0,0,
										(char*)"FROM MEMORY",							&Servo_Read_Memory_Page,  0,  0,  1, SHOW_NUM,0,0,
                                    };
struct PAGE Data_Read_Page={&Setting_Page, Menu_Three_CallBack, Data_Read_item,sizeof(Data_Read_item)/sizeof(struct Item)};
//----3
struct PAGE Reset_Data_Read_Page ={&Setting_Page, Reset_Data_Read_Page_CallBack, 0, 0};
//----3
struct Item Servo_Bd_Set_item[]={ 		(char*)"19200",							0,  0,  0,  1, SHOW_NUM,0,0,
										(char*)"9600",							0,  0,  0,  1, SHOW_NUM,0,0,
										(char*)"38400",							0,  0,  0,  1, SHOW_NUM,0,0,
										(char*)"115200",							0,  0,  0,  1, SHOW_NUM,0,0,
                                    };
struct PAGE Servo_Bd_Set_Page={&Lcd_Page, Servo_Bd_Set_CallBack, Servo_Bd_Set_item,sizeof(Servo_Bd_Set_item)/sizeof(struct Item)};
//----3
struct Item Host_Bd_Set_item[]={ 		(char*)"19200",							0,  0,  0,  1, SHOW_NUM,0,0,
										(char*)"9600",							0,  0,  0,  1, SHOW_NUM,0,0,
										(char*)"38400",							0,  0,  0,  1, SHOW_NUM,0,0,
										(char*)"115200",							0,  0,  0,  1, SHOW_NUM,0,0,
                                    };
struct PAGE Host_Bd_Set_Page={&Lcd_Page, Lcd_Bd_Set_CallBack, Host_Bd_Set_item,sizeof(Host_Bd_Set_item)/sizeof(struct Item)};

struct PAGE Lcd_Upgrade_Page={&Lcd_Page, Lcd_Upgrade_CallBack, 0, 0};
//----3
struct PAGE Servo_Version_Page1 ={&Info_Page, Servo_Version_Page_CallBack, 0, 0};


struct Item Brodband_item[]={           (char*)"PWM 500US",						&Broadband_servo_Page,  0,  0,  1, SHOW_NUM,0,0,
										(char*)"PWM 900US",						&Broadband_servo_Page,  0,  0,  1, SHOW_NUM,0,0,
										(char*)"PWM 1500US",					&Broadband_servo_Page,  0,  0,  1, SHOW_NUM,0,0,
										(char*)"PWM 2100US",					&Broadband_servo_Page,  0,  0,  1, SHOW_NUM,0,0,
										(char*)"PWM 2500US",					&Broadband_servo_Page,  0,  0,  1, SHOW_NUM,0,0,
                                    };
struct PAGE Broadband_servo_Page={&Position_Page, Narrowband_Page_CallBack,Brodband_item,sizeof(Brodband_item)/sizeof(struct Item)};

struct Item Narrowband_item[]={         (char*)"PWM 500US",						&Narrowband_servo_Page,  0,  0,  1, SHOW_NUM,0,0,
										(char*)"PWM 750US",						&Narrowband_servo_Page,  0,  0,  1, SHOW_NUM,0,0,
										(char*)"PWM 1000US",					&Narrowband_servo_Page,  0,  0,  1, SHOW_NUM,0,0,
                                    };
struct PAGE Narrowband_servo_Page ={&Position_Page, Broadband_Page_CallBack, Narrowband_item, sizeof(Narrowband_item)/sizeof(struct Item)};

//----4
struct PAGE Servo_Write_Memory_Page ={&Data_Save_Page, Servo_Write_Memory_CallBack, 0, 0};
//----4
struct PAGE Servo_Read_Memory_Page ={&Data_Read_Page, Servo_Read_Memory_CallBack, 0, 0};
/*************************************************FUNCTIONS**************************************************************/
// struct Servo_Data_Stru_  para_01= {2200,1500,800,3448,2048,648,2,0,0,1,0,1,380,1,16,248,1,1500,10,1,1,10,0,100,0,1020,1020,1500,1500,1500,1,0,3,1,8,16,32,1,1};
// struct Servo_Data_Stru_  para_02= {2200,1500,800,3448,2048,648,2,0,0,1,0,1,380,1,16,248,1,1500,10,1,1,10,0,100,0,1020,1020,1500,1500,1500,1,0,3,1,8,16,32,1,1};
// struct Servo_Data_Stru_  para_03= {2200,1500,800,3448,2048,648,2,0,0,1,0,1,380,1,16,248,1,1500,10,1,1,10,0,100,0,1020,1020,1500,1500,1500,1,0,3,1,8,16,32,1,1};
/*----------------------------------
»Øµ÷º¯Êý
----------------------------------*/

uint8_t keep(uint8_t state)
{
	uint8_t rec_buff[2];
	uint8_t i;
	uint16_t time = 0;
	uint8_t sh = 0;
	
	if(state == 0){
		LCD_Write_Str(1,0,(char*)">FAILED!");
	}
	else if (state == 1){
		LCD_Write_Str(1,0,(char*)">SUCCESS");
	}
	else{
		LCD_Write_Str(0,0,(char*)">CONFORM?");
	}
	
	for(i=0; i<5; i++){
		rt_mq_recv(&key_mq, &rec_buff, 2, RT_WAITING_NO);
	}
	while(1){
		if(rt_mq_recv(&key_mq, &rec_buff, 2, RT_WAITING_NO)== RT_EOK){
			if(rec_buff[0] == 2){
				return 2; //return
			}
			else if(rec_buff[0] == 3){
				return 3; //ok
			}
		}
		rt_thread_delay(RT_TICK_PER_SECOND/10);
		time ++;
		
		if(time >=BLINK_TIME){
			time = 0;
		}
		if(time <BLINK_TIME/2){
			if(sh ==1){
				if(state == 0){
					LCD_Write_Str(1,0,(char*)">FAILED!");
				}
				else if (state == 1){
					LCD_Write_Str(1,0,(char*)">SUCCESS");
				}
				else{
					LCD_Write_Str(0,0,(char*)">CONFORM?");
				}
				sh =0;
			}
		}
		else{
			if(sh == 0){
				LCD_Write_Str(1,0,(char*)"         ");
				sh =1;
			}
		}
	}
}

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
			if(pPage == &Position_Page){
				if(Menu_GetSelItem() == 0){
					ShowItemPage_Num(2);
				}
				else{
					ShowItemPage_Num(1);
				}		
			}
			else{
				ShowItemPage();
			}
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
				LCD_Write_Str(0,0,(char*)"<WRITTING>...");
				Copy_Data_To_Stru();
				menu_combine_prom_work_parm();
				if(menu_combine_verify_work_parm()){
					if(keep(1) ==2){
						ShowParentPage_Num(Item_Num_[1]);
					}
					else{
						SetMainPage(&Setting_Page);
						ShowPage(pPage);
					}
				}
				else{
					keep(0);
					SetMainPage(&Data_Save_Page);
					ShowPage(pPage);
				}
				break;
			}
			else if(pPage == &Data_Read_Page && Menu_GetSelItem() == 0){
				Lcd_Clr_Scr();
				LCD_Write_Str(0,0,(char*)"<READING>...");
				if(menu_combine_fb_work_parm()){
					if(keep(1) ==2){
						ShowParentPage_Num(Item_Num_[1]);
					}
					else{
						SetMainPage(&Setting_Page);
						ShowPage(pPage);
					}
				}
				else{
					keep(0);
					SetMainPage(&Data_Read_Page);
					ShowPage(pPage);
				}
				break;
			}
            ShowItemPage();
			break;
	}
}
void Lcd_Upgrade_CallBack(u8 key)
{
	Lcd_Clr_Scr();
	put_chars_middle(0,"LCD UPGRADE");
	usart_state = USB_SERIAL_PROGRAM;
	usart1_init(115200);
	switch (key)
	{
		case KEY_Return:
			ShowParentPage_Num(2);
			break;
	}
}


void Servo_Bd_Set_CallBack(u8 key)
{
	uint32_t bd_set;
	switch (key)
	{
		case KEY_UP:	
		case KEY_Down:
		case KEY_UP_L:
		case KEY_Down_L:			
			KeySelItem(key);
			break;
		case KEY_Return:
			ShowParentPage();
			break;
		case KEY_Ok:
			if(Menu_GetSelItem() == 0){
				bd_set =19200;
			}
			else if(Menu_GetSelItem() == 1){
				bd_set =9600;
			}
			else if(Menu_GetSelItem() == 2){
				bd_set =38400;
			}
			else if(Menu_GetSelItem() == 3){
				bd_set =115200;
			}
			usart2_init_rx(bd_set);
			LCD_Write_Str(0,0,(char*)"<BAUD SET>...");
			rt_thread_delay(RT_TICK_PER_SECOND);
			keep(1);
			ShowParentPage();
			break;
	}
}

void Lcd_Bd_Set_CallBack(u8 key)
{
	uint32_t bd_set;
	switch (key)
	{
		case KEY_UP:	
		case KEY_Down:
		case KEY_UP_L:
		case KEY_Down_L:			
			KeySelItem(key);
			break;
		case KEY_Return:
			ShowParentPage();
			break;
		case KEY_Ok:
			if(Menu_GetSelItem() == 0){
				bd_set =19200;
			}
			else if(Menu_GetSelItem() == 1){
				bd_set =9600;
			}
			else if(Menu_GetSelItem() == 2){
				bd_set =38400;
			}
			else if(Menu_GetSelItem() == 3){
				bd_set =115200;
			}
			usart1_init(bd_set);
			LCD_Write_Str(0,0,(char*)"<BAUD SET>...");
			rt_thread_delay(RT_TICK_PER_SECOND);
			keep(1);
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
			ShowParentPage_Num(1);
			return;
		case KEY_Ok:
			Lcd_Clr_Scr();
			version = (uint16_t) servoDataStru.work_p12;
			data_version = version/100*100+ num;
			
			Copy_Data_To_Stru();
			LCD_Write_Str(0,0,(char*)"<WRITTING>...");
			save_servo_data_in_flash(num + USER_OUT_SPACE, data_version);
			rt_thread_delay(RT_TICK_PER_SECOND);
			if(keep(1) ==2){
				ShowParentPage_Num(1);
			}
			else{
				SetMainPage(&Setting_Page);
				ShowPage(pPage);
			}
			return;
	}
	Lcd_Clr_Scr();
	buf[11] += num / 10 %10;
	buf[12] += num / 1 %10;
	put_chars_middle(0,"WRITE MEMORY");
	LCD_Write_Str(1,0,(char*)buf);
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
			ShowParentPage_Num(1);
			return;
		case KEY_Ok:
			Lcd_Clr_Scr();
			LCD_Write_Str(0,0,(char*)"<READDING>...");
			read_servo_data_in_flash(num + USER_OUT_SPACE);
			rt_thread_delay(RT_TICK_PER_SECOND);
			if(keep(1) ==2){
				ShowParentPage_Num(1);
			}
			else{
				SetMainPage(&Setting_Page);
				ShowPage(pPage);
			}
			return;
	}
	Lcd_Clr_Scr();
	buf[11] += num / 10 %10;
	buf[12] += num / 1 %10;
	put_chars_middle(0, "READ MEMORY");
	LCD_Write_Str(1,0,(char*)buf);
}

void Servo_Set_Factory_CallBack(u8 key)
{
	static uint8_t num = 0;
	char buf[] = "  FDATA_X00XX  ";
	uint16_t data_version;
	uint16_t version;
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
			version = (uint16_t) servoDataStru.work_p12;
			data_version = version/10000 *10000 + num *100 + 0;
			Copy_Data_To_Stru();
			LCD_Write_Str(0,0,(char*)"<SETTING>...");
			save_servo_data_in_flash(num + CUSTOMER_OUT_SPACE, data_version);
			rt_thread_delay(RT_TICK_PER_SECOND);
			if(keep(1) ==2){
				ShowParentPage_Num(Item_Num_[1]);
			}
			else{
				SetMainPage(&Setting_Page);
				ShowPage(pPage);
			}
			return;
	}
	Lcd_Clr_Scr();
	buf[9] += num / 10 %10;
	buf[10] += num / 1 %10;
	put_chars_middle(0,(char*)"SETTING FACTORY");
	LCD_Write_Str(1,0,(char*)buf);
}

void Servo_Version_Page_CallBack(u8 key)
{
	char buf[5] ={'0', '0', '0', '0', '0'};
	char buf_show[16];
	uint8_t i;
	uint16_t data_version;
	for(i=0; i<16; i++){
		buf_show[i] = ' ';
	}
	
	Lcd_Clr_Scr();
	put_chars_middle(0,"SERVO VERSION");
	data_version = (uint16_t)servoDataStru.work_p12 ;
	buf[0] += data_version / 10000 %10;
	buf[1] += data_version / 1000 %10;
	buf[2] += data_version / 100 %10;
	buf[3] += data_version / 10 %10;
	buf[4] += data_version / 1 %10;
	for(i=0; i<5; i++){
		buf_show[i+6] = buf[i];
	}
	LCD_Write_Str(1,0,buf_show);
	
	if (key == KEY_Return){
        ShowParentPage();
    }
}

void Broadband_Page_CallBack(u8 key)
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
			ShowParentPage();
			return;
		case KEY_Ok:
			break;
	}
	
	switch(Menu_GetSelItem()){
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
	switch (key)
	{
		case KEY_UP:	
		case KEY_Down:
		case KEY_UP_L:
		case KEY_Down_L:			
			KeySelItem(key);
			break;
		case KEY_Return:
			ShowParentPage();
			return;
		case KEY_Ok:
			break;
	}
	
	switch(Menu_GetSelItem()){
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
	
	if(fisrt ==0){
		Lcd_Clr_Scr();
		if(keep(3) == 2){
			ShowParentPage_Num(Item_Num_[1]);
			fisrt = 0;
			return;
			//return
		}
	}
	fisrt = 1;
	
	
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
			fisrt = 0;
			return;
		case KEY_Ok:
			Lcd_Clr_Scr();
			LCD_Write_Str(0,0,(char*)"<RESTORING>...");
			read_servo_data_in_flash(num + CUSTOMER_OUT_SPACE);
			rt_thread_delay(RT_TICK_PER_SECOND);
			keep(1);
			ShowParentPage();
			fisrt = 0;
			return;
	}
	buf[9] += num / 10 %10;
	buf[10] += num / 1 %10;
	Lcd_Clr_Scr();
	put_chars_middle(0,(char*)"FACTORY RESET");
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
	Setting_item[7].data = servoDataStru.set_p14;
}

void Copy_Data_To_Stru(void)
{
	servoDataStru.set_p11 = Setting_item[1].data;
	servoDataStru.set_p15 = Setting_item[2].data;
	servoDataStru.work_p6 = Setting_item[3].data;
	servoDataStru.debug_p0 = Setting_item[4].data;
	servoDataStru.debug_p5 = Setting_item[5].data;
	servoDataStru.debug_p2 = Setting_item[6].data ;
	servoDataStru.set_p14 = Setting_item[7].data;
}


//-------------------------------------end line----------------------------------------
