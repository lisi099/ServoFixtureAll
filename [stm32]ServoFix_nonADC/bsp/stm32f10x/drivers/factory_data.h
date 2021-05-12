
/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#ifndef _FACTORY_DATA_H_
#define	_FACTORY_DATA_H_

#include "servo_serial.h"

struct Servo_Tai_Data_
{
	uint16_t max_power;
	uint16_t boost;
	uint16_t deadband;
	uint16_t tension;
	uint16_t farce;
	uint16_t brake;
	uint16_t senter;
	uint16_t soft_start;
	uint16_t version;
	uint16_t init_speed;
	uint16_t init_timeout;
};

int get_total_num(void);

char *get_ver_char(int mum);

int get_ver_num(int num);

void get_factory_data(struct Servo_Data_Stru_ *data, int num);

void get_tai_factory_data(uint8_t *data, int num);
//--------------------------------------------------------------

#endif
