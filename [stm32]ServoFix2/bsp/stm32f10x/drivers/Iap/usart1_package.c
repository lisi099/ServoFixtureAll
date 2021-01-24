/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#include "usart1_package.h"
#include "usart1_fifo.h"
#include "usart1.h"
#include "crc16.h"
#include "flash.h"
#include "board_info.h"
#include <rtthread.h>

uint8_t Mcu_Reset_Flag = 0;
/*************************************************************
  Function   :
  Description: ����������
  Input      : none
  return     : none
*************************************************************/
void usart1_length_13_data_receieve(void)
{
    uint8_t *data_ptr;
    uint16_t crc;
    uint16_t crc_usart;
    struct Usart1Package package;

    if(usart1_get_counter() == 13)
    {
        usart1_reset_rx_flag();
        data_ptr = usart1_get_rx_ptr();
        crc = Cal_CRC16(&data_ptr[3], 8);
        crc_usart = data_ptr[11] | (data_ptr[12] << 8);

        if(crc == crc_usart)
        {
            package.cmd = data_ptr[3];
            package.data[0] = data_ptr[4];
            package.data[1] = data_ptr[5];
            package.data[2] = data_ptr[6];
            package.data[3] = data_ptr[7];
            package.data[4] = data_ptr[8];
            package.data[5] = data_ptr[9];
            package.data[6] = data_ptr[10];
            usart1_fifo_rx_insert(&package);
        }
    }
}
/*************************************************************
  Function   :
  Description: �ظ�PC�˲�ѯָ��
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
    data_ptr[4] = HARDWEAR_INFO;//Ӳ���汾
    data_ptr[5] = SOFTWEAR_INFO;//����汾
    data_ptr[6] = 0;//����λ��
    data_ptr[7] = 0;//��д״̬
    data_ptr[8] = 0;//��״̬
    data_ptr[9] = 0;
    data_ptr[10] = 0;
    crc = Cal_CRC16(&data_ptr[3], 8);
    data_ptr[11] = (uint8_t)crc;
    data_ptr[12] = (uint8_t)(crc >> 8);

    if(usart1_get_tx_flag() == 0)
    {
        usart1_send_buff(data_ptr, TX_BUFF_SIZE);
    }
}
/*************************************************************
  Function   :
  Description: �ظ�PC�˲�ѯָ��
  Input      : none
  return     : none
*************************************************************/
void reply_request(void)
{
    uint16_t crc;
    uint8_t *data_ptr = usart1_get_tx_ptr();
    data_ptr[0] = 0x41;
    data_ptr[1] = 0x42;
    data_ptr[2] = 0x43;
    data_ptr[3] = CMD_REQUEST + 1;
    data_ptr[4] = 1;//Ӳ���汾
    data_ptr[5] = 1;//����汾
    data_ptr[6] = 0;//����λ��
    data_ptr[7] = 0;//��д״̬
    data_ptr[8] = 0;//��״̬
    data_ptr[9] = 0;
    data_ptr[10] = 0;
    crc = Cal_CRC16(&data_ptr[3], 8);
    data_ptr[11] = (uint8_t)crc;
    data_ptr[12] = (uint8_t)(crc >> 8);

    if(usart1_get_tx_flag() == 0)
    {
        usart1_send_buff(data_ptr, TX_BUFF_SIZE);
    }
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
void usart1_length_13_data_process(void)
{
    struct Usart1Package package;

    if(usart1_fifo_rx_get(&package) == 1)
    {
        switch (package.cmd)
        {
            case 0xc1:
                erase_flash_part(FLASH_INFO_ADDR, FLASH_END_ADDR);
                flash_write_one_word(FLASH_INFO_ADDR, 0x00);
                reply_request();
                Mcu_Reset_Flag = 1;

                if(get_reset_mcu_flag() == 1)
                {
                    rt_thread_delay(100);
                    NVIC_SystemReset();
                }

                break;

            case 0xb1:
                reply_info();
                break;
        }
    }
}
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
uint8_t get_reset_mcu_flag(void)
{
    return Mcu_Reset_Flag;
}


/******************* (C) COPYRIGHT 2016*****END OF FILE****/
