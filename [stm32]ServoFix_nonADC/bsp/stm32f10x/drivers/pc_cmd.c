/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#include "usart1.h"
#include "string.h"
#include "servo_serial.h"
#include <rtthread.h>
#include "tai_servo.h"

#define CONNECT_CMD 0xFE
#define DISCONNECT_CMD (CONNECT_CMD -0x2)
#define READ_CMD  0x02
#define WRITE_CMD  (READ_CMD +0x2)

#define DATA_SIZE 86

volatile uint8_t pc_data_state_ =0;
volatile uint8_t test_data_state_ =0;
volatile uint8_t connect_servo_state_ =0;

extern volatile uint8_t is_tai_servo_;

extern struct Servo_Data_Stru_ servoDataStru;

extern void enter_pc_page(void);
extern void exit_pc_page(void);
extern void menu_combine_position(uint16_t pos);

uint16_t sum_check(uint8_t *data, uint16_t size)
{
		uint16_t sum = 0;
		for(uint16_t i=0; i<size; i++){
				sum += data[i];
		}
		return sum;
}

void response_connect(void)
{
		struct Servo_Tai_Data_ tai_servo;
		uint8_t data[DATA_SIZE +6];
		memset(data, 0, sizeof(data));
		rt_thread_delay(2000);
		data[0] = 0x5A;
		if(is_tai_servo_){
			data[1] = 0xA6;
			get_tai_stru(&tai_servo);
		}
		else{
			data[1] = 0xA5;
		}
		data[2] = CONNECT_CMD -1;
		data[3] = 0x00;
		
		if(is_tai_servo_){
			memcpy(&data[4], &tai_servo, sizeof(tai_servo));
		}
		else{
			memcpy(&data[4], &servoDataStru, DATA_SIZE);
		}
		
		uint16_t sum = sum_check(data, sizeof(data) -2);
		data[DATA_SIZE +4] = (uint8_t)sum;
		data[DATA_SIZE +5] = (uint8_t)(sum >> 8);
		usart1_send_buff(data, sizeof(data));
}

void response_disconnect(void)
{
		uint8_t data[DATA_SIZE +6];
		memset(data, 0, sizeof(data));
		data[0] = 0x5A;
		data[1] = 0xA5;
		data[2] = DISCONNECT_CMD -1;
		data[3] = 0x00;
		memcpy(&data[4], &servoDataStru, DATA_SIZE);
		uint16_t sum = sum_check(data, sizeof(data) -2);
		data[DATA_SIZE +4] = (uint8_t)sum;
		data[DATA_SIZE +5] = (uint8_t)(sum >> 8);
		usart1_send_buff(data, sizeof(data));
}

void response_read(void)
{
		struct Servo_Tai_Data_ tai_servo;
		uint8_t data[DATA_SIZE +6];
		memset(data, 0, sizeof(data));
		rt_thread_delay(2000);
		data[0] = 0x5A;
		if(is_tai_servo_){
			data[1] = 0xA6;
			get_tai_stru(&tai_servo);
		}
		else{
			data[1] = 0xA5;
		}
		data[2] = READ_CMD -1;
		data[3] = 0x00;
		if(is_tai_servo_){
			memcpy(&data[4], &tai_servo, sizeof(tai_servo));
		}
		else{
			memcpy(&data[4], &servoDataStru, DATA_SIZE);
		}
		uint16_t sum = sum_check(data, sizeof(data) -2);
		data[DATA_SIZE +4] = (uint8_t)sum;
		data[DATA_SIZE +5] = (uint8_t)(sum >> 8);
		usart1_send_buff(data, sizeof(data));
	
}

extern void Copy_Data_To_Stru(void);

void response_write(uint8_t *data_write)
{
		uint8_t data[DATA_SIZE +6];
		struct Servo_Tai_Data_ tai_servo;
		if(is_tai_servo_){
			memcpy(&tai_servo, &data[4], sizeof(tai_servo));
			Copy_Data_To_Stru();
			set_tai_stru(&tai_servo);
//			copy_write_data();
		}
		else{
			memcpy(&servoDataStru, &data_write[4], DATA_SIZE);
		}
		
		menu_combine_prom_work_parm();
		if(menu_combine_verify_work_parm())
		{
			data[3] = 0x01;
		}
		else{
			data[3] = 0x00;
		}
		
		memset(data, 0, sizeof(data));
		data[0] = 0x5A;
		data[1] = 0xA5;
		data[2] = READ_CMD -1;
		
		memcpy(&data[4], &servoDataStru, DATA_SIZE);
		uint16_t sum = sum_check(data, sizeof(data) -2);
		data[DATA_SIZE +4] = (uint8_t)sum;
		data[DATA_SIZE +5] = (uint8_t)(sum >> 8);
		usart1_send_buff(data, sizeof(data));
}


void process_pc_data(void)
{
	if(pc_data_state_ ==0){
		return;
	}
	uint8_t *buff =usart1_get_rx_ptr();
	switch(buff[2]){
		case CONNECT_CMD:
			if(connect_servo_state_){
				response_connect();
				enter_pc_page();
			}
			break;
		case DISCONNECT_CMD:
			response_disconnect();
			exit_pc_page();
			break;
		case READ_CMD:
			response_read();
			break;
		case WRITE_CMD:
			response_write(buff);
			break;
		default:
			break;
	}
	pc_data_state_ = 0;
	
}

void process_test_data(void)
{
	if(test_data_state_ ==0){
		return;
	}
	uint8_t *buff =usart1_get_rx_ptr();
	uint16_t pwm = (uint16_t)(buff[4] | (buff[5]<<8));
	menu_combine_position(pwm);
	test_data_state_ = 0;
}

/******************* (C) COPYRIGHT 2016*****END OF FILE****/

