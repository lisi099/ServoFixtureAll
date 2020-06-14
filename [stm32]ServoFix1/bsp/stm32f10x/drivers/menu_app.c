/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#include "menu_app.h"
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
extern  struct PAGE None_Page;

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
extern  struct PAGE Load_Page;
extern  struct PAGE Save_Page;
extern  struct PAGE Clear_Page;
extern struct PAGE Program_Page;
extern struct PAGE Download_Page;
extern struct PAGE Verify_Page;

extern struct PAGE *pPage;

extern struct Servo_Data_Stru_ servoDataStru;

extern Usart_State usart_state;

struct Servo_Data_Stru_ servo_load_data;

uint8_t Item_Num_[2] ={0, 0};
/************************************************MAIN******************************************************/
//----1
 struct Item Main_item[]={
                                    (char*)"01.SERVOSETTING",       &Setting_Page,  	0, 0,   0, SHOW_BOOL,
									(char*)"02.SENDPOSITION",		&Position_Page,     0, 0,   0, SHOW_BOOL,
									(char*)"03.INFORMATION",		&Info_Page,     	0, 0,   0, SHOW_BOOL,
                                    (char*)"04.LCDSETTINGS",        &Special_Page,  	0, 0,   0, SHOW_BOOL,
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
										(char*)"08.SET DAT SAVE",				&Save_Page,  0,  0,  3, SHOW_BOOL,
										(char*)"09.SET DAT READ",				&Read_Page,  0,  0,  3, SHOW_BOOL,
										(char*)"10.RES DAT READ",				&Clear_Page,  0,  0,  3, SHOW_BOOL,
                                    };

 struct PAGE Setting_Page={&mainPage,Setting_CallBack,Setting_item,sizeof(Setting_item)/sizeof(struct Item),DISPLAY_MODE_1_COLUMN};
 //----3
 struct Item Read_item[]={           	(char*)"01.Ver",				0,  0,  11,  1, SHOW_U16,
										(char*)"02.DB",					0,  0,  11,  2, SHOW_U8,
                                    };
 struct PAGE Read_Page={&Setting_Page,read_CallBack,0,0,DISPLAY_MODE_1_COLUMN};

 //----3
 struct Item Save_item[]={           	(char*)"01.AZHD1501000",		0,  0,  13,  0, SHOW_U8,
										(char*)"02.SaveInFlash",		0,  0,  0,   0, SHOW_BOOL,
                                    };
 struct PAGE Save_Page={&Setting_Page, Save_CallBack, Save_item, sizeof(Save_item)/sizeof(struct Item), DISPLAY_MODE_1_COLUMN};
 
 //----2
 struct PAGE Program_Page	={&Setting_Page, program_CallBack, 0, 0, DISPLAY_MODE_1_COLUMN};
 struct PAGE Download_Page	={&Setting_Page, download_CallBack, 0, 0, DISPLAY_MODE_1_COLUMN};
 struct PAGE Verify_Page	={&Setting_Page, verify_CallBack, 0, 0, DISPLAY_MODE_1_COLUMN};
 struct PAGE Clear_Page		={&Setting_Page, clear_CallBack, 0, 0, DISPLAY_MODE_1_COLUMN};
 
/************************************************USB***************************************************************/
 //----2
  struct Item USB_Online_item[]={       (char*)"00.B9600",				&USB_Online_Page_9600,  0,  0,  0, SHOW_BOOL,
                                        (char*)"01.B19200",				&USB_Online_Page_19200,  0,  0,  0, SHOW_BOOL,
										(char*)"02.B38400",				&USB_Online_Page_38400,  0,  0,  0, SHOW_BOOL,
										(char*)"03.B115200",			&USB_Online_Page_115200,  0,  0,  0, SHOW_BOOL,
                                    };
 
 struct Item USB_item[]={               (char*)"00.USB online",				&USB_Online_Page,  0,  0,  0, SHOW_BOOL,
                                        (char*)"01.USB program",			&USB_Program_Page,  0,  0,  0, SHOW_BOOL,
                                    };
 struct PAGE USB_Page={&mainPage,Common_CallBack_rmb,USB_item,sizeof(USB_item)/sizeof(struct Item),DISPLAY_MODE_1_COLUMN};
 
 struct PAGE USB_Online_Page={&USB_Page, Common_CallBack, USB_Online_item, sizeof(USB_Online_item)/sizeof(struct Item),DISPLAY_MODE_1_COLUMN};
 
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
 
 struct PAGE Special_Page={&mainPage,Common_CallBack_rmb,Special_item,sizeof(Special_item)/sizeof(struct Item),DISPLAY_MODE_1_COLUMN};
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
 
 struct PAGE Position_Page={&mainPage,Common_CallBack_rmb,Position_item,sizeof(Position_item)/sizeof(struct Item),DISPLAY_MODE_1_COLUMN};
