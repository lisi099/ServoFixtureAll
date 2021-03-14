/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#ifndef USART1_FIFO_H
#define	USART1_FIFO_H

#include "stm32f10x.h"


struct Usart1Package
{
    uint8_t cmd;
    uint8_t	data[7];
};

void usart1_fifo_rx_init(void);
void usart1_fifo_rx_insert(struct Usart1Package *msg);
uint8_t usart1_fifo_rx_get(struct Usart1Package *msg);


void usart1_fifo_tx_init(void);
void usart1_fifo_tx_insert(struct Usart1Package *msg);
uint8_t usart1_fifo_tx_get(struct Usart1Package *msg);



#endif
