/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#ifndef __LCD1602_H
#define	__LCD1602_H

#include "stm32f10x.h"
#include <stdio.h>

/********************���˿ڶ���*********************************/
#define GPIO_EN       GPIOB                 //  ʹ�ܶ˿���
#define GPIO_EN_PIN   GPIO_Pin_5            //  ʹ�ܶ˿ں�
#define RCC_GPIO_EN   RCC_APB2Periph_GPIOB  //  ʹ��ʱ����

#define GPIO_RW       GPIOB                 //  ��/дѡ��˿���
#define GPIO_RW_PIN   GPIO_Pin_0            //  ��/дѡ��˿ں�
#define RCC_GPIO_RW   RCC_APB2Periph_GPIOB  //  ��/дʱ����

#define GPIO_RS       GPIOB                 //  ����/����˿���
#define GPIO_RS_PIN   GPIO_Pin_1            //  ����/����˿ں�
#define RCC_GPIO_RS   RCC_APB2Periph_GPIOB  //  ����/����ʱ����

#define GPIO_DATA_0       GPIOB         //  ������0_�˿���
#define GPIO_DATA_0_PIN   GPIO_Pin_6    //  ������0_�˿ں�
#define GPIO_DATA_1       GPIOB         //  ������1_�˿���
#define GPIO_DATA_1_PIN   GPIO_Pin_7    //  ������1_�˿ں�
#define GPIO_DATA_2       GPIOB         //  ������2_�˿���
#define GPIO_DATA_2_PIN   GPIO_Pin_8   //  ������2_�˿ں�
#define GPIO_DATA_3       GPIOB         //  ������3_�˿���
#define GPIO_DATA_3_PIN   GPIO_Pin_9   //  ������3_�˿ں�
#define GPIO_DATA_4       GPIOB         //  ������4_�˿���
#define GPIO_DATA_4_PIN   GPIO_Pin_12   //  ������4_�˿ں�
#define GPIO_DATA_5       GPIOB         //  ������5_�˿���
#define GPIO_DATA_5_PIN   GPIO_Pin_13   //  ������5_�˿ں�
#define GPIO_DATA_6       GPIOB         //  ������6_�˿���
#define GPIO_DATA_6_PIN   GPIO_Pin_14   //  ������6_�˿ں�
#define GPIO_DATA_7       GPIOB         //  ������7_�˿���
#define GPIO_DATA_7_PIN   GPIO_Pin_15   //  ������7_�˿ں�
#define RCC_GPIO_DATA   RCC_APB2Periph_GPIOB  //  ������ʱ����

/******************************************************************/

/***********************����ָ��***********************************/
#define SET_EN  GPIO_SetBits(GPIO_EN, GPIO_EN_PIN)		//EN ʹ��  ���1
#define CLE_EN  GPIO_ResetBits(GPIO_EN, GPIO_EN_PIN)	//      ���0 
#define SET_RW  GPIO_SetBits(GPIO_RW, GPIO_RW_PIN)      //RW ��д  ���1
#define CLE_RW  GPIO_ResetBits(GPIO_RW, GPIO_RW_PIN)	//      ���0
#define SET_RS  GPIO_SetBits(GPIO_RS, GPIO_RS_PIN)		//RS ָ��  ���1
#define CLE_RS  GPIO_ResetBits(GPIO_RS, GPIO_RS_PIN)	//      ���0
/******************************************************************/


void Lcd_GPIO_init(void);  //�˿ڳ�ʼ��
void Lcd_Init( void );     //LCD��ʼ��
void Lcd_En_Toggle(void);  //��ʹ������
void Lcd_Busy(void);       //�ж�æ
void Gpio_data(unsigned char x);  //�˿�д����
void Lcd_Write_Command(unsigned char x,char Busy);  //д����
void Lcd_Write_Data(unsigned char x);               //д���� 
void Lcd_SetXY(unsigned char x,unsigned char y);   //λ���趨��x��ʾ�У�y��ʾ�� 
void Lcd_Puts(unsigned char x,unsigned char y, char *string); //��1602дһ���ַ��� 
unsigned char *tr_2(unsigned int outData_2);	 //�ѱ���ת��Ϊ2λ�ַ���ʾ
unsigned char *tr_3(unsigned int outData_3);	 //�ѱ���ת��Ϊ3λ�ַ���ʾ
unsigned char *tr_4(unsigned int outData_4);	 //�ѱ���ת��Ϊ4λ�ַ���ʾ
unsigned char *tr_5(unsigned int outData_5);	 //�ѱ���ת��Ϊ5λ�ַ���ʾ
unsigned char *tr_6(unsigned int outData_6);	 //�ѱ���ת��Ϊ6λ�ַ���ʾ
unsigned char *tr_7(unsigned int outData_7);	 //�ѱ���ת��Ϊ7λ�ַ���ʾ

#endif /* __LCD1602_H */
