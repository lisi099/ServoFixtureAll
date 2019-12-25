/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#include "MenuAPP.h"
#include <rtthread.h>
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "servo_serial.h"
#include "string.h"

extern  struct PAGE Setting_Page;
extern  struct PAGE USB_Page;
extern  struct PAGE Ble_Page;
extern  struct PAGE Special_Page;
extern  struct PAGE Position_Page;
extern  struct PAGE Info_Page;
extern  struct PAGE USB_Online_Page;
extern  struct PAGE USB_Online_Page_9600;
extern  struct PAGE USB_Online_Page_19200;
extern  struct PAGE USB_Online_Page_38400;
extern  struct PAGE USB_Online_Page_115200;
extern  struct PAGE USB_Program_Page;
extern  struct PAGE Para01;
extern  struct PAGE Para02;
extern  struct PAGE Para03;
extern  struct PAGE Para500;
extern  struct PAGE Para1000;
extern  struct PAGE Para1500;
extern  struct PAGE Para2000;
extern  struct PAGE Para2500;

extern  struct PAGE Read_Page;
extern  struct PAGE Write_Page;
extern  struct PAGE Load_Page;
extern  struct PAGE Save_Page;
extern  struct PAGE Clear_Page;

extern struct PAGE *pPage;

extern struct Servo_Data_Stru_ servoDataStru;

extern Usart_State usart_state;

struct Servo_Data_Stru_ servo_load_data;

/************************************************MAIN******************************************************/
 struct Item main_item[]={
                                    (char*)"01.ServoSetting",       &Setting_Page,  0, 0,   0, SHOW_BOOL,
                                    (char*)"02.USB",                &USB_Page,      0, 0,   0, SHOW_BOOL,
                                    (char*)"03.Bluetooth",			&Ble_Page,      0, 0,   0, SHOW_BOOL,
                                    (char*)"04.SpecialLoad",        &Special_Page,  0, 0,   0, SHOW_BOOL,
									(char*)"05.SendPosition",		&Position_Page,     0, 0,   0, SHOW_BOOL,
                                    (char*)"06.Infomation",			&Info_Page,     0, 0,   0, SHOW_BOOL,
};
 struct PAGE mainPage={0,mainPageCallBack,main_item,sizeof(main_item)/sizeof(struct Item),DISPLAY_MODE_1_COLUMN};
