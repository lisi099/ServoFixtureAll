/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#include "stm32f10x.h"
#include "usart3.h"
#include "usart2.h"
#include "usart1.h"
#include "string.h"

#define COM_PORT_DR_Base        (USART3_BASE+0x04)
#define COM_DMA_TX              DMA1_Channel2
#define COM_DMA_RX              DMA1_Channel3
#define COM_DMA_CLOCK           RCC_AHBPeriph_DMA1

#define COM_PORT                USART3
#define COM_PORT_APB1_CLOCK     RCC_APB1Periph_USART3
#define COM_PORT_APB2_CLOCK     RCC_APB2Periph_GPIOB
#define COM_PORT_SOURCE         GPIOB
#define COM_PORT_PIN_TX         GPIO_Pin_10
#define COM_PORT_PIN_RX         GPIO_Pin_11
#define COM_BAUDRATE            9600


uint8_t  Txd3_Buffer[TXD3_BUFFSIZE];
uint8_t  Rcv3_Buffer[RCV3_BUFFSIZE];

volatile uint32_t Rcv3_Counter;
volatile uint8_t  Rcv3_Flag;
volatile uint8_t  Txd3_Flag;
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
uint8_t usart3_get_tx_flag(void)
{
  return Txd3_Flag;
}
uint8_t usart3_get_rx_flag(void)
{
  return Rcv3_Flag;
}
void usart3_reset_rx_flag(void)
{
  Rcv3_Flag = 0;
}
uint8_t* usart3_get_tx_ptr(void)
{
  return Txd3_Buffer;
}
uint8_t* usart3_get_rx_ptr(void)
{
  return Rcv3_Buffer;
}
uint32_t usart3_get_counter(void)
{
  return Rcv3_Counter;
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
static void usart3_config(void)
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

	USART_DeInit(USART3);
    USART_InitStructure.USART_BaudRate = COM_BAUDRATE;
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
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
static void usart3_DMA_config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_DeInit(COM_DMA_TX);
    DMA_InitStructure.DMA_PeripheralBaseAddr = COM_PORT_DR_Base;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)Txd3_Buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = TXD3_BUFFSIZE;
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
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)Rcv3_Buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = RCV3_BUFFSIZE;
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
static void usart3_NVIC_config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
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
void usart3_init(void)
{
   usart3_config();
   usart3_DMA_config();
   usart3_NVIC_config();
   Rcv3_Counter = 0;
   Rcv3_Flag = 0;
   Txd3_Flag = 0;
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
void usart3_send_buff(uint8_t *pbuffer, uint32_t size)
{
	if(size > TXD3_BUFFSIZE){
		size = TXD3_BUFFSIZE;
	}
   memcpy(Txd3_Buffer, pbuffer, size);
   DMA_Cmd (COM_DMA_TX,DISABLE);
   COM_DMA_TX->CMAR =  (u32)Txd3_Buffer;
   COM_DMA_TX->CNDTR = size;
   DMA_Cmd (COM_DMA_TX,ENABLE);
   Txd3_Flag    =1;
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
void DMA1_Channel2_IRQHandler(void)
{
  if(DMA_GetFlagStatus(DMA1_FLAG_TC2)==SET)
  {
      DMA_ClearITPendingBit(DMA1_IT_GL2);
      DMA_ClearFlag(DMA1_FLAG_TC2);
      DMA_Cmd (COM_DMA_TX,DISABLE);
      Txd3_Flag = 0;
  }
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
extern Usart_State usart_state;

void USART3_IRQHandler(void)
{
  if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
  {
      USART_ReceiveData(USART3);
      Rcv3_Counter = RCV3_BUFFSIZE - DMA1_Channel3->CNDTR;
      if(Rcv3_Counter)
      {
         Rcv3_Flag = 1;
      }
      DMA_Cmd (DMA1_Channel3, DISABLE);
      DMA1_Channel3->CMAR = (u32)Rcv3_Buffer;
      DMA1_Channel3->CNDTR = RCV3_BUFFSIZE;
      DMA_Cmd(DMA1_Channel3, ENABLE);
	  
	  if(Rcv3_Flag ==1 && Rcv3_Counter==12){
		Rcv3_Flag = 0;
		switch(usart_state){
			case BLUETOOTH_MODE:
				usart2_send_buff(Rcv3_Buffer, Rcv3_Counter);
				break;
		  default:
			  break;
		}
	 }
  }
}
/******************* (C) COPYRIGHT 3016*****END OF FILE****/

