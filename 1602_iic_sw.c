#include "1602_iic_sw.h"

const u8 Ainit[42] = {
	0x30, 0x34, 0x30,
	0x30, 0x34, 0x30,
	0x30, 0x34, 0x30,
	0x20, 0x24, 0x20,
	0x20, 0x24, 0x20,
	0x80, 0x84, 0x80,
	0x00, 0x04, 0x00,
	0xc0, 0xc4, 0xc0,
	0x00, 0x04, 0x00,
	0x10, 0x14, 0x10,
	0x00, 0x04, 0x00,
	0x60, 0x64, 0x60,
	0x00, 0x04, 0x00,
	0x20, 0x24, 0x20,
};

//set default status of backlight to ON
static u8 backLight = BACKLIGHT_ON;


size_t strlen(const char * s)
{
   size_t count = 0;
	while(*s++) count++;
	return count;

}
/***
	initial 1602 pins and mode
***/
void init_1602(void)
{
	u8 i = 0;
	IIC_Init();
	i2cAction(0);
	Delay1ms(500);

	for(i = 0; i < 42; ++i)
	{
		i2cAction(Ainit[i]);
		
		if((i == 2) || (i == 5))Delay1ms(5);
		else if((i == 29) || (i == 41))Delay1ms(2);
		else if(i == 8)Delay1us(230);
		else Delay1us(20);
	}
}

/***
	send one frame by iic
***/
void i2cAction(u8 status)
{
	IIC_Start();               // 产生i2c开始信号
	IIC_Send_Byte(ADDRWRITE);  //ADDR<<1 ADDR=0X27
	IIC_Wait_Ack();            //等待应答信号
	IIC_Send_Byte(status);     // 信号发送一个字节
	IIC_Wait_Ack();            // 读一个字节
	IIC_Stop();                // 产生i2c停止信号
}

void writeOneChar(const u8 cmdOrData, const u8 Data)
{
	u8 LCM_Data = 0;
	LCM_Data = ((Data & 0xF0) | (cmdOrData) | (backLight));
	i2cAction(LCM_Data);

	i2cAction(LCM_Data | 0x04);
	Delay1us(20);

	LCM_Data = (((Data & 0x0F)<<4) | (cmdOrData) | (backLight));
	i2cAction(LCM_Data);

	i2cAction(LCM_Data | 0x04);
	Delay1us(20);
	i2cAction(LCM_Data);
}
/***
	Output charactors on 1602
***/
void lcdPrint(const char* charactor)
{
	u8 c = 0;
	for(c = 0; c < (u8)strlen(charactor); ++c)
	writeOneChar(DATA ,*(charactor + c));
	printf("\r\n strlen(charactor)=%d\r\n",strlen(charactor));
}

void disOneChar(unsigned char X, unsigned char Y, char DData)
{
	Y &= 0x1;
	X &= 0xF;   //限制 X 不能大于 15，Y 不能大于 1
	if (Y) X |= 0x40;   //当要显示第二行时地址码+0x40;
	X |= 0x80;       //算出指令码
	writeOneChar(CMD, X);    //这里不检测忙信号，发送地址码
	writeOneChar(DATA, DData);
}



/***
	set position of cursor
***/
void setCursor(u8 colmn, u8 line)
{
	line &= 0x01;
	colmn &= 0x0F;   //限制 X 不能大于 15，line 不能大于 1
	if (line) colmn |= 0x40;   //当要显示第二行时地址码+0x40;
	colmn |= 0x80;       //算出指令码
	writeOneChar(CMD, colmn);    //这里不检测忙信号，发送地址码
}


