#include "1602_iic_sw.h"



//-------------------------------------写对比度函数--------------------------------------------------
#define  	RST    PBout(5)  
#define  	SCL    PBout(6)   
#define  	SDA    PBout(7)
#define  	READ_SDA   PBin(7)
extern void delay_about_ms(uint16_t nms);
								
void iic1_init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	
	   
	GPIO_InitStructure.GPIO_Pin = I2C_SCL | I2C_SDA | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;       //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB,I2C_SCL | I2C_SDA | GPIO_Pin_5); 
}								

unsigned char CUSTOM[]={//自定义字库
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	//全黑
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//全白
0xff,0x00,0xff,0x00,0xff,0x00,0xff,0x00,	//横线1
0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff,	//横线2
0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,	//竖线1
0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,	//竖线2
0x1f,0x11,0x11,0x11,0x11,0x11,0x11,0x1f,	//网点
0x0a,0x15,0x0a,0x15,0x0a,0x15,0x0a,0x15,	//网点
};

//---------------------------
void _nop_(void)
{
	
}
void delay(long dt)
{
	int i;
	while(dt--)
	for(i=125;i>0;i--);
}

void CheckACK(void)
{
	SDA = 1;
	SCL = 1;
	delay(100);
//	while(SDA);
	SCL = 0;
}

void Send8Bit(unsigned char Byte)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		SDA = Byte>>(7-i)&0x01;
		SCL = 1;
		_nop_();
		SCL = 0;
	}
}
//---------------------------------
void wcom_i2c(char cmd){
	SCL = 1;
	SDA = 1;
	SDA = 0;
	SCL = 0;
	Send8Bit(0x7C);
	CheckACK();
	Send8Bit(0x00);
	CheckACK();
	Send8Bit(cmd);
	CheckACK();
	SCL = 1;
	SDA = 0;
	SDA = 1;
}

//----------------------------------
void wdata_i2c(char dat){
	SCL = 1;
	SDA = 1;
	SDA = 0;
	SCL = 0;
	Send8Bit(0x7C);
	CheckACK();
	Send8Bit(0x40);
	CheckACK();
	Send8Bit(dat);
	CheckACK();
	SCL = 1;
	SDA = 0;
	SDA = 1;
}
//---------------------------------
void test(char fillchar){
	unsigned char i;
	wcom_i2c(0x80);	//AC=00H
	delay(1);
	for(i=0;i<128;i++)
	{
		wdata_i2c(fillchar);
		_nop_();
		_nop_();
	}
}
//---------------------------------
void test2(char fillchar){
	unsigned char i;
	wcom_i2c(0x39);delay(1);//Function set:8bit,2-line,IS=1
        wcom_i2c(0x40);delay(1);//Set ICON RAM Address
        wcom_i2c(0x38);delay(1);//Function set:8bit,2-line,IS=0
	for(i=0;i<15;i++)
	{
		wdata_i2c(fillchar);
		_nop_();
		_nop_();
	}
}
//void SetCGRAM( unsigned char *puts )
//{
//	unsigned char i;
//	wcom_i2c(0x38);	//Function set:8bit,2-line,IS=0
//	delay(1);
//	wcom_i2c(0x40);		//CGRAM start Address
//	delay(10);
//	for(i=0;i<64;i++)
//	{
//		wdata_i2c( puts[i] );
//		_nop_();
//		_nop_();
//	}
//}
//-----------------------------
void init_st7032(void)
{
	int CR;
	CR = 0x7f;  //VOP    微调? [0111 C3:C0]
	RST = 0;
	delay(100);
	RST = 1;
	delay(100);

	wcom_i2c(0x38);
	delay(1);
	wcom_i2c(0x39);
	delay(1);
	wcom_i2c(0x14);   //bias  osc [0001 BS F2 F1 F0 ]
	delay(1);

	wcom_i2c(CR);   //对比度调节  .7f
	delay(1);
 	wcom_i2c(0x6F);   //跟随控制//6A .6f  ra/rb [0110 Fon Rab Rab Rab]内阻
	delay(100);

	wcom_i2c(0x5E);   //icon
	delay(1);
	wcom_i2c(0x0C);   //开显示
	delay(1);
	wcom_i2c(0x01);   //清屏显示
	delay(2);
	wcom_i2c(0x06);   //
	delay(1);

//	SetCGRAM(CUSTOM);
}
//--------------------------
void lcd1_test(void)
{
	while(1)
	{
	init_st7032();

	test(0x00);//全显
        test2(0xff);
	delay(500);

	test(0x01);//清屏
        test2(0x00);
	delay(500);

	test(0x02);//横条1
        test2(0xaa);
	delay(500);

	test(0x03);//横条2
        test2(0x55);
	delay(500);

	test(0x04);//竖条1
        test2(0xaa);
	delay(500);

	test(0x05);//竖条2
        test2(0x55);
	delay(500);

	test(0x06);//雪花1
        test2(0xaa);
	delay(500);

	test(0x07);//雪花2
        test2(0x55);
	delay(500);
	}
}
