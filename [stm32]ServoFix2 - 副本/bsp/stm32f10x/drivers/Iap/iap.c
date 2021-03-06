/******************** (C) COPYRIGHT 2016 ********************
 * 文件名  ：iap.c
 * 描述    ：iap 应用函数库
 *
 * 实验平台：
 * 硬件连接：

***************************************************************/
#include "includes.h"
#include "iap.h"
#include "flash.h"
#include "board_info.h"
#include "usart1.h"
#include "crc16.h"
#include "stm32_crc32.h"

#define TX_BUFF_SIZE	13

typedef  void (*pFunction)(void);
pFunction Jump_To_Application;

struct 	Iap_Status_Stru 	Iap_Starus = {0, 0, 0, {0}};
static 	uint8_t Erro_Erase	=	0;
static 	uint8_t update_status = 0;
/*************************************************************
  Function   : IAP_JumpToApplication
  Description:
  Input      : none
  return     : none
*************************************************************/
uint8_t get_update_status(void)
{
    return update_status;
}
/*************************************************************
  Function   : IAP_JumpToApplication
  Description:
  Input      : none
  return     : none
*************************************************************/
void IAP_JumpToApplication(void)
{
    u32 JumpAddress;

    if(((*(__IO u32 *)FLASH_IAP_ADDR) & 0x2FFE0000) == 0x20000000)//
    {
        JumpAddress = *(__IO u32 *)(FLASH_IAP_ADDR + 4); //
        Jump_To_Application = (pFunction)JumpAddress;//
        __set_MSP(*(__IO u32*)FLASH_IAP_ADDR);
        Jump_To_Application();
    }
}
/*************************************************************
  Function   :
  Description: 完成后回复PC端
  Input      : none
  return     : none
*************************************************************/
void reply_request(uint8_t CMD_TYPE)
{
    uint16_t crc;
    uint8_t *data_ptr = usart1_get_tx_ptr();
    data_ptr[0] = 0x41;
    data_ptr[1] = 0x42;
    data_ptr[2] = 0x43;
    data_ptr[3] = CMD_TYPE + 1;
    crc = Cal_CRC16(&data_ptr[3], 8);
    data_ptr[11] = (uint8_t)crc;
    data_ptr[12] = (uint8_t)(crc >> 8);
    usart1_send_buff(data_ptr, TX_BUFF_SIZE);
}
/*************************************************************
  Function   :
  Description: 回复PC端查询指令
  Input      : none
  return     : none
*************************************************************/
void reply_info(void)
{
    uint16_t crc;
    uint8_t *data_ptr = usart1_get_tx_ptr();
    data_ptr[0] = 0x41;
    data_ptr[1] = 0x42;
    data_ptr[2] = 0x43;
    data_ptr[3] = CMD_READ + 1;
    data_ptr[4] = HARDWEAR_INFO;//硬件版本
    data_ptr[5] = SOFTWEAR_INFO;//软件版本
    data_ptr[6] = 0;//程序位置
    data_ptr[7] = Erro_Erase;//擦写状态
    data_ptr[8] = Iap_Starus.Lock_flag;//锁状态
    data_ptr[9] = 0;
    data_ptr[10] = 0;
    crc = Cal_CRC16(&data_ptr[3], 8);
    data_ptr[11] = (uint8_t)crc;
    data_ptr[12] = (uint8_t)(crc >> 8);
    usart1_send_buff(data_ptr, TX_BUFF_SIZE);
}
/*************************************************************
  Function   :
  Description:数据拷贝
  Input      : none
  return     : none
*************************************************************/
static void iap_data_copy(uint8_t cmd, uint8_t *data)
{
    uint8_t i;
    Iap_Starus.Iap_Cmd = cmd;

    for(i = 0; i < 7; i++)
    {
        Iap_Starus.Data[i] = data[i];
    }
}
/*************************************************************
  Function   :
  Description: 检查接收数据
  Input      : none
  return     : none
*************************************************************/
uint8_t iap_data_receieve(void)
{
    uint8_t *data_ptr;
    uint16_t crc;
    uint16_t crc_usart;

    if(usart1_get_rx_flag())
    {
        usart1_reset_rx_flag();
        data_ptr = usart1_get_rx_ptr();
        crc = Cal_CRC16(&data_ptr[3], 8);
        crc_usart = data_ptr[11] | (data_ptr[12] << 8);

        if(crc == crc_usart)
        {
            iap_data_copy(data_ptr[3], &data_ptr[4]);
            return RECEVIEVE_DONE;
        }
    }

    return RECEVIEVE_DOING;
}
/*************************************************************
  Function   :
  Description: 擦除FLASH
  Input      : none
  return     : none
*************************************************************/
uint8_t  iap_erase_flash(void)
{
    uint8_t erro;

    if(Iap_Starus.Lock_flag == LOCK_OFF)
    {
        if(Iap_Starus.Erase_flag == ERASE_OK)
        {
            erro = erase_flash();
            return erro;
        }
    }

    return erro;
}
/*************************************************************
  Function   : iap_update
  Description: 程序更新与响应
  Input      : none
  return     : none
*************************************************************/
void iap_update(void)
{
    switch (Iap_Starus.Iap_Cmd)
    {
        case CMD_REQUEST:
        {
            reply_request(CMD_REQUEST);
            break;
        }

        case CMD_UNLOCK:
        {
            Iap_Starus.Lock_flag = LOCK_OFF;
            reply_request(CMD_UNLOCK);
            break;
        }

        case CMD_ERASE:
        {
            Iap_Starus.Erase_flag = ERASE_OK;
            Erro_Erase = iap_erase_flash();
            erase_flash_part(FLASH_INFO_ADDR, FLASH_IAP_ADDR);
            reply_request(CMD_ERASE);
            break;
        }

        case CMD_UPDATE:
        {
            uint32_t data = 0;
            uint32_t adress = 0;
            data = BYTETOINT(Iap_Starus.Data[6], Iap_Starus.Data[5], Iap_Starus.Data[4], Iap_Starus.Data[3]);
            adress = BYTETOINT(0, Iap_Starus.Data[2], Iap_Starus.Data[1], Iap_Starus.Data[0]);
            flash_write_one_word(FLASH_IAP_ADDR + adress, data);
            reply_request(CMD_UPDATE);
            break;
        }

        case  CMD_FINISH:
        {
            uint32_t length = 0;
            uint32_t crc32 = 0;
            uint32_t stm32_crc = 0;
            crc32 = BYTETOINT(Iap_Starus.Data[6], Iap_Starus.Data[5], Iap_Starus.Data[4], Iap_Starus.Data[3]);
            length = BYTETOINT(0, Iap_Starus.Data[2], Iap_Starus.Data[1], Iap_Starus.Data[0]);
            CRC_ResetDR();
            stm32_crc = CRC_CalcBlockCRC((uint32_t *)FLASH_IAP_ADDR, length);

            if( stm32_crc == crc32)
            {
                Iap_Starus.Lock_flag = LOCK_ON;
                reply_request(CMD_FINISH);
                flash_write_one_word(FLASH_INFO_ADDR, 0x02040608);
                update_status = 1;
            }

            break;
        }

        case CMD_READ:
        {
            reply_info();
            break;
        }

        default:
            break;
    }
}
/******************* (C) COPYRIGHT 2016*****END OF FILE****/


