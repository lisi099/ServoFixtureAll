#ifndef SERIAL_RECEIVE_H
#define SERIAL_RECEIVE_H

#include <QMainWindow>
#include <QByteArray>
#include <QDebug>
#include <inttypes.h>

class serial_receive
{
public:
    serial_receive();
    uint16_t UpdateCRC16(uint16_t crcIn, uint8_t byte);
    uint16_t Cal_CRC16(uint8_t *data, uint32_t size);
    uint8_t  check_data(QByteArray &data);
    uint8_t  check_package(QByteArray &data);
    uint8_t* get_package_data(void);
    uint8_t  get_package_cmd(void);

    uint64_t    package_count;

private:
    uint64_t    size_of_buff;
    uint64_t    size_of_check;
    uint8_t     package_cmd;
    uint8_t     package_data[7];
};

#endif // SERIAL_RECEIVE_H
