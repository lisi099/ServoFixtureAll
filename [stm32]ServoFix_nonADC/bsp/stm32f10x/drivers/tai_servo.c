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
#include "tai_pwm.h"

#define PWM_HIGH() 				GPIO_SetBits(GPIOA, GPIO_Pin_2)
#define PWM_LOW()					GPIO_ResetBits(GPIOA, GPIO_Pin_2)

#define MAX_POWER_INDEX 	13
#define BOOST_INDEX 		79
#define TENSION_INDEX		81
#define DEAD_BAND_INDEX		55
#define FORCE_INDEX			61
#define BRAKE_INDEX			65
#define CENTER_INDEX		43
#define SOFT_START_INDEX	91

volatile uint8_t is_tai_servo_ = 0;
volatile uint8_t is_tai_servo_e_ = 0;

static 	uint8_t read_servo_data[136];
static 	uint8_t write_servo_data[136];
volatile uint8_t receive_tai_data_flag = 0;
extern volatile uint8_t receive_uart_data_flag;
extern volatile uint8_t  write_read_busy_state_;
extern rt_mutex_t servo_mutex;
extern volatile uint16_t current_servo_version_;
extern volatile uint8_t connect_servo_state_;

void get_tai_stru(struct Servo_Tai_Data_ *tai_data)
{
    uint8_t* data = read_servo_data;
    tai_data->boost = (uint16_t)(data[BOOST_INDEX] | data[BOOST_INDEX + 1] << 8);
    tai_data->max_power = (uint16_t)(data[MAX_POWER_INDEX] | data[MAX_POWER_INDEX + 1] << 8);
    tai_data->tension = (uint16_t)(data[TENSION_INDEX] | data[TENSION_INDEX + 1] << 8);
    tai_data->deadband = (uint16_t)(data[DEAD_BAND_INDEX] | data[DEAD_BAND_INDEX + 1] << 8);
    tai_data->farce = (uint16_t)(data[FORCE_INDEX] | data[FORCE_INDEX + 1] << 8);
    tai_data->brake = (uint16_t)(data[BRAKE_INDEX] | data[BRAKE_INDEX + 1] << 8);
    tai_data->senter = (uint16_t)(data[CENTER_INDEX] | data[CENTER_INDEX + 1] << 8);
    tai_data->soft_start = (uint16_t)(data[SOFT_START_INDEX + 2] | data[SOFT_START_INDEX + 3] << 8);
    tai_data->version = get_version();
}

void set_tai_stru(struct Servo_Tai_Data_ *tai_data)
{
		uint8_t* data = write_servo_data;
		data[BOOST_INDEX] = (uint8_t)(tai_data->boost);
		data[BOOST_INDEX+1] = (uint8_t)(tai_data->boost >> 8);
	
		data[MAX_POWER_INDEX] = (uint8_t)(tai_data->max_power);
		data[MAX_POWER_INDEX+1] = (uint8_t)(tai_data->max_power >> 8);
	
		data[TENSION_INDEX] = (uint8_t)(tai_data->tension);
		data[TENSION_INDEX+1] = (uint8_t)(tai_data->tension >> 8);
	
		data[DEAD_BAND_INDEX] = (uint8_t)(tai_data->deadband);
		data[DEAD_BAND_INDEX+1] = (uint8_t)(tai_data->deadband >> 8);
	
		data[FORCE_INDEX] = (uint8_t)(tai_data->farce);
		data[FORCE_INDEX+1] = (uint8_t)(tai_data->farce >> 8);
	
		data[BRAKE_INDEX] = (uint8_t)(tai_data->brake);
		data[BRAKE_INDEX+1] = (uint8_t)(tai_data->brake >> 8);
	
		data[CENTER_INDEX] = (uint8_t)(tai_data->senter);
		data[CENTER_INDEX+1] = (uint8_t)(tai_data->senter >> 8);
		
		data[SOFT_START_INDEX+2] = (uint8_t)(tai_data->soft_start);
		data[SOFT_START_INDEX+3] = (uint8_t)(tai_data->soft_start >> 8);
}


uint8_t* get_taiwan_read_data(void)
{
    return read_servo_data;
}

uint8_t* get_taiwan_write_data(void)
{
    return write_servo_data;
}

uint8_t is_same(void)
{
    if(memcmp(write_servo_data, read_servo_data, 110) == 0)
    {
        return 1;
    }
    return 0;
}

