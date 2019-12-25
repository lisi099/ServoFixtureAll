#ifndef SERIAL_SEND_H
#define SERIAL_SEND_H


#include <QMainWindow>
#include <inttypes.h>
class serial_send
{
public:
    serial_send();
    uint16_t UpdateCRC16(uint16_t crcIn, uint8_t byte);
    uint16_t Cal_CRC16(uint8_t *data, uint32_t size);
    void Cal_CRC16_Buf(uint8_t *buf);
    void serial_send_request(QByteArray &buftomcu);
    void serial_send_open_lock(QByteArray &buftomcu);
    void serial_send_update(QByteArray &buftomcu, uint8_t *adress, uint8_t *data);
    void serial_send_finish(QByteArray &buftomcu, uint8_t *adress, uint8_t *data);
    void serial_erase_flash(QByteArray &buftomcu);
    void serial_send_info(QByteArray &buftomcu);

};

#endif
