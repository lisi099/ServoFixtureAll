/******************** (C) COPYRIGHT 2016 ********************
* Filename:
* Description:
* Platform:
* Other info:
* Author         Date          Notes
xiangbin->huang   2017->04->11    The first version template
***************************************************************/
#include "usart1_fifo.h"
#include <string.h>
#include <stdio.h>
#include "common_fifo.h"
#include "usart1.h"

#define     USART1_FIFO_PAKAGE_SIZE    	512
struct      Common_Fifo_Stru        		Usart1_Fifo_Rx;
struct 			Usart1Package        				Usart1_Package_Rx[USART1_FIFO_PAKAGE_SIZE];

struct      Common_Fifo_Stru        		Usart1_Fifo_Tx;
struct 			Usart1Package        				Usart1_Package_Tx[USART1_FIFO_PAKAGE_SIZE];

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



/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
void usart1_fifo_tx_init(void)
{
   common_fifo_init(&Usart1_Fifo_Tx, USART1_FIFO_PAKAGE_SIZE, sizeof(struct Usart1Package), Usart1_Package_Tx);
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
void usart1_fifo_tx_insert(struct Usart1Package *msg)
{
   common_fifo_insert_package(&Usart1_Fifo_Tx, msg);
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
uint8_t usart1_fifo_tx_get(struct Usart1Package *msg)
{
  return common_fifo_get_package(&Usart1_Fifo_Tx, msg);
}
/******************* (C) COPYRIGHT 2016*****END OF FILE****/
