#include "1602_iic_sw.h"
#include <rtthread.h>

#define DelayMS(x) rt_thread_delay(x)



//           SPLC792A 模拟I2C测试程序
//***************************************************************************
//连线表:  CPU=STC11L60XE       Fosc=22.1184Mhz                             *
//***************************************************************************
//         SCL  SDA 须接4.7K上拉电阻                                        *
//***************************************************************************

//写指令函数
void WriteCommand(unsigned char CommandByte)
{
	unsigned char ControlByte=0x00;		//Control Byte,(Co)(A0)000000(A) Co=0 A0=0=后面的I2C数据是显示指令
										//Co=0是表示这是最后一个Control Byte(此处的指令意思为I2C总线控制命令,非显示指令)
										//后面的I2C数据是显示指令
	I2C_BufferWrite(&CommandByte, ControlByte, 1);
}

//写数据函数
void WriteData(unsigned char *pDAT,unsigned char length)
{
	unsigned char ControlByte=0x40;		//Control Byte,(Co)(A0)000000(A) Co=0 A0=1=后面的I2C数据是显示数据
										//Co=0是表示这是最后一个Control Byte(此处的指令意思为I2C总线控制命令,非显示指令)						
	I2C_BufferWrite(pDAT, ControlByte, length);
}


//初始化
void LcdInit(void)
{
	IIC_Init();
	DelayMS(50);
//	#define CONTRAST 20
//	WriteCommand(0x38);	//Function set:8bit,2-line,IS=0
//	DelayMS(1);
//	WriteCommand(0x39);	//Function set:8bit,2-line,IS=1
//	DelayMS(1);
//	WriteCommand(0x50+(CONTRAST>>4));	//ICON OFF,bit[1:0]=Contrast bit[5:4]
//	DelayMS(1);
//	WriteCommand(0x70+(CONTRAST&0x0F));	//Contrast = 101000b
//	DelayMS(1);
//	WriteCommand(0x6B);	//Follower control ON,Rab=011b,即使是外供VLCD电压,此处V0还是受限于Rab这个曲线图
//	DelayMS(100);
//	WriteCommand(0x0C);	//Display ON,Cursor off,Blink off
//	DelayMS(1);
//	WriteCommand(0x01);	//Clear Display
//	DelayMS(2);
//	WriteCommand(0x06);	//Entry Mode Set
//	DelayMS(1);
	WriteCommand(0x38); /*function select*/ 
	WriteCommand(0x01); /*clear screen*/ 
	DelayMS(5); 
	WriteCommand(0x06); /*setdisplay mode*/ 
	DelayMS(5); 
	WriteCommand(0x0c); /*turn on display*/ 
	DelayMS(5); 
	WriteCommand(0x39); /*extension instruction*/ 
	WriteCommand(0x1c); // 
	DelayMS(5); 
	WriteCommand(0x6d); // 
	DelayMS(5); 
	WriteCommand(0x55); //粗调对比度,范围 0x54-0x57 
	DelayMS(5); 
	WriteCommand(0x7a); //微调对比度,范围 0x70-0x7f 
	DelayMS(5);
}

void SetCGRAM( unsigned char *puts )
{
	WriteCommand(0x38);	//Function set:8bit,2-line,IS=0
	DelayMS(1);
	WriteCommand(0x40);		//CGRAM start Address
	DelayMS(10);
	WriteData(puts,64);
}

void PutStr(unsigned char *puts)
{
	WriteCommand(0x80);	//AC=00H
	DelayMS(1);
	WriteData(puts,16);
	WriteCommand(0xC0);	//AC=40H
	DelayMS(1);
	WriteData(puts,16);
}



