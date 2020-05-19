#ifndef __MONIIIC_H__
#define __MONIIIC_H__

#include "stm32f10x.h"
#include "delay.h"
#include "system.h"


#define  IIC_SCL    PBout(6)   
#define  IIC_SDA    PBout(7)
#define  READ_SDA   PBin(7)

//IO方向设置  PB7
#define SDA_IN()  {GPIOB->CRL &= 0X0FFFFFFF;GPIOB->CRL |= 0X80000000;}//输入
/*前一句是将对应的低位寄存器清0，后一句是设置对应的输出 1:0 位设置对应的端口模式，3：2位设置对应的配置位*/
#define SDA_OUT() {GPIOB->CRL &= 0X0FFFFFFF;GPIOB->CRL |= 0X30000000;}//推挽复用输出

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	

#endif
