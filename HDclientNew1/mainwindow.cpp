#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QStandardItemModel"
#include "QMessageBox"
#include "QFileDialog"
#include <windows.h>
#include "servo_protocol.h"
#include "QListView"

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

const struct Servo_Data_Stru_ factory_para[] =
{
//{2100,1500,900,3100,2048,996,2,0,0,1,1,1,4021,1,14,0,0,1500,10,1,1,10,1080,100,20,1020,900,1350,1500,1500,0,50,6,3,8,8,20,1,1,1,1,1,1}, //04021=V1.1 PGC-D12-00.bat"
/*{2100,1500,900,3100,2048,996,2,0,0,1,1,1,4021,1,19,0,0,1500,10,1,1,10,1080,100,20,100,900,1350,1500,1500,0,50,6,3,8,8,20,1,1,1,1,1,1}, //04021=V1.1 PGC-D12-00new.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,10100,1,14,0,1,1500,10,1,1,10,1080,50,20,5,800,1350,1500,1500,0,20,6,1,20,4,20,1,50,1,1,1,1}, //10100=V1.2 D1206G2-00.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,10200,1,14,0,1,1500,10,1,1,10,1080,50,20,5,800,1050,1500,1500,0,20,6,1,20,4,20,1,50,1,1,1,1}, //10200=V1.2 WH20KG-00.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,10300,1,14,0,1,1500,10,1,1,10,1080,50,20,5,800,1050,1500,1500,0,20,6,1,20,4,20,1,50,1,1,1,1}, //10300=V1.2 WH30KG-00.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,10400,1,14,0,1,1500,10,1,1,10,1080,50,20,5,800,1350,1500,1500,0,20,6,1,20,4,20,1,50,1,1,1,1}, //10400=V1.2 WH40KG-00.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,10500,1,14,0,1,1500,10,1,1,10,1080,50,20,5,800,1350,1500,1500,0,20,6,1,20,4,20,1,50,1,1,1,1}, //10500=V1.2 PGC-Drift-00.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,10600,1,14,0,0,1500,10,1,1,10,960,50,20,5,800,1200,1500,1500,0,20,8,1,35,4,20,1,50,1,1,1,1}, //10600=V1.2 PGC-D15-00.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,10700,1,14,0,0,1500,10,1,1,10,1080,50,20,5,800,1350,1500,1500,0,20,8,1,35,4,20,1,50,1,1,1,1}, //10700=V1.2 PGC-R12S-00.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,10800,1,14,0,1,1500,10,1,1,10,1080,50,20,5,800,1350,1500,1500,0,20,6,1,20,4,20,1,50,1,1,1,1}, //10800=V1.2 PGC-A20-00.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,10900,1,14,0,1,1500,10,1,1,10,1080,50,20,5,800,1350,1500,1500,0,20,6,1,20,4,20,1,50,1,1,1,1}, //10900=V1.2 PGC-A30-00.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,11000,1,14,0,1,1500,10,1,1,10,1080,50,20,5,800,1350,1500,1500,0,20,6,1,20,4,20,1,50,1,1,1,1}, //11000=V1.2 PGC-A40-00.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,11100,1,14,0,1,1500,10,1,1,10,1080,50,20,5,800,1350,1500,1500,0,20,6,1,20,4,20,1,50,1,1,1,1}, //11100=V1.2 PGC-A50-00.bat"
*/

{2100,1500,900,3100,2048,996,2,0,0,1,1,1,15021,1,15,0,0,1500,10,1,1,10,1080,100,20,1020,900,1350,1500,1500,0,50,8,3,9,8,20,1,1,1,1,1,1}, //15021=v1.2 CLS-CH712S.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,16100,1,15,0,1,1500,10,1,1,10,1080,50,20,5,800,1350,1500,1500,0,20,6,1,20,4,20,1,50,1,1,1,1}, //16100=v1.2 CLS-D1-00.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,16300,1,15,0,1,1500,10,1,1,10,800,50,20,5,800,1000,1500,1500,0,50,6,1,20,4,20,1,50,1,1,1,1}, //16300=v1.2 CLS-W35-00.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,16400,1,15,0,1,1500,10,1,1,10,1080,50,20,5,800,1350,1500,1500,0,50,6,1,20,4,20,1,50,1,1,1,1}, //16400=v1.2 CLS-W45-00.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,16600,1,15,0,1,1500,10,1,1,10,1080,50,20,5,800,1350,1500,1500,0,20,6,1,20,4,20,1,50,1,1,1,1}, //16600=v1.2 CLS-D1S-00.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,16800,1,15,0,1,1500,10,1,1,10,1010,50,500,5,800,1000,1500,1500,0,20,6,1,20,4,20,1,50,1,1,1,1}, //16800=v1.2 CLS-T45-00.bat"
{2100,1500,900,3100,2048,996,3,0,0,1,1,1,16900,1,15,0,1,1500,10,1,1,10,1080,50,20,5,800,1350,1500,1500,0,20,6,1,20,4,20,1,50,1,1,1,1}, //16900=v1.2 CLS-T55-00.bat"
{2100,1500,900,2900,2048,1200,2,10,0,0,0,7,10800,0,18,0,0,1500,10,1,1,8,1510,100,100,10,500,1500,800,3000,0,10,7,1,28,2,8,1,0,2,0,0,0}, //10800-PGC-M8.-00.bat"

};

