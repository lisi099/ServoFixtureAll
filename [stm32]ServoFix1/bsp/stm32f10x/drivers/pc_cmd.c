/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#include "usart1.h"
#include "string.h"
#include "servo_serial.h"

#define CONNECT_CMD 0xFE
#define DISCONNECT_CMD (CONNECT_CMD -0x2)
#define READ_CMD  0x02
#define WRITE_CMD  (READ_CMD +0x2)

#define DATA_SIZE 86

volatile uint8_t pc_data_state_ =0;
extern struct Servo_Data_Stru_ servoDataStru;

extern void enter_pc_page(void);
extern void exit_pc_page(void);

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
		uint8_t data[DATA_SIZE +6];
		memset(data, 0, sizeof(data));
		data[0] = 0x5A;
		data[1] = 0xA5;
		data[2] = CONNECT_CMD -1;
		data[3] = 0x00;
		memcpy(&data[4], &servoDataStru, DATA_SIZE);
		uint16_t sum = sum_check(data, sizeof(data) -2);
		data[DATA_SIZE +4] = (uint8_t)sum;
		data[DATA_SIZE +5] = (uint8_t)(sum >> 8);
		usart1_send_buff(data, sizeof(data));
}

void response_read(void)
{
		uint8_t data[DATA_SIZE +6];
		memset(data, 0, sizeof(data));
		data[0] = 0x5A;
		data[1] = 0xA5;
		data[2] = WRITE_CMD -1;
		data[3] = 0x00;
		memcpy(&data[4], &servoDataStru, DATA_SIZE);
		uint16_t sum = sum_check(data, sizeof(data) -2);
		data[DATA_SIZE +4] = (uint8_t)sum;
		data[DATA_SIZE +5] = (uint8_t)(sum >> 8);
		usart1_send_buff(data, sizeof(data));
	
}

void response_write(uint8_t *data_write)
{
		uint8_t data[DATA_SIZE +6];
		memcpy(&servoDataStru, &data_write[4], DATA_SIZE);
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
			response_connect();
			enter_pc_page();
			break;
		case DISCONNECT_CMD:
			exit_pc_page();
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

/******************* (C) COPYRIGHT 2016*****END OF FILE****/

