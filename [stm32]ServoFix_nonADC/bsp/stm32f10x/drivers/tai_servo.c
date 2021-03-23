/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */
#include <rtthread.h>
#include <stm32f10x.h>
#include "tai_servo.h"
#include "string.h"

#define PWM_HIGH() 				GPIO_SetBits(GPIOA, GPIO_Pin_2)
#define PWM_LOW()					GPIO_ResetBits(GPIOA, GPIO_Pin_2)

#define MAX_POWER_INDEX 13
#define BOOST_INDEX 		79
#define TENSION_INDEX		81
#define DEAD_BAND_INDEX	55
#define FORCE_INDEX			61
#define BRAKE_INDEX			65
#define CENTER_INDEX		43
#define SOFT_START_INDEX	91

volatile uint8_t is_tai_servo = 0;
static 	uint8_t read_servo_data[136];


void set_soft_start(uint8_t state)
{
	uint8_t *data = read_servo_data;
	uint16_t *base1 = (uint16_t *)(&data[SOFT_START_INDEX]);
	uint16_t *base2 = (uint16_t *)(&data[SOFT_START_INDEX+2]);
	if(state){
		*base1 = 20;
		*base2 = 2;
	}
	else{
		*base1 = 20;
		*base2 = 0;
	}
}

void get_soft_start(uint8_t *state)
{
	uint8_t *data = read_servo_data;
	//uint16_t *base1 = (uint16_t *)(&data[SOFT_START_INDEX]);
	uint16_t *base2 = (uint16_t *)(&data[SOFT_START_INDEX+2]);
	if(*base2 != 0){
		*state = 1;
	}
	else{
		*state = 0;
	}
}

void set_senter(int8_t state)
{
	uint8_t *data = read_servo_data;
	uint16_t *base1 = (uint16_t *)(&data[CENTER_INDEX]);
	*base1 = 2048 + 28*state;
}

void get_senter(int8_t *state)
{
	uint8_t *data = read_servo_data;
	uint16_t *base1 = (uint16_t *)(&data[CENTER_INDEX]);
	*state = (*base1 -2048)/28;
}

void set_brake(int8_t state)
{
	uint8_t *data = read_servo_data;
	uint16_t *base1 = (uint16_t *)(&data[BRAKE_INDEX]);
	*base1 = 20 + state *10;
}

void get_brake(int8_t *state)
{
	uint8_t *data = read_servo_data;
	uint16_t *base1 = (uint16_t *)(&data[BRAKE_INDEX]);
	*state = (*base1 - 20)/10;
}

void set_force(int8_t state)
{
	uint8_t *data = read_servo_data;
	uint16_t *base1 = (uint16_t *)(&data[FORCE_INDEX]);
	switch(state){
		case 1:
			*base1 = 90;
			break;
		case 2:
			*base1 = 100;
			break;
		case 3:
			*base1 = 110;
			break;
		case 4:
			*base1 = 120;
			break;
		case 5:
			*base1 = 130;
			break;
		case 6:
			*base1 = 155;
			break;
		case 7:
			*base1 = 165;
			break;
		case 8:
			*base1 = 175;
			break;
		case 9:
			*base1 = 185;
			break;
		case 10:
			*base1 = 200;
			break;
	}
}

void get_force(int8_t *state)
{
	uint8_t *data = read_servo_data;
	uint16_t *base1 = (uint16_t *)(&data[FORCE_INDEX]);
	if(*base1 <=90){
		*state = 1;
	}
	else if(*base1 <=100){
		*state = 2;
	}
	else if(*base1 <=110){
		*state = 3;
	}
	else if(*base1 <=120){
		*state = 4;
	}
	else if(*base1 <=130){
		*state = 5;
	}
	else if(*base1 <=155){
		*state = 6;
	}
	else if(*base1 <=165){
		*state = 7;
	}
	else if(*base1 <=175){
		*state = 8;
	}
	else if(*base1 <=185){
		*state = 9;
	}
	else if(*base1 <=200){
		*state = 10;
	}
}

void set_deadband(int8_t state)
{
	uint8_t *data = read_servo_data;
	uint16_t *base1 = (uint16_t *)(&data[DEAD_BAND_INDEX]);
	*base1 = state;
}

void get_deadband(int8_t *state)
{
	uint8_t *data = read_servo_data;
	uint16_t *base1 = (uint16_t *)(&data[DEAD_BAND_INDEX]);
	*state = *base1;
}

void set_tension(int8_t state)
{
	uint8_t *data = read_servo_data;
	uint16_t *base1 = (uint16_t *)(&data[TENSION_INDEX]);
		switch(state){
		case 1:
			*base1 = 130;
			break;
		case 2:
			*base1 = 200;
			break;
		case 3:
			*base1 = 270;
			break;
	}
}

