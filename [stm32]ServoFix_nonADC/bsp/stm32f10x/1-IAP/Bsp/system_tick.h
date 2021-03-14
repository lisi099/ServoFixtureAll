/******************** (C) COPYRIGHT 2016 ********************
* Filename:
* Description:
* Platform:
* Other info:
* Author         Date          Notes
xiangbin.huang   2017.02.21    The first version template
***************************************************************/
#ifndef SYSTEM_TICK_H
#define SYSTEM_TICK_H

#include "stm32f10x.h"


#define CPU_FREQ              72000000L
#define SYSTICK_1MS_TICKS    (CPU_FREQ/1000)
#define _NULL                 0

#ifndef NULL
  #define NULL  _NULL
#endif


void set_board_systick(void);
void set_user_systick_fun(void (*func) (void));
uint32_t get_system_ms(void);

#endif

