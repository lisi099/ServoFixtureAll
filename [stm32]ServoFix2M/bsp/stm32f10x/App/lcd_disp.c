#include "lcd_driver.h"
#include "lcd_disp.h"


static unsigned int menu_disp_page_now;

void Lcd_Clr_Scr(void)
{
	char show_data0[16], show_data1[16];
	uint8_t i;
	for(i=0; i<16; i++){
		show_data0[i] = ' ';
		show_data1[i] = ' ';
	}
	put_chars(0, 0, show_data0);
	put_chars(0, 0, show_data1);
}

void LCD_disp_one_row (char* menu_disp_one_row_char, int16_t menu_disp_one_row_value)
{
	put_chars(0, 0, menu_disp_one_row_char);
}

void LCD_disp_two_row (char* menu_disp_two_row_char, int16_t menu_disp_one_row_value)
{
	put_chars(1, 0, menu_disp_two_row_char);
}

void LCD_menu_disp(unsigned int menu_disp_page_new,unsigned int disp_uprow_data ,unsigned int disp_dnrow_data)
{
	switch (menu_disp_page_new)
	{
		case MENU_DISP_LOGO:
			LCD_disp_one_row ((char*)"POWEHD",						0);  		//第一行显示字符
			LCD_disp_two_row ((char*)"WELCOME TO POWER", 	0);  		//第二行显示字符         
		break;	
		case MENU_DISP_CONNECT:
			LCD_disp_one_row ((char*)"CONNECT TO SERVO",	0);  		//第一行显示字符
			LCD_disp_two_row ((char*)"                ", 	0);  		//第二行显示字符
		break;	
		case MENU_DISP_CONNECT_OK:
			LCD_disp_one_row ((char*)"CONNECT TO SERVO",	0);  		//第一行显示字符
			LCD_disp_two_row ((char*)"CONNECT OK      ", 	0);  		//第二行显示字符
		break;
		case MENU_DISP_READING:
			LCD_disp_one_row ((char*)"READING.........",	0);  		//第一行显示字符
			LCD_disp_two_row ((char*)"                ",  0);  		//第二行显示字符

		break;	
		case MENU_DISP_CLASS_VER:
			LCD_disp_one_row ((char*)"*VER",							disp_uprow_data);  							//第一行显示字符
			LCD_disp_two_row ((char*)" MAX POWER", 				disp_uprow_data);  		//第二行显示字符
		break;	
		case MENU_DISP_CLASS_VER_EDIT:
			LCD_disp_one_row ((char*)"/VER",							disp_uprow_data);  							//第一行显示字符
			LCD_disp_two_row ((char*)" MAX POWER", 				disp_uprow_data);  		//第二行显示字符
		break;	
		case MENU_DISP_CLASS_MAXPOW:
			LCD_disp_one_row ((char*)" VER",							disp_uprow_data);  							//第一行显示字符
			LCD_disp_two_row ((char*)"*MAX POWER", 				disp_uprow_data);  		//第二行显示字符
		break;
		case MENU_DISP_CLASS_MAXPOW_EDIT:
			LCD_disp_one_row ((char*)" VER",							disp_uprow_data);  							//第一行显示字符
			LCD_disp_two_row ((char*)"/MAX POWER", 				disp_uprow_data);  		//第二行显示字符
		break;		
		case MENU_DISP_CLASS_BOOST:
			LCD_disp_one_row ((char*)"*BOOST    ",	      disp_uprow_data);  							//第一行显示字符
			LCD_disp_two_row ((char*)" DEAD BAND",        disp_uprow_data);  		//第二行显示字符
		break;
		case MENU_DISP_CLASS_BOOST_EDIT:
			LCD_disp_one_row ((char*)"/BOOST    ",	      disp_uprow_data);  							//第一行显示字符
			LCD_disp_two_row ((char*)" DEAD BAND",        disp_uprow_data);  		//第二行显示字符
		break;		
    case MENU_DISP_CLASS_DBAND:
			LCD_disp_one_row ((char*)" BOOST    ",	      disp_uprow_data);  							//第一行显示字符
			LCD_disp_two_row ((char*)"*DEAD BAND",        disp_uprow_data);  		//第二行显示字符
		break;		
		case MENU_DISP_CLASS_FORCE:
			LCD_disp_one_row ((char*)"*FORCE    ",	      disp_uprow_data);  							//第一行显示字符
			LCD_disp_two_row ((char*)" SENSITY  ",        disp_uprow_data);  		//第二行显示字符
		break;
    case MENU_DISP_CLASS_FORCE_EDIT:
			LCD_disp_one_row ((char*)"/FORCE    ",	      disp_uprow_data);  							//第一行显示字符
			LCD_disp_two_row ((char*)" SENSITY  ",        disp_uprow_data);  		//第二行显示字符
		break;			
		case MENU_DISP_CLASS_STERETC:
			LCD_disp_one_row ((char*)" FORCE    ",	      disp_uprow_data);  							//第一行显示字符
			LCD_disp_two_row ((char*)"*SENSITY  ",        disp_uprow_data);  		//第二行显示字符
		break;	
		case MENU_DISP_CLASS_STERETC_EDIT:
			LCD_disp_one_row ((char*)" FORCE    ",	      disp_uprow_data);  							//第一行显示字符
			LCD_disp_two_row ((char*)"/SENSITY  ",        disp_uprow_data);  		//第二行显示字符
		break;
		case MENU_DISP_CLASS_BRAKE:
			LCD_disp_one_row ((char*)"*BREAK",						disp_uprow_data);  							//第一行显示字符
			LCD_disp_two_row ((char*)" SOFTSTART", 				disp_uprow_data);  		//第二行显示字符
		break;
    case MENU_DISP_CLASS_BRAKE_EDIT:
			LCD_disp_one_row ((char*)"/BREAK",						disp_uprow_data);  							//第一行显示字符
			LCD_disp_two_row ((char*)" SOFTSTART", 				disp_uprow_data);  		//第二行显示字符
		break;			
		case MENU_DISP_CLASS_SFTSTA:
			LCD_disp_one_row ((char*)" BREAK",						disp_uprow_data);  							//第一行显示字符
			LCD_disp_two_row ((char*)"*SOFTSTART", 				disp_uprow_data);  		//第二行显示字符
		break;	
		case MENU_DISP_CLASS_SFTSTA_EDIT:
			LCD_disp_one_row ((char*)" BREAK",						disp_uprow_data);  							//第一行显示字符
			LCD_disp_two_row ((char*)"/SOFTSTART", 				disp_uprow_data);  		//第二行显示字符
		break;
		case MENU_DISP_CLASS_DATAWRITE:
			LCD_disp_one_row ((char*)"WRITEDATA ",				disp_uprow_data);  							//第一行显示字符
			LCD_disp_two_row ((char*)"          ",  			disp_uprow_data);  		//第二行显示字符
		break;
		case MENU_DISP_CLASS_WRITE_SERVO:
			LCD_disp_one_row ((char*)"SERVO     ",				disp_uprow_data);  							//第一行显示字符
			LCD_disp_two_row ((char*)"          ",  			disp_uprow_data);  		//第二行显示字符
		break;	
		case MENU_DISP_CLASS_WRITE_MEMORY:
			LCD_disp_one_row ((char*)"MEMORY    ",				disp_uprow_data);  							//第一行显示字符
			LCD_disp_two_row ((char*)"          ",  			disp_uprow_data);  		//第二行显示字符
		break;	
		case MENU_DISP_CALSS_WIRTEING:
			LCD_disp_one_row ((char*)"WIRTERING ",				disp_uprow_data);  							//第一行显示字符
			LCD_disp_two_row ((char*)"          ",  			disp_uprow_data);  		//第二行显示字符
		break;	
		case MENU_DISP_CALSS_WIRTE_FAIL:
			LCD_disp_one_row ((char*)"WIRTEFALI ",				disp_uprow_data);  							//第一行显示字符
			LCD_disp_two_row ((char*)"          ",  			disp_uprow_data);  		//第二行显示字符
		break;	
		case MENU_DISP_CALSS_WIRTE_SUCCESS:
			LCD_disp_one_row ((char*)"WRITESUCC ",				disp_uprow_data);  							//第一行显示字符
			LCD_disp_two_row ((char*)"          ",  			disp_uprow_data);  		//第二行显示字符
		break;	
		case MENU_DISP_CALSS_EEPROM_FILENANE:
			LCD_disp_one_row ((char*)"UDATA     ", 				disp_uprow_data);  							//第一行显示字符
			LCD_disp_two_row ((char*)"UDATA     ", 				disp_dnrow_data);  		//第二行显示字符
		break;	
		default:
		break;
	}
}