#ifndef __MONIIIC_H__
#define __MONIIIC_H__

#include "stm32f10x.h"
#include "delay.h"
#include "system.h"


#define  IIC_SCL    PBout(6)   
#define  IIC_SDA    PBout(7)
#define  READ_SDA   PBin(7)

//IO��������  PB7
#define SDA_IN()  {GPIOB->CRL &= 0X0FFFFFFF;GPIOB->CRL |= 0X80000000;}//����
/*ǰһ���ǽ���Ӧ�ĵ�λ�Ĵ�����0����һ�������ö�Ӧ����� 1:0 λ���ö�Ӧ�Ķ˿�ģʽ��3��2λ���ö�Ӧ������λ*/
#define SDA_OUT() {GPIOB->CRL &= 0X0FFFFFFF;GPIOB->CRL |= 0X30000000;}//���츴�����

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	

#endif
