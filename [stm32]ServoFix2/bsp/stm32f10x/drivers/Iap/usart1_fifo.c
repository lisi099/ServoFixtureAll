/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#include "usart1_fifo.h"
#include <string.h>
#include <stdio.h>
#include "common_fifo.h"
#include "usart1.h"

#define     USART1_FIFO_PAKAGE_SIZE    		5
struct      Common_Fifo_Stru        		Usart1_Fifo_Rx;
struct 		Usart1Package        			Usart1_Package_Rx[USART1_FIFO_PAKAGE_SIZE];

/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
void usart1_fifo_rx_init(void)
{
    common_fifo_init(&Usart1_Fifo_Rx, USART1_FIFO_PAKAGE_SIZE, sizeof(struct Usart1Package), Usart1_Package_Rx);
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
void usart1_fifo_rx_insert(struct Usart1Package *msg)
{
    common_fifo_insert_package(&Usart1_Fifo_Rx, msg);
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
uint8_t usart1_fifo_rx_get(struct Usart1Package *msg)
{
    return common_fifo_get_package(&Usart1_Fifo_Rx, msg);
}

/******************* (C) COPYRIGHT 2016*****END OF FILE****/
