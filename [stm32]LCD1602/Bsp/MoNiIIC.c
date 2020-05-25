#include "moniiic.h"


//初始化IIC IO
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	  //开启GPIOB的外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	
	   
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;       //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
//	GPIO_SetBits(GPIOB,GPIO_Pin_6 | GPIO_Pin_7); 
	
	GPIO_InitStructure.GPIO_Pin = I2C_SCL | I2C_SDA;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;       //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_14 | GPIO_Pin_15); 
}

void SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Pin   =I2C_SDA;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode  =GPIO_Mode_Out_OD;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Pin   =I2C_SDA;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  =GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void Delay1us(uint16_t time)
{
	uint16_t i, j;
	for( i=0; i< time; i++)
		for(j=0; j<72; j++)
			;
}

//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出 PB10-SCL
	IIC_SDA = 1;	  	 //  PB11-SDA
	IIC_SCL = 1;    //将对应的SCL时钟信号为高电平，数据保持稳定
	Delay1us(4);
 	IIC_SDA = 0;//START:when CLK is high,DATA change form high to low 
	Delay1us(4);
	IIC_SCL = 0;//钳住I2C总线，准备发送或接收数据 
}

//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL = 0;
	IIC_SDA = 0;//STOP:when CLK is high DATA change form low to high
 	Delay1us(4);
	IIC_SCL = 1; 
	IIC_SDA = 1;//发送I2C总线结束信号
	Delay1us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA = 1;Delay1us(1);	   
	IIC_SCL = 1;Delay1us(1);	 
	while(READ_SDA)
	{
		ucErrTime ++;
		if(ucErrTime > 250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL = 0;//时钟输出0 	   
	return 0;  
}

//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL = 0;
	SDA_OUT();
	IIC_SDA = 0;
	Delay1us(2);
	IIC_SCL = 1;
	Delay1us(2);
	IIC_SCL = 0;
}

//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL = 0;
	SDA_OUT();
	IIC_SDA = 1;
	Delay1us(2);
	IIC_SCL = 1;
	Delay1us(2);
	IIC_SCL = 0;
}

//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL = 0;//拉低时钟开始数据传输
    for(t = 0;t < 8;t ++)
    {              
		if((txd&0x80) >> 7)
			IIC_SDA = 1;
		else
			IIC_SDA = 0;
		txd <<= 1; 	  
		Delay1us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL = 1;
		Delay1us(2); 
		IIC_SCL = 0;	
		Delay1us(2);
    }	 
}


//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive = 0;
	SDA_IN();//SDA设置为输入
    for(i = 0;i < 8;i ++ )
	{
        IIC_SCL = 0; 
        Delay1us(2);
		IIC_SCL = 1;
        receive <<= 1;
		Delay1us(2);
        if(READ_SDA)    receive ++;   
		Delay1us(1); 
  }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}






