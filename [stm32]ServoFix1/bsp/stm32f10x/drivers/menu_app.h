/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#ifndef _MenuAPP_H_BAB
#define _MenuAPP_H_BAB

#include "Menu.h"

extern struct PAGE mainPage;
extern struct PAGE Setting_Page;

void Menu_Three_CallBack(u8 key);
void Menu_One_CallBack(u8 key);
void Menu_Two_CallBack(u8 key);

void Data_Save_Callback(u8 key);
void Servo_Write_Memory_CallBack(u8 key);
void Servo_Read_Memory_CallBack(u8 key);
void Servo_Version_Page_CallBack(u8 key);
void Lcd_Version_Page_CallBack(u8 key);
void Reset_Data_Read_Page_CallBack(u8 key);
	
void Copy_Data_To_Show(void);
void Copy_Data_To_Stru(void);
#endif
