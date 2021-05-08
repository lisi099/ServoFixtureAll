/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */

#ifndef __KEEP_CALLBACK_H__
#define __KEEP_CALLBACK_H__
#include <stm32f10x.h>
#include <rtthread.h>

void keep_string(uint8_t state);
uint8_t keep(uint8_t state);

#endif
