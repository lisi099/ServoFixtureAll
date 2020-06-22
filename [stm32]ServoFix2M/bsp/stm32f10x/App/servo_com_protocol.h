

#ifndef _MENU_COM_PROTOCOL_H_
#define _MENU_COM_PROTOCOL_H_

#include "stm32f10x.h"

//舵机指令码
#define		SERVO_COM_TEST_PULSE_UPPER     			210  //最大脉宽
#define 	SERVO_COM_TEST_PULSE_MID       			211  //中间脉宽
#define  	SERVO_COM_TEST_PULSE_LOWER     			212  //最小脉宽

#define  	SERVO_COM_READ_WORK_PARM       			220  //读取舵机工作参数
#define  	SERVO_COM_READ_DEBUG_PARM      			221  //读取舵机配置参数
#define  	SERVO_COM_READ_ALL_PARM        			222  //读取舵机所有参数
#define  	SERVO_COM_CHECKSUM _ALL_PARM      	223	 //读取校验舵机参数

#define  	SERVO_COM_WRITE_WORK_PARM       		230  //下载舵机工作参数
#define  	SERVO_COM_WRITE _DEBUG_PARM      		231  //下载舵机配置参数
#define  	SERVO_COM_PROGRAM _ALL_PARM        	232  //下载 读取 校验

#define  	SERVO_COM_SERVO_RESET_ORGINAL       240	 //复位舵机出厂参数
#define  	SERVO_COM_FEEDBACK_ALL_PARM         270	 //反馈液晶内存参数

#endif