const struct Servo_Tai_Data_ factory_tai_para[]=
{
    /*{100, 1200, 1, 200, 100, 50, 2048, 0, 0, 45, 0},
    {95, 1100, 1, 130, 175, 35, 2048, 0, 0, 45, 0},
    {95, 1100, 1, 130, 175, 35, 2048, 0, 0, 45, 0},
    {95, 1100, 1, 130, 175, 35, 2048, 0, 0, 45, 0},
    {95, 1100, 1, 130, 175, 35, 2048, 0, 0, 45, 0},
    {95, 1100, 1, 130, 175, 35, 2018, 0, 0, 45, 0},*/
    {100, 1100, 1, 270, 155, 120, 2048, 0, 0, 45, 0},
    {95, 1100, 1, 130, 175, 35, 2048, 0, 0, 45, 0},
    {95, 1100, 1, 130, 175, 35, 2048, 0, 0, 45, 0},
    {95, 1100, 1, 130, 175, 35, 2048, 0, 0, 45, 0},
    {95, 1100, 1, 130, 155, 35, 2048, 0, 0, 45, 0},
};


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    operate_states_(IDLE),ticks_(0),cmd_ticks_(0),time_out_set_(100)
{
    ui->setupUi(this);
    this->setWindowTitle("SRT");
    this->setFixedSize(708, 450);
    setWindowIcon(QIcon(":/logo.ico"));
//    ui->gaugeCircle->setPointerStyle(GaugeCircle::PointerStyle_Indicator);
    ui->textEdit->setDisabled(1);
    ui->tabWidget->setCurrentIndex(0);
    text_show.clear();
    ui->pushButton_open->setStyleSheet(
                "QPushButton{background-color:white; color:black;   border-radius: 5px;  border: 2px; groove gray;border-style: outset;}"
                "QPushButton:hover{background-color:rgb(128,138, 138); color: white;}"
                "QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }"
                );
    ui->pushButton_save->setStyleSheet(
                "QPushButton{background-color:white; color:black;   border-radius: 5px;  border: 2px; groove gray;border-style: outset;}"
                "QPushButton:hover{background-color:rgb(128,138, 138); color: white;}"
                "QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }"
                );
    ui->pushButton_writeData->setStyleSheet(
                "QPushButton{background-color:white; color:black;   border-radius: 5px;  border: 2px; groove gray;border-style: outset;}"
                "QPushButton:hover{background-color:rgb(128,138, 138); color: white;}"
                "QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }"
                );
    ui->pushButton_readData->setStyleSheet(
                "QPushButton{background-color:white; color:black;   border-radius: 5px;  border: 2px; groove gray;border-style: outset;}"
                "QPushButton:hover{background-color:rgb(128,138, 138); color: white;}"
                "QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }"
                );
    ui->pushButton_Default->setStyleSheet(
                "QPushButton{background-color:white; color:black;   border-radius: 5px;  border: 2px; groove gray;border-style: outset;}"
                "QPushButton:hover{background-color:rgb(128,138, 138); color: white;}"
                "QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }"
                );

    ui->pushButton_connect->setStyleSheet(
                "QPushButton{background-color:rgb(0, 191, 255); color:black;   border-radius: 5px;  border: 2px; groove gray;border-style: outset;}"
                "QPushButton:hover{background-color:rgb(128,138, 138); color: white;}"
                "QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }"
                );
    ui->pushButton_lcdfirmware->setStyleSheet(
                "QPushButton{background-color:white; color:black;   border-radius: 5px;  border: 2px; groove gray;border-style: outset;}"
                "QPushButton:hover{background-color:rgb(128,138, 138); color: white;}"
                "QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }"
                );
    ui->pushButton_lcdupgrade->setStyleSheet(
                "QPushButton{background-color:white; color:black;   border-radius: 5px;  border: 2px; groove gray;border-style: outset;}"
                "QPushButton:hover{background-color:rgb(128,138, 138); color: white;}"
                "QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }"
                );
    ui->lineEdit->setStyleSheet("QLineEdit {background-color:white; color:black;}");
    ui->lineEdit_lcdfirmware->setStyleSheet("QLineEdit {background-color:white; color:black;}");
//    ui->lineEdit_pos->setStyleSheet("QLineEdit {background-color:white; color:black;}");
    ui->textEdit->setStyleSheet(
                "QTextEdit {background-color:rgba(29, 29, 29, 100%); color:white; border-width: 2px;  border-color: white;}"
                );
    ui->comboBox_com->setStyleSheet("QComboBox {background-color:white; color: black;}"
                                    "QComboBox:QAbstractItemView {background-color: white; color: white}");


    QPushButton *prt[16];
    prt[0] = ui->pushButton_3;
    prt[1] = ui->pushButton_4;
    prt[2] = ui->pushButton_5;
    prt[3] = ui->pushButton_6;
    for(int i=0; i<4; i++){
        prt[i]->setStyleSheet(
                        "QPushButton {background-color:white; color:black;   border-radius: 5px;  border: 2px; groove gray;border-style: outset;}"
                        );
    }

    prt[0] = ui->pushButton_s1;
    prt[1] = ui->pushButton_s2;
    prt[2] = ui->pushButton_s3;
    prt[3] = ui->pushButton_s4;
    prt[4] = ui->pushButton_s5;
    prt[5] = ui->pushButton_s6;
    prt[6] = ui->pushButton_s7;
    prt[7] = ui->pushButton_s8;
    for(int i=0; i<8; i++){
        prt[i]->setStyleSheet(
                        "QPushButton {background-color:white; color:black;   border-radius: 5px;  border: 2px; groove gray;border-style: outset;}"
                        );
    }

    prt[0] = ui->pushButton_p1;
    prt[1] = ui->pushButton_p2;
    prt[2] = ui->pushButton_p3;
    prt[3] = ui->pushButton_p4;
    prt[4] = ui->pushButton_p5;
    prt[5] = ui->pushButton_p6;
    prt[6] = ui->pushButton_p7;
    prt[7] = ui->pushButton_p8;
    for(int i=0; i<8; i++){
        prt[i]->setStyleSheet(
                        "QPushButton {background-color:black; color:white;   border-radius: 5px;  border: 2px; groove gray;border-style: outset;}"
                        "QPushButton:hover{background-color:rgb(128,138, 138); color: white;}"
                        "QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }"
                        );
    }

//    prt[0] = ui->pushButton_i1;
//    prt[1] = ui->pushButton_i2;
//    for(int i=0; i<2; i++){
//        prt[i]->setStyleSheet(
//                        "QPushButton {background-color:white; color:black;   border-radius: 5px;  border: 2px; groove gray;border-style: outset;}"
//                        );
//    }

    QLabel *label_prt[8];
    label_prt[0] = ui->label_4;
    label_prt[1] = ui->label_4;
    label_prt[2] = ui->label_4;
    label_prt[3] = ui->label_5;
    for(int i=0; i<4; i++){
        label_prt[i]->setStyleSheet(
                        "QLabel {background-color:white; color: black;}"
                        );
    }

    label_prt[0] = ui->label_s1;
    label_prt[1] = ui->label_s2;
    label_prt[2] = ui->label_s3;
    label_prt[3] = ui->label_s4;
    label_prt[4] = ui->label_s5;
    label_prt[5] = ui->label_s6;
    label_prt[6] = ui->label_s7;
    label_prt[7] = ui->label_s8;
    for(int i=0; i<8; i++){
        label_prt[i]->setStyleSheet(
                        "QLabel {background-color:white; color: black;}"
                        );
    }
    ui->label_ver->setStyleSheet(
                "QLabel {color: white;}"
                );

    QComboBox *combox_ptr[8];
    combox_ptr[0] = ui->comboBox_s1;
    combox_ptr[1] = ui->comboBox_s2;
    combox_ptr[2] = ui->comboBox_s3;
    combox_ptr[3] = ui->comboBox_s4;
    combox_ptr[4] = ui->comboBox_s5;
    combox_ptr[5] = ui->comboBox_s6;
    combox_ptr[6] = ui->comboBox_s7;
    combox_ptr[7] = ui->comboBox_s8;
    for(int i=0; i<8; i++){
        combox_ptr[i]->setStyleSheet(
                        "QComboBox {background-color:black; color: white;}"
                        "QComboBox:QAbstractItemView {background-color: white; color: white}"
                    );
    }
    for(int i=0; i<7; i++){
        QStandardItemModel *pItemModel = qobject_cast<QStandardItemModel*>(combox_ptr[i]->model());
        for(int j=0; j<11; j++){
            pItemModel->item(j)->setBackground(QColor(255,255,255));
        }
    }
    QStandardItemModel *pItemModel = qobject_cast<QStandardItemModel*>(combox_ptr[7]->model());
    for(int j=0; j<2; j++){
        pItemModel->item(j)->setBackground(QColor(255,255,255));
    }

    QSpinBox *spinbox_ptr[8];
    spinbox_ptr[0] = ui->spinBox_1;
    spinbox_ptr[1] = ui->spinBox_2;
    spinbox_ptr[2] = ui->spinBox_3;
    spinbox_ptr[3] = ui->spinBox_4;
    spinbox_ptr[4] = ui->spinBox_5;
    spinbox_ptr[5] = ui->spinBox_6;
    spinbox_ptr[6] = ui->spinBox_7;
    for(int i=0; i<7; i++){
        spinbox_ptr[i]->setStyleSheet(
                        "QSpinBox {background-color:black; color: white;}"
                        "QSpinBox:QAbstractItemView {background-color: white; color: white}"
                    );
    }

//    ui->tabWidget->setStyleSheet("QTabBar::tab {background-color:rgb(29, 29, 29); color: white;}"
//                                 "QTabWidget::pane {background-color:rgb(29, 29, 29);}"
//                                 "QTabWidget::tab-bar {background-color:rgb(29, 29, 29);}"
////                               "QTabBar::tab {border-color: black; border-radius: 1px; border: 2px; padding: 2px;}"
//                                 );


//    ui->tabWidget->setStyleSheet(

//                                 );
    ui->tabWidget->setStyleSheet(
//                                "background-color: rgb(29, 29, 29);"
                                "QWidget#tab_1 {background-color:rgb(29, 29, 29);}"
                                "QWidget#tab_2 {background-color:rgb(29, 29, 29);}"
                                "QWidget#tab_3 {background-color:rgb(29, 29, 29);}"
                                 "QTabBar::pane {background-color:rgb(29, 29, 29);}"
                                 "QTabBar::tab {background-color:rgb(29, 29, 29); color:white}"
                                 "QTabBar::tab:selected {background-color:white; color:black}"
                                 "QTabWidget::tab-bar {background-color:rgb(29, 29, 29);}"
                                 );

    ui->comboBox_s1->setVisible(false);
    ui->comboBox_s2->setVisible(false);
    ui->comboBox_s3->setVisible(false);
    ui->comboBox_s4->setVisible(false);
    ui->comboBox_s5->setVisible(false);
    ui->comboBox_s6->setVisible(false);
    ui->comboBox_s7->setVisible(false);
    //--------------------------------------------------
    serialport_= new QSerialPort();
    connect(serialport_,SIGNAL(readyRead()),this,SLOT(receieve_bytes_update()));
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial1;
        serial1.setPort(info);
        if(serial1.open(QIODevice::ReadWrite))
        {
            init_usart_list_.append(info.portName());
            ui->comboBox_com->addItem(info.portName());
            serial1.close();
        }
    }
    com_detect_timer_= new QTimer;
    com_detect_timer_->start(1000);
    connect(com_detect_timer_, SIGNAL(timeout()), this, SLOT(com_detect_timeout()));

    timer_1_= new QTimer;
    connect(timer_1_, SIGNAL(timeout()), this, SLOT( System_Ticks() ));//处理数据回调函数
    timer_1_->start(1);
    //--------------------------------------------------
    timer_2_= new QTimer;
    connect(timer_2_, SIGNAL(timeout()), this, SLOT( lcd_Ticks() ));//处理数据回调函数
    timer_2_->start(10);
    lcd_protocol_ = new lcd_protocol(serialport_);
