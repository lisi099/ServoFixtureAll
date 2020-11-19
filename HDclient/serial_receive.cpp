#include "serial_receive.h"


/*----------------------------------------------
Location:  B0      B1      B2      B3      B4      B5      B6      B7      B8      B9      B10     B11     B12
HexNumber: 0x41    0x42    0x43    CMD     data1   data2   data3   data4   data5   data6   data7   crcL    crcH

CMD:       0xc1        0xc3        0xc5        0xc7        0xc9
Function:  request     openlock    update      finish      erase_flash
------------------------------------------------*/
#define BUFF_COUNT      13
#define CRC_LOW_BYTE    11
#define CRC_HIGH_BYTE   12
#define CRC_BEGAIN      3
#define CRC_COUNT       8

#define PACKAGE_FIRST   0x41
#define PACKAGE_SECOND  0x42
#define PACKAGE_THIRD   0x43
/*--------------------------------------------------
 * function:
 * description:
 * input:
 * output:
---------------------------------------------------*/
serial_receive::serial_receive()
{
    size_of_buff    = 0;
    size_of_check   = 0;
    package_count   = 0;
}
/*--------------------------------------------------
 * function:
 * description:
 * input:
 * output:
---------------------------------------------------*/
uint8_t serial_receive::get_package_cmd(void)
{
    return package_cmd;
}
/*--------------------------------------------------
 * function:
 * description:
 * input:
 * output:
---------------------------------------------------*/
uint8_t* serial_receive::get_package_data(void)
{
    return package_data;
}
/*--------------------------------------------------
 * function:
 * description:
 * input:
 * output:
---------------------------------------------------*/
uint16_t serial_receive:: UpdateCRC16(uint16_t crcIn, uint8_t byte)
{
   crcIn = crcIn ^ ((uint16_t)byte << 8);
    for ( uint16_t i = 0; i < 8; i++)
    {
        if ((crcIn & 0x8000) == 0x8000)
            crcIn = (crcIn << 1) ^ 0x1021;
        else
            crcIn = crcIn << 1;
    }
    return crcIn & 0xffff;
}
/*--------------------------------------------------
 * function:
 * description:
 * input:
 * output:
---------------------------------------------------*/
uint16_t serial_receive::Cal_CRC16(uint8_t *data, uint32_t size)
{
    uint16_t crc = 0;
    for (uint16_t i = 0; i < size; i++)
    {
        crc = UpdateCRC16(crc, data[i]);
    }
    return crc & 0xffffu;
}
/*--------------------------------------------------
 * function:
 * description:
 * input:
 * output:
---------------------------------------------------*/
uint8_t serial_receive::check_data(QByteArray &data)
{
    size_of_buff = data.size();
    uint64_t  remain_count = size_of_buff - size_of_check;
    while(1)
    {
      if(remain_count < BUFF_COUNT)
      {
           return  0;
      }
      if( (quint8)(data.data()[size_of_check]) == PACKAGE_FIRST && (quint8)(data.data()[size_of_check+1]) == PACKAGE_SECOND
       && (quint8)(data.data()[size_of_check+2]) == PACKAGE_THIRD)
      {
          return  1;
      }
      remain_count --;
      size_of_check++;
    }
}
/*--------------------------------------------------
 * function:
 * description:
 * input:
 * output:
---------------------------------------------------*/
uint8_t serial_receive::check_package(QByteArray &data)
{
    uint8_t buff[BUFF_COUNT] = {0};
    uint16_t crc1 = 0;
    uint16_t crc2 = 0;
    if( check_data(data) )
    {
        for(uint8_t i=0; i<BUFF_COUNT; i++)
        {
            buff[i] = (uint8_t)(data.data()[size_of_check]);
            size_of_check++;
        }
        crc1 = Cal_CRC16(&buff[CRC_BEGAIN], CRC_COUNT);
        crc2 = (uint16_t)(buff[CRC_LOW_BYTE] | (buff[CRC_HIGH_BYTE] <<8));
        if(crc1 == crc2)
        {
            package_cmd = buff[CRC_BEGAIN];
            package_data[0] = buff[CRC_BEGAIN+1];
            package_data[1] = buff[CRC_BEGAIN+2];
            package_data[2] = buff[CRC_BEGAIN+3];
            package_data[3] = buff[CRC_BEGAIN+4];
            package_data[4] = buff[CRC_BEGAIN+5];
            package_data[5] = buff[CRC_BEGAIN+6];
            package_data[6] = buff[CRC_BEGAIN+7];
            package_count++;
            qDebug()<<"[Total]--"<<size_of_buff;
            qDebug()<<"[Check]--"<<size_of_check;
            return 1;
        }
    }
    return 0;
}