/****************************************INFO**************************************************************/

 struct PAGE Info_Page={&mainPage, info_CallBack, 0, 0, DISPLAY_MODE_0_COLUMN};
 
 struct PAGE None_Page={&mainPage, none_CallBack, 0, 0, DISPLAY_MODE_0_COLUMN};
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
            if((Menu_GetSelItem() == sizeof(Main_item)/sizeof(struct Item) -1)){
                return;
            }
            Lcd_Clr_Scr();
            ShowItemPage();
            Item_Num_[1] = 0;
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

void info_CallBack(u8 key)
{
    LCD_Write_Str(0,0,(char*)"-Information-");
    LCD_Write_Str(1,0,(char*)"-Version: V2.9");
    if (key ==KEY_Return){
        Lcd_Clr_Scr();
        ShowParentPage_Num(5);
    }
}

void none_CallBack(u8 key)
{
    (void*) (key);
}

void ble_CallBack(u8 key)
{
    LCD_Write_Str(0,0,(char*)"-BluetoothMode-");
	LCD_Write_Str(1,0,(char*)"---------------");
	usart_state = BLUETOOTH_MODE;
    if (key ==KEY_Return){
		usart_state = USB_SERIAL_DISABLE;
        Lcd_Clr_Scr();
        ShowParentPage_Num(2);
    }
}
/*----------------------------------
回调函数
----------------------------------*/
void read_CallBack(u8 key)
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
	}
	else{ //5s
		LCD_Write_Str(0,0,(char*)" ---Fail!---");
		LCD_Write_Str(1,0,(char*)" -----------");
	}
	rt_thread_delay(1500);
    ShowParentPage_Num(Item_Num_[1]);
}

void program_CallBack(u8 key)
{
	Lcd_Clr_Scr();
	LCD_Write_Str(0,0,(char*)"--Programing--");
    LCD_Write_Str(1,0,(char*)"--------------");
	copy_data_to_stru();
	menu_combine_prom_work_parm();
	
	if(menu_combine_verify_work_parm()){
		LCD_Write_Str(0,0,(char*)"--Program Ok!--");
		LCD_Write_Str(1,0,(char*)"--------------");
		rt_thread_delay(1500);
		ShowParentPage();
	}
	else{
		LCD_Write_Str(0,0,(char*)"----Erro!----");
		LCD_Write_Str(1,0,(char*)"-------------");
		rt_thread_delay(1500);
		ShowParentPage();
	}
}
void download_CallBack(u8 key)
{
	//lcd show reading
	Lcd_Clr_Scr();
	LCD_Write_Str(0,0,(char*)"--Downloading--");
    LCD_Write_Str(1,0,(char*)"--------------");
	copy_data_to_stru();
	menu_combine_prom_work_parm();
	LCD_Write_Str(0,0,(char*)"---Download Ok!---");
	LCD_Write_Str(1,0,(char*)"-----------------");
	rt_thread_delay(1500);
	ShowParentPage_Num(3);
}
void verify_CallBack(u8 key)
{
	Lcd_Clr_Scr();
	LCD_Write_Str(0,0,(char*)"---Verifying---");
	LCD_Write_Str(1,0,(char*)"----------------");
	if(menu_combine_verify_work_parm()){
		LCD_Write_Str(0,0,(char*)"--Verify Ok!--");
		LCD_Write_Str(1,0,(char*)"--------------");
	}
	else{
		LCD_Write_Str(0,0,(char*)"----Erro!----");
		LCD_Write_Str(1,0,(char*)"-------------");
	}
	rt_thread_delay(1500);
	ShowParentPage_Num(4);
}

void clear_CallBack(u8 key)
{
    LCD_Write_Str(0,0,(char*)"-Press ok clear-");
	LCD_Write_Str(1,0,(char*)"----------------");
    if (key ==KEY_Return){
        ShowParentPage_Num(Item_Num_[1]);
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
            if(Menu_GetSelItem() == sizeof(Setting_item)/sizeof(struct Item) -1){
                return;
            }
            ShowItemPage();
            break;
    }
}

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
            usart_state = USB_SERIAL_DISABLE;
            ShowParentPage();
            break;
        case KEY_Ok:
            ShowItemPage();
            break;
    }
}

void Common_CallBack_rmb(u8 key)
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