void copy_read_data(void)
{
    memcpy(write_servo_data, read_servo_data, 129);
}

void copy_write_data(void)
{
    memcpy(read_servo_data, write_servo_data, 129);
}


void set_read_servo_data(const uint8_t* data)
{
    memcpy(read_servo_data, data, 129);
    receive_tai_data_flag = 1;
}


void set_soft_start(int8_t state)
{
    uint8_t* data = write_servo_data;
    uint16_t* base1 = (uint16_t*)(&data[SOFT_START_INDEX]);
    uint16_t* base2 = (uint16_t*)(&data[SOFT_START_INDEX + 2]);
    if(state)
    {
        *base1 = 20;
        *base2 = 2;
    }
    else
    {
        *base1 = 20;
        *base2 = 0;
    }
}

void get_soft_start(int8_t* state)
{
    uint8_t* data = read_servo_data;
    //uint16_t *base1 = (uint16_t *)(&data[SOFT_START_INDEX]);
    uint16_t* base2 = (uint16_t*)(&data[SOFT_START_INDEX + 2]);
    if(*base2 != 0)
    {
        *state = 1;
    }
    else
    {
        *state = 0;
    }
}

void set_senter(int8_t state)
{
    uint8_t* data = write_servo_data;
    uint16_t* base1 = (uint16_t*)(&data[CENTER_INDEX]);
    *base1 = 2048 + 28 * state;
}

void get_senter(int8_t* state)
{
    uint8_t* data = read_servo_data;
    uint16_t* base1 = (uint16_t*)(&data[CENTER_INDEX]);
    *state = (*base1 - 2048) / 28;
}

void set_brake(int8_t state)
{
    uint8_t* data = write_servo_data;
    uint16_t* base1 = (uint16_t*)(&data[BRAKE_INDEX]);
    *base1 = 20 + state * 10;
}

void get_brake(int8_t* state)
{
    uint8_t* data = read_servo_data;
    uint16_t* base1 = (uint16_t*)(&data[BRAKE_INDEX]);
    *state = (*base1 - 20) / 10;
}

void set_force(int8_t state)
{
    uint8_t* data = write_servo_data;
    uint16_t* base1 = (uint16_t*)(&data[FORCE_INDEX]);
    switch(state)
    {
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
        *base1 = 220;
        break;
    }
}

void get_force(int8_t* state)
{
    uint8_t* data = read_servo_data;
    uint16_t* base1 = (uint16_t*)(&data[FORCE_INDEX]);
    if(*base1 <= 90)
    {
        *state = 1;
    }
    else if(*base1 <= 100)
    {
        *state = 2;
    }
    else if(*base1 <= 110)
    {
        *state = 3;
    }
    else if(*base1 <= 120)
    {
        *state = 4;
    }
    else if(*base1 <= 130)
    {
        *state = 5;
    }
    else if(*base1 <= 155)
    {
        *state = 6;
    }
    else if(*base1 <= 165)
    {
        *state = 7;
    }
    else if(*base1 <= 175)
    {
        *state = 8;
    }
    else if(*base1 <= 185)
    {
        *state = 9;
    }
    else if(*base1 <= 220)
    {
        *state = 10;
    }
}

void set_deadband(int8_t state)
{
    uint8_t* data = write_servo_data;
    uint16_t* base1 = (uint16_t*)(&data[DEAD_BAND_INDEX]);
    *base1 = state;
}

void get_deadband(int8_t* state)
{
    uint8_t* data = read_servo_data;
    uint16_t* base1 = (uint16_t*)(&data[DEAD_BAND_INDEX]);
    *state = *base1;
}