//    Servo_Data_Stru_ struc_temp;
//    struc_temp.work_p12 = 4001;
//    lcd_protocol_->get_version(struc_temp);
//    qDebug() << sizeof(Servo_Data_Stru_);
    data_send_receive = new serial_send_receive;
    servoProtocl_ = new servo_protocol(serialport_);
    //--------------------------------------------------
//    const char *servo_version[] = {"PGC-D12", "D1206G2", "WH-20KG", "WH-30KG", "WH-40KG",
//                             "PGC-DRF", "PGC-D15", "PGC-R12", "PGC-A20",
//                             "PGC-A30", "PGC-A40", "PGC-A50"
//                            };
//    ui->listWidget->clear();
//    for(int i=0; i<11; i++){
//        ui->listWidget->addItem(servo_version[i]);
//    }
    ui->listWidget->setVisible(false);
//    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem *)), this,SLOT(items_callback(QListWidgetItem *)));
    //--------------------------------------------------
    ui->pushButton_connect->setText("Connect");
    ui->progressBar->setValue(0);
}

void MainWindow::items_callback(QListWidgetItem *item)
{
//    qDebug() << item->text();
//    ui->listWidget->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::text_append_string(const char *data)
{
    QString res_data;
    if(text_show.size() >=4){
        text_show.erase(text_show.begin());
    }
    text_show.append(data);
    for(int i=0; i<text_show.size(); i++){
        res_data.append(text_show[i]);
    }
    return res_data;
}

void MainWindow::com_detect_timeout()
{
    QStringList usart_list;
    if(!serialport_->isOpen()){
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        {
            QSerialPort serial1;
            serial1.setPort(info);
            if(serial1.open(QIODevice::ReadWrite))
            {
                usart_list.append(info.portName());
                serial1.close();
            }
        }
        if(usart_list != init_usart_list_){
            ui->comboBox_com->clear();
            init_usart_list_.clear();
            for(int i=0; i<usart_list.size(); i++){
                ui->comboBox_com->addItem(usart_list.at(i));
                init_usart_list_.append(usart_list.at(i));
            }
        }
    }
}

void MainWindow::receieve_bytes_update(void)
{
  QByteArray temp = serialport_->readAll();
  qDebug() << "------" << temp.size();
  if(temp.size() != 13){
    lcd_data_.append(temp);
  }
  else{
    requestData.append(temp);
  }

}

void MainWindow::on_pushButton_open_clicked()
{
    QString fileName = QFileDialog::getOpenFileName();
    if(fileName.isEmpty()){
        return;
    }

    int tai_size = sizeof(lcd_protocol_->servo_tai_data_);
    QFile  file(fileName);
    bool res = file.open(QIODevice::ReadOnly);//|QIODevice::Text
    if( !res ) return ;
    QByteArray  t = file.readAll();
    qDebug() << "t size " << t.size() << " tai size " << tai_size;
    if( t.size() == tai_size )
    {
        qDebug() << "tai servo";
        memcpy((char*)&lcd_protocol_->servo_tai_data_, t.data(), tai_size);
        lcd_protocol_->get_data(ui_data_);

        ui->spinBox_1->setValue(ui_data_.max_power);
        ui->spinBox_2->setValue(ui_data_.boost);
        ui->spinBox_3->setValue(ui_data_.dead_band);
        ui->spinBox_4->setValue(ui_data_.tension);
        ui->spinBox_5->setValue(ui_data_.force);
        ui->spinBox_6->setValue(ui_data_.brake);
        ui->spinBox_7->setValue(ui_data_.center_num);
        ui->comboBox_s8->setCurrentIndex(ui_data_.soft_start);
        lcd_protocol_->get_version(lcd_protocol_->servo_data_);
        ui->lineEdit->setText(lcd_protocol_->version_);
        ui->textEdit->setHtml(text_append_string("<span style=' color:#0080ff;'>Open file success!</span> <br/>"));
        return;
    }


    int     iSize = sizeof(lcd_protocol_->servo_data_);
    char    *buf = new char[iSize+1];
    bool    bhr = CSysFile::read(fileName,buf,iSize);
    if (bhr){
        memcpy((char*)&lcd_protocol_->servo_data_,buf,iSize);
        lcd_protocol_->get_data(ui_data_);

        ui->spinBox_1->setValue(ui_data_.max_power);
        ui->spinBox_2->setValue(ui_data_.boost);
        ui->spinBox_3->setValue(ui_data_.dead_band);
        ui->spinBox_4->setValue(ui_data_.tension);
        ui->spinBox_5->setValue(ui_data_.force);
        ui->spinBox_6->setValue(ui_data_.brake);
        ui->spinBox_7->setValue(ui_data_.center_num);
        ui->comboBox_s8->setCurrentIndex(ui_data_.soft_start);
        lcd_protocol_->get_version(lcd_protocol_->servo_data_);
        ui->lineEdit->setText(lcd_protocol_->version_);
        ui->textEdit->setHtml(text_append_string("<span style=' color:#0080ff;'>Open file success!</span> <br/>"));
    }
    else{
        ui->textEdit->setHtml(text_append_string("<span style=' color:#ff0000;'>Open file fail!</span> <br/>"));

    }
}

void MainWindow::on_pushButton_connect_clicked()
{
    if(ui->pushButton_connect->text() == "Connect"){
        serialport_->setPortName(ui->comboBox_com->currentText());
        serialport_->setBaudRate(115200);
        serialport_->setDataBits(QSerialPort::Data8);
        serialport_->setParity(QSerialPort::NoParity);
        serialport_->setStopBits(QSerialPort::OneStop);
        serialport_->setFlowControl(QSerialPort::NoFlowControl);
        if(serialport_->open(QIODevice::ReadWrite))
        {
            lcd_protocol_->send_connect_data();
            operate_states_ = CONNECT_SERVO_DATA;
            cmd_ticks_ = ticks_;
            time_out_set_ = 300;
            lcd_data_.clear();
            qDebug() <<"cmd_ticks_" << cmd_ticks_;
        }
        else
        {
            QMessageBox::critical(this, QString::fromLocal8Bit("Error"), serialport_->errorString());
            return;
        }
    }
    else{
        operate_states_ = DISCONECT_SERVO_DATA;
        cmd_ticks_ = ticks_;
        time_out_set_ = 100;
        lcd_data_.clear();
        lcd_protocol_->send_disconnect_data();
    }
}

void MainWindow::on_pushButton_save_clicked()
{
    QFileDialog fileDialog;
    QString fileName = fileDialog.getSaveFileName(this,"Save File","","bat File(*.bat)");
    if(fileName == ""){
        return;
    }
    if(!fileName.contains(".bat")) fileName += ".bat";

    ui_data_.max_power =ui->spinBox_1->value();
    ui_data_.boost = ui->spinBox_2->value();
    ui_data_.dead_band = ui->spinBox_3->value();
    ui_data_.tension = ui->spinBox_4->value();
    ui_data_.force = ui->spinBox_5->value();
    ui_data_.brake = ui->spinBox_6->value();
    ui_data_.center_num = ui->spinBox_7->value();
    ui_data_.soft_start = ui->comboBox_s8->currentIndex();

    lcd_protocol_->set_data(ui_data_);
    if(lcd_protocol_->is_tai_servo_)
    {
        int  iSize = sizeof(lcd_protocol_->servo_tai_data_);
        QByteArray buf;
        uint8_t *data = (uint8_t *)&lcd_protocol_->servo_tai_data_;
        for(int i=0; i<iSize; i++)
        {
            buf.append(data[i]);
        }
        QFile file(fileName);
        if(!file.open(QIODevice::WriteOnly)){
            qDebug() << "Can't open file for writing";
            return;
        }
        file.write(buf);
        file.flush();
        file.close();
    }
    else
    {
        int  iSize = sizeof(lcd_protocol_->servo_data_);
        char *buf = new char[iSize+1];
        memcpy(buf,(char*)&lcd_protocol_->servo_data_,iSize);
        CSysFile::write(fileName,buf,iSize);
    }

    ui->textEdit->setHtml(text_append_string("<span style=' color:#0080ff;'>Save file success!</span><br/>"));

}

void MainWindow::on_pushButton_lcdfirmware_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,QString::fromLocal8Bit("Open"),QDir::currentPath(),QString::fromLocal8Bit("binary(*.bin)"));
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
            qDebug("open file");
            QDataStream bin(&file);
            file_bin.clear();
            while(!bin.atEnd())
            {
             char num;
             bin.readRawData(&num,1);
             file_bin.append(num);
            }
            quint8 tem_num =  (file_bin.length() % 4);
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
            ui->lineEdit_lcdfirmware->setText(filename);
        }
    }
}

