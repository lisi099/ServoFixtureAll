/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#ifndef __USART3_H
#define	__USART3_H


#define USART3_DR_Base  (USART3_BASE+0x04)
#define TXD3_BUFFSIZE 36
#define RCV3_BUFFSIZE 36


extern uint8_t  Txd3_Buffer[TXD3_BUFFSIZE];
extern uint8_t  Rcv3_Buffer[RCV3_BUFFSIZE];
extern volatile uint32_t Rcv3_Counter;
extern volatile uint8_t  Rcv3_Flag;
extern volatile uint8_t  Txd3_Flag;


void usart3_init(void);
void usart3_send_buff(uint8_t *pbuffer, uint32_t size);


#endif