/************************************************SET*********************************************************/
 struct Item Setting_item[]={           (char*)"01.Read",			&Read_Page,  0,  0,  1, SHOW_BOOL,
                                        (char*)"02.Write",			&Write_Page,  0,  0,  2, SHOW_BOOL,
                                        (char*)"03.Load",			&Load_Page,  0,  0,  3, SHOW_BOOL,
										(char*)"04.Save",			&Save_Page,  0,  0,  3, SHOW_BOOL,
										(char*)"05.Clear",			&Clear_Page,  0,  0,  3, SHOW_BOOL,
                                    };

 struct PAGE Setting_Page={&mainPage,Setting_CallBack,Setting_item,sizeof(Setting_item)/sizeof(struct Item),DISPLAY_MODE_1_COLUMN};
 
 struct Item Read_item[]={           	(char*)"01.Ver",				0,  0,  11,  1, SHOW_U16,
										(char*)"02.DB",					0,  0,  11,  2, SHOW_U8,
										(char*)"03.Maxdty",				0,  0,  11,  3, SHOW_16,
										(char*)"04.Boost",				0,  0,  11,  3, SHOW_16,
										(char*)"05.PMIN",				0,  0,  11,  3, SHOW_U16,
										(char*)"06.PMAX",				0,  0,  11,  3, SHOW_U16,
										(char*)"07.AngLft",				0,  0,  11,  3, SHOW_U16,
										(char*)"08.AngMid",				0,  0,  11,  3, SHOW_U16,
										(char*)"09.AngRht",				0,  0,  11,  3, SHOW_U16,
										(char*)"10.SftSta",				0,  0,  11,  3, SHOW_BOOL,
										(char*)"11.LPPMLk",				0,  0,  11,  3, SHOW_BOOL,
										(char*)"12.Invert",				0,  0,  11,  3, SHOW_BOOL,
										(char*)"13.DrivF1",				0,  0,  11,  3, SHOW_16,
										(char*)"14.DrivF2",				0,  0,  11,  3, SHOW_U16,
										(char*)"15.PlugPr",				0,  0,  11,  3, SHOW_U8,
										(char*)"16.ProTim",				0,  0,  11,  3, SHOW_U16,
										(char*)"17.ProDty",				0,  0,  11,  3, SHOW_16,
										(char*)"18.PT",					0,  0,  11,  3, SHOW_16,
										(char*)"19.PB",					0,  0,  11,  3, SHOW_16,
										(char*)"20.PP",					0,  0,  11,  3, SHOW_16,
										(char*)"-----end-------",		0,  0,  0,  0, SHOW_BOOL,
                                    };
 struct PAGE Read_Page={&Setting_Page,Setting_CallBack,Read_item,sizeof(Read_item)/sizeof(struct Item),DISPLAY_MODE_1_COLUMN};
 
 
 struct Item Write_item[]={           	(char*)"01.Ver",				0,  0,  11,  1, SHOW_U16,
										(char*)"02.DB",					0,  0,  11,  2, SHOW_U8,
										(char*)"03.Maxdty",				0,  0,  11,  3, SHOW_16,
										(char*)"04.Boost",				0,  0,  11,  3, SHOW_16,
										(char*)"05.PMIN",				0,  0,  11,  3, SHOW_U16,
										(char*)"06.PMAX",				0,  0,  11,  3, SHOW_U16,
										(char*)"07.AngLft",				0,  0,  11,  3, SHOW_U16,
										(char*)"08.AngMid",				0,  0,  11,  3, SHOW_U16,
										(char*)"09.AngRht",				0,  0,  11,  3, SHOW_U16,
										(char*)"10.SftSta",				0,  0,  11,  3, SHOW_BOOL,
										(char*)"11.LPPMLk",				0,  0,  11,  3, SHOW_BOOL,
										(char*)"12.Invert",				0,  0,  11,  3, SHOW_BOOL,
										(char*)"13.DrivF1",				0,  0,  11,  3, SHOW_16,
										(char*)"14.DrivF2",				0,  0,  11,  3, SHOW_U16,
										(char*)"15.PlugPr",				0,  0,  11,  3, SHOW_U8,
										(char*)"16.ProTim",				0,  0,  11,  3, SHOW_U16,
										(char*)"17.ProDty",				0,  0,  11,  3, SHOW_16,
										(char*)"18.PT",					0,  0,  11,  3, SHOW_16,
										(char*)"19.PB",					0,  0,  11,  3, SHOW_16,
										(char*)"20.PP",					0,  0,  11,  3, SHOW_16,
										(char*)"21.WriteInServo",		0,  0,  0,  0,  SHOW_BOOL,
										(char*)"-----end-------",		0,  0,  0,  0,  SHOW_BOOL,
                                    };
 struct PAGE Write_Page={&Setting_Page,Setting_CallBack,Write_item,sizeof(Write_item)/sizeof(struct Item),DISPLAY_MODE_1_COLUMN};
 
 struct Item Load_item[]={           	(char*)"01.AZHD1501000",		&Write_Page,  0,  0,  1,SHOW_BOOL,
										(char*)"02.AZHD1501001",		&Write_Page,  0,  0,  2,SHOW_BOOL,
										(char*)"03.AZHD1501002",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"04.AZHD1501003",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"05.AZHD1501004",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"06.AZHD1501005",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"07.AZHD1501006",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"08.AZHD1501007",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"09.AZHD1501008",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"10.AZHD1501009",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"11.AZHD1501010",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"12.AZHD1501011",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"13.AZHD1501012",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"14.AZHD1501013",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"15.AZHD1501014",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"16.AZHD1501015",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"17.AZHD1501016",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"18.AZHD1501017",		&Write_Page,  0,  0,  0,SHOW_BOOL,
										(char*)"19.AZHD1501018",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"20.AZHD1501019",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"21.AZHD1501020",		&Write_Page,  0,  0,  1,SHOW_BOOL,
										(char*)"22.AZHD1501021",		&Write_Page,  0,  0,  2,SHOW_BOOL,
										(char*)"23.AZHD1501022",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"24.AZHD1501023",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"25.AZHD1501024",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"26.AZHD1501025",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"27.AZHD1501026",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"28.AZHD1501027",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"29.AZHD1501028",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"30.AZHD1501029",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"31.AZHD1501030",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"32.AZHD1501031",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"33.AZHD1501032",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"34.AZHD1501033",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"35.AZHD1501034",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"36.AZHD1501035",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"37.AZHD1501036",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"38.AZHD1501037",		&Write_Page,  0,  0,  0,SHOW_BOOL,
										(char*)"39.AZHD1501038",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"40.AZHD1501039",		&Write_Page,  0,  0,  3,SHOW_BOOL,
										(char*)"-----end-------",		0,  0,  0,  0,
                                    };
 struct PAGE Load_Page={&Setting_Page,Setting_CallBack,Load_item,sizeof(Load_item)/sizeof(struct Item),DISPLAY_MODE_1_COLUMN};
 
 
 struct Item Save_item[]={           	(char*)"01.AZHD1501000",		0,  0,  13,  0, SHOW_U8,
										(char*)"02.SaveInFlash",		0,  0,  0,   0, SHOW_BOOL,
                                    };
 struct PAGE Save_Page={&Setting_Page, Save_CallBack, Save_item, sizeof(Save_item)/sizeof(struct Item), DISPLAY_MODE_1_COLUMN};
 
 
 struct PAGE Clear_Page={&Setting_Page, clear_CallBack, 0, 0, DISPLAY_MODE_1_COLUMN};
 
