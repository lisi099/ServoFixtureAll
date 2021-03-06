/******************** (C) COPYRIGHT 2016 ********************
* Filename:
* Description:
* Platform:
* Other info:
* Author         Date          Notes
xiangbin.huang   2017.02.21    The first version template
***************************************************************/
#ifndef USART1_H
#define USART1_H




void usart1_init(void);
void usart1_send_buff(uint8_t *pbuffer, uint32_t size);

uint8_t 	usart1_get_tx_flag(void);
uint8_t 	usart1_get_rx_flag(void);
void 			usart1_reset_rx_flag(void);
uint8_t* 	usart1_get_tx_ptr(void);
uint8_t* 	usart1_get_rx_ptr(void);
uint32_t 	usart1_get_counter(void);

#endif

