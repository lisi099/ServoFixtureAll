/******************** (C) COPYRIGHT 2016 ********************

***************************************************************/
#include "1602_iic_sw.h"
#include "servo_adc.h"
#include "string.h"

static unsigned char show_data0_[16];
static unsigned char show_data1_[16];

extern void delay_about_ms(uint16_t nms);
								
void iic_init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	
	   
	GPIO_InitStructure.GPIO_Pin = I2C_SCL | I2C_SDA | I2C_RST;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;       //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB,I2C_SCL | I2C_SDA | I2C_RST);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;       //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_9);
	GPIO_ResetBits(GPIOB,GPIO_Pin_8); 	 

	GPIO_ResetBits(GPIOB,GPIO_Pin_9);
	GPIO_SetBits(GPIOB,GPIO_Pin_8); 		
}								
							
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

void writeVop(unsigned char vop)
{
	start();
	writecommand(0x7A);//
	writecommand(0x80);//
	writecommand(0x31);//
	writecommand(0x80);//
	writecommand(vop); //set VOP
	stop();
}

void lcd_init(void)
{
	#define CONTRAST 20
	iic_init();
	res=1;
	delay(100);
	res=0;
	delay(100);
	res=1;
	delay(100);
	start();
	writecommand(0x7C);//set address
	delay(100);
	writecommand(0x01);//clear display
	delay(100);
//	writecommand(0x02);//clear display
//	delay(100);
	
	writecommand(0x28);//FUNCTION SET 
	delay(100);
	writecommand(0x0C);//set Display ON/OFF
	delay(100);
	writecommand(0x06);//set Entry Mode
	delay(100);	
	writecommand(0x14);
	delay(100);

//	writeVop(00);
	
	writecommand(0x29);//FUNCTION SET
	delay(100);
	writecommand(0x40);//set icon address
	delay(100);
	writecommand(0x59);//icon and booster control:icon on c5,c4=0,0
	
	writecommand(127);//contrast set  VOP=5.2V
	delay(100);
	writecommand(0x64);//follower control
	delay(100);
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
	for(i=0;i<56;i++){
		writecommand(*q++);
	}
	stop();
}

void displayAddress0()
{
	 writecommand(0x7C);//set address
//	 writecommand(0x80);
//	 writecommand(0x14);//function set
	 writecommand(0x80);
	 writecommand(0x80);//SET DDRAM ADRESS 
	 writecommand(0x40);
}

void displayAddress1()
{
	 writecommand(0x7C);//set address
//	 writecommand(0x80);
//	 writecommand(0x14);//function set
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
	displayAddress0();          //第一行的地址
	for (row=0;row<16;row++)
	{
		writecommand(*p++); 
	}
	stop();
	delay(100);
	start();
	displayAddress1();        //第二行的地址
	for (row=0;row<16;row++)
	{ 
		writecommand(*p++); 
	}
	stop();
}

void lcd_clear(void)
{
	unsigned char row, i;
	start();
	displayAddress0();          //第一行的地址
	for (row=0;row<16;row++)
	{
		writecommand(' '); 
	}
	stop();
	delay(100);
	start();
	displayAddress1();        //第二行的地址
	for (row=0;row<16;row++)
	{ 
		writecommand(' '); 
	}
	stop();
	for(i=0; i<16; i++){
		show_data0_[i] = ' ';
		show_data1_[i] = ' ';
	}
}

void put_chars_middle(unsigned char row, char *p)
{
	uint8_t len = strlen(p);
	uint8_t col = 0;
	if(len <16){
		col = (16- len)/2;
	}
	put_chars(row, col, p);
	
}

void put_chars(unsigned char row, unsigned char col, char *p)
{
	static uint8_t first_in =1; 
	uint8_t i;
	if(first_in){
		for(i=0; i<16; i++){
			show_data0_[i] = ' ';
			show_data1_[i] = ' ';
		}
		first_in = 0;
	}
	
	if(strlen(p) >(16-col)){
		if(row == 0){
			memcpy(&show_data0_[col], p, 16-col);
		}
		else{
			memcpy(&show_data1_[col], p, 16-col);
		}
	}
	else{
		if(row == 0){
			memcpy(&show_data0_[col], p, strlen(p));
		}
		else{
			memcpy(&show_data1_[col], p, strlen(p));
		}
	}
	
	start();
	if(row == 0){
		displayAddress0();
		for (i=0; i<16; i++){
			writecommand(show_data0_[i]); 
		}
	}
	else{
		displayAddress1();
		for (i=0; i<16; i++){
			writecommand(show_data1_[i]); 
		}
	}
	stop();
}
//--------------------------------------

void test_lcd(void)
{
	iic_init();
	delay(1000);
	lcd_init();
	while(1)
	{  
		put_chars(0, 0, "FIRST LINE");
		put_chars(1, 0, "SECOND LINE");
	 }
}

