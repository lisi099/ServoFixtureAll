#include "mainwindow.h"
#include "ui_mainwindow.h"

#define ANSWER_TIME_OUT 1000

#define RECEIEVE_TIMEOUT    0
#define SEND_FINISH         1
#define RECEIEVE_FINISH     2


#define UPDATE_REQUEST      1
#define UPDATE_TRAGER       2
#define UPDATE_OPENLOCK     3
#define UPDATE_ERASE        4
#define UPDATE_CMD          5
#define UPDATE_FINISH       6
#define UPDATE_COMPLETE     7

#define UPDATE_REQUEST_FINISH      10


#define CMD_REQUEST     0xc1
#define CMD_OPENLOCK    0xc3
#define CMD_UPDATE      0xc5
#define CMD_FINISH      0xc7
#define CMD_ERASE       0xc9
#define CMD_INFO        0xb1


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromLocal8Bit("舵机夹具固件升级工具"));
    ui->pushButton->setEnabled(0);
    ui->pushButton_2->setEnabled(1);

    system_tick = 0;
    system_state = 0;
    system_package = 0;
    crc32_length = 0;
    adress = 0;

    update_send_receieve_finish =0;
    memset(system_data, 0, sizeof(system_data));
    memset(system_adress, 0, sizeof(system_adress));
    memset(crc32_data, 0, sizeof(crc32_data));

//    ui->pushButton->setStyleSheet("QPushButton{background-color:rgb(160, 160, 160); color:rgb(255, 255, 255);}"
//                                  "QPushButton:hover{background-color:rgb(190, 190, 190);}");
//    ui->pushButton_2->setStyleSheet("QPushButton{background-color:rgb(160, 160, 160); color:rgb(255, 255, 255);}"
//                                  "QPushButton:hover{background-color:rgb(190, 190, 190);}");
//    ui->pushButton_3->setStyleSheet("QPushButton{background-color:rgb(160, 160, 160); color:rgb(255, 255, 255);}"
//                                  "QPushButton:hover{background-color:rgb(190, 190, 190);}");
//    ui->lineEdit->setStyleSheet("QPushButton{background-color:rgb(160, 160, 160); color:rgb(255, 255, 255);}");

//    ui->progressBar->setStyleSheet("QProgressBar::chunk { background-color: rgb(160, 160, 160) }");



    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if (serial.open(QIODevice::ReadWrite))
        {
            ui->comboBox->addItem(info.portName());
            ui->textEdit->append(QString::fromLocal8Bit("检测到端口号:")+info.portName());
            serial.close();
        }
    }
    my_serialport= new QSerialPort();

    timer  = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT( System_Ticks() ));//处理数据回调函数
    timer->start(1);
}


MainWindow::~MainWindow()
{
    delete ui;
}