void MainWindow::on_pushButton_lcdupgrade_clicked()
{
    if(ui->lineEdit_lcdfirmware->text().isEmpty()){
        QMessageBox::critical(this, QString::fromLocal8Bit("Error"), "Please load firmware");
        return;
    }

    QSerialPort *my_serialport = serialport_;
    if (my_serialport->isOpen())
    {
        system_state = UPDATE_REQUEST;
        crc32_length = 0;
        system_package = 0;
        adress = 0;
        update_send_receieve_finish = 1;
        requestData.clear();
        ui->pushButton_connect->setDisabled(1);

    }
    else{
        if(ui->comboBox_com->currentIndex() <0){
            QMessageBox::critical(this, QString::fromLocal8Bit("Error"), "Please check connection");
            return;
        }
        my_serialport->setPortName(ui->comboBox_com->currentText());
        my_serialport->setBaudRate(115200);
        my_serialport->setDataBits(QSerialPort::Data8);
        my_serialport->setParity(QSerialPort::NoParity);
        my_serialport->setStopBits(QSerialPort::OneStop);
        my_serialport->setFlowControl(QSerialPort::NoFlowControl);
        if(my_serialport->open(QIODevice::ReadWrite))
        {
            system_state = UPDATE_REQUEST;
            crc32_length = 0;
            system_package = 0;
            adress = 0;
            update_send_receieve_finish = 1;
            requestData.clear();
            ui->pushButton_connect->setDisabled(1);
        }
        else
        {
            QMessageBox::critical(this, QString::fromLocal8Bit("Error"), my_serialport->errorString());
            return;
        }
    }
}

