
#ifndef	_LCD_DISP_H_
#define _LCD_DISP_H_

#include "stm32f10x.h"

#define MENU_DISP_NULL          								00
#define MENU_DISP_LOGO          								01
#define MENU_DISP_CONNECT       								11
#define MENU_DISP_CONNECT_OK       							12
#define MENU_DISP_READING       								13
#define MENU_DISP_WRITING       								14
#define MENU_DISP_OK            								15
#define MENU_DISP_ERROR         								16

#define MENU_DISP_FILE_NAME 									250    //显示文件名
#define MENU_DISP_DATA_VALUE									251    //显示数值

#define MENU_DISP_CLASS_SETTING    						    100
#define MENU_DISP_CLASS_VER    						        110
#define MENU_DISP_CLASS_VER_EDIT    						  115
#define MENU_DISP_CLASS_MAXPOW             				120
#define MENU_DISP_CLASS_MAXPOW_EDIT             	125
#define MENU_DISP_CLASS_BOOST             				130
#define MENU_DISP_CLASS_BOOST_EDIT             		135
#define MENU_DISP_CLASS_DBAND             				140
#define MENU_DISP_CLASS_DBAND_EDIT             		145
#define MENU_DISP_CLASS_FORCE             				150
#define MENU_DISP_CLASS_FORCE_EDIT             		155
#define MENU_DISP_CLASS_STERETC             			160
#define MENU_DISP_CLASS_STERETC_EDIT             	165
#define MENU_DISP_CLASS_BRAKE            				  170
#define MENU_DISP_CLASS_BRAKE_EDIT            		175
#define MENU_DISP_CLASS_SFTSTA             				180
#define MENU_DISP_CLASS_SFTSTA_EDIT             	185

#define MENU_DISP_CLASS_DATAWRITE             		200
#define MENU_DISP_CALSS_WIRTEING                  201
#define MENU_DISP_CLASS_WRITE_SERVO             	202
#define MENU_DISP_CLASS_WRITE_MEMORY             	203
#define MENU_DISP_CALSS_WIRTE_FAIL                204
#define MENU_DISP_CALSS_WIRTE_SUCCESS             205
#define MENU_DISP_CALSS_EEPROM_FILENANE           206


#define MENU_DISP_CLASS_DATAREAD             			210
#define MENU_DISP_CLASS_READ_SERVO             			211
#define MENU_DISP_CLASS_READ_MEMORY             		212

#define MENU_DISP_CLASS_DATARESET             			220
#define MENU_DISP_CLASS_RESET_SERVO             		221
#define MENU_DISP_CLASS_RESET_MEMORY             		222

#define MENU_DISP_CLASS_POSITION    					230
#define MENU_DISP_CLASS_PULSE_MID    					231
#define MENU_DISP_CLASS_PULSE_UPPER    					232
#define MENU_DISP_CLASS_PULSE_LOWER    					233

#define MENU_DISP_CALSS_INFORMATION                    	240
#define MENU_DISP_CALSS_LCD_VERSION                    	241
#define MENU_DISP_CALSS_LDC_UPGRADE                   	242
#define MENU_DISP_CALSS_SERVO_VERSION                  	243

void LCD_menu_disp(unsigned int menu_disp_page_new,unsigned int disp_uprow_data ,unsigned int disp_dnrow_data);

#endif
