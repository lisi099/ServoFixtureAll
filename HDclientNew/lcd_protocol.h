#ifndef LED_PROTOCOL_H
#define LED_PROTOCOL_H

#include "stdint.h"
#include "servo_protocol.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "QVector"
#include "QMap"

#define CONNECT_CMD 0xFE
#define DISCONNECT_CMD (CONNECT_CMD -0x2)
#define READ_CMD  0x02
#define WRITE_CMD  (READ_CMD +0x2)
#define TEST_CMD  (WRITE_CMD +0x2)
/******************************************************
0x5A 0xA5 cmd seq datas checksum_l checksum_h
*******************************************************/

#define DATA_SIZE sizeof(Servo_Data_Stru_)
#define TOTAL_VERSION_SIZE 12

struct ui_data{
    int16_t version;
    int16_t max_power;
    int16_t boost;
    int16_t dead_band;
    int16_t tension;
    int16_t force;
    int16_t brake;
    int16_t soft_start;
    int16_t center_num;
};

class lcd_protocol{
public:
    lcd_protocol(QSerialPort *port):serial_(port)
    {
        version_map_.clear();
        version_map_.insert(40, "PGC-D12");
        version_map_.insert(1, "D1206G2");
        version_map_.insert(2, "WH-20KG");
        version_map_.insert(3, "WH-30KG");
        version_map_.insert(4, "WH-40KG");
        version_map_.insert(5, "PGC-DRF");
        version_map_.insert(6, "PGC-D15");
        version_map_.insert(7, "PGC-R12");
        version_map_.insert(8, "PGC-A20");
        version_map_.insert(9, "PGC-A30");
        version_map_.insert(10, "PGC-A40");
        version_map_.insert(11, "PGC-A50");

        version_map_index_.clear();
        version_map_index_.insert(40, 0);
        version_map_index_.insert(1, 1);
        version_map_index_.insert(2, 2);
        version_map_index_.insert(3, 3);
        version_map_index_.insert(4, 4);
        version_map_index_.insert(5, 5);
        version_map_index_.insert(6, 6);
        version_map_index_.insert(7, 7);
        version_map_index_.insert(8, 8);
        version_map_index_.insert(9, 9);
        version_map_index_.insert(10, 10);
        version_map_index_.insert(11, 11);
    }

    ~lcd_protocol(){

    }

public:
    quint16 sum_check(uint8_t *data, uint16_t size)
    {
        quint16 sum = 0;
        for(uint16_t i=0; i<size; i++){
            sum += data[i];
        }
        return sum;
    }

    void send_connect_data(void){
        uint8_t data[DATA_SIZE +6];
        memset(data, 0, sizeof(data));
        data[0] = 0x5A;
        data[1] = 0xA5;
        data[2] = CONNECT_CMD;
        data[3] = 0x00;
        uint16_t sum = sum_check(data, sizeof(data) -2);
        data[DATA_SIZE +4] = (uint8_t)sum;
        data[DATA_SIZE +5] = (uint8_t)(sum >> 8);
        char *data_send = (char *)data;
        serial_->write(data_send, sizeof(data));
    }

    void send_disconnect_data(void){
        uint8_t data[DATA_SIZE +6];
        memset(data, 0, sizeof(data));
        data[0] = 0x5A;
        data[1] = 0xA5;
        data[2] = DISCONNECT_CMD;
        data[3] = 0x00;
        uint16_t sum = sum_check(data, sizeof(data) -2);
        data[DATA_SIZE +4] = (uint8_t)sum;
        data[DATA_SIZE +5] = (uint8_t)(sum >> 8);
        char *data_send = (char *)data;
        serial_->write(data_send, sizeof(data));
    }

    void send_read_data(void){
        uint8_t data[DATA_SIZE +6];
        memset(data, 0, sizeof(data));
        data[0] = 0x5A;
        data[1] = 0xA5;
        data[2] = READ_CMD;
        data[3] = 0x00;
        uint16_t sum = sum_check(data, sizeof(data) -2);
        data[DATA_SIZE +4] = (uint8_t)sum;
        data[DATA_SIZE +5] = (uint8_t)(sum >> 8);
        char *data_send = (char *)data;
        serial_->write(data_send, sizeof(data));
    }

    void send_write_data(Servo_Data_Stru_ *servo_data){
        uint8_t data[DATA_SIZE +6];
        memset(data, 0, sizeof(data));
        data[0] = 0x5A;
        data[1] = 0xA5;
        data[2] = WRITE_CMD;
        data[3] = 0x00;
        memcpy(&data[4], servo_data, sizeof(Servo_Data_Stru_));
        uint16_t sum = sum_check(data, sizeof(data) -2);
        data[DATA_SIZE +4] = (uint8_t)sum;
        data[DATA_SIZE +5] = (uint8_t)(sum >> 8);
        char *data_send = (char *)data;
        serial_->write(data_send, sizeof(data));
    }

