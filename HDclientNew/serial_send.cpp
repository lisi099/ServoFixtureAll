#include "serial_send.h"

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

serial_send::serial_send()
{

}
/*--------------------------------------------------
 * function:
 * description:
 * input:
 * output:
---------------------------------------------------*/
uint16_t serial_send:: UpdateCRC16(uint16_t crcIn, uint8_t byte)
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
uint16_t serial_send::Cal_CRC16(uint8_t *data, uint32_t size)
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
 void serial_send:: Cal_CRC16_Buf(uint8_t *buf)
{
    uint16_t crc = this->Cal_CRC16(&buf[3], CRC_COUNT);
    buf[CRC_LOW_BYTE]   =   (uint8_t)(crc);
    buf[CRC_HIGH_BYTE]  =   (uint8_t)(crc>>8);
}
/*--------------------------------------------------
 * function:
 * description:
 * input:
 * output:
---------------------------------------------------*/
void serial_send:: serial_send_request(QByteArray &buftomcu)
{
    uint8_t txbuf[13]={0};
    txbuf[0]=0x41; txbuf[1]=0x42; txbuf[2]=0x43;txbuf[3]=0xC1;
    Cal_CRC16_Buf(txbuf);
    for(quint8 j=0; j<BUFF_COUNT; j++)
    {
     buftomcu.append((char)(txbuf[j]));
    }
}
/*--------------------------------------------------
 * function:
 * description:
 * input:
 * output:
---------------------------------------------------*/
void serial_send:: serial_send_open_lock(QByteArray &buftomcu)
{
    uint8_t txbuf[13]={0};
    txbuf[0]=0x41; txbuf[1]=0x42; txbuf[2]=0x43;txbuf[3]=0xC3;
    Cal_CRC16_Buf(txbuf);
    for(quint8 j=0; j<BUFF_COUNT; j++)
    {
     buftomcu.append((char)(txbuf[j]));
    }
}
/*--------------------------------------------------
 * function:
 * description:
 * input:
 * output:
---------------------------------------------------*/
void serial_send:: serial_send_update(QByteArray &buftomcu, uint8_t *adress, uint8_t *data)
{
    uint8_t txbuf[13]={0};
    txbuf[0] = 0x41;        txbuf[1] = 0x42;        txbuf[2] = 0x43;        txbuf[3] = 0xC5;
    txbuf[4] = adress[0];   txbuf[5] = adress[1];   txbuf[6] = adress[2];
    txbuf[7] = data[0];     txbuf[8] = data[1];     txbuf[9] = data[2];     txbuf[10] = data[3];
    //qDebug("[data-1]%d, %d, %d, %d",data[0], data[1], data[2], data[3]);
    Cal_CRC16_Buf(txbuf);
    for(quint8 j=0; j<BUFF_COUNT; j++)
    {
     buftomcu.append((char)(txbuf[j]));
    }
}
/*--------------------------------------------------
 * function:
 * description:
 * input:
 * output:
---------------------------------------------------*/
void serial_send:: serial_send_finish(QByteArray &buftomcu, uint8_t *adress, uint8_t *data)
{
    uint8_t txbuf[13]={0};
    txbuf[0]=0x41;           txbuf[1]=0x42;         txbuf[2]=0x43;          txbuf[3]=0xC7;
    txbuf[4] = adress[0];   txbuf[5] = adress[1];   txbuf[6] = adress[2];
    txbuf[7] = data[0];     txbuf[8] = data[1];     txbuf[9] = data[2];     txbuf[10] = data[3];
    Cal_CRC16_Buf(txbuf);
    for(quint8 j=0; j<BUFF_COUNT; j++)
    {
     buftomcu.append((char)(txbuf[j]));
    }
}
/*--------------------------------------------------
 * function:
 * description:
 * input:
 * output:
---------------------------------------------------*/
void serial_send:: serial_erase_flash(QByteArray &buftomcu)
{
    uint8_t txbuf[13]={0};
    txbuf[0]=0x41; txbuf[1]=0x42; txbuf[2]=0x43;txbuf[3]=0xC9;
    Cal_CRC16_Buf(txbuf);
    for(quint8 j=0; j<BUFF_COUNT; j++)
    {
     buftomcu.append((char)(txbuf[j]));
    }
}

/*--------------------------------------------------
 * function:
 * description:
 * input:
 * output:
---------------------------------------------------*/
void serial_send:: serial_send_info(QByteArray &buftomcu)
{
    uint8_t txbuf[13]={0};
    txbuf[0]=0x41; txbuf[1]=0x42; txbuf[2]=0x43;txbuf[3]=0xb1;
    Cal_CRC16_Buf(txbuf);
    for(quint8 j=0; j<BUFF_COUNT; j++)
    {
     buftomcu.append((char)(txbuf[j]));
    }
}
//-----------------------------------end of file----------------------------------------
