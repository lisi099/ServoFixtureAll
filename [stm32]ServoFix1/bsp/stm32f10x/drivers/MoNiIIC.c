#include "moniiic.h"

void Delay1us(uint16_t time)
{
	uint16_t i, j;
	for( i=0; i< time; i++)
		for(j=0; j<50; j++)
			;
}

//初始化IIC IO
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	  //开启GPIOB的外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;       //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	
	GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7);
	GPIO_SetBits(GPIOB, GPIO_Pin_5);	
	
}

/* Private typedef -----------------------------------------------------------*/
typedef enum {FALSE = 0, TRUE = !FALSE} bool; 


/* Private define ------------------------------------------------------------*/
#define I2C_SLAVE_ADDRESS7		0x3F 

#define I2C_SCL_H()     I2C_SCL = 1
#define I2C_SCL_L()     I2C_SCL = 0

#define I2C_SDA_H()     I2C_SDA = 1
#define I2C_SDA_L()     I2C_SDA = 0

/* Private macro -------------------------------------------------------------*/

void I2C_delay(void)
{
	u8 i=5;
	while(i--);
}

//??I2C??
bool I2C_Start(void)
{
	I2C_SDA_H();
	I2C_SCL_H();
	I2C_delay();
	I2C_SDA_L();
	I2C_delay();
	I2C_SDA_L();
	I2C_delay();
	return TRUE;
}

//??I2C??
void I2C_Stop(void)
{
	I2C_SCL_L();
	I2C_delay();
	I2C_SDA_L();
	I2C_delay();
	I2C_SCL_H();
	I2C_delay();
	I2C_SDA_H();
	I2C_delay();
}

//???????????
void I2C_SendAck( void)
{
	I2C_SCL_L();
	I2C_delay();
	I2C_SDA_L();
	I2C_delay();
	I2C_SCL_H();
	I2C_delay();
	I2C_SCL_L();
	I2C_delay();
}

//????????????,?????????????????,????NoACK???
//I2C read???????(??????????),?????ACK,??????????
//???,????????SDA??????0(SDA=0),?????STOP??(SDA????
//??),????????
void I2C_SendNoAck( void)
{
	I2C_SCL_L();
	I2C_delay();
	I2C_SDA_H();
	I2C_delay();
	I2C_SCL_H();
	I2C_delay();
	I2C_SCL_L();
	I2C_delay();
}

//???????????
bool I2C_WaitAck(void)	//???:=1?ACK,=0?ACK
{
	I2C_SCL_L();
	I2C_delay();
	I2C_SDA_H();                        
	I2C_delay();
	I2C_SCL_H();
	I2C_delay();
	I2C_SCL_L();
	return TRUE;
}

//????????
void I2C_SendByte(u8 SendByte)
{
	u8 i=8;
	while(i--)
	{
		I2C_SCL_L();
        I2C_delay();
      	if(SendByte&0x80)
			I2C_SDA_H();  
		else
			I2C_SDA_L();   
        SendByte<<=1;
        I2C_delay();
		I2C_SCL_H();
		I2C_delay();
	}
	I2C_SCL_L();
}

//????????
u8 I2C_ReceiveByte(void)  //????????//
{ 
	u8 i=8;
	u8 ReceiveByte=0;
	
    I2C_SDA_H();
    while(i--)
    {
		ReceiveByte<<=1;      
		I2C_SCL_L();
		I2C_delay();
		I2C_SCL_H();
		I2C_delay();
//		if(I2C_SDA_read)
//		{
//			ReceiveByte|=0x01;
//		}
	}
	I2C_SCL_L();
    return ReceiveByte;
}

/*******************************************************************************
* ?    ?: I2C_BufferWrite(u8* pBuffer, u8 WriteAddr, u16 NumByteToWrite)
* ?    ?: ??????????I2C???
* ????: - pBuffer         ?????
*           - WriteAddr       ?????
*           - NumByteToWrite  ??????
* ????: - TURE or FALSE   ??or??
* ?    ?:
* ????: I2C_BufferWrite(buf,0x00,8)
*******************************************************************************/
uint8_t I2C_BufferWrite(u8* pBuffer, u8 WriteAddr, u16 NumByteToWrite)
{
	if (!I2C_Start()) 
	{
		//printf("\r\nI2C_Start Fail!\r\n");
		return FALSE;
	}
	I2C_SendByte((I2C_SLAVE_ADDRESS7<<1) | 0);	//I2C???+???
	if (!I2C_WaitAck())
	{
		I2C_Stop();
		return FALSE;
	}
	I2C_SendByte(WriteAddr);					//?????????
	I2C_WaitAck();
	while(NumByteToWrite--)
	{
		I2C_SendByte(* pBuffer);
		I2C_WaitAck();
		pBuffer++;
	}
	I2C_Stop();
	return TRUE;
}

/*******************************************************************************
* ?    ?: I2C_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead)
* ?    ?: ??I2C???????????
* ????: - pBuffer         ?????
*           - ReadAddr        ???
*           - NumByteToRead   ??????
* ????: - TURE or FALSE   ??or??
* ?    ?:
* ????: I2C_BufferRead(buf,0x00,16)
*******************************************************************************/
uint8_t I2C_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead)
{
	if (!I2C_Start()) return FALSE;
	I2C_SendByte((I2C_SLAVE_ADDRESS7<<1) | 0);	//I2C???+???
	if (!I2C_WaitAck()) 
	{
		I2C_Stop(); 
		return FALSE;
	}
    I2C_SendByte(ReadAddr);						//?????????
	I2C_WaitAck();
	I2C_Stop(); 
	I2C_Start();
	I2C_SendByte((I2C_SLAVE_ADDRESS7<<1) | 0x01);	//I2C???+???
	I2C_WaitAck();
	while(NumByteToRead)
	{
		*pBuffer = I2C_ReceiveByte();
		if(NumByteToRead == 1)
			I2C_SendNoAck();
		else
			I2C_SendAck();
		pBuffer++;
		NumByteToRead--;
	}
	I2C_Stop();
	return TRUE;
}



/* -------------------------------- END --------------------------------------*/