/*--------------------------------------------------
 * function:
 * description:
 * input:
 * output:
---------------------------------------------------*/
void MainWindow::Receieve_Bytes(void)
{
  QByteArray temp = my_serialport->readAll();
  requestData.append(temp);
}
/*--------------------------------------------------
 * function:
 * description:
 * input:
 * output:
---------------------------------------------------*/
void MainWindow::System_Ticks()
{
    static uint32_t ticks_info=0;
    system_tick ++;
    Update_process();
    if(system_tick - ticks_info > 500)
    {
        if(system_state == UPDATE_REQUEST_FINISH)
        {
            switch( data_send_receive->send_receive(my_serialport, requestData, system_tick, CMD_INFO,system_adress, system_data) )
            {
                case RECEIEVE_TIMEOUT:
                {
                    qDebug("time_out");
                    break;
                }
                case RECEIEVE_FINISH:
                {
                    ticks_info = system_tick;
                    uint8_t *version = data_send_receive->serial_receive_data->get_package_data();
                    ui->lineEdit_2->setText("[HV:"+(QString::number(version[0]/10.0))+"] "+"[SV:"+(QString::number(version[1]/10.0))+"]");
                    break;
                }
            }
        }
    }
}
/*--------------------------------------------------
 * function:
 * description:
 * input:
 * output:
---------------------------------------------------*/
uint8_t MainWindow::data_prepare(void)
{
    uint32_t total = file_bin.size();

    system_data[0] = (uint8_t)file_bin[adress+0];
    system_data[1] = (uint8_t)file_bin[adress+1];
    system_data[2] = (uint8_t)file_bin[adress+2];
    system_data[3] = (uint8_t)file_bin[adress+3];

    crc32_data[crc32_length] = (uint32_t)(system_data[0] | (system_data[1]<<8) | (system_data[2]<<16) | (system_data[3]<<24));
    crc32_length ++;

    system_adress[0] = (uint8_t)(adress);
    system_adress[1] = (uint8_t)(adress>>8);
    system_adress[2] = (uint8_t)(adress>>16);

     qDebug()<<"[program]--"<<adress;
    //qDebug("[data]%d, %d, %d, %d",system_data[0], system_data[1], system_data[2], system_data[3]);
    //qDebug("[adress]%d, %d, %d",system_adress[0], system_adress[1], system_adress[2]);
    adress = adress + 4;
    qDebug()<<adress;
    ui->progressBar->setValue(adress*100/total);
    if(adress >= file_bin.size())
    {
        return 1;
    }
    else
    {
        return 0;
    }

}
/*--------------------------------------------------
 * function:
 * description:
 * input:
 * output:
#define CMD_REQUEST     0xc1
#define CMD_OPENLOCK    0xc3
#define CMD_UPDATE      0xc5
#define CMD_FINISH      0xc7
#define CMD_ERASE       0xc9
---------------------------------------------------*/
void MainWindow::Update_process(void)
{
    static uint8_t file_end_flag = 0;
    switch (system_state)
    {
        case UPDATE_REQUEST:
        {
            switch( data_send_receive->send_receive(my_serialport, requestData, system_tick, CMD_REQUEST,system_adress, system_data) )
            {
                case RECEIEVE_TIMEOUT:
                {
                    system_state = 0;
                    qDebug("Request_timeout!");
                    my_serialport->clear();
                    if (my_serialport->isOpen())
                    {
                        my_serialport->close();
                        ui->textEdit->append(QString::fromLocal8Bit("连接失败！串口")+my_serialport->portName()+QString::fromLocal8Bit("已关闭"));
                    }
                    ui->pushButton->setEnabled(0);
                    break;
                }
                case RECEIEVE_FINISH:
                {
                    qDebug("[]--request!");
                    ui->textEdit->append(QString::fromLocal8Bit("连接成功！串口")+my_serialport->portName()+QString::fromLocal8Bit("已打开"));
                    system_state = UPDATE_REQUEST_FINISH;
                    ui->pushButton->setEnabled(1);
                    ui->pushButton_2->setText(QString::fromLocal8Bit("断开"));
                    break;
                }
                default:
                {
                  if(system_tick %1000 ==0)
                  {
                    ui->textEdit->append(QString::fromLocal8Bit("连接中......"));
                  }
                }
            }
        break;
        }
        case UPDATE_TRAGER:
        {
            system_state = UPDATE_OPENLOCK;
            break;
        }
        case UPDATE_OPENLOCK:
        {
            switch( data_send_receive->send_receive(my_serialport, requestData, system_tick, CMD_OPENLOCK, system_adress, system_data) )
            {
                case RECEIEVE_TIMEOUT:
                {
                    system_state = 0;
                    qDebug("Request_timeout!");
                    ui->pushButton->setEnabled(0);
                    break;
                }
                case RECEIEVE_FINISH:
                {
                    qDebug("[]--openlock!");
                    system_state = UPDATE_ERASE;
                    break;
                }
            }
            break;
        }
        case UPDATE_ERASE:
        {
            switch( data_send_receive->send_receive(my_serialport, requestData, system_tick, CMD_ERASE, system_adress, system_data) )
            {
                case RECEIEVE_TIMEOUT:
                {
                    system_state = 0;
                    qDebug("Request_timeout!");
                    ui->pushButton->setEnabled(0);
                    break;
                }
                case RECEIEVE_FINISH:
                {
                    qDebug("[]--erase!");
                    system_state = UPDATE_CMD;
                    break;
                }
            }
            break;
        }
        case UPDATE_CMD:
        {
            if(update_send_receieve_finish == 1)
            {
                update_send_receieve_finish =0;
                if(data_prepare()==1)
                {
                    file_end_flag = 1;
                }
            }
            switch( data_send_receive->send_receive(my_serialport, requestData, system_tick, CMD_UPDATE,system_adress, system_data) )
            {
                case RECEIEVE_TIMEOUT:
                {
                    system_state = 0;
                    qDebug("Request_timeout!");
                    break;
                }
                case RECEIEVE_FINISH:
                {
                    qDebug("[]--update!");
                    if(file_end_flag ==1)
                    {
                       system_state = UPDATE_FINISH;
                       file_end_flag = 0;
                    }
                    else
                    {
                        system_state = UPDATE_CMD;
                    }
                    update_send_receieve_finish =1;
                    break;
                }
            }
            break;
        }
        case UPDATE_FINISH:
        {
            uint32_t crc32 = CRC32Software(crc32_data, crc32_length);
            system_adress[0] = (uint8_t)crc32_length;
            system_adress[1] = (uint8_t)(crc32_length>>8);
            system_adress[2] = (uint8_t)(crc32_length>>16);
            system_data[0] = (uint8_t)(crc32);
            system_data[1] = (uint8_t)(crc32>>8);
            system_data[2] = (uint8_t)(crc32>>16);
            system_data[3] = (uint8_t)(crc32>>24);
            switch( data_send_receive->send_receive(my_serialport, requestData, system_tick, CMD_FINISH, system_adress, system_data) )
            {
                case RECEIEVE_TIMEOUT:
                {
                    system_state = 0;
                    qDebug("Request_timeout!");
                    ui->pushButton->setEnabled(0);
                    break;
                }
                case RECEIEVE_FINISH:
                {
                    qDebug("[]--finish!");
                    system_state = UPDATE_COMPLETE;
                    break;
                }
            }
            break;
        }
        case UPDATE_COMPLETE:
        {
            system_state = 0;
            delete data_send_receive;
            my_serialport->clear();
            if (my_serialport->isOpen())
            {
                my_serialport->close();
                ui->textEdit->append(QString::fromLocal8Bit("升级完成！串口")+my_serialport->portName()+QString::fromLocal8Bit("已关闭"));
            }
            ui->pushButton_2->setText(QString::fromLocal8Bit("连接"));
            ui->pushButton->setEnabled(0);
            break;
        }
    }
}
/*--------------------------------------------------
 * function:
 * description:
 * input:
 * output:
---------------------------------------------------*/
void MainWindow::on_pushButton_clicked(void)
{
    if(system_state == UPDATE_REQUEST_FINISH)
    {
        system_state = UPDATE_TRAGER;
        ui->pushButton->setDisabled(1);
    }
}
/*--------------------------------------------------
 * function:
 * description:
 * input:
 * output:
---------------------------------------------------*/
void MainWindow::on_pushButton_3_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,QString::fromLocal8Bit("打开"),QDir::currentPath(),QString::fromLocal8Bit("二进制文件(*.bin)"));
    if (!filename.isEmpty())
    {
        qDebug("file is not empty");
        QFile file(filename);
        if(!file.open(QIODevice::ReadOnly))
        {
          qDebug("can not open file");
        }
        else
        {
            ui->textEdit->clear();
            qDebug("open file");
            QDataStream bin(&file);
            file_bin.clear();
            while(!bin.atEnd())
            {
             char num;
             bin.readRawData(&num,1);
             file_bin.append(num);
            }
            uint8_t tem_num =  (file_bin.length() % 4);
            switch (tem_num)
            {
            case 3:
                file_bin.append(0xff);
                break;
            case 2:
                file_bin.append(0xff);
                file_bin.append(0xff);
                break;
            case 1:
                file_bin.append(0xff);
                file_bin.append(0xff);
                file_bin.append(0xff);
                break;
            default:
                break;
            }
            ui->lineEdit->setText(filename);
            ui->pushButton_2->setEnabled(1);
            //qDebug()<<file_bin.toHex();
            //qDebug()<<file_bin.size();
        }
    }
}
/*--------------------------------------------------
 * function:
 * description:
 * input:
 * output:
---------------------------------------------------*/
void MainWindow::on_pushButton_2_clicked()
{
    if(ui->pushButton_2->text() == QString::fromLocal8Bit("连接"))
    {
        my_serialport->setPortName(ui->comboBox->currentText());
        my_serialport->setBaudRate(115200);
        my_serialport->setDataBits(QSerialPort::Data8);
        my_serialport->setParity(QSerialPort::NoParity);
        my_serialport->setStopBits(QSerialPort::OneStop);
        my_serialport->setFlowControl(QSerialPort::NoFlowControl);
        if(my_serialport->open(QIODevice::ReadWrite))
        {
            data_send_receive = new serial_send_receive;
            connect(my_serialport,SIGNAL(readyRead()),this,SLOT(Receieve_Bytes()));
            system_state = UPDATE_REQUEST;
            crc32_length = 0;
            system_package = 0;
            adress = 0;
            update_send_receieve_finish = 1;
            requestData.clear();
        }
        else
        {
            QMessageBox::critical(this, QString::fromLocal8Bit("Error"), my_serialport->errorString());
        }
    }
    else
    {
        my_serialport->clear();
        if (my_serialport->isOpen())
        {
            my_serialport->close();
            ui->textEdit->append("串口"+my_serialport->portName()+"已关闭");
        }
        ui->pushButton_2->setText(QString::fromLocal8Bit("连接"));
        ui->pushButton->setEnabled(0);
        system_state = 0;
        requestData.clear();
    }
}
/*--------------------------------------------------
 * function:
 * description:
 * input:
 * output:
---------------------------------------------------*/
const uint32_t Crc32Table[ 256 ] =
{
  0x00000000, 0x04C11DB7, 0x09823B6E, 0x0D4326D9, 0x130476DC, 0x17C56B6B,
  0x1A864DB2, 0x1E475005, 0x2608EDB8, 0x22C9F00F, 0x2F8AD6D6, 0x2B4BCB61,
  0x350C9B64, 0x31CD86D3, 0x3C8EA00A, 0x384FBDBD, 0x4C11DB70, 0x48D0C6C7,
  0x4593E01E, 0x4152FDA9, 0x5F15ADAC, 0x5BD4B01B, 0x569796C2, 0x52568B75,
  0x6A1936C8, 0x6ED82B7F, 0x639B0DA6, 0x675A1011, 0x791D4014, 0x7DDC5DA3,
  0x709F7B7A, 0x745E66CD, 0x9823B6E0, 0x9CE2AB57, 0x91A18D8E, 0x95609039,
  0x8B27C03C, 0x8FE6DD8B, 0x82A5FB52, 0x8664E6E5, 0xBE2B5B58, 0xBAEA46EF,
  0xB7A96036, 0xB3687D81, 0xAD2F2D84, 0xA9EE3033, 0xA4AD16EA, 0xA06C0B5D,
  0xD4326D90, 0xD0F37027, 0xDDB056FE, 0xD9714B49, 0xC7361B4C, 0xC3F706FB,
  0xCEB42022, 0xCA753D95, 0xF23A8028, 0xF6FB9D9F, 0xFBB8BB46, 0xFF79A6F1,
  0xE13EF6F4, 0xE5FFEB43, 0xE8BCCD9A, 0xEC7DD02D, 0x34867077, 0x30476DC0,
  0x3D044B19, 0x39C556AE, 0x278206AB, 0x23431B1C, 0x2E003DC5, 0x2AC12072,
  0x128E9DCF, 0x164F8078, 0x1B0CA6A1, 0x1FCDBB16, 0x018AEB13, 0x054BF6A4,
  0x0808D07D, 0x0CC9CDCA, 0x7897AB07, 0x7C56B6B0, 0x71159069, 0x75D48DDE,
  0x6B93DDDB, 0x6F52C06C, 0x6211E6B5, 0x66D0FB02, 0x5E9F46BF, 0x5A5E5B08,
  0x571D7DD1, 0x53DC6066, 0x4D9B3063, 0x495A2DD4, 0x44190B0D, 0x40D816BA,
  0xACA5C697, 0xA864DB20, 0xA527FDF9, 0xA1E6E04E, 0xBFA1B04B, 0xBB60ADFC,
  0xB6238B25, 0xB2E29692, 0x8AAD2B2F, 0x8E6C3698, 0x832F1041, 0x87EE0DF6,
  0x99A95DF3, 0x9D684044, 0x902B669D, 0x94EA7B2A, 0xE0B41DE7, 0xE4750050,
  0xE9362689, 0xEDF73B3E, 0xF3B06B3B, 0xF771768C, 0xFA325055, 0xFEF34DE2,
  0xC6BCF05F, 0xC27DEDE8, 0xCF3ECB31, 0xCBFFD686, 0xD5B88683, 0xD1799B34,
  0xDC3ABDED, 0xD8FBA05A, 0x690CE0EE, 0x6DCDFD59, 0x608EDB80, 0x644FC637,
  0x7A089632, 0x7EC98B85, 0x738AAD5C, 0x774BB0EB, 0x4F040D56, 0x4BC510E1,
  0x46863638, 0x42472B8F, 0x5C007B8A, 0x58C1663D, 0x558240E4, 0x51435D53,
  0x251D3B9E, 0x21DC2629, 0x2C9F00F0, 0x285E1D47, 0x36194D42, 0x32D850F5,
  0x3F9B762C, 0x3B5A6B9B, 0x0315D626, 0x07D4CB91, 0x0A97ED48, 0x0E56F0FF,
  0x1011A0FA, 0x14D0BD4D, 0x19939B94, 0x1D528623, 0xF12F560E, 0xF5EE4BB9,
  0xF8AD6D60, 0xFC6C70D7, 0xE22B20D2, 0xE6EA3D65, 0xEBA91BBC, 0xEF68060B,
  0xD727BBB6, 0xD3E6A601, 0xDEA580D8, 0xDA649D6F, 0xC423CD6A, 0xC0E2D0DD,
  0xCDA1F604, 0xC960EBB3, 0xBD3E8D7E, 0xB9FF90C9, 0xB4BCB610, 0xB07DABA7,
  0xAE3AFBA2, 0xAAFBE615, 0xA7B8C0CC, 0xA379DD7B, 0x9B3660C6, 0x9FF77D71,
  0x92B45BA8, 0x9675461F, 0x8832161A, 0x8CF30BAD, 0x81B02D74, 0x857130C3,
  0x5D8A9099, 0x594B8D2E, 0x5408ABF7, 0x50C9B640, 0x4E8EE645, 0x4A4FFBF2,
  0x470CDD2B, 0x43CDC09C, 0x7B827D21, 0x7F436096, 0x7200464F, 0x76C15BF8,
  0x68860BFD, 0x6C47164A, 0x61043093, 0x65C52D24, 0x119B4BE9, 0x155A565E,
  0x18197087, 0x1CD86D30, 0x029F3D35, 0x065E2082, 0x0B1D065B, 0x0FDC1BEC,
  0x3793A651, 0x3352BBE6, 0x3E119D3F, 0x3AD08088, 0x2497D08D, 0x2056CD3A,
  0x2D15EBE3, 0x29D4F654, 0xC5A92679, 0xC1683BCE, 0xCC2B1D17, 0xC8EA00A0,
  0xD6AD50A5, 0xD26C4D12, 0xDF2F6BCB, 0xDBEE767C, 0xE3A1CBC1, 0xE760D676,
  0xEA23F0AF, 0xEEE2ED18, 0xF0A5BD1D, 0xF464A0AA, 0xF9278673, 0xFDE69BC4,
  0x89B8FD09, 0x8D79E0BE, 0x803AC667, 0x84FBDBD0, 0x9ABC8BD5, 0x9E7D9662,
  0x933EB0BB, 0x97FFAD0C, 0xAFB010B1, 0xAB710D06, 0xA6322BDF, 0xA2F33668,
  0xBCB4666D, 0xB8757BDA, 0xB5365D03, 0xB1F740B4 };
/*************************************************************
  Function   :
  Description:
  Input      :
  return     :
*************************************************************/
uint32_t MainWindow::CRC32Software( uint32_t *pData, uint16_t Length )
{
  uint32_t nReg;
  uint32_t nTemp = 0;
  uint16_t i, n;
  nReg = 0xFFFFFFFF;
  for ( n = 0; n < Length; n++ )
  {
    nReg ^= (uint32_t) pData[ n ];
    for ( i = 0; i < 4; i++ )
    {
      nTemp = Crc32Table[ ( uint8_t )( ( nReg >> 24 ) & 0xff ) ];
      nReg <<= 8;
      nReg ^= nTemp;
    }
  }
  return nReg;
}
