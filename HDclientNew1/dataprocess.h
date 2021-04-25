#ifndef DATAPROCESS_H
#define DATAPROCESS_H


#include <QByteArray>
#include <qdebug.h>

class DataProcess
{
    #define frame_length    100
public:
    DataProcess();
    ~DataProcess();

    bool data_process(QByteArray &data);
    void set_pack_len(quint16 len);
    quint8 frame[frame_length];
    qint16 data_receive[4];
    quint16 Checksum_u16(quint8* pdata, quint16 len)
    {
        quint16 sum = 0;
        quint16 i;
        for(i=0; i<len; i++){
            sum += pdata[i];
        }
        sum = ~sum;
        return sum;
    }
signals:
    void signalData(DataProcess *ptr)
    {

    }


private:
    quint16 usart_state, data_pos, pack_len, pack_cmd;

};

#endif // DATAPROCESS_H
