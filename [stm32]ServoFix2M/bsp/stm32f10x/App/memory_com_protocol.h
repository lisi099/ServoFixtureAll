
#ifndef _MEM_COM_PROTOCOL_H_
#define _MEM_COM_PROTOCOL_H_

#include "stm32f10x.h"

#define  MEMORY_COM_READ_WORK_PARM       	340  //读取内存工作参数
#define  MEMORY_COM_READ_DEBUG_PARM      	341  //读取内存配置参数
#define  MEMORY_COM_READ_ALL_PARM        	342  //读取内存所有参数
#define  MEMORY_COM_CHECKSUM_ALL_PARM     343	 //读取校验内存参数

#define  MEMORY_COM_WRITE_WORK_PARM       350  //下载内存工作参数
#define  MEMORY_COM_WRITE_DEBUG_PARM      351  //下载内存配置参数
#define  MEMORY_COM_PROGRAM_ALL_PARM      352	 //下载 读取 校验

#endif