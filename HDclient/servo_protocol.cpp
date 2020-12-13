
#include "stdint.h"
#include "servo_protocol.h"
#include <windows.h>

servo_protocol::servo_protocol(QSerialPort *port):serialport_(port),servo_unique_address_id(16)
{

}
servo_protocol::~servo_protocol()
{

}
void servo_protocol::set_serial_port(QSerialPort *port)
{
    serialport_ = port;
}
void servo_protocol::uart_send_p_command(void)
{
    quint8 uart_write_datas[12];
    uart_write_datas[0]		= 0xfa;
    uart_write_datas[1] 	= 0x10;
    uart_write_datas[2] 	= 0x10;
    uart_write_datas[3] 	= 0x01;
    uart_write_datas[4] 	= 0x00;
    uart_write_datas[5] 	= 0x00;
    uart_write_datas[6]  	= 0x00;
    uart_write_datas[7] 	= 0x00;
    uart_write_datas[8] 	= 0x00;
    uart_write_datas[9] 	= 0x00;
    uart_write_datas[10] 	= 0x11;
    uart_write_datas[11] 	= 0xfe;
    QByteArray buftomcu;
    for(quint8 j=0; j<12; j++){
        buftomcu.append((uart_write_datas[j]));
    }
    serialport_->write(buftomcu);
}

void servo_protocol::uart_send_common_command(void)
{
    quint8 uart_write_datas[12];
    uart_write_datas[0]		= 0xfa;
    uart_write_datas[1] 	= 0x10;
    uart_write_datas[2] 	= 0x12;
    uart_write_datas[3] 	= 0x01;
    uart_write_datas[4] 	= 0x00;
    uart_write_datas[5] 	= 0x00;
    uart_write_datas[6]  	= 0x00;
    uart_write_datas[7] 	= 0x00;
    uart_write_datas[8] 	= 0x00;
    uart_write_datas[9] 	= 0x00;
    uart_write_datas[10] 	= 0x13;
    uart_write_datas[11] 	= 0xfe;
    QByteArray buftomcu;
    for(quint8 j=0; j<12; j++){
        buftomcu.append((uart_write_datas[j]));
    }
    serialport_->write(buftomcu);
}


void servo_protocol::uart_send_clear_command(void)
{
    quint8 uart_write_datas[12];
    uart_write_datas[0]		= 0xfa;
    uart_write_datas[1] 	= 0x55;
    uart_write_datas[2] 	= 0x55;
    uart_write_datas[3] 	= 0x55;
    uart_write_datas[4] 	= 0x55;
    uart_write_datas[5] 	= 0x55;
    uart_write_datas[6]  	= 0x55;
    uart_write_datas[7] 	= 0x55;
    uart_write_datas[8] 	= 0x55;
    uart_write_datas[9] 	= 0x55;
    uart_write_datas[10] 	= 0x55;
    uart_write_datas[11] 	= 0xfe;
    QByteArray buftomcu;
    for(quint8 j=0; j<12; j++){
        buftomcu.append((uart_write_datas[j]));
    }
    serialport_->write(buftomcu);
}

void servo_protocol::uart_send_command(quint8 id, quint8 cmd, quint8 state, quint8 para_id1, quint16 para1, quint8 para_id2, quint16 para2)
{
    quint8 temp;
    quint8 uart_write_datas[12];
    uart_write_datas[0]		= 0xfa;
    uart_write_datas[1] 	= id;
    uart_write_datas[2] 	= cmd;
    uart_write_datas[3] 	= state;
    uart_write_datas[4] 	= para_id1;
    uart_write_datas[5] 	= (quint8)(para1 >> 8);
    uart_write_datas[6]  	= (quint8)(para1);
    uart_write_datas[7] 	= para_id2;
    uart_write_datas[8] 	= (quint8)(para2 >> 8);
    uart_write_datas[9] 	= (quint8)(para2);
    temp = cmd + state + para_id1; //校验码不能超过250
    uart_write_datas[10] = temp;
    uart_write_datas[11] = 0xfe;
    QByteArray buftomcu;
    for(quint8 j=0; j<12; j++)
    {
     buftomcu.append((uart_write_datas[j]));
    }
    serialport_->write(buftomcu);
}

