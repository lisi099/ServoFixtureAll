/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#include "usart1.h"
#include "usart2.h"
#include "string.h"
#include "usart1_package.h"
#include <rtthread.h>

#define COM_PORT_DR_Base       (USART1_BASE + 0x04)
#define COM_DMA_TX              DMA1_Channel4
#define COM_DMA_RX              DMA1_Channel5
#define COM_DMA_CLOCK           RCC_AHBPeriph_DMA1

#define COM_PORT                USART1
#define COM_PORT_APB2_CLOCK     RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1
#define COM_PORT_SOURCE         GPIOA
#define COM_PORT_PIN_TX         GPIO_Pin_9
#define COM_PORT_PIN_RX         GPIO_Pin_10
#define COM_BAUDRATE            115200

#define         TXD1_BUFFSIZE           128
#define         RCV1_BUFFSIZE           128
uint8_t  Txd1_Buffer[TXD1_BUFFSIZE];
uint8_t  Rcv1_Buffer[RCV1_BUFFSIZE];
volatile uint32_t Rcv1_Counter;
volatile uint8_t  Rcv1_Flag;
volatile uint8_t  Txd1_Flag;

uint32_t data_count_rx = 0;
uint32_t data_count_tx = 0;
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
inline uint8_t usart1_get_tx_flag(void)
{
    return Txd1_Flag;
}
inline uint8_t usart1_get_rx_flag(void)
{
    return Rcv1_Flag;
}
inline void usart1_reset_rx_flag(void)
{
    Rcv1_Flag = 0;
}
inline uint8_t* usart1_get_tx_ptr(void)
{
    return Txd1_Buffer;
}
inline uint8_t* usart1_get_rx_ptr(void)
{
    return Rcv1_Buffer;
}
inline uint32_t usart1_get_counter(void)
{
    return Rcv1_Counter;
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
static void usart1_config(uint32_t bd)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    RCC_APB2PeriphClockCmd(COM_PORT_APB2_CLOCK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = COM_PORT_PIN_TX;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(COM_PORT_SOURCE, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = COM_PORT_PIN_RX;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(COM_PORT_SOURCE, &GPIO_InitStructure);

    USART_DeInit(COM_PORT);
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
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
static void usart1_DMA_config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_DeInit(COM_DMA_TX);
    DMA_InitStructure.DMA_PeripheralBaseAddr = COM_PORT_DR_Base;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)Txd1_Buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = sizeof(Txd1_Buffer);
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(COM_DMA_TX, &DMA_InitStructure);
    DMA_ITConfig(COM_DMA_TX, DMA_IT_TC, ENABLE);
    DMA_Cmd(COM_DMA_TX, DISABLE);

    DMA_DeInit(COM_DMA_RX);
    DMA_InitStructure.DMA_PeripheralBaseAddr = COM_PORT_DR_Base;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)Rcv1_Buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = RCV1_BUFFSIZE;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(COM_DMA_RX, &DMA_InitStructure);
    DMA_ITConfig(COM_DMA_RX, DMA_IT_TC, DISABLE);
    DMA_Cmd(COM_DMA_RX, ENABLE);
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
static void usart1_NVIC_config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
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
void usart1_init(uint32_t bd)
{
    usart1_config(bd);
    usart1_DMA_config();
    usart1_NVIC_config();
    Rcv1_Counter = 0;
    Rcv1_Flag = 0;
    Txd1_Flag = 0;
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
void usart1_send_buff(uint8_t* pbuffer, uint32_t size)
{
    data_count_tx += size;

    if(size >= TXD1_BUFFSIZE)
    {
        size = TXD1_BUFFSIZE;
    }

    memcpy(Txd1_Buffer, pbuffer, size);
    DMA_Cmd(COM_DMA_TX, DISABLE);
    COM_DMA_TX->CMAR = (u32)Txd1_Buffer;
    COM_DMA_TX->CNDTR = size;
    DMA_Cmd(COM_DMA_TX, ENABLE);
    Txd1_Flag    = 1;
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
void DMA1_Channel4_IRQHandler(void)
{
    if(DMA_GetFlagStatus(DMA1_FLAG_TC4) == SET)
    {
        DMA_ClearITPendingBit(DMA1_IT_GL4);
        DMA_ClearFlag(DMA1_FLAG_TC4);
        DMA_Cmd(COM_DMA_TX, DISABLE);
        Txd1_Flag = 0;
    }
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
extern volatile uint8_t pc_data_state_;
extern volatile uint8_t test_data_state_;

void USART1_IRQHandler(void)
{
    uint8_t i;

    if(USART_GetITStatus(COM_PORT, USART_IT_IDLE) != RESET)
    {
        USART_ReceiveData(COM_PORT);
        Rcv1_Counter = RCV1_BUFFSIZE - COM_DMA_RX->CNDTR;

        if(Rcv1_Counter)
        {
            Rcv1_Flag = 1;
            data_count_rx += Rcv1_Counter;
        }

        DMA_Cmd(COM_DMA_RX, DISABLE);
        COM_DMA_RX->CMAR = (u32)Rcv1_Buffer;
        COM_DMA_RX->CNDTR =  RCV1_BUFFSIZE;
        DMA_Cmd(COM_DMA_RX, ENABLE);
    }

    if(USART_GetFlagStatus(COM_PORT, USART_FLAG_ORE) == SET)
    {
        int c = COM_PORT->DR;
        COM_PORT->SR = (uint16_t)~USART_FLAG_ORE;
    }

    if(Rcv1_Flag == 1)
    {
        Rcv1_Flag = 0;
        if(Rcv1_Counter == 13 && Rcv1_Buffer[0] == 0x41)
        {
            usart1_length_13_data_receieve();
        }
        else if(Rcv1_Counter == 92 && Rcv1_Buffer[0] == 0x5A)
        {
            pc_data_state_ = 1;
        }
        else if(Rcv1_Counter == 8 && Rcv1_Buffer[0] == 0x5A)
        {
            test_data_state_ = 1;
        }

    }
}

/******************* (C) COPYRIGHT 2016*****END OF FILE****/