/************************************************USB***************************************************************/
  struct Item USB_Online_item[]={       (char*)"00.B9600",				&USB_Online_Page_9600,  0,  0,  0, SHOW_BOOL,
                                        (char*)"01.B19200",				&USB_Online_Page_19200,  0,  0,  0, SHOW_BOOL,
										(char*)"02.B38400",				&USB_Online_Page_38400,  0,  0,  0, SHOW_BOOL,
										(char*)"03.B115200",			&USB_Online_Page_115200,  0,  0,  0, SHOW_BOOL,
                                    };
 
 struct Item USB_item[]={               (char*)"00.USB online",				&USB_Online_Page,  0,  0,  0, SHOW_BOOL,
                                        (char*)"01.USB program",			&USB_Program_Page,  0,  0,  0, SHOW_BOOL,
                                    };
 struct PAGE USB_Page={&mainPage,Setting_CallBack,USB_item,sizeof(USB_item)/sizeof(struct Item),DISPLAY_MODE_1_COLUMN};
 
 struct PAGE USB_Online_Page={&USB_Page, Setting_CallBack, USB_Online_item, sizeof(USB_Online_item)/sizeof(struct Item),DISPLAY_MODE_1_COLUMN};
 
 struct PAGE USB_Online_Page_9600={&USB_Online_Page, usb_online_CallBack_B9600, 0, 0,DISPLAY_MODE_1_COLUMN};
 struct PAGE USB_Online_Page_19200={&USB_Online_Page, usb_online_CallBack_B19200, 0, 0,DISPLAY_MODE_1_COLUMN};
 struct PAGE USB_Online_Page_38400={&USB_Online_Page, usb_online_CallBack_B38400, 0, 0,DISPLAY_MODE_1_COLUMN};
 struct PAGE USB_Online_Page_115200={&USB_Online_Page, usb_online_CallBack_B115200, 0, 0,DISPLAY_MODE_1_COLUMN};
 
 struct PAGE USB_Program_Page={&USB_Page, usb_program_CallBack, 0, 0,DISPLAY_MODE_1_COLUMN};
 
 
/*************************************************BLE**************************************************************/

 struct PAGE Ble_Page={&mainPage,ble_CallBack,0,0,DISPLAY_MODE_0_COLUMN};
