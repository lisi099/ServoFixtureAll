/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/

#include "crc16.h"
/*************************************************************
  Function   :  
  Description: 
  Input      : none        
  return     : none    
*************************************************************/
static uint16_t UpdateCRC16(uint16_t crcIn, uint8_t byte) 
{
     uint16_t i;
     crcIn = crcIn ^ ((uint16_t)byte << 8);

    for ( i = 0; i < 8; i++)
    {
        if ((crcIn & 0x8000) == 0x8000)
            crcIn = (crcIn << 1) ^ 0x1021;
        else
            crcIn = crcIn << 1;
    }
    return crcIn & 0xffff;
}
/*************************************************************
  Function   :  
  Description: 
  Input      : none        
  return     : none    
*************************************************************/
uint16_t Cal_CRC16(uint8_t *data, uint32_t size)
{
    uint16_t crc = 0;
    uint32_t i;
    for (i = 0; i < size; i++)
    {
      crc = UpdateCRC16(crc, data[i]);
    }
    return crc & 0xffffu;
}
/******************* (C) COPYRIGHT 2016*****END OF FILE****/