void servo_protocol::read_work_param(uint8_t seq)
{
    int16_t work_param[16];
    int16_t temp_param;
    work_param[0] = MENU_FB_SERVO_WORK_PARM0;
    work_param[1] = MENU_FB_SERVO_WORK_PARM1;
    work_param[2] = MENU_FB_SERVO_WORK_PARM2;
    work_param[3] = MENU_FB_SERVO_WORK_PARM3;
    work_param[4] = MENU_FB_SERVO_WORK_PARM4;
    work_param[5] = MENU_FB_SERVO_WORK_PARM5;
    work_param[6] = MENU_FB_SERVO_WORK_PARM6;
    work_param[7] = MENU_FB_SERVO_WORK_PARM7;
    work_param[8] = MENU_FB_SERVO_WORK_PARM8;
    work_param[9] = MENU_FB_SERVO_WORK_PARM9;
    work_param[10] = MENU_FB_SERVO_WORK_PARM10;
    work_param[11] = MENU_FB_SERVO_WORK_PARM11;
    work_param[12] = MENU_FB_SERVO_WORK_PARM12;
    work_param[13] = MENU_FB_SERVO_WORK_PARM13;
    work_param[14] = MENU_FB_SERVO_WORK_PARM14;
    work_param[15] = MENU_FB_SERVO_WORK_PARM15;
    temp_param = *(work_param + seq);
    uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
}

void servo_protocol::read_setup_param(uint8_t seq)
{
    int16_t setup_param[16];
    int16_t temp_param;
    setup_param[0] = MENU_FB_SERVO_SETUP_PARM0;
    setup_param[1] = MENU_FB_SERVO_SETUP_PARM1;
    setup_param[2] = MENU_FB_SERVO_SETUP_PARM2;
    setup_param[3] = MENU_FB_SERVO_SETUP_PARM3;
    setup_param[4] = MENU_FB_SERVO_SETUP_PARM4;
    setup_param[5] = MENU_FB_SERVO_SETUP_PARM5;
    setup_param[6] = MENU_FB_SERVO_SETUP_PARM6;
    setup_param[7] = MENU_FB_SERVO_SETUP_PARM7;
    setup_param[8] = MENU_FB_SERVO_SETUP_PARM8;
    setup_param[9] = MENU_FB_SERVO_SETUP_PARM9;
    setup_param[10] = MENU_FB_SERVO_SETUP_PARM10;
    setup_param[11] = MENU_FB_SERVO_SETUP_PARM11;
    setup_param[12] = MENU_FB_SERVO_SETUP_PARM12;
    setup_param[13] = MENU_FB_SERVO_SETUP_PARM13;
    setup_param[14] = MENU_FB_SERVO_SETUP_PARM14;
    setup_param[15] = MENU_FB_SERVO_SETUP_PARM15;
    temp_param = *(setup_param + seq);
    uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
}

void servo_protocol::read_debug_param(uint8_t seq)
{
    int16_t debug_param[7];
    int16_t temp_param;
    debug_param[0] = MENU_FB_SERVO_DEBUG_PARM0;
    debug_param[1] = MENU_FB_SERVO_DEBUG_PARM1;
    debug_param[2] = MENU_FB_SERVO_DEBUG_PARM2;
    debug_param[3] = MENU_FB_SERVO_DEBUG_PARM3;
    debug_param[4] = MENU_FB_SERVO_DEBUG_PARM4;
    debug_param[5] = MENU_FB_SERVO_DEBUG_PARM5;
    debug_param[6] = MENU_FB_SERVO_DEBUG_PARM6;
    temp_param = *(debug_param + seq);
    uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
}

