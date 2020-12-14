#include "dataprocess.h"

DataProcess::DataProcess()
{
    usart_state =0; data_pos =0; pack_len =2; pack_cmd =0;
}

DataProcess::~DataProcess()
{

}

void DataProcess::set_pack_len(quint16 len)
{
    pack_len = len;
}

bool DataProcess::data_process(QByteArray &data)
{
    bool res = false;
    quint16 checksum, checksum1;
    quint8 tmp;
    for(quint16 i=0; i<data.length(); i++){
        tmp = (quint8)data[i];
        switch(usart_state){
        case 0:
            if(tmp == 0x55){
                usart_state =1;
            }
            break;
        case 1:
            if(tmp == 0xaa){
                usart_state =2;
                data_pos =0;
            }
            break;
        case 2:
            frame[data_pos++] =tmp;
            if(data_pos >= pack_len +2){
                usart_state = 0;
                checksum = Checksum_u16(frame, pack_len);
                checksum1 = quint16(frame[pack_len]|(frame[pack_len+1]<<8));
                if(checksum == checksum1){
                    for(quint8 j=0; j<pack_len; ){
                        data_receive[j/2] = qint16(frame[j] | (frame[j+1]<<8));
                        j+=2;
                    }
                    res = true;
                }
                else{
                    qDebug() << "check fail:" << checksum << checksum1;
                }
            }
            break;
        default:
            qDebug() << "erro";
            break;
        }
    }
    return res;
}
