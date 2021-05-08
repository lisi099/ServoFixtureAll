/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */

#ifndef __DATA_BASE_FLASH_H__
#define __DATA_BASE_FLASH_H__

#include <rtthread.h>
#include "servo_serial.h"

void save_servo_data_in_flash(uint8_t seq, uint16_t version);
void read_servo_data_in_flash(uint8_t seq);

uint8_t read_servo_data_in_flash_(uint8_t seq, struct Servo_Data_Stru_  *data);
uint8_t read_servo_data_in_flash_taiwan(uint8_t seq, uint8_t* const data);

#endif
