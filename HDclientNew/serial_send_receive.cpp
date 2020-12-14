#include "serial_send_receive.h"



#define ANSWER_TIME_OUT     2000
#define RECEIEVE_TIMEOUT    0
#define SEND_FINISH         1
#define RECEIEVE_FINISH     2

serial_send_receive::serial_send_receive()
{
    state    = 0;
    times    = 0;
    ticks    = 0;
    cmd      = 0;
    serial_receive_data =   new serial_receive;
    serial_send_data    =   new serial_send;
}

serial_send_receive::~serial_send_receive()
{
    delete serial_receive_data;
    delete serial_send_data;
}
/*--------------------------------------------------
 * function:
 * description:
 * input:
 * output:
---------------------------------------------------*/
/*CMD:       0xc1        0xc3        0xc5        0xc7        0xc9
Function:  request     openlock    update      finish      erase_flash

void serial_send_request(QByteArray &buftomcu);
void serial_send_open_lock(QByteArray &buftomcu);
void serial_send_update(QByteArray &buftomcu, uint8_t *adress, uint8_t *data);
void serial_send_finish(QByteArray &buftomcu);
void serial_erase_flash(QByteArray &buftomcu);
*/
uint8_t serial_send_receive::send_receive(QSerialPort *serialport, QByteArray &data_ar, uint64_t system_tick, uint8_t CMD, uint8_t *adress, uint8_t *data)
{
    uint8_t tmp = 0;
    switch (state)
    {
    case 0:
    {
        QByteArray buftomcu;
        cmd = CMD;
        switch (cmd)
        {
        case 0xc1:
            serial_send_data->serial_send_request(buftomcu);
            break;
        case 0xc3:
            serial_send_data->serial_send_open_lock(buftomcu);
            break;
        case 0xc5:
            serial_send_data->serial_send_update(buftomcu, adress, data);
            break;
        case 0xc7:
            serial_send_data->serial_send_finish(buftomcu, adress, data);
            break;
        case 0xc9:
            serial_send_data->serial_erase_flash(buftomcu);
            break;
        case 0xb1:
            serial_send_data->serial_send_info(buftomcu);
            break;
        }
        serialport->write(buftomcu);
        state = 1;
        times ++;
        tmp = SEND_FINISH;
        ticks = system_tick;
        qDebug("Request_send_finish!");
        break;
    }
    case 1:
    {
       tmp = SEND_FINISH;
       if(serial_receive_data->check_package(data_ar))
       {
        state = 0;
        times = 0;
        tmp = RECEIEVE_FINISH;
        qDebug("Request_receieve_finish!");
       }
       if(system_tick - ticks >= ANSWER_TIME_OUT)
       {
         state = 0;
         if(times >= 3)
         {
           times = 0;
           tmp = RECEIEVE_TIMEOUT;
         }
       }
       break;
    }
    default:
        break;
    }
    return tmp;
}