void Load_CallBack(u8 key)
{
//    uint8_t i ;
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
            ShowParentPage_Num(Item_Num_[1]);
            break;
        case KEY_Ok:
//            for(i=0; i<sizeof(Edit_item)/sizeof(struct Item); i++){
//                Edit_item[i].state = 0;
//            }
            Load_servo_parameter(Menu_GetSelItem());
            break;
    }
}

void Edit_CallBack(u8 key)
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
            usart_state = USB_SERIAL_DISABLE;
            ShowParentPage_Num(Item_Num_[1]);
            break;
        case KEY_Ok:
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
            ShowParentPage_Num(Item_Num_[1]);
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
//	Read_item[0].data = servoDataStru.work_p12;
//	Read_item[1].data = servoDataStru.work_p6;
//	Read_item[2].data = servoDataStru.set_p11;
//	Read_item[3].data = servoDataStru.set_p15;
//	Read_item[4].data = servoDataStru.work_p2;
//	Read_item[5].data = servoDataStru.work_p0;
//	Read_item[6].data = servoDataStru.work_p5;
//	Read_item[7].data = servoDataStru.work_p4;
//	Read_item[8].data = servoDataStru.work_p3;
//	Read_item[9].data = servoDataStru.set_p0;
//	Read_item[10].data = servoDataStru.work_p8;
//	Read_item[11].data = servoDataStru.work_p9;
//	Read_item[12].data = servoDataStru.set_p12;
//	Read_item[13].data = servoDataStru.set_p13;
//	Read_item[14].data = servoDataStru.set_p6;
//	Read_item[15].data = servoDataStru.set_p8;
//	Read_item[16].data = servoDataStru.set_p10;
//	Read_item[17].data = servoDataStru.debug_p0;
//	Read_item[18].data = servoDataStru.debug_p2;
//	Read_item[19].data = servoDataStru.debug_p5;
	
}

void copy_data_to_write_menu(void)
{
//    Edit_item[0].data = servoDataStru.work_p12;
//    Edit_item[1].data = servoDataStru.work_p6;
//    Edit_item[2].data = servoDataStru.set_p11;
//    Edit_item[3].data = servoDataStru.set_p15;
//    Edit_item[4].data = servoDataStru.work_p2;
//    Edit_item[5].data = servoDataStru.work_p0;
//    Edit_item[6].data = servoDataStru.work_p5;
//    Edit_item[7].data = servoDataStru.work_p4;
//    Edit_item[8].data = servoDataStru.work_p3;
//    Edit_item[9].data = servoDataStru.set_p0;
//    Edit_item[10].data = servoDataStru.work_p8;
//    Edit_item[11].data = servoDataStru.work_p9;
//    Edit_item[12].data = servoDataStru.set_p12;
//    Edit_item[13].data = servoDataStru.set_p13;
//    Edit_item[14].data = servoDataStru.set_p6;
//    Edit_item[15].data = servoDataStru.set_p8;
//    Edit_item[16].data = servoDataStru.set_p10;
//    Edit_item[17].data = servoDataStru.debug_p0;
//    Edit_item[18].data = servoDataStru.debug_p2;
//    Edit_item[19].data = servoDataStru.debug_p5;
}

// datas to menu
void copy_data_to_stru(void)
{
//    servoDataStru.work_p12 = Edit_item[0].data;
//    servoDataStru.work_p6 = Edit_item[1].data;
//    servoDataStru.set_p11 = Edit_item[2].data;
//    servoDataStru.set_p15 = Edit_item[3].data;
//    servoDataStru.work_p2 = Edit_item[4].data;
//	
//    servoDataStru.work_p0 = Edit_item[5].data;
//    servoDataStru.work_p5 = Edit_item[6].data;
//    servoDataStru.work_p4 = Edit_item[7].data;
//    servoDataStru.work_p3 = Edit_item[8].data;
//    servoDataStru.set_p0 = Edit_item[9].data;
//    servoDataStru.work_p8 = Edit_item[10].data;
//    servoDataStru.work_p9 = Edit_item[11].data;
//    servoDataStru.set_p12 = Edit_item[12].data;
//    servoDataStru.set_p13 = Edit_item[13].data;
//    servoDataStru.set_p6 = Edit_item[14].data;
//    servoDataStru.set_p8 = Edit_item[15].data;
//    servoDataStru.set_p10 = Edit_item[16].data;
//    servoDataStru.debug_p0 = Edit_item[17].data;
//    servoDataStru.debug_p2 = Edit_item[18].data;
//    servoDataStru.debug_p5 = Edit_item[19].data;
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
//	pPage = &Edit_Page;
//	ShowPage(pPage);
	//lcd back
}
//-------------------------------------end line----------------------------------------