void MainWindow::System_Ticks()
{
    static uint32_t ticks_info=0;
    system_tick ++;
    QSerialPort *my_serialport = serialport_;
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
                    qDebug() << version;
                    system_state = UPDATE_TRAGER;
                    break;
                }
            }
        }
    }
}

void MainWindow::lcd_Ticks()
{
    static qint64 show_ticks = 0;
    static quint8 count = 0;
    ticks_++;
    if(operate_states_ != IDLE){
        //time out check
        if(ticks_ - cmd_ticks_ > time_out_set_){
            qDebug() << "Recv data time out!";
            switch (operate_states_) {
            case READ_SERVO_DATA:
                QMessageBox::critical(this, QString::fromLocal8Bit("Error"), "Read data fail!");
                break;
            case WRITE_SERVO_DATA:
                QMessageBox::critical(this, QString::fromLocal8Bit("Error"), "Write data fail!");
                break;
            case DEFAULT_SERVO_DATA:
                QMessageBox::critical(this, QString::fromLocal8Bit("Error"), "Default data fail!");
                break;
            case CONNECT_SERVO_DATA:
                serialport_->close();
                QMessageBox::critical(this, QString::fromLocal8Bit("Error"), "Connect servo fail!");
                break;
            case DISCONECT_SERVO_DATA:
                QMessageBox::critical(this, QString::fromLocal8Bit("Error"), "Disconnect servo fail!");
                break;
            default:
                break;
            }
            operate_states_ = IDLE;
            return;
        }

        if(ticks_ - show_ticks >100){
            show_ticks = ticks_;
            count++;
            QString data = "Please wait! It needs seconds to write.";
            QString data1 = "Please wait! It needs seconds to read.";
            QString data2 = "Please wait! It needs seconds to connect.";
            for(int i=0; i<count; i++){
                data.append('.');
                data1.append('.');
                data2.append('.');
            }
            switch (operate_states_) {
            case WRITE_SERVO_DATA:
                data.append("<br/>");
                ui->textEdit->setHtml(text_append_string(data.toLatin1().data()).toLatin1().data());
                break;
            case DEFAULT_SERVO_DATA:
                data.append("<br/>");
                ui->textEdit->setHtml(text_append_string(data.toLatin1().data()).toLatin1().data());
                break;
            case READ_SERVO_DATA:
                data1.append("<br/>");
                ui->textEdit->setHtml(text_append_string(data1.toLatin1().data()).toLatin1().data());
                break;
            case CONNECT_SERVO_DATA:
                data2.append("<br/>");
                ui->textEdit->setHtml(text_append_string(data2.toLatin1().data()).toLatin1().data());
                break;
            default:
                break;
            }
        }

        //data check
        if(lcd_data_.size() == (sizeof(Servo_Data_Stru_) +6)){
            qDebug() << lcd_data_.at(20);
            qDebug() << lcd_data_.at(21);
            if(lcd_protocol_->data_process(lcd_data_)){
                switch (operate_states_) {
                case READ_SERVO_DATA:
                    qDebug() << "READ_SERVO_DATA";
                    lcd_protocol_->get_data(ui_data_);
                    ui->spinBox_1->setValue(ui_data_.max_power);
                    ui->spinBox_2->setValue(ui_data_.boost);
                    ui->spinBox_3->setValue(ui_data_.dead_band);
                    ui->spinBox_4->setValue(ui_data_.tension);
                    ui->spinBox_5->setValue(ui_data_.force);
                    ui->spinBox_6->setValue(ui_data_.brake);
                    ui->spinBox_7->setValue(ui_data_.center_num);
                    ui->comboBox_s8->setCurrentIndex(ui_data_.soft_start);
                    if(lcd_protocol_->is_tai_servo_){
                        lcd_protocol_->get_version(lcd_protocol_->servo_tai_data_);
                    }
                    else{
                        lcd_protocol_->get_version(lcd_protocol_->servo_data_);
                    }
                    ui->lineEdit->setText(lcd_protocol_->version_);
                    ui->textEdit->setHtml(text_append_string("<span style=' color:#0080ff;'>Read servo success!</span> <br/>"));
                    servo_version_read_ = ui_data_.version;
                    qDebug() << servo_version_read_;
                    break;
                case WRITE_SERVO_DATA:
                    ui->textEdit->setHtml(text_append_string("<span style=' color:#0080ff;'>Write servo success!</span> <br/>"));
                    break;
                case DEFAULT_SERVO_DATA:
                    ui->textEdit->setHtml(text_append_string("<span style=' color:#0080ff;'>Default servo  success!</span> <br/>"));
                    break;
                case CONNECT_SERVO_DATA:
                    qDebug() << "CONNECT_SERVO_DATA";
                    ui->textEdit->setHtml(text_append_string("<span style='color:#0080ff'>Connect servo success!</span> <br/>"));
                    lcd_protocol_->get_data(ui_data_);
                    ui->spinBox_1->setValue(ui_data_.max_power);
                    ui->spinBox_2->setValue(ui_data_.boost);
                    ui->spinBox_3->setValue(ui_data_.dead_band);
                    ui->spinBox_4->setValue(ui_data_.tension);
                    ui->spinBox_5->setValue(ui_data_.force);
                    ui->spinBox_6->setValue(ui_data_.brake);
                    ui->spinBox_7->setValue(ui_data_.center_num);
                    ui->comboBox_s8->setCurrentIndex(ui_data_.soft_start);
                    if(lcd_protocol_->is_tai_servo_){
                        lcd_protocol_->get_version(lcd_protocol_->servo_tai_data_);
                    }
                    else{
                        lcd_protocol_->get_version(lcd_protocol_->servo_data_);
                    }
                    ui->lineEdit->setText(lcd_protocol_->version_);
                    ui->pushButton_connect->setText("Disconnect");
                    servo_version_read_ = ui_data_.version;
                    qDebug() << servo_version_read_;
                    break;
                case DISCONECT_SERVO_DATA:
                    ui->pushButton_connect->setText("Connect");
                    ui->textEdit->setHtml(text_append_string("<span style='color:#0080ff'>Disconnect servo success!</span> <br/>"));
                    serialport_->close();
                    break;
                default:
                    break;
                }
                operate_states_ = IDLE;
            }
            else{
                qDebug() << "erro process data";
            }
            lcd_data_.clear();
        }

    }
    else{
        if(lcd_data_.size() == (sizeof(Servo_Data_Stru_) +6)){
            QMessageBox::critical(this, QString::fromLocal8Bit("Error"), "Servo disconnect!");
            ui->pushButton_connect->setText("Connect");
            serialport_->close();
            lcd_data_.clear();
        }
        count = 0;
    }
    lcd_data_.clear();
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
#define CMD_REQUEST     0xc1
#define CMD_OPENLOCK    0xc3
#define CMD_UPDATE      0xc5
#define CMD_FINISH      0xc7
#define CMD_ERASE       0xc9
---------------------------------------------------*/
void MainWindow::Update_process(void)
{
    static uint8_t file_end_flag = 0;
    QSerialPort *my_serialport = serialport_;
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
                        ui->textEdit->setHtml(text_append_string("<span style=' color:#ff0000;'>Connect servo fail!</span><br/>"));
                    }
                    break;
                }
                case RECEIEVE_FINISH:
                {
                    qDebug("[]--request!");
                    ui->textEdit->setHtml(text_append_string("<span style=' color:#0080ff;'>Connect servo success!</span><br/>"));
                    system_state = UPDATE_REQUEST_FINISH;
                    ui->textEdit->setHtml(text_append_string("<span style=' color:#0080ff;'>Begain update servo success!</span><br/>"));
                    break;
                }
                default:
                {
                  if(system_tick %1000 ==0)
                  {
                      ui->textEdit->setHtml(text_append_string("Connect.<pre>\r\n</pre>"));

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
//                    ui->pushButton->setEnabled(0);
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
//                    ui->pushButton->setEnabled(0);
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
//                    ui->pushButton->setEnabled(0);
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
                ui->textEdit->setHtml(text_append_string("<span style=' color:#0080ff;'>Upgrade servo success!</span><br/>"));
            }
            ui->pushButton_connect->setDisabled(0);
            ui->pushButton_connect->setText("Connect");
            break;
        }
    }
}
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

