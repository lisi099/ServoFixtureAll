/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#ifndef USART1_PACKAGE
#define	USART1_PACKAGE

#include "stm32f10x.h"

#define CMD_REQUEST 	0xC1
#define CMD_READ 		0xB1
#define TX_BUFF_SIZE	13

void usart1_length_13_data_process(void);
uint8_t get_reset_mcu_flag(void);
void usart1_length_13_data_receieve(void);


#endif 
