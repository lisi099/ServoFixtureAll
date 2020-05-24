/******************** (C) COPYRIGHT 2016 ********************
* Filename:
* Description:
* Platform:
* Other info:
* Author         Date          Notes
xiangbin.huang   2017.02.21    The first version template
***************************************************************/

#include "system_tick.h"

static volatile uint32_t  systick_value = 0;
static void (*user_systick)(void);
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
void SysTick_Handler(void)
{
    systick_value ++;
    if(user_systick != NULL)
		{
      user_systick();
		}
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
void set_user_systick_fun(void (*func) (void))
{
		user_systick = func;
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
void set_board_systick(void)
{
    SysTick->LOAD = SYSTICK_1MS_TICKS - 1;
    NVIC_SetPriority(SysTick_IRQn, 0);
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
uint32_t get_system_ms(void)
{
		return systick_value;
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
void delay_ms(uint32_t ms)
{
		uint32_t now = get_system_ms();
    while(get_system_ms() < now + ms);
}
/******************* (C) COPYRIGHT 2016*****END OF FILE****/