void set_tension(int8_t state)
{
    uint8_t* data = write_servo_data;
    uint16_t* base1 = (uint16_t*)(&data[TENSION_INDEX]);
    switch(state)
    {
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

void get_tension(int8_t* state)
{
    uint8_t* data = read_servo_data;
    uint16_t* base1 = (uint16_t*)(&data[TENSION_INDEX]);
    if(*base1 <= 130)
    {
        *state = 1;
    }
    else if(*base1 <= 200)
    {
        *state = 2;
    }
    else if(*base1 <= 270)
    {
        *state = 3;
    }
}

void set_boost(int8_t state)
{
    uint8_t* data = write_servo_data;
    uint16_t* base1 = (uint16_t*)(&data[BOOST_INDEX]);
    switch(state)
    {
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

void get_boost(int8_t* state)
{
    uint8_t* data = read_servo_data;
    uint16_t* base1 = (uint16_t*)(&data[BOOST_INDEX]);
    if(*base1 >= 1300)
    {
        *state = 1;
    }
    else if(*base1 >= 1200)
    {
        *state = 2;
    }
    else if(*base1 >= 1100)
    {
        *state = 3;
    }
    else if(*base1 >= 1000)
    {
        *state = 4;
    }
    else if(*base1 >= 900)
    {
        *state = 5;
    }
    else if(*base1 >= 800)
    {
        *state = 6;
    }
    else if(*base1 >= 700)
    {
        *state = 7;
    }
    else if(*base1 >= 600)
    {
        *state = 8;
    }
    else if(*base1 >= 500)
    {
        *state = 9;
    }
    else if(*base1 >= 400)
    {
        *state = 10;
    }
}

void set_maxpower(int8_t state)
{
    uint8_t* data = write_servo_data;
    uint16_t* base1 = (uint16_t*)(&data[MAX_POWER_INDEX]);
    *base1 = 50 + 5 * state;
}

void get_maxpower(int8_t* state)
{
    uint8_t* data = read_servo_data;
    uint16_t base1 = (uint16_t) data[MAX_POWER_INDEX] | (uint16_t)(data[MAX_POWER_INDEX + 1] << 8);
    *state = (base1 - 50) / 5;
}

uint16_t get_version(void)
{
    uint8_t* data = read_servo_data;
    uint16_t base1 = 0;
    base1 = (data[95] - '0') * 10000;
    base1 += (data[96] - '0') * 1000;
    base1 += (data[97] - '0') * 100;
    base1 += (data[98] - '0') * 10;
    base1 += (data[99] - '0') ;
    return base1;
}

uint16_t get_distributor(void)
{
    uint8_t* data = read_servo_data;
    uint16_t base1 = 0;
    base1 = (data[96] - '0') * 10;
    base1 += (data[97] - '0') * 1;
    return base1;
}


uint8_t chech_sum_xor(const uint8_t* data, const uint8_t size)
{
    uint8_t sum = 0;
    for(int i = 0; i < size; i++)
    {
        sum ^= data[i];
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
	produce_pwm_count(7000, 5);
    data[0] = 0x22;
    data[1] = 0x32;
    data[2] = 110;
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
    usart2_send_buff(data, 11);
    rt_thread_delay(100);

    uint8_t* data_cpy = &write_servo_data[0];
    memcpy(&data[0], data_cpy, 111); //data 110byte
//    data[95] = 1 + 0x30;	//CS
//    data[96] = 8 + 0x30;	//CS
//    data[97] = 0 + 0x30;	//CS
//    data[98] = 0 + 0x30;	//CS
//    data[99] = 0 + 0x30;	//CS
    data[111] = chech_sum_xor(data, 111);	//
    data[112] = 0xED;	//end
		
    usart2_send_buff(data, 113);
    rt_thread_delay(500);
}


uint8_t connect_taiwan(void)
{
    uint16_t time_count = 0;
    write_read_busy_state_ = 1;
    receive_tai_data_flag = 0;
    taiwan_send_read_data();
    //check response data
    while(receive_tai_data_flag == 0)
    {
        time_count++;
        if(time_count > 300) //300ms
        {
            write_read_busy_state_ = 0;
            return 0;
        }
        rt_thread_delay(1); //0.001 s
    }
    write_read_busy_state_ = 0;
    current_servo_version_ = get_version();
    return 1;
}

uint8_t is_taiwan_servo(void)
{
    uint8_t try_count = 2;
    write_read_busy_state_ = 1;

    produce_pwm_count(7000, 5);

    write_read_busy_state_ = 0;

    do
    {
        if(connect_taiwan())
        {
            connect_servo_state_ = 1;
            is_tai_servo_e_ = 1;
            return 1;
        }
        try_count--;
    }
    while(try_count);

    is_tai_servo_e_ = 0;
    return 0; //no find servo
}

uint8_t read_tai_servo_data(void)
{
    uint8_t try_count = 3;

    do
    {
        if(connect_taiwan())
        {
            connect_servo_state_ = 1;
//            is_tai_servo_e_ = 1;
            return 1;
        }
        try_count--;
    }
    while(try_count);

//    is_tai_servo_e_ = 0;
    return 0; //no find servo
}


