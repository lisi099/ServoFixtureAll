/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */

#ifndef __TAI_H__
#define __TAI_H__

#include <rtthread.h>
#include "usart2.h"


void taiwan_send_read_data(void);
void taiwan_servo_init(void);

void set_soft_start(uint8_t state);
void get_soft_start(uint8_t *state);
	
void set_senter(int8_t state);
void get_senter(int8_t *state);

void set_brake(int8_t state);
void get_brake(int8_t *state);
	
void set_force(int8_t state);
void get_force(int8_t *state);
	
void set_deadband(int8_t state);
void get_deadband(int8_t *state);

void set_tension(int8_t state);
void get_tension(int8_t *state);

void set_boost(int8_t state);
void get_boost(int8_t *state);

void set_maxpower(int8_t state);
void get_maxpower(int8_t *state);

uint8_t is_taiwan_servo(void);

#endif