void MainWindow::band_send(quint16 pwm, bool brod)
{
    if(ui->pushButton_connect->text() == "Connect"){
        QMessageBox::critical(this, QString::fromLocal8Bit("Error"), "Please connect!");
        return;
    }
    if(brod){
        switch(pwm){
        case 500:
            ui->gaugeCircle->setValue(-90);
            break;
        case 900:
            ui->gaugeCircle->setValue(-60);
            break;
        case 1500:
            ui->gaugeCircle->setValue(0);
            break;
        case 2100:
            ui->gaugeCircle->setValue(60);
            break;
        case 2500:
            ui->gaugeCircle->setValue(90);
            break;
        }
    }
    else{
        switch(pwm){
        case 500:
            ui->gaugeCircle->setValue(-60);
            break;
        case 750:
            ui->gaugeCircle->setValue(0);
            break;
        case 1000:
            ui->gaugeCircle->setValue(60);
            break;
        }
    }
    lcd_protocol_->send_test_data(pwm);
}

void MainWindow::on_pushButton_p1_clicked()
{
    band_send(500, true);
}

void MainWindow::on_pushButton_p2_clicked()
{
    band_send(900, true);
}

void MainWindow::on_pushButton_p3_clicked()
{
    band_send(1500, true);
}

void MainWindow::on_pushButton_p4_clicked()
{
    band_send(2100, true);
}

