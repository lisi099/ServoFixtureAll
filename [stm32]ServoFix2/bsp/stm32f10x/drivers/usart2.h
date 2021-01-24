/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#ifndef __USART2_H
#define	__USART2_H


#define USART2_DR_Base  (USART2_BASE+0x04)
#define TXD2_BUFFSIZE           168
#define RCV2_BUFFSIZE           168

extern uint8_t  Txd2_Buffer[TXD2_BUFFSIZE];
extern uint8_t  Rcv2_Buffer[RCV2_BUFFSIZE];
extern volatile uint32_t Rcv2_Counter;
extern volatile uint8_t  Rcv2_Flag;
extern volatile uint8_t  Txd2_Flag;
extern volatile uint8_t  usart2_mode;
extern volatile uint32_t bd_set_;


//void usart2_init(uint32_t bd);
void usart2_init_tx(uint32_t bd);
void usart2_init_rx(uint32_t bd);
void usart2_send_buff(uint8_t *pbuffer, uint32_t size);



#endif

