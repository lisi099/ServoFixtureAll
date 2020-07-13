/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#include "stm32f10x.h"
#include "usart2.h"
#include "servo_serial.h"
#include "string.h"
#include "usart1.h"
#include "usart3.h"
#include <rtthread.h>

#define COM_PORT_DR_Base        (USART2_BASE+0x04)
#define COM_DMA_TX              DMA1_Channel7
#define COM_DMA_RX              DMA1_Channel6
#define COM_DMA_CLOCK           RCC_AHBPeriph_DMA1

#define COM_PORT                USART2 
#define COM_PORT_APB1_CLOCK     RCC_APB1Periph_USART2
#define COM_PORT_APB2_CLOCK     RCC_APB2Periph_GPIOA
#define COM_PORT_SOURCE         GPIOA
#define COM_PORT_PIN_TX         GPIO_Pin_2
#define COM_PORT_PIN_RX         GPIO_Pin_3
#define COM_BAUDRATE            9600

#define USART_TX_HIGH GPIO_SetBits(GPIOA, COM_PORT_PIN_TX);

uint8_t  Txd2_Buffer[TXD2_BUFFSIZE];
uint8_t  Rcv2_Buffer[RCV2_BUFFSIZE];
volatile uint32_t Rcv2_Counter;
volatile uint8_t  Rcv2_Flag;
volatile uint8_t  Txd2_Flag;

volatile uint8_t  usart2_mode = 0;
volatile uint32_t bd_set_ = 9600;

/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
uint8_t usart2_get_tx_flag(void)
{
  return Txd2_Flag;
}
uint8_t usart2_get_rx_flag(void)
{
  return Rcv2_Flag;
}
void usart2_reset_rx_flag(void)
{
  Rcv2_Flag = 0;
}
uint8_t* usart2_get_tx_ptr(void)
{
  return Txd2_Buffer;
}
uint8_t* usart2_get_rx_ptr(void)
{
  return Rcv2_Buffer;
}
uint32_t usart2_get_counter(void)
{
  return Rcv2_Counter;
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
//void usart2_config(uint32_t bd)
//{
//    GPIO_InitTypeDef GPIO_InitStructure;
//    USART_InitTypeDef USART_InitStructure;
//    RCC_APB2PeriphClockCmd(COM_PORT_APB2_CLOCK, ENABLE);
//    RCC_APB1PeriphClockCmd(COM_PORT_APB1_CLOCK, ENABLE );
//	
//    GPIO_InitStructure.GPIO_Pin = COM_PORT_PIN_TX;
////    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(COM_PORT_SOURCE, &GPIO_InitStructure);

////    GPIO_InitStructure.GPIO_Pin = COM_PORT_PIN_RX;
////    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
////    GPIO_Init(COM_PORT_SOURCE, &GPIO_InitStructure);

//	USART_DeInit(USART2);
//    USART_InitStructure.USART_BaudRate = bd;
//    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//    USART_InitStructure.USART_StopBits = USART_StopBits_1;
//    USART_InitStructure.USART_Parity = USART_Parity_No ;
//    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//    USART_Init(COM_PORT, &USART_InitStructure);

//    USART_ITConfig(COM_PORT, USART_IT_IDLE, ENABLE);
//    USART_ITConfig(COM_PORT, USART_IT_TXE, DISABLE);
//    USART_ITConfig(COM_PORT, USART_IT_RXNE, DISABLE);
//    USART_DMACmd(COM_PORT, USART_DMAReq_Tx, ENABLE);
//    USART_DMACmd(COM_PORT, USART_DMAReq_Rx, ENABLE);
//    USART_Cmd(COM_PORT, ENABLE);
//	
//	USART_HalfDuplexCmd(USART2, ENABLE);
//}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
void usart2_DMA_config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_DeInit(COM_DMA_TX);
    DMA_InitStructure.DMA_PeripheralBaseAddr = COM_PORT_DR_Base;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)Txd2_Buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = TXD2_BUFFSIZE;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(COM_DMA_TX, &DMA_InitStructure);
    DMA_ITConfig(COM_DMA_TX,DMA_IT_TC,ENABLE);
    DMA_Cmd (COM_DMA_TX,DISABLE);

    DMA_DeInit(COM_DMA_RX);
    DMA_InitStructure.DMA_PeripheralBaseAddr = COM_PORT_DR_Base;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)Rcv2_Buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = RCV2_BUFFSIZE;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(COM_DMA_RX, &DMA_InitStructure);
    DMA_ITConfig(COM_DMA_RX,DMA_IT_TC,DISABLE);
    DMA_Cmd (COM_DMA_RX,ENABLE);
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
void usart2_NVIC_config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
//void usart2_init(uint32_t bd)
//{
//   usart2_config(bd);
//   usart2_DMA_config();
//   usart2_NVIC_config();
//   Rcv2_Counter = 0;
//   Rcv2_Flag = 0;
//   Txd2_Flag = 0;
//}
void usart2_init_tx(uint32_t bd)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    RCC_APB2PeriphClockCmd(COM_PORT_APB2_CLOCK, ENABLE);
    RCC_APB1PeriphClockCmd(COM_PORT_APB1_CLOCK, ENABLE );
	
    GPIO_InitStructure.GPIO_Pin = COM_PORT_PIN_TX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(COM_PORT_SOURCE, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = COM_PORT_PIN_RX;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(COM_PORT_SOURCE, &GPIO_InitStructure);

	USART_DeInit(USART2);
    USART_InitStructure.USART_BaudRate = bd;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(COM_PORT, &USART_InitStructure);

    USART_ITConfig(COM_PORT, USART_IT_IDLE, ENABLE);
    USART_ITConfig(COM_PORT, USART_IT_TXE, DISABLE);
    USART_ITConfig(COM_PORT, USART_IT_RXNE, DISABLE);
    USART_DMACmd(COM_PORT, USART_DMAReq_Tx, ENABLE);
    USART_DMACmd(COM_PORT, USART_DMAReq_Rx, ENABLE);
    USART_Cmd(COM_PORT, ENABLE);
	
	
	usart2_DMA_config();
	usart2_NVIC_config();
	Rcv2_Counter = 0;
	Rcv2_Flag = 0;
	Txd2_Flag = 0;
	usart2_mode = 0;
}

