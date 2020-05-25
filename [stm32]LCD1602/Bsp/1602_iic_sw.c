#include "1602_iic_sw.h"

unsigned char vop=0X78;

unsigned char cgram[56]={ 0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,//全显
                               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//不显
                               0x1F,0x00,0x1F,0x00,0x1F,0x00,0x1F,0x00,//横线
                               0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,//竖线
	 	                       0x1F,0x11,0x11,0x11,0x11,0x11,0x11,0x1F,//方框
		                     };

unsigned char da_font1[32]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00                                  
                                };//全显

unsigned char da_font2[32]={0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
                                 0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01                                 
                                };//不显

unsigned char da_font3[32]={0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,
                                 0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02                                 
                                };//横线

unsigned char da_font4[32]={0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
                                 0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03                                  
                                };//竖线

unsigned char da_font5[32]={0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,
                                 0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04                                 
                                };//方框
                                                   
unsigned char da_font6[32]={0x08,0x09,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
		                         0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F	
                                };//字库

unsigned char da_font7[32]={0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
		                         0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F                               
                                };//字库

unsigned char da_font8[32]={0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
		                         0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xB9	                              
		                        };//字库	   

//-------------------------------------写对比度函数--------------------------------------------------

extern void delay_about_ms(uint16_t nms);
								
void iic_init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	
	   
	GPIO_InitStructure.GPIO_Pin = I2C_SCL | I2C_SDA | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;       //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB,I2C_SCL | I2C_SDA | GPIO_Pin_5); 
}								
								
void writeVop(void)
{
	start();
	writecommand(0x7A);//set address
	writecommand(0x80);//
	writecommand(0x31);//
	writecommand(0x80);//
	writecommand(vop); //set VOP
	stop();
}

//----------------------------------延时函数------------------------------------------------------------

void delay(unsigned int t)
{
   unsigned char tt;
   while (t>0)
   { 
	tt=60;
	while(--tt){}
	--t; 
   }
}

void flash(void)
{ 
	unsigned char t1=200;
	while(t1>0)t1--;
}


void start(void)
{ 
	sda=1; 
	flash();
	scl=1;
	flash(); 
	sda=0; 
	flash(); 
	scl=0;
	flash();
}

void stop(void)
{ 
	scl=0; 
	flash();
	sda=0;
	flash(); 
	scl=1; 
	flash(); 
	sda=1; 
	flash(); 
}

void writecommand(unsigned char dat)
{
	unsigned char k;
//	dat = dat | 0x01;
	for(k=0; k<8; k++)
	{  
		if((dat&0x80) >> 7)
			sda =1; 
		else
			sda =0; 
		flash();
		scl =1; 
		flash();
		scl =0; 
		flash();
		dat <<= 1;
	}
	sda=1;
	flash();  
	scl=1; //ack
	flash();
	scl=0;
	flash();
}

void lcd_init(void)
{
	res=1;
	delay(100);
	res=0;
	delay(100);
	res=1;
	delay(100);
	start();
	writecommand(0x7C);//set address
	writecommand(0x00);
	writecommand(0x01);//clear display
	delay(100);
	writecommand(0x28);//FUNCTION SET 
	writecommand(0x0C);//set Display ON/OFF 
	
	writecommand(0x06);//set Entry Mode 
	writecommand(0x29);//FUNCTION SET 
	delay(1);
	writecommand(0x40);//set icon address
	writecommand(0x59);//icon and booster control:icon on c5,c4=0,0
	
	writecommand(0x48);//contrast set  VOP=5.2V
	writecommand(0x64);//follower control
	stop();
}


void setcgrom(unsigned char *q)
{
	char  i;
	start();
	writecommand(0x7C);//set address

	writecommand(0x80);//c=1:write command
	writecommand(0x28);//select H=0

	writecommand(0x80);//c=1:write command
	writecommand(0x40);

	writecommand(0x40);
	for(i=0;i<56;i++)
	{
		writecommand(*q++);
	}
	stop();
}

//---------------------------------------

void displayAddress()
{
	 writecommand(0x7C);//set address
	 writecommand(0x80);
	 writecommand(0x28);//function set
	 writecommand(0x80);
	 writecommand(0x80);//SET DDRAM ADRESS 
	 writecommand(0x40);
}


void displayAddress1()
{
	 writecommand(0x7C);//set address
	 writecommand(0x80);
	 writecommand(0x28);//function set
	 writecommand(0x80);
	 writecommand(0xC0);//SET DDRAM ADRESS 
	 writecommand(0x40);
}


void displayAddressICON()
{
	writecommand(0x7C);//set address
	writecommand(0x00);
	writecommand(0x40);//SET ICONRAM ADRESS 
	writecommand(0x40);
}


void displaychar(unsigned char *p)
{
	unsigned char row;
	start();
	displayAddress();          //第一行的地址
	for (row=0;row<16;row++)
	{
		writecommand(*p++); 
	}
	stop();

	start();
	displayAddress1();        //第二行的地址
	for (row=0;row<16;row++)
	{ 
		writecommand(*p++); 
	}
	stop();
}

//--------------------------------------

void test_lcd(void)
{
	iic_init();
	delay(1000);
	lcd_init();
	setcgrom(cgram);
	while(1)
	{  
// 		displaychar(da_font1);
		displaychar(da_font7);
		delay_about_ms(500);
//		start();
//		writecommand(0x01);
//		stop();
		delay_about_ms(500);
		//wait();
//		displaychar(da_font2);
//		delay_about_ms(1000);
//		//wait();
//		displaychar(da_font3);
//		delay_about_ms(1000);
//		//wait();
//		displaychar(da_font4);
//		delay_about_ms(1000);
//		//wait();
//		displaychar(da_font5);
//		delay_about_ms(1000);
//		//wait();
//		displaychar(da_font6);
//		delay_about_ms(1000);
//		//wait();
//		displaychar(da_font7);
//		delay_about_ms(1000);
//		//wait();
//		displaychar(da_font8);
//		delay_about_ms(1000);
	 }

}