void get_tension(int8_t *state)
{
	uint8_t *data = read_servo_data;
	uint16_t *base1 = (uint16_t *)(&data[TENSION_INDEX]);
	if(*base1 <=130){
		*state = 1;
	}
	else if(*base1 <=200){
		*state = 2;
	}
	else if(*base1 <=270){
		*state = 3;
	}
}

void set_boost(int8_t state)
{
	uint8_t *data = read_servo_data;
	uint16_t *base1 = (uint16_t *)(&data[BOOST_INDEX]);
	switch(state){
		case 1:
			*base1 = 1300;
			break;
		case 2:
			*base1 = 1200;
			break;
		case 3:
			*base1 = 1100;
			break;
		case 4:
			*base1 = 1000;
			break;
		case 5:
			*base1 = 900;
			break;
		case 6:
			*base1 = 800;
			break;
		case 7:
			*base1 = 700;
			break;
		case 8:
			*base1 = 600;
			break;
		case 9:
			*base1 = 500;
			break;
		case 10:
			*base1 = 400;
			break;
	}
}

void get_boost(int8_t *state)
{
	uint8_t *data = read_servo_data;
	uint16_t *base1 = (uint16_t *)(&data[BOOST_INDEX]);
	if(*base1 >=1300){
		*state = 1;
	}
	else if(*base1 >=1200){
		*state = 2;
	}
	else if(*base1 >=1100){
		*state = 3;
	}
	else if(*base1 >=1000){
		*state = 4;
	}
	else if(*base1 >=900){
		*state = 5;
	}
	else if(*base1 >=800){
		*state = 6;
	}
	else if(*base1 >=700){
		*state = 7;
	}
	else if(*base1 >=600){
		*state = 8;
	}
	else if(*base1 >=500){
		*state = 9;
	}
	else if(*base1 >=400){
		*state = 10;
	}
}

void set_maxpower(int8_t state)
{
	uint8_t *data = read_servo_data;
	uint16_t *base1 = (uint16_t *)(&data[MAX_POWER_INDEX]);
	*base1 = 50 + 5 * state;
}

void get_maxpower(int8_t *state)
{
	uint8_t *data = read_servo_data;
	uint16_t *base1 = (uint16_t *)(&data[MAX_POWER_INDEX]);
	*state =  (*base1 - 50)/5;
}

uint8_t chech_sum_xor(const uint8_t *data, const uint8_t size)
{
	uint8_t sum = data[0];
	for(int i=1; i<size; i++){
		sum^= data[i];
	}
	return sum;
}

void taiwan_send_read_data(void)
{
	uint8_t data[11];
	data[0] = 0x22;
	data[1] = 0x32;
	data[2] = 0x00;
	///addr
	data[3] = 0x00;
	data[4] = 0x00;
	///data
	data[5] = 0x00;
	data[6] = 0x00;
	data[7] = 0x00;
	data[8] = 0x00;
	///check sum
	data[9] = chech_sum_xor(data, 9);
	///end
	data[10] = 0xed;
	usart2_send_buff(data, sizeof(data));
}

void taiwan_send_write_data(void)
{
	uint8_t data[124];
	uint8_t *data_cpy;
	data[0] = 0x22;
	data[1] = 0x32;
	data[2] = 70;
	///addr
	data[3] = 0x03;
	data[4] = 0x00;
	///data
	data[5] = 0x00;
	data[6] = 0x00;
	data[7] = 0x00;
	data[8] = 0x00;
	///check sum
	data[9] = chech_sum_xor(data, 9);
	///end
	data[10] = 0xed;
	
	data[11] = 0xDA; 	//head
	memcpy(&data[12], data_cpy, 110); //data 110byte
	
	data[122] = chech_sum_xor(data, 122);	//CS
	data[123] = 0xED;	//end
	
	usart2_send_buff(data, sizeof(data));
}

void taiwan_servo_init(void)
{
	usart2_init_pwm();
	PWM_LOW();
	for(int i=0; i<5; i++){
		PWM_HIGH();
		rt_thread_delay(7);
		PWM_LOW();
		rt_thread_delay(10);
	}
	usart2_init_rx(115200);
}

extern volatile uint8_t receive_uart_data_flag;

uint8_t connect_taiwan(void)
{
		uint16_t time_count;
	
		receive_uart_data_flag = 0;
		taiwan_send_read_data();
		//check response data
		while(receive_uart_data_flag == 0)
		{
			time_count++;
			if(time_count > 200) //200ms
			{
				return 0;
			}
			rt_thread_delay(1); //0.001 s
		}
		return 1;
}

uint8_t is_taiwan_servo(void)
{
	uint8_t try_count = 3;
	usart2_init_pwm();
	PWM_LOW();
	for(int i=0; i<5; i++){
		PWM_HIGH();
		rt_thread_delay(7);
		PWM_LOW();
		rt_thread_delay(10);
	}
	usart2_init_rx(115200);
	
	do{
		if(connect_taiwan()){
			return 1;
		}
		try_count--;
	}while(try_count);
	
	return 0; //no find servo 
}