    bool data_process(const QByteArray &data)
    {
        bool res =false;
//        char *buff = new char(data.size());
//        qDebug() <<"---psize:" << data.size();
//        for(int i=0; i<data.size(); i++){
//            buff[i] = data.at(i);
//            qDebug() << buff[i];
//        }
        const char *buff = data.data();
        qDebug() << QString::number((uint8_t)buff[2]);
        switch((uint8_t)buff[2]){
            case CONNECT_CMD-1:
                memcpy(&servo_data_, &buff[4], sizeof(servo_data_));
                get_version(servo_data_);
                qDebug() << "CONNECT_CMD -1";
                res= true;
                break;
            case READ_CMD -1:
                memcpy(&servo_data_, &buff[4], sizeof(servo_data_));
                get_version(servo_data_);
                res= true;
                break;
            case WRITE_CMD -1:
                res= true;
                break;
             case DISCONNECT_CMD-1:
                res= true;
                break;
            default:
                break;
        }
//        delete buff;
        return res;
    }

    void get_version(Servo_Data_Stru_ &data)
    {
        uint8_t buf_2[2], buf[2];
        buf_2[0] = data.work_p12 / 1000 % 10;
        buf_2[1] = data.work_p12 / 100 % 10;
        uint16_t distribtor = buf_2[0] * 10 + buf_2[1];
        if(!version_map_.contains(distribtor)){
            version_ = "XX-XX-XX";
            return;
        }

        buf[0] = data.work_p12 / 10 % 10;
        buf[1] = data.work_p12 / 1 % 10;
        uint16_t costormer = buf[0] * 10 + buf[1];

        QString costormer_str;
        if(costormer >= 21){
             costormer_str ="00";
        }
        else if(costormer >= 10){
            costormer_str =QString::number(costormer);
        }
        else{
            costormer_str = "0" +QString::number(costormer);
        }

        uint8_t factory = data.work_p12 / 10000 % 10 + 1;
        version_ = QString("v1.%1 %2-%3").arg(factory).arg(version_map_.find(distribtor).value()).arg(costormer_str);
        qDebug() << version_;
    }

    int16_t round_f(float data)
    {
        float data_f;
        int16_t d = (int16_t)data;
        data_f = d + 0.5f;

        if(data > data_f) return d + 1;
        else return d;
    }

    void set_data(const ui_data &data)
    {
        int16_t offset = data.center_num;
        servo_data_.work_p3 = 3100 + (offset) * 28;
        servo_data_.work_p4 = 2048 + (offset) * 28;
        servo_data_.work_p5 = 996 + (offset) * 28;

        servo_data_.work_p12 = data.version; //version
        servo_data_.set_p11 = round_f(data.max_power * 72.2 + 727.7f); //servo_max_pwm_set
        servo_data_.set_p15 = round_f(data.boost * 5.6f + 4.3f); //servo_work_base_voltage
        servo_data_.work_p6 = data.dead_band; //servo_zero_zone_set
        servo_data_.debug_p5 = data.tension; //servo_position_pid_parm_p_set
        servo_data_.debug_p0 = data.force; //servo_speed_pid_parm_p_set
        servo_data_.debug_p2 = round_f(data.brake * 4.3f - 3.2f); //servo_speed_run_sample_k_set
        servo_data_.set_p14 = data.soft_start;//servo_init_flag_set
    }

    void get_data(ui_data &data)
    {
        int16_t center = (servo_data_.work_p3 - 3100) / 28;
        data.center_num =  center;
        data.version = servo_data_.work_p12;
        data.max_power = round_f((servo_data_.set_p11 - 727.7f) / 72.2f);
        data.boost = round_f((servo_data_.set_p15 - 4.3f) / 5.6f);
        data.dead_band = servo_data_.work_p6;
        data.tension = servo_data_.debug_p5;
        data.force = servo_data_.debug_p0;
        data.brake = round_f((servo_data_.debug_p2 + 3.2f) / 4.3f);
        data.soft_start = servo_data_.set_p14; //
        qDebug() << "soft_start" <<data.soft_start;
    }

    uint16_t find_version_index(void)
    {
        uint8_t buf_2[2], buf[2];
        buf_2[0] = servo_data_.work_p12 / 1000 % 10;
        buf_2[1] = servo_data_.work_p12 / 100 % 10;
        uint16_t distribtor = buf_2[0] * 10 + buf_2[1];
        if(!version_map_index_.contains(distribtor)){
            return 1000;
        }
        return version_map_index_.find(distribtor).value();
    }

    void send_test_data(uint16_t pwm)
    {
        uint8_t data[8];
        memset(data, 0, sizeof(data));
        data[0] = 0x5A;
        data[1] = 0xA5;
        data[2] = TEST_CMD;
        data[3] = 0x00;
        data[4] = (uint8_t)pwm;
        data[5] = (uint8_t)(pwm >> 8);
        uint16_t sum = sum_check(data, sizeof(data) -2);
        data[6] = (uint8_t)sum;
        data[7] = (uint8_t)(sum >> 8);
        char *data_send = (char *)data;
        serial_->write(data_send, sizeof(data));
    }


private:
    QSerialPort *serial_;

    QMap<uint16_t, QString> version_map_;
    QMap<uint16_t, uint16_t> version_map_index_;

public:
    Servo_Data_Stru_ servo_data_;
    QString version_;

};



#endif