/**************************************************SPECIAL*************************************************************/
 struct Item Special_item[]={           (char*)"00.Prameter01",			&Para01,  0,  0,  0, SHOW_BOOL,
                                        (char*)"01.Prameter02",			&Para02,  0,  0,  0, SHOW_BOOL,
										(char*)"02.Prameter03",			&Para03,  0,  0,  0, SHOW_BOOL,
                                    };
 struct PAGE Para01 ={&Special_Page, para_01_callBack, 0, 0,DISPLAY_MODE_1_COLUMN};
 struct PAGE Para02 ={&Special_Page, para_02_callBack, 0, 0,DISPLAY_MODE_1_COLUMN};
 struct PAGE Para03 ={&Special_Page, para_03_callBack, 0, 0,DISPLAY_MODE_1_COLUMN};
 
 struct PAGE Special_Page={&mainPage,Setting_CallBack,Special_item,sizeof(Special_item)/sizeof(struct Item),DISPLAY_MODE_1_COLUMN};
/***************************************************************************************************************/
 struct Item Position_item[]={          (char*)"00.P500",			&Para500,  0,  0,  0, SHOW_BOOL,
                                        (char*)"01.P1000",			&Para1000,  0,  0,  0, SHOW_BOOL,
										(char*)"02.P1500",			&Para1500,  0,  0,  0, SHOW_BOOL,
										(char*)"03.P2000",			&Para2000,  0,  0,  0, SHOW_BOOL,
										(char*)"04.P2500",			&Para2500,  0,  0,  0, SHOW_BOOL,
                                    };
 struct PAGE Para500  ={&Position_Page, para_500_callBack, 0, 0,DISPLAY_MODE_1_COLUMN};
 struct PAGE Para1000 ={&Position_Page, para_1000_callBack, 0, 0,DISPLAY_MODE_1_COLUMN};
 struct PAGE Para1500 ={&Position_Page, para_1500_callBack, 0, 0,DISPLAY_MODE_1_COLUMN};
 struct PAGE Para2000 ={&Position_Page, para_2000_callBack, 0, 0,DISPLAY_MODE_1_COLUMN};
 struct PAGE Para2500 ={&Position_Page, para_2500_callBack, 0, 0,DISPLAY_MODE_1_COLUMN};
 
 struct PAGE Position_Page={&mainPage,Setting_CallBack,Position_item,sizeof(Position_item)/sizeof(struct Item),DISPLAY_MODE_1_COLUMN};
/****************************************INFO**************************************************************/

 struct PAGE Info_Page={&mainPage, info_CallBack, 0, 0, DISPLAY_MODE_0_COLUMN};
 
 
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
			break;
		case KEY_Return:
			ShowPage(&mainPage);
			break;
		case KEY_Ok:
			Lcd_Clr_Scr();
			ShowItemPage();
			break;
	}
}
/*----------------------------------
回调函数
----------------------------------*/
void para_500_callBack(u8 key)
{
	Lcd_Clr_Scr();
	menu_combine_position(500);
	//send data
	LCD_Write_Str(0,0,(char*)"-----Success!---");
	LCD_Write_Str(1,0,(char*)"-------500------");
	rt_thread_delay(1000);
	ShowParentPage();
}
void para_1000_callBack(u8 key)
{
	Lcd_Clr_Scr();
	//send data
	menu_combine_position(1000);
	LCD_Write_Str(0,0,(char*)"-----Success!---");
	LCD_Write_Str(1,0,(char*)"------1000------");
	rt_thread_delay(1000);
	ShowParentPage();
}
void para_1500_callBack(u8 key)
{
	Lcd_Clr_Scr();
	//send data
	menu_combine_position(1500);
	LCD_Write_Str(0,0,(char*)"-----Success!---");
	LCD_Write_Str(1,0,(char*)"------1500------");
	rt_thread_delay(1000);
	ShowParentPage();
}
void para_2000_callBack(u8 key)
{
	Lcd_Clr_Scr();
	//send data
	menu_combine_position(2000);
	LCD_Write_Str(0,0,(char*)"-----Success!---");
	LCD_Write_Str(1,0,(char*)"------2000------");
	rt_thread_delay(1000);
	ShowParentPage();
}
void para_2500_callBack(u8 key)
{
	Lcd_Clr_Scr();
	//send data
	menu_combine_position(2500);
	LCD_Write_Str(0,0,(char*)"-----Success!---");
	LCD_Write_Str(1,0,(char*)"------2500------");
	rt_thread_delay(1000);
	ShowParentPage();
}

