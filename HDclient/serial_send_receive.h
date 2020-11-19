#ifndef SERIAL_SEND_RECEIVE_H
#define SERIAL_SEND_RECEIVE_H

#include <QMainWindow>
#include <QDebug>
#include "serial_receive.h"
#include "serial_send.h"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <inttypes.h>
class serial_send_receive
{
public:
    serial_send_receive();
    ~serial_send_receive();
    serial_send    *serial_send_data;
    serial_receive *serial_receive_data;
    uint8_t send_receive(QSerialPort *serialport, QByteArray &data_ar, uint64_t system_tick, uint8_t CMD, uint8_t *adress, uint8_t *data);
private:
    uint8_t    state = 0;
    uint8_t    times = 0;
    uint64_t   ticks;
    uint8_t    cmd;
};

#endif // SERIAL_SEND_RECEIVE_H
