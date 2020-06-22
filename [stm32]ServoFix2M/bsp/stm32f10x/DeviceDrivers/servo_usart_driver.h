/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#ifndef _USART2_H_
#define	_USART2_H_

#include "stm32f10x.h"

void usart2_init_tx(uint32_t bd);
void usart2_init_rx(uint32_t bd);
void usart2_send_buff(uint8_t *pbuffer, uint32_t size);

#endif

