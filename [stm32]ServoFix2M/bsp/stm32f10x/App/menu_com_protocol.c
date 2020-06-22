
#include "menu_com_protocol.h"
#include "key_check.h"
#include "lcd_disp.h"
#include <rtthread.h>
unsigned int  menu_command_value_now = 0;           //界面的指令码
unsigned char menu_disp_menu_now 	= MENU_DISP_NULL;
unsigned char menu_disp_menu_last 	= MENU_DISP_NULL;

unsigned int menu_disp_protocol(unsigned char key_value_now, unsigned char menu_disp_page_now)//完成反馈 OK/ERROR	
{
	switch(menu_disp_menu_now)//MENU_DISP_LOGO 
	{
		case MENU_DISP_NULL:
		  rt_thread_delay(30);
		  menu_disp_menu_last  = menu_disp_menu_now;
		  menu_disp_menu_now   = MENU_DISP_LOGO;
		  LCD_menu_disp(menu_disp_menu_now,0,0);
		  menu_command_value_now = 0;
		break;
		////////////////////////////////////////////////////////////
		case MENU_DISP_LOGO:
		  menu_disp_menu_last  = menu_disp_menu_now;
		  menu_disp_menu_now   = MENU_DISP_CONNECT;
		  LCD_menu_disp(menu_disp_menu_now,0,0);
			rt_thread_delay(2000);
		break;
		////////////////////////////////////////////////////////////
		case MENU_DISP_CONNECT:
			switch(key_value_now)
			{
				case KEY_BUTTON_OK:
					menu_disp_menu_last  = menu_disp_menu_now;
					menu_disp_menu_now   = MENU_DISP_CONNECT_OK;
					LCD_menu_disp(menu_disp_menu_now,0,0);
				
				//执行对变量处理等操作等
				//例如 结合当前界面码 和键值自动生成界面操作命令码 menu_command_value_now = MENU_COM_SERVO_TEST_PULSE_UPPER;
				break;
				case KEY_BUTTON_ESC:
					menu_disp_menu_last  = menu_disp_menu_now;
					menu_disp_menu_now   = MENU_DISP_CLASS_SETTING;
					LCD_menu_disp(menu_disp_menu_now,0,0);
				break;
				case KEY_BUTTON_UP:
				break;
				case KEY_BUTTON_DOWN:
				break;
				default:					
				break;		
			}
		break;
		////////////////////////////////////////////////////////////
		case MENU_DISP_CONNECT_OK:
			switch(key_value_now)
			{
				case KEY_BUTTON_OK:
					menu_disp_menu_last  = menu_disp_menu_now;
					menu_disp_menu_now   = MENU_DISP_READING;
				  LCD_menu_disp(menu_disp_menu_now,0,0);
				break;
				case KEY_BUTTON_ESC:
				break;
				case KEY_BUTTON_UP:
				break;
				case KEY_BUTTON_DOWN:
				break;
				default:
				break;		
			}
		break;
		////////////////////////////////////////////////////////////
		case MENU_DISP_READING:
			rt_thread_delay(30);
		  menu_disp_menu_last  = menu_disp_menu_now;
		  menu_disp_menu_now   = MENU_DISP_CLASS_VER;
		  LCD_menu_disp(menu_disp_menu_now,0,0);
		break;
		////////////////////////////////////////////////////////////
		case MENU_DISP_CLASS_VER:
			switch(key_value_now)
			{
				case KEY_BUTTON_OK:
					menu_disp_menu_last  = menu_disp_menu_now;
					menu_disp_menu_now   = MENU_DISP_CLASS_VER_EDIT;
				  LCD_menu_disp(menu_disp_menu_now,0,0);
				break;
				case KEY_BUTTON_ESC:
				break;
				case KEY_BUTTON_UP:
				break;
				case KEY_BUTTON_DOWN:
					menu_disp_menu_last  = menu_disp_menu_now;
					menu_disp_menu_now   = MENU_DISP_CLASS_MAXPOW;
				  LCD_menu_disp(menu_disp_menu_now,0,0);
				break;
				default:
				break;		
			}
		break;
		////////////////////////////////////////////////////////////
		case MENU_DISP_CLASS_VER_EDIT:
			switch(key_value_now)
			{
				case KEY_BUTTON_OK:
				break;
				case KEY_BUTTON_ESC:
				break;
				case KEY_BUTTON_UP:
				break;
				case KEY_BUTTON_DOWN:
					menu_disp_menu_last  = menu_disp_menu_now;
					menu_disp_menu_now   = MENU_DISP_CLASS_VER;
				  LCD_menu_disp(menu_disp_menu_now,0,0);
				break;
				default:
				break;		
			}
		break;	
		////////////////////////////////////////////////////////////
		case MENU_DISP_CLASS_MAXPOW:
			switch(key_value_now)
			{
				case KEY_BUTTON_OK:
					menu_disp_menu_last  = menu_disp_menu_now;
					menu_disp_menu_now   = MENU_DISP_CLASS_MAXPOW_EDIT;
				  LCD_menu_disp(menu_disp_menu_now,0,0);
				break;
				case KEY_BUTTON_ESC:
				break;
				case KEY_BUTTON_UP:
				break;
				case KEY_BUTTON_DOWN:
					menu_disp_menu_last  = menu_disp_menu_now;
					menu_disp_menu_now   = MENU_DISP_CLASS_BOOST;
				  LCD_menu_disp(menu_disp_menu_now,0,0);
				break;
				default:
				break;		
			}
		break;
		////////////////////////////////////////////////////////////
		case MENU_DISP_CLASS_MAXPOW_EDIT:
			switch(key_value_now)
			{
				case KEY_BUTTON_OK:
				break;
				case KEY_BUTTON_ESC:
					menu_disp_menu_last  = menu_disp_menu_now;
					menu_disp_menu_now   = MENU_DISP_CLASS_MAXPOW;
				  LCD_menu_disp(menu_disp_menu_now,0,0);
				break;
				case KEY_BUTTON_UP:
				break;
				case KEY_BUTTON_DOWN:

				break;
				default:
				break;		
			}
		break;
			
			
			
			
		default:
		break;
		
	}
	return 0;
}
