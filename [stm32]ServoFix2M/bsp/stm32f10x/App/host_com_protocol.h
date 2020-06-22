
#ifndef _HOST_COM_PROTOCOL_
#define _HOST_COM_PROTOCOL_

#include "stm32f10x.h"

#define		HOST_COM_SERVO_TEST_PULSE_UPPER     				110  //最大脉宽
#define  	HOST_COM_SERVO_TEST_PULSE_MID       				111  //中间脉宽
#define  	HOST_COM_SERVO_TEST_PULSE_LOWER     				112  //最小脉宽

#define  	HOST_COM_SERVO_READ_WORK_PARM       				120  //读取舵机工作参数
#define  	HOST_COM_SERVO_READ_DEBUG_PARM      				121  //读取舵机配置参数
#define  	HOST_COM_SERVO_READ_ALL_PARM        				122  //读取舵机所有参数
#define  	HOST_COM_SERVO_CHECKSUM _ALL_PARM      			123	 //读取校验舵机参数

#define  	HOST_COM_SERVO_WRITE_WORK_PARM       				130  //下载舵机工作参数
#define  	HOST_COM_SERVO_WRITE _DEBUG_PARM      			131  //下载舵机配置参数
#define  	HOST_COM_SERVO_PROGRAM _ALL_PARM        		132  //下载 读取 校验
#define  	HOST_COM_MEMORY_READ_WORK_PARM       				140  //读取内存工作参数
#define  	HOST_COM_MEMORY_READ_DEBUG_PARM      				141  //读取内存配置参数
#define  	HOST_COM_MEMORY_READ_ALL_PARM        				142  //读取内存所有参数
#define  	HOST_COM_MEMORY_CHECKSUM _ALL_PARM      		143	 //读取校验内存参数

#define  	HOST_COM_MEMORY_WRITE_WORK_PARM       			150  //下载内存工作参数
#define  	HOST_COM_MEMORY_WRITE _DEBUG_PARM      			151  //下载内存配置参数
#define  	HOST_COM_MEMORY_PROGRAM _ALL_PARM       		152	 //下载 读取 校验

#define  	HOST_COM_SERVO_RESET_FACTORY                160	 //复位舵机客户参数
#define  	HOST_COM_SERVO_RESET_ORGINAL                161	 //复位舵机出厂参数

#define  	HOST_COM_FEEDBACK_ALL_PARM                  170	 //反馈液晶内存参数


#endif