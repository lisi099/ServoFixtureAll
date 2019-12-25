/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#ifndef __LCD1602_H
#define	__LCD1602_H

#include "stm32f10x.h"
#include <stdio.h>

/********************各端口定义*********************************/
#define GPIO_EN       GPIOB                 //  使能端口组
#define GPIO_EN_PIN   GPIO_Pin_5            //  使能端口号
#define RCC_GPIO_EN   RCC_APB2Periph_GPIOB  //  使能时钟组

#define GPIO_RW       GPIOB                 //  读/写选择端口组
#define GPIO_RW_PIN   GPIO_Pin_0            //  读/写选择端口号
#define RCC_GPIO_RW   RCC_APB2Periph_GPIOB  //  读/写时钟组

#define GPIO_RS       GPIOB                 //  数据/命令端口组
#define GPIO_RS_PIN   GPIO_Pin_1            //  数据/命令端口号
#define RCC_GPIO_RS   RCC_APB2Periph_GPIOB  //  数据/命令时钟组

#define GPIO_DATA_0       GPIOB         //  数据线0_端口组
#define GPIO_DATA_0_PIN   GPIO_Pin_6    //  数据线0_端口号
#define GPIO_DATA_1       GPIOB         //  数据线1_端口组
#define GPIO_DATA_1_PIN   GPIO_Pin_7    //  数据线1_端口号
#define GPIO_DATA_2       GPIOB         //  数据线2_端口组
#define GPIO_DATA_2_PIN   GPIO_Pin_8   //  数据线2_端口号
#define GPIO_DATA_3       GPIOB         //  数据线3_端口组
#define GPIO_DATA_3_PIN   GPIO_Pin_9   //  数据线3_端口号
#define GPIO_DATA_4       GPIOB         //  数据线4_端口组
#define GPIO_DATA_4_PIN   GPIO_Pin_12   //  数据线4_端口号
#define GPIO_DATA_5       GPIOB         //  数据线5_端口组
#define GPIO_DATA_5_PIN   GPIO_Pin_13   //  数据线5_端口号
#define GPIO_DATA_6       GPIOB         //  数据线6_端口组
#define GPIO_DATA_6_PIN   GPIO_Pin_14   //  数据线6_端口号
#define GPIO_DATA_7       GPIOB         //  数据线7_端口组
#define GPIO_DATA_7_PIN   GPIO_Pin_15   //  数据线7_端口号
#define RCC_GPIO_DATA   RCC_APB2Periph_GPIOB  //  数据线时钟组

/******************************************************************/

/***********************基本指令***********************************/
#define SET_EN  GPIO_SetBits(GPIO_EN, GPIO_EN_PIN)		//EN 使能  输出1
#define CLE_EN  GPIO_ResetBits(GPIO_EN, GPIO_EN_PIN)	//      输出0 
#define SET_RW  GPIO_SetBits(GPIO_RW, GPIO_RW_PIN)      //RW 读写  输出1
#define CLE_RW  GPIO_ResetBits(GPIO_RW, GPIO_RW_PIN)	//      输出0
#define SET_RS  GPIO_SetBits(GPIO_RS, GPIO_RS_PIN)		//RS 指令  输出1
#define CLE_RS  GPIO_ResetBits(GPIO_RS, GPIO_RS_PIN)	//      输出0
/******************************************************************/


void Lcd_GPIO_init(void);  //端口初始化
void Lcd_Init( void );     //LCD初始化
void Lcd_En_Toggle(void);  //发使能脉冲
void Lcd_Busy(void);       //判断忙
void Gpio_data(unsigned char x);  //端口写数据
void Lcd_Write_Command(unsigned char x,char Busy);  //写命令
void Lcd_Write_Data(unsigned char x);               //写数据 
void Lcd_SetXY(unsigned char x,unsigned char y);   //位置设定，x表示列，y表示行 
void Lcd_Puts(unsigned char x,unsigned char y, char *string); //向1602写一个字符串 
unsigned char *tr_2(unsigned int outData_2);	 //把变量转换为2位字符显示
unsigned char *tr_3(unsigned int outData_3);	 //把变量转换为3位字符显示
unsigned char *tr_4(unsigned int outData_4);	 //把变量转换为4位字符显示
unsigned char *tr_5(unsigned int outData_5);	 //把变量转换为5位字符显示
unsigned char *tr_6(unsigned int outData_6);	 //把变量转换为6位字符显示
unsigned char *tr_7(unsigned int outData_7);	 //把变量转换为7位字符显示

#endif /* __LCD1602_H */