void usart2_init_rx(uint32_t bd)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(COM_PORT_APB2_CLOCK, ENABLE);
	RCC_APB1PeriphClockCmd(COM_PORT_APB1_CLOCK, ENABLE );

	GPIO_InitStructure.GPIO_Pin = COM_PORT_PIN_TX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(COM_PORT_SOURCE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = COM_PORT_PIN_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(COM_PORT_SOURCE, &GPIO_InitStructure);

	USART_DeInit(USART2);
	USART_InitStructure.USART_BaudRate = bd;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(COM_PORT, &USART_InitStructure);

	USART_ITConfig(COM_PORT, USART_IT_IDLE, ENABLE);
	USART_ITConfig(COM_PORT, USART_IT_TXE, DISABLE);
	USART_ITConfig(COM_PORT, USART_IT_RXNE, DISABLE);
	USART_DMACmd(COM_PORT, USART_DMAReq_Tx, ENABLE);
	USART_DMACmd(COM_PORT, USART_DMAReq_Rx, ENABLE);
	USART_Cmd(COM_PORT, ENABLE);

	usart2_DMA_config();
	usart2_NVIC_config();
	Rcv2_Counter = 0;
	Rcv2_Flag = 0;
	Txd2_Flag = 0;
	usart2_mode = 1;
	bd_set_ = bd;
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
void usart2_send_buff(uint8_t *pbuffer, uint32_t size)
{
//	int i;
//	DMA_Cmd (DMA1_Channel6, DISABLE);
//	USART_DMACmd(USART2, USART_DMAReq_Rx, DISABLE);
//	USART_TX_HIGH;
//	for(i =0; i<5000; i++){
//		;
//	}
	if(usart2_mode == 1){
		usart2_init_tx(bd_set_);
	}
	if(size >= TXD2_BUFFSIZE){
		size = TXD2_BUFFSIZE;
	}
	memcpy(Txd2_Buffer, pbuffer, size);
	DMA_Cmd (COM_DMA_TX,DISABLE);
	COM_DMA_TX->CMAR =  (u32)Txd2_Buffer;
	COM_DMA_TX->CNDTR = size;
	DMA_Cmd (COM_DMA_TX,ENABLE);
	Txd2_Flag    =1;
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
void DMA1_Channel7_IRQHandler(void)
{
  if(DMA_GetFlagStatus(DMA1_FLAG_TC7)==SET){
      DMA_ClearITPendingBit(DMA1_IT_GL7);
      DMA_ClearFlag(DMA1_FLAG_TC7);
      DMA_Cmd (COM_DMA_TX,DISABLE);
      Txd2_Flag = 0;
	  
//	  USART_ClearFlag(USART2, USART_FLAG_RXNE);
//	  DMA_Cmd (DMA1_Channel6, ENABLE);
//	  USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
  }
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
extern Usart_State usart_state;
extern struct rt_messagequeue usart2_r_mq;

void USART2_IRQHandler(void)
{
	uint32_t i;
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	{
		USART_ReceiveData(USART2);
		Rcv2_Counter = RCV2_BUFFSIZE - DMA1_Channel6->CNDTR;
		if(Rcv2_Counter)
		{
			Rcv2_Flag = 1;
		}

		DMA_Cmd (DMA1_Channel6, DISABLE);
		DMA1_Channel6->CMAR =  (u32)Rcv2_Buffer;
		DMA1_Channel6->CNDTR =  RCV2_BUFFSIZE;
		DMA_Cmd (DMA1_Channel6,ENABLE);
	}
	if(Rcv2_Flag ==1 && Rcv2_Counter ==24){
		Rcv2_Flag = 0;
		switch(usart_state){
			case USB_SERIAL_DISABLE:
				for(i =0; i<12; i++){
					uart_receive_data(Rcv2_Buffer[i+12]);
				}
				break;
			case USB_SERIAL_TO_SERVO:
				for(i =0; i<12; i++){
					rt_mq_send(&usart2_r_mq, &Rcv2_Buffer[i+12], 1);		
				}
				break;
			case BLUETOOTH_MODE:
				usart3_send_buff(&Rcv2_Buffer[12], 12);
				break;
			default:
				break;
		}
	}
	else if(Rcv2_Flag ==1 && Rcv2_Counter ==12){
		Rcv2_Flag = 0;
		if(Rcv2_Buffer[0] ==0xFA && Rcv2_Buffer[11] ==0xFE && Rcv2_Buffer[3] ==0x02){	
			switch(usart_state){
				case USB_SERIAL_DISABLE:
					for(i =0; i<12; i++){
						uart_receive_data(Rcv2_Buffer[i]);
					}
					break;
				case USB_SERIAL_TO_SERVO:
					for(i =0; i<12; i++){
						rt_mq_send(&usart2_r_mq, &Rcv2_Buffer[i], 1);		
					}
//					usart1_send_buff(&Rcv2_Buffer[0], 12);
					break;
				case BLUETOOTH_MODE:
					usart3_send_buff(&Rcv2_Buffer[0], 12);
					break;
				default:
					break;
			}
		}
	}
}


/******************* (C) COPYRIGHT 2016*****END OF FILE****/