void MainWindow::on_pushButton_p5_clicked()
{
    band_send(2500, true);
}

void MainWindow::on_pushButton_p6_clicked()
{
    band_send(500, false);
}

void MainWindow::on_pushButton_p7_clicked()
{
    band_send(750,false);
}

void MainWindow::on_pushButton_p8_clicked()
{
    band_send(1000,false);
}

void MainWindow::on_pushButton_readData_clicked()
{
    if(ui->pushButton_connect->text() == "Connect"){
        QMessageBox::critical(this, QString::fromLocal8Bit("Error"), "Please connect!");
        return;
    }

    lcd_protocol_->send_read_data();
    operate_states_ = READ_SERVO_DATA;
    cmd_ticks_ = ticks_;
    time_out_set_ = 300;
    lcd_data_.clear();
}

void MainWindow::on_pushButton_writeData_clicked()
{
    if(ui->pushButton_connect->text() == "Connect"){
        QMessageBox::critical(this, QString::fromLocal8Bit("Error"), "Please connect!");
        return;
    }

    if(servo_version_read_ != ui_data_.version){
//        QMessageBox::critical(this, QString::fromLocal8Bit("Error"), "Different kinds of servo!");
        ui->textEdit->setHtml(text_append_string("<span style=' color:#ff0000;'>Different kinds of servo!</span><br/>"));
        return;
    }


    ui_data_.max_power =ui->spinBox_1->value();
    ui_data_.boost = ui->spinBox_2->value();
    ui_data_.dead_band = ui->spinBox_3->value();
    ui_data_.tension = ui->spinBox_4->value();
    ui_data_.force = ui->spinBox_5->value();
    ui_data_.brake = ui->spinBox_6->value();
    ui_data_.center_num = ui->spinBox_7->value();
    ui_data_.soft_start = ui->comboBox_s8->currentIndex();

    lcd_protocol_->set_data(ui_data_);

    if(lcd_protocol_->is_tai_servo_){
        lcd_protocol_->send_write_tai_data(&lcd_protocol_->servo_tai_data_);
    }
    else{
        lcd_protocol_->send_write_data(&lcd_protocol_->servo_data_);
    }
    operate_states_ = WRITE_SERVO_DATA;
    cmd_ticks_ = ticks_;
    time_out_set_ = 1000;
    lcd_data_.clear();
}

