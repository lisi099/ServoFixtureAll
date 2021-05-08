/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */

#ifndef __SERVO_RW_CALLBACK_H__
#define __SERVO_RW_CALLBACK_H__
#include <stm32f10x.h>
#include <rtthread.h>

uint8_t servo_write_callback(void);
uint8_t servo_read_callback(void);

#endif