/*----------------------------------
回调函数
----------------------------------*/
void para_01_callBack(u8 key)
{
	struct Servo_Data_Stru_ servo_data_t;
	
	LCD_Write_Str(0,0,(char*)"-Press ok write-!");
    LCD_Write_Str(1,0,(char*)"----------------");
    if (key ==KEY_Return){
        ShowParentPage();
    }
	else if(key ==KEY_Ok){
		Lcd_Clr_Scr();
		LCD_Write_Str(0,0,(char*)"-----Success!---");
		LCD_Write_Str(1,0,(char*)"-------OK-------");
		memcpy(&servo_data_t, &servoDataStru, sizeof(servoDataStru));
		memcpy(&servoDataStru, &para_01, sizeof(servoDataStru));
		menu_combine_prom_work_parm();
		memcpy(&servoDataStru, &servo_data_t, sizeof(servoDataStru));
		rt_thread_delay(2000);
	}
}
/*----------------------------------
回调函数
----------------------------------*/
void para_02_callBack(u8 key)
{
	struct Servo_Data_Stru_ servo_data_t;
	
	LCD_Write_Str(0,0,(char*)"-Press ok write-!");
    LCD_Write_Str(1,0,(char*)"----------------");
    if (key ==KEY_Return){
        ShowParentPage();
    }
	else if(key ==KEY_Ok){
		Lcd_Clr_Scr();
		LCD_Write_Str(0,0,(char*)"-----Success!---");
		LCD_Write_Str(1,0,(char*)"-------OK-------");
		memcpy(&servo_data_t, &servoDataStru, sizeof(servoDataStru));
		memcpy(&servoDataStru, &para_02, sizeof(servoDataStru));
		menu_combine_prom_work_parm();
		memcpy(&servoDataStru, &servo_data_t, sizeof(servoDataStru));
		rt_thread_delay(2000);
	}
}
/*----------------------------------
回调函数
----------------------------------*/
void para_03_callBack(u8 key)
{
	struct Servo_Data_Stru_ servo_data_t;
	
	LCD_Write_Str(0,0,(char*)"-Press ok write-!");
    LCD_Write_Str(1,0,(char*)"----------------");
    if (key ==KEY_Return){
        ShowParentPage();
    }
	else if(key ==KEY_Ok){
		Lcd_Clr_Scr();
		LCD_Write_Str(0,0,(char*)"-----Success!---");
		LCD_Write_Str(1,0,(char*)"-------OK-------");
		memcpy(&servo_data_t, &servoDataStru, sizeof(servoDataStru));
		memcpy(&servoDataStru, &para_03, sizeof(servoDataStru));
		menu_combine_prom_work_parm();
		memcpy(&servoDataStru, &servo_data_t, sizeof(servoDataStru));
		rt_thread_delay(2000);
	}
}
/*----------------------------------
回调函数
----------------------------------*/
void usb_online_CallBack_B9600(u8 key)
{
	usart1_init(9600);
	usart2_init_rx(9600);
    LCD_Write_Str(0,0,(char*)"-USB Online OK!");
    LCD_Write_Str(1,0,(char*)"-----9600------");
	usart_state = USB_SERIAL_TO_SERVO;
    if (key ==KEY_Return){
		usart_state = USB_SERIAL_DISABLE;
        ShowParentPage();
    }
}
/*----------------------------------
回调函数
----------------------------------*/
void usb_online_CallBack_B19200(u8 key)
{
	usart1_init(19200);
	usart2_init_rx(19200);
    LCD_Write_Str(0,0,(char*)"-USB Online OK!");
    LCD_Write_Str(1,0,(char*)"-----19200-----");
	usart_state = USB_SERIAL_TO_SERVO;
    if (key ==KEY_Return){
		usart_state = USB_SERIAL_DISABLE;
        ShowParentPage();
    }
}
/*----------------------------------
回调函数
----------------------------------*/
void usb_online_CallBack_B38400(u8 key)
{
	usart1_init(38400);
	usart2_init_rx(38400);
    LCD_Write_Str(0,0,(char*)"-USB Online OK!");
    LCD_Write_Str(1,0,(char*)"-----38400-----");
	usart_state = USB_SERIAL_TO_SERVO;
    if (key ==KEY_Return){
		usart_state = USB_SERIAL_DISABLE;
        ShowParentPage();
    }
}
/*----------------------------------
回调函数
----------------------------------*/
void usb_online_CallBack_B115200(u8 key)
{
	usart1_init(115200);
	usart2_init_rx(115200);
    LCD_Write_Str(0,0,(char*)"-USB Online OK!");
    LCD_Write_Str(1,0,(char*)"----115200----");
	usart_state = USB_SERIAL_TO_SERVO;
    if (key ==KEY_Return){
		usart_state = USB_SERIAL_DISABLE;
        ShowParentPage();
    }
}
/*----------------------------------
回调函数
----------------------------------*/
void usb_program_CallBack(u8 key)
{
	usart1_init(115200);
    LCD_Write_Str(0,0,(char*)"-USB Program OK!");
    LCD_Write_Str(1,0,(char*)"-----115200-----");
	usart_state = USB_SERIAL_PROGRAM;
    if (key ==KEY_Return){
		usart_state = USB_SERIAL_DISABLE;
        ShowParentPage();
    }
}
/*----------------------------------
回调函数
----------------------------------*/
void info_CallBack(u8 key)
{
    LCD_Write_Str(0,0,(char*)"-Information-");
    LCD_Write_Str(1,0,(char*)"-Version: V2.6");
    if (key ==KEY_Return){
        ShowParentPage();
    }
}
/*----------------------------------
回调函数
----------------------------------*/
void ble_CallBack(u8 key)
{
    LCD_Write_Str(0,0,(char*)"-BluetoothMode-");
	LCD_Write_Str(1,0,(char*)"---------------");
	usart_state = BLUETOOTH_MODE;
    if (key ==KEY_Return){
		usart_state = USB_SERIAL_DISABLE;
        ShowParentPage();
    }
}
/*----------------------------------
回调函数
----------------------------------*/
void clear_CallBack(u8 key)
{
    LCD_Write_Str(0,0,(char*)"-Press ok clear-");
	LCD_Write_Str(1,0,(char*)"----------------");
    if (key ==KEY_Return){
        ShowParentPage();
    }
	else if(key ==KEY_Ok){
		menu_combine_prom_work_clear_parm();
		LCD_Write_Str(0,0,(char*)"---Success!---");
		LCD_Write_Str(1,0,(char*)"-----OK-------");
		rt_thread_delay(2000);
		ShowParentPage();
	}
	
}
/*----------------------------------
回调函数
----------------------------------*/
void Setting_CallBack(u8 key)
{
	uint8_t i ;
    switch (key)
    {
        case KEY_UP:
        case KEY_Down:
		case KEY_UP_L:
		case KEY_Down_L:
            KeySelItem(key);
            break;
        case KEY_Return:
			usart_state = USB_SERIAL_DISABLE;
            ShowParentPage();
            break;
        case KEY_Ok:
			if(pPage == &Read_Page){
				return;
			}
			else if(pPage == &Setting_Page){ //read the serevo data
				if(Menu_GetSelItem() == 0){
					Read_servo_parameter();
					return;
				}
			}
			else if(pPage == &Write_Page){ //write the data to servo
				if(Menu_GetSelItem() == 20){
					Write_servo_parameter();
					return;
				}
			}
			else if(pPage == &Load_Page){ //load the flash data
				for(i=0; i<sizeof(Write_item)/sizeof(struct Item); i++){
					Write_item[i].state = 0;
				}
				Load_servo_parameter(Menu_GetSelItem());
				return;
			}
            ShowItemPage();
            break;
    }
}
/*----------------------------------
回调函数
----------------------------------*/
void Save_CallBack(u8 key)
{
	s8 index;
    char data[] = ">";
    char str[10] ={0};
    switch (key)
    {
        case KEY_UP:
			if(pPage->pItem[Menu_GetSelItem()].state ==1){
				index=Menu_GetSelItem();
				pPage->pItem[index].data ++;
				if(pPage->pItem[index].data > 39){
					pPage->pItem[index].data = 39;
					break;
				}
				if(pPage->pItem[index].data > 9){
					sprintf(str, "%d", pPage->pItem[index].data);
				}
				else{
					str[0] ='0';
					sprintf(&str[1], "%d", pPage->pItem[index].data);
				}
				SelItemOfListPara(index, str);
				break;
			}

			index=Menu_GetSelItem()-1;
			if(index<0){
				break;
			}
			SelItemOfList(index, data);
			break;
        case KEY_Down:
                //edit
                if(pPage->pItem[Menu_GetSelItem()].state ==1){
                    index=Menu_GetSelItem();
                    pPage->pItem[index].data --;
					if(pPage->pItem[index].data <0){
						pPage->pItem[index].data = 0;
						break;
					}
					if(pPage->pItem[index].data > 9){
						sprintf(str, "%d", pPage->pItem[index].data);
					}
					else{
						str[0] ='0';
						sprintf(&str[1], "%d", pPage->pItem[index].data);
					}
                    SelItemOfListPara(index, str);
                    break;
                }

                index=Menu_GetSelItem()+1;
                if(index>(pPage->ItemNum-1)){
					ShowPage(pPage);
                    break;
                }
                SelItemOfList(index, data);
                break;
        case KEY_Return:
            ShowParentPage();
            break;
        case KEY_Ok:
			if(Menu_GetSelItem() == 1){//save
				copy_data_to_stru();	
				save_servo_data_in_flash(Save_item[0].data);
				LCD_Write_Str(0,0,(char*)"-Save Success!-");
				LCD_Write_Str(1,0,(char*)"-----OK--------");
				rt_thread_delay(2000);
				pPage = &Save_Page;
				ShowPage(pPage);
				return;
			}
			ShowItemPage();
            break;
    }
}
//------------------------- datas to menu
void copy_data_to_read_menu(void)
{
	Read_item[0].data = servoDataStru.work_p12;
	Read_item[1].data = servoDataStru.work_p6;
	Read_item[2].data = servoDataStru.set_p11;
	Read_item[3].data = servoDataStru.set_p15;
	Read_item[4].data = servoDataStru.work_p2;
	Read_item[5].data = servoDataStru.work_p0;
	Read_item[6].data = servoDataStru.work_p5;
	Read_item[7].data = servoDataStru.work_p4;
	Read_item[8].data = servoDataStru.work_p3;
	Read_item[9].data = servoDataStru.set_p0;
	Read_item[10].data = servoDataStru.work_p8;
	Read_item[11].data = servoDataStru.work_p9;
	Read_item[12].data = servoDataStru.set_p12;
	Read_item[13].data = servoDataStru.set_p13;
	Read_item[14].data = servoDataStru.set_p6;
	Read_item[15].data = servoDataStru.set_p8;
	Read_item[16].data = servoDataStru.set_p10;
	Read_item[17].data = servoDataStru.debug_p0;
	Read_item[18].data = servoDataStru.debug_p2;
	Read_item[19].data = servoDataStru.debug_p5;
	
}