void MainWindow::on_pushButton_Default_clicked()
{
    if(ui->pushButton_connect->text() == "Connect"){
        QMessageBox::critical(this, QString::fromLocal8Bit("Error"), "Please connect!");
        return;
    }

    if(servo_version_read_ != ui_data_.version){
        ui->textEdit->setHtml(text_append_string("<span style=' color:#ff0000;'>Different kinds of servo!</span><br/>"));
        return;
    }

    int index = lcd_protocol_->find_version_index(servo_version_read_);
    if(index == 1000){
        QMessageBox::critical(this, QString::fromLocal8Bit("Error"), "The servo not support!");
        return;
    }
    if(lcd_protocol_->is_tai_servo_){ //Servo_Data_Stru_ factory_para
        index = index - (sizeof(factory_para)/ sizeof(Servo_Data_Stru_));
        qDebug() << "----index" <<index;
        memcpy(&lcd_protocol_->servo_tai_data_, &factory_tai_para[index], sizeof(Servo_Tai_Data_));
    }
    else{
        memcpy(&lcd_protocol_->servo_data_, &factory_para[index], sizeof(Servo_Data_Stru_));
    }

    //ui show
    lcd_protocol_->get_data(ui_data_);
    ui->spinBox_1->setValue(ui_data_.max_power);
    ui->spinBox_2->setValue(ui_data_.boost);
    ui->spinBox_3->setValue(ui_data_.dead_band);
    ui->spinBox_4->setValue(ui_data_.tension);
    ui->spinBox_5->setValue(ui_data_.force);
    ui->spinBox_6->setValue(ui_data_.brake);
    ui->spinBox_7->setValue(ui_data_.center_num);
    ui->comboBox_s8->setCurrentIndex(ui_data_.soft_start);
    //write data
    if(lcd_protocol_->is_tai_servo_){
        lcd_protocol_->send_write_tai_data(&lcd_protocol_->servo_tai_data_);
    }
    else{
        lcd_protocol_->send_write_data(&lcd_protocol_->servo_data_);
    }
    operate_states_ = DEFAULT_SERVO_DATA;
    cmd_ticks_ = ticks_;
    time_out_set_ = 1000;
    lcd_data_.clear();
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    static int count = 0;
    count ++;
    if(count <2){
        return;
    }
    if(index != 1){
        band_send(0,false);
    }
    qDebug() << index;
}