void servo_protocol::read_config_param(uint8_t seq)
{
    int16_t config_param[4];
    int16_t temp_param;
    config_param[0] = MENU_FB_SERVO_CONFIG_PARM0;
    config_param[1] = MENU_FB_SERVO_CONFIG_PARM1;
    config_param[2] = MENU_FB_SERVO_CONFIG_PARM2;
    config_param[3] = MENU_FB_SERVO_CONFIG_PARM3;
    temp_param = *(config_param + seq);
    uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
}

void servo_protocol::send_work_param(uint8_t seq, struct Servo_Data_Stru_ *servo_data)
{
    int16_t work_param[16];
    int16_t temp_param;
    int16_t temp_param1;
    work_param[0] = MENU_WORK_SERVO_PARM0;
    work_param[1] = MENU_WORK_SERVO_PARM1;
    work_param[2] = MENU_WORK_SERVO_PARM2;
    work_param[3] = MENU_WORK_SERVO_PARM3;
    work_param[4] = MENU_WORK_SERVO_PARM4;
    work_param[5] = MENU_WORK_SERVO_PARM5;
    work_param[6] = MENU_WORK_SERVO_PARM6;
    work_param[7] = MENU_WORK_SERVO_PARM7;
    work_param[8] = MENU_WORK_SERVO_PARM8;
    work_param[9] = MENU_WORK_SERVO_PARM9;
    work_param[10] = MENU_WORK_SERVO_PARM10;
    work_param[11] = MENU_WORK_SERVO_PARM11;
    work_param[12] = MENU_WORK_SERVO_PARM12;
    work_param[13] = MENU_WORK_SERVO_PARM13;
    work_param[14] = MENU_WORK_SERVO_PARM14;
    work_param[15] = MENU_WORK_SERVO_PARM15;
    temp_param = *(work_param + seq);
    temp_param1 = *(&servo_data->work_p0 + seq);
    uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
}

void servo_protocol::send_setup_param(uint8_t seq, struct Servo_Data_Stru_ *servo_data)
{
    int16_t setup_param[16];
    int16_t temp_param;
    int16_t temp_param1;
    setup_param[0] = MENU_SETUP_SERVO_PARM0;
    setup_param[1] = MENU_SETUP_SERVO_PARM1;
    setup_param[2] = MENU_SETUP_SERVO_PARM2;
    setup_param[3] = MENU_SETUP_SERVO_PARM3;
    setup_param[4] = MENU_SETUP_SERVO_PARM4;
    setup_param[5] = MENU_SETUP_SERVO_PARM5;
    setup_param[6] = MENU_SETUP_SERVO_PARM6;
    setup_param[7] = MENU_SETUP_SERVO_PARM7;
    setup_param[8] = MENU_SETUP_SERVO_PARM8;
    setup_param[9] = MENU_SETUP_SERVO_PARM9;
    setup_param[10] = MENU_SETUP_SERVO_PARM10;
    setup_param[11] = MENU_SETUP_SERVO_PARM11;
    setup_param[12] = MENU_SETUP_SERVO_PARM12;
    setup_param[13] = MENU_SETUP_SERVO_PARM13;
    setup_param[14] = MENU_SETUP_SERVO_PARM14;
    setup_param[15] = MENU_SETUP_SERVO_PARM15;
    temp_param = *(setup_param + seq);
    temp_param1 = *(&servo_data->set_p0 + seq);
    uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
}

