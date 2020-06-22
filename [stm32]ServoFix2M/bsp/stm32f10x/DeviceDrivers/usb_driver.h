/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#ifndef _USART1_DMA_H_
#define	_USART1_DMA_H_

#include "stm32f10x.h"

void usart1_init(uint32_t bd);
void usart1_send_buff(uint8_t *pbuffer, uint32_t size);

uint8_t usart1_get_tx_flag(void);
uint8_t usart1_get_rx_flag(void);
void usart1_reset_rx_flag(void);
uint8_t* usart1_get_tx_ptr(void);
uint8_t* usart1_get_rx_ptr(void);
uint32_t usart1_get_counter(void);


#endif

