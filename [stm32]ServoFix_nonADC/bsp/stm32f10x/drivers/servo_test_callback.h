/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */

#ifndef __SERVO_TEST_CALLBACK_H__
#define __SERVO_TEST_CALLBACK_H__
#include <stm32f10x.h>
#include <rtthread.h>

void Broadband_Page_CallBack(u8 key);
void Narrowband_Page_CallBack(u8 key);

#endif