void servo_protocol::send_debug_param(uint8_t seq, struct Servo_Data_Stru_ *servo_data)
{
    int16_t debug_param[11];
    int16_t temp_param;
    int16_t temp_param1;
    debug_param[0] = MENU_DEBUG_SERVO_PARM0;
    debug_param[1] = MENU_DEBUG_SERVO_PARM1;
    debug_param[2] = MENU_DEBUG_SERVO_PARM2;
    debug_param[3] = MENU_DEBUG_SERVO_PARM3;
    debug_param[4] = MENU_DEBUG_SERVO_PARM4;
    debug_param[5] = MENU_DEBUG_SERVO_PARM5;
    debug_param[6] = MENU_DEBUG_SERVO_PARM6;

    debug_param[7] = MENU_CONFIG_SERVO_PARM0;
    debug_param[8] = MENU_CONFIG_SERVO_PARM1;
    debug_param[9] = MENU_CONFIG_SERVO_PARM2;
    debug_param[10] = MENU_CONFIG_SERVO_PARM3;

    temp_param = *(debug_param + seq);
    temp_param1 = *(&servo_data->debug_p0 + seq);

        if(seq <7){
            uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_DEBUG, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
        }
        else{
            uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_CONFIG, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
        }
}

void servo_protocol::menu_combine_prom_work_parm(void)
{
    uint8_t i = 0;
    struct Servo_Data_Stru_ servo_data;
    memcpy(&servo_data, &servoDataStru_, sizeof(servo_data));

    uart_send_clear_command();
    Sleep(SERVO_DELAY_TIME);

    uart_send_p_command();//
    Sleep(SERVO_DELAY_TIME);

    uart_send_clear_command();//
    Sleep(SERVO_DELAY_TIME);

    uart_send_common_command();//
    Sleep(SERVO_DELAY_TIME);

    uart_send_clear_command();//
    Sleep(SERVO_DELAY_TIME);

    for(i = 0; i < 14; i++)
    {
        Sleep(SERVO_DELAY_TIME);
        send_work_param(i, &servo_data);
    }

    for(i = 0; i < 16; i++)
    {
        Sleep(SERVO_DELAY_TIME);
        send_setup_param(i, &servo_data);
    }

    Sleep(SERVO_DELAY_TIME);
    uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_DOWNLOAD, SERVO_STATE_COM, MENU_DOWMLOAD_WORK_PARM, 0, 0, 0);
    Sleep(SERVO_DELAY_TIME);

    for(i = 0; i < 11; i++)
    {
        Sleep(SERVO_DELAY_TIME);
        send_debug_param(i, &servo_data);
    }

    Sleep(SERVO_DELAY_TIME);
    uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_DOWNLOAD, SERVO_STATE_COM, MENU_DOWMLOAD_DEBUG_PARM, 0, 0, 0);
}
#include "QDateTime"
uint8_t servo_protocol::menu_combine_fb_work_parm(void)
{
    uint8_t i = 0;
    uint8_t time_count;
    uint8_t chech_sum;
    int16_t* buff;
    struct Servo_Data_Stru_ servo_data;
//    QDateTime current_date_time = QDateTime::currentDateTime();
//    QString current_time = current_date_time.toString("yyyyMMdd-hhmmss.zzz");
//    qDebug("---%s",current_time);
    uart_send_clear_command();
    Sleep(SERVO_DELAY_TIME);
    uart_send_common_command();
//    current_date_time = QDateTime::currentDateTime();
//    current_time = current_date_time.toString("yyyyMMdd-hhmmss.zzz");
//    qDebug("----%s",current_time);

    for(i = 0; i < 43; i++)
    {
        receive_uart_data_flag_ = 0;
        Sleep(SERVO_DELAY_TIME);

        if(i < 16)
        {
            read_work_param(i);
        }
        else if(i < 32)
        {
            read_setup_param(i - 16);
        }
        else if(i < 39)
        {
            read_debug_param(i - 32);
        }
        else if(i < 43)
        {
            read_config_param(i - 39);
        }

        //wait for feedback datas
        time_count = 0;

        while(receive_uart_data_flag_ == 0)
        {
            time_count++;
            if(time_count > 200) //200ms
            {
                return 0; //erro
            }
            Sleep(1); //0.001 s
        }

        //process datas
        receive_uart_data_flag_ = 0;
        chech_sum = uart_read_datas[2] + uart_read_datas[3] + uart_read_datas[4];

        if(chech_sum != uart_read_datas[10])
        {
            return 0;
        }

        if(uart_read_datas[3] != SERVO_STATE_FB)
        {
            return 0;
        }

        if(i < 16)
        {
            buff = &servo_data.work_p0 + i;
            *buff = (uart_read_datas[9] | (uart_read_datas[8] << 8));
        }
        else if(i < 32)
        {
            buff = &servo_data.set_p0 + i - 16;
            *buff = (uart_read_datas[9] | (uart_read_datas[8] << 8));
        }
        else if(i < 39)
        {
            buff = &servo_data.debug_p0 + i - 32;
            *buff = (uart_read_datas[9] | (uart_read_datas[8] << 8));
        }
        else if(i < 43)
        {
            buff = &servo_data.debug_p7 + i - 39;
            *buff = (uart_read_datas[9] | (uart_read_datas[8] << 8));
        }
    }

    memcpy(&servoDataStru_, &servo_data, sizeof(servo_data));
    Sleep(SERVO_DELAY_TIME_S);
    current_servo_version_ = servoDataStru_.work_p12;
    return 1;
}

