
#include "At24c128.h"


void IIC_EEInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	   
	GPIO_InitStructure.GPIO_Pin = IIC_SCL_PIN | IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(IIC_SDA_PORT, &GPIO_InitStructure);
	GPIO_SetBits(IIC_SDA_PORT, IIC_SCL_PIN | IIC_SDA_PIN); 
}

void SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Pin   =IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode  =GPIO_Mode_Out_OD;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Pin   =IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode  =GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void delay_nop(void)
{
	unsigned int i;
	for(i=0;i<50;i++)
		__nop();
}

void IIC_EEStart(void)
{
	SDA_OUT();
	SetSDA(HIGH);
	SetSCL(HIGH);
	delay_nop();
	SetSDA(LOW);
	delay_nop();
	SetSCL(LOW);
}

void IIC_EEStart1(void)
{
	SDA_OUT(); 
	SetSDA(HIGH);
	delay_nop();
	SetSCL(HIGH);
	delay_nop();
 	SetSDA(LOW);	
	delay_nop();
	SetSCL(LOW);
}

void IIC_EEStop(void)
{
	SetSDA(LOW);
	SDA_OUT();
	SetSCL(LOW);
	delay_nop();
	SetSCL(HIGH);
	delay_nop();
    SetSDA(HIGH);
	delay_nop();
}

u8 IIC_EEWait_Ack(void)
{
	u8 flag;
		
	SDA_IN(); 
	SetSDA(HIGH);
	delay_nop();
	SetSCL(HIGH);
	delay_nop();
	if(GetSDA())
		flag=1;
	else
		flag=0;
	SetSCL(LOW);
	return flag;
}

void IIC_EEAck(void)
{
	SetSCL(LOW);
	SDA_OUT();
	SetSDA(LOW);
	delay_nop();
	SetSCL(HIGH);
	delay_nop();
	SetSCL(LOW);
}

void IIC_EENAck(void)
{
	SetSCL(LOW);
	SDA_OUT();
	SetSDA(HIGH);
	delay_nop();
	SetSCL(HIGH);
	delay_nop();
	SetSCL(LOW);
}

void IIC_EESend_Byte(u8 txd)
{
	u8 t;
	SDA_OUT();
	SetSCL(LOW);
	for(t=0;t<8;t++)
	{
		if((txd&0x80)>>7)
            SetSDA(HIGH);
        else SetSDA(LOW);
		txd<<=1;
		delay_nop();
		SetSCL(HIGH);
		delay_nop();
		SetSCL(LOW);
	}
}

u8 IIC_EERead_Byte(u8 ack)
{
	u8 i,receive=0;
	SDA_IN();
	for(i=0;i<8;i++ )
	{
		SetSCL(LOW);
		delay_nop();
		SetSCL(HIGH);
		delay_nop();
		
		receive<<=1;
		if(GetSDA())
			receive++;
	}
	if (!ack)
	    IIC_EENAck();
	else
	    IIC_EEAck();
	return receive;
}

u8 AT24CXX_ReadOneByte(u32 ReadAddr)
{
	u8 temp;
	u8 addrH,addrL;

	addrH=ReadAddr>>8;
	addrL=ReadAddr&0xff;

	IIC_EEStart();

	IIC_EESend_Byte(0xa0);

	IIC_EEWait_Ack();
	IIC_EESend_Byte(addrH); 
	IIC_EEWait_Ack();
   	IIC_EESend_Byte(addrL); 
	IIC_EEWait_Ack();
	IIC_EEStart();
	IIC_EESend_Byte(0xa1); 

	IIC_EEWait_Ack();
	temp=	IIC_EERead_Byte(0);
	IIC_EEStop();

	return temp;
}

void AT24CXX_WriteOneByte(u32 WriteAddr,u8 DataToWrite)
{
	u8 addrH,addrL;

	addrH=WriteAddr>>8;
	addrL=WriteAddr&0xff;

	IIC_EEStart();

	IIC_EESend_Byte(0xa0);

	IIC_EEWait_Ack();
	IIC_EESend_Byte(addrH); 
	IIC_EEWait_Ack();
	IIC_EESend_Byte(addrL);
	IIC_EEWait_Ack();
	IIC_EESend_Byte(DataToWrite); 
	IIC_EEWait_Ack();
	IIC_EEStop();
}

void AT24CXX_Read(u32 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	u8 addrH,addrL;

	if(NumToRead==0)
		return;

	addrH=ReadAddr>>8;
	addrL=ReadAddr&0xff;

	IIC_EEStart();

	IIC_EESend_Byte(0xa0);
	IIC_EEWait_Ack();

	IIC_EESend_Byte(addrH);
	IIC_EEWait_Ack();
   	IIC_EESend_Byte(addrL);
	IIC_EEWait_Ack();

	IIC_EEStart();
	IIC_EESend_Byte(0xa1); 
	IIC_EEWait_Ack();

	NumToRead--;
	while(NumToRead)
	{
		*pBuffer=IIC_EERead_Byte(1);
		pBuffer++;
		ReadAddr++;
		NumToRead--;
	}
	*pBuffer=IIC_EERead_Byte(0);
	IIC_EEStop();
}

void AT24CXX_Write(u32 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	u8 addrH,addrL;
	u16 lentmp,lentow,lens;
	u8 tmp;

	if(NumToWrite==0)
		return;

	lentmp=NumToWrite;
	while(lentmp)
	{
		tmp=WriteAddr%PAGE_SIZE;
		tmp=PAGE_SIZE-tmp;
		if(tmp>=lentmp)
		{
			lentow=lentmp;
		}
		else
		{
			lentow=tmp;
		}
		lentmp=lentmp-lentow;
		lens=lentow;

		addrH=WriteAddr>>8;
		addrL=WriteAddr&0xff;

		IIC_EEStart();

		IIC_EESend_Byte(0xa0);

		IIC_EEWait_Ack();
		IIC_EESend_Byte(addrH);
		IIC_EEWait_Ack();
		IIC_EESend_Byte(addrL);
		IIC_EEWait_Ack();

		while(lentow--)
		{
			IIC_EESend_Byte(*pBuffer);
			IIC_EEWait_Ack();
			pBuffer++;
		}
		IIC_EEStop();

		WriteAddr=WriteAddr+lens;
	}
}
void AT24CXX_clr(u32 WriteAddr,u32 NumToWrite)
{
	u8 addrH,addrL;
	u16 lentmp,lentow,lens;
	u8 tmp;

	lentmp=NumToWrite;
	while(lentmp)
	{
		tmp=WriteAddr%PAGE_SIZE;
		tmp=PAGE_SIZE-tmp;
		if(tmp>=lentmp)
		{
			lentow=lentmp;
		}
		else
		{
			lentow=tmp;
		}
		lentmp=lentmp-lentow;
		lens=lentow;

		addrH=WriteAddr>>8;
		addrL=WriteAddr&0xff;

		IIC_EEStart();

		IIC_EESend_Byte(0xa0);

		IIC_EEWait_Ack();
		IIC_EESend_Byte(addrH); 
		IIC_EEWait_Ack();
		IIC_EESend_Byte(addrL); 
		IIC_EEWait_Ack();

		while(lentow--)
		{
			IIC_EESend_Byte(0xff);
			IIC_EEWait_Ack();
		}
		IIC_EEStop();

		WriteAddr=WriteAddr+lens;
	}
}

//----------------------EEPROM---------------------------//