void copy_data_to_write_menu(void)
{
	Write_item[0].data = servoDataStru.work_p12;
	Write_item[1].data = servoDataStru.work_p6;
	Write_item[2].data = servoDataStru.set_p11;
	Write_item[3].data = servoDataStru.set_p15;
	Write_item[4].data = servoDataStru.work_p2;
	Write_item[5].data = servoDataStru.work_p0;
	Write_item[6].data = servoDataStru.work_p5;
	Write_item[7].data = servoDataStru.work_p4;
	Write_item[8].data = servoDataStru.work_p3;
	Write_item[9].data = servoDataStru.set_p0;
	Write_item[10].data = servoDataStru.work_p8;
	Write_item[11].data = servoDataStru.work_p9;
	Write_item[12].data = servoDataStru.set_p12;
	Write_item[13].data = servoDataStru.set_p13;
	Write_item[14].data = servoDataStru.set_p6;
	Write_item[15].data = servoDataStru.set_p8;
	Write_item[16].data = servoDataStru.set_p10;
	Write_item[17].data = servoDataStru.debug_p0;
	Write_item[18].data = servoDataStru.debug_p2;
	Write_item[19].data = servoDataStru.debug_p5;
}

// datas to menu
void copy_data_to_stru(void)
{
	servoDataStru.work_p12 = Write_item[0].data;
	servoDataStru.work_p6 = Write_item[1].data;
	servoDataStru.set_p11 = Write_item[2].data;
	servoDataStru.set_p15 = Write_item[3].data;
	servoDataStru.work_p2 = Write_item[4].data;
	
	servoDataStru.work_p0 = Write_item[5].data;
	servoDataStru.work_p5 = Write_item[6].data;
	servoDataStru.work_p4 = Write_item[7].data;
	servoDataStru.work_p3 = Write_item[8].data;
	servoDataStru.set_p0 = Write_item[9].data;
	servoDataStru.work_p8 = Write_item[10].data;
	servoDataStru.work_p9 = Write_item[11].data;
	servoDataStru.set_p12 = Write_item[12].data;
	servoDataStru.set_p13 = Write_item[13].data;
	servoDataStru.set_p6 = Write_item[14].data;
	servoDataStru.set_p8 = Write_item[15].data;
	servoDataStru.set_p10 = Write_item[16].data;
	servoDataStru.debug_p0 = Write_item[17].data;
	servoDataStru.debug_p2 = Write_item[18].data;
	servoDataStru.debug_p5 = Write_item[19].data;
	
	
}
//read servo parameter
void Read_servo_parameter(void)
{
	//lcd show reading
	Lcd_Clr_Scr();
	LCD_Write_Str(0,0,(char*)" ---Reading---");
    LCD_Write_Str(1,0,(char*)" -------------");

		//receive msg 
	if(menu_combine_fb_work_parm()){
		LCD_Write_Str(0,0,(char*)" ---Success!---");
		LCD_Write_Str(1,0,(char*)" -----OK-------");
		copy_data_to_read_menu();
		copy_data_to_write_menu();
		rt_thread_delay(2000);
		pPage = &Read_Page;
		ShowPage(pPage);
	}
	else{ //5s
		LCD_Write_Str(0,0,(char*)" ---Fail!---");
		LCD_Write_Str(1,0,(char*)" -----------");
		rt_thread_delay(2000);
		pPage = &Setting_Page;
		ShowPage(pPage);
	}
}

//write servo parameter
void Write_servo_parameter(void)
{
	//lcd show reading
	Lcd_Clr_Scr();
	LCD_Write_Str(0,0,(char*)"----Writing---");
    LCD_Write_Str(1,0,(char*)"--------------");
	copy_data_to_stru();
	menu_combine_prom_work_parm();
	LCD_Write_Str(0,0,(char*)"------Success!---");
	LCD_Write_Str(1,0,(char*)"--------OK-------");
	rt_thread_delay(2000);
	pPage = &Write_Page;
	ShowPage(pPage);
	//lcd back
}

//load servo parameter
void Load_servo_parameter(uint8_t data)
{
	//lcd show Loading
	Lcd_Clr_Scr();
	LCD_Write_Str(0,0,(char*)"----Loading---");
    LCD_Write_Str(1,0,(char*)"--------------");
	read_servo_data_in_flash(data, &servo_load_data);
	memcpy(&servoDataStru, &servo_load_data, sizeof(servoDataStru));
	copy_data_to_write_menu();
	pPage = &Write_Page;
	ShowPage(pPage);
	//lcd back
}
//-------------------------------------end line----------------------------------------