uint8_t servo_protocol::menu_combine_verify_work_parm(void)
{
    uint8_t i = 0;
    uint16_t j = 0;
    int16_t* buff;
    uint8_t* buff1, *buff2;
    uint8_t time_count;
    uint8_t chech_sum;
    struct Servo_Data_Stru_ servo_data;

    uart_send_clear_command();
    Sleep(SERVO_DELAY_TIME);
    uart_send_common_command();

    for(i = 0; i < 43; i++)
    {
        receive_uart_data_flag_ = 0;
        Sleep(SERVO_DELAY_TIME);

        if(i < 16)
        {
            read_work_param(i);
        }
        else if(i < 32)
        {
            read_setup_param(i - 16);
        }
        else if(i < 39)
        {
            read_debug_param(i - 32);
        }
        else if(i < 43)
        {
            read_config_param(i - 39);
        }

        //wait for feedback datas
        time_count = 0;

        while(receive_uart_data_flag_ == 0)
        {
            time_count++;

            if(time_count > 200) //200ms
            {
                return 0; //erro
            }

            Sleep(1); //0.001 s
        }

        //process datas
        receive_uart_data_flag_ = 0;
        chech_sum = uart_read_datas[2] + uart_read_datas[3] + uart_read_datas[4];

        if(chech_sum != uart_read_datas[10])
        {
            return 0;
        }

        if(uart_read_datas[3] != SERVO_STATE_FB)
        {
            return 0;
        }

        if(i < 16)
        {
            buff = &servo_data.work_p0 + i;
            *buff = (uart_read_datas[9] | (uart_read_datas[8] << 8));
        }
        else if(i < 32)
        {
            buff = &servo_data.set_p0 + i - 16;
            *buff = (uart_read_datas[9] | (uart_read_datas[8] << 8));
        }
        else if(i < 39)
        {
            buff = &servo_data.debug_p0 + i - 32;
            *buff = (uart_read_datas[9] | (uart_read_datas[8] << 8));
        }
        else if(i < 43)
        {
            buff = &servo_data.debug_p7 + i - 39;
            *buff = (uart_read_datas[9] | (uart_read_datas[8] << 8));
        }
    }
    servoDataStru_.work_p14 = servo_data.work_p14;
    servoDataStru_.work_p15 = servo_data.work_p15;
    servoDataStru_.set_p6 = servo_data.set_p6;
    buff1 = (uint8_t*)(&servo_data);
    buff2 = (uint8_t*)(&servoDataStru_);

    for(j = 0; j < sizeof(servo_data); j++)
    {
        if(buff1[j] != buff2[j])
        {
            return 0;
        }
    }

    Sleep(SERVO_DELAY_TIME);
    uart_send_p_command();
    Sleep(SERVO_DELAY_TIME_S);
    return 1;
}
