
#ifndef _MENU_COM_PROTOCOL_H_
#define _MENU_COM_PROTOCOL_H_

#include "stm32f10x.h"

#define		MENU_COM_SERVO_TEST_PULSE_UPPER     		10  //运行舵机到最大脉宽
#define		MENU_COM_SERVO_TEST_PULSE_MID       		11  //运行舵机到中间脉宽
#define		MENU_COM_SERVO_TEST_PULSE_LOWER     		12  //运行舵机到最小脉宽

#define 	MENU_COM_SERVO_READ_WORK_PARM       		20  //读取舵机工作参数
#define  	MENU_COM_SERVO_READ_DEBUG_PARM      		21  //读取舵机配置参数
#define  	MENU_COM_SERVO_READ_ALL_PARM        		22  //读取舵机所有参数
#define  	MENU_COM_SERVO_CHECKSUM_ALL_PARM      	23	//读取校验舵机参数

#define  	MENU_COM_SERVO_WRITE_WORK_PARM       		30  //下载舵机工作参数
#define  	MENU_COM_SERVO_WRITE_DEBUG_PARM      		31  //下载舵机配置参数
#define  	MENU_COM_SERVO_PROGRAM_ALL_PARM        	32 	//下载 读取 校验

#define  	MENU_COM_MEMORY_READ_WORK_PARM       		40  //读取内存工作参数
#define  	MENU_COM_MEMORY_READ_DEBUG_PARM      		41  //读取内存配置参数
#define  	MENU_COM_MEMORY_READ_ALL_PARM        		42  //读取内存所有参数
#define  	MENU_COM_MEMORY_CHECKSUM_ALL_PARM      	43	//读取校验内存参数

#define  	MENU_COM_MEMORY_WRITE_WORK_PARM       	50  //下载内存工作参数
#define  	MENU_COM_MEMORY_WRITE_DEBUG_PARM      	51  //下载内存配置参数
#define  	MENU_COM_MEMORY_PROGRAM_ALL_PARM       	52	//下载 读取 校验

#define  	MENU_COM_SERVO_RESET_FACTORY            60	//复位舵机客户参数
#define  	MENU_COM_SERVO_RESET_ORGINAL            61	//复位舵机出厂参数


unsigned int menu_disp_protocol(unsigned char key_value_now, unsigned char menu_disp_page_now);//完成反馈 OK/ERROR	

#endif

