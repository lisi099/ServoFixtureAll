#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QStandardItemModel"
#include "QMessageBox"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(708, 425);
    ui->pushButton_open->setStyleSheet(
                "QPushButton{background-color:white; color:black;   border-radius: 5px;  border: 2px; groove gray;border-style: outset;}"
                "QPushButton:hover{background-color:blue; color: white;}"
                "QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }"
                );
    ui->pushButton_save->setStyleSheet(
                "QPushButton{background-color:white; color:black;   border-radius: 5px;  border: 2px; groove gray;border-style: outset;}"
                "QPushButton:hover{background-color:blue; color: white;}"
                "QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }"
                );
    ui->pushButton_connect->setStyleSheet(
                "QPushButton{background-color:red; color:black;   border-radius: 5px;  border: 2px; groove gray;border-style: outset;}"
                "QPushButton:hover{background-color:blue; color: white;}"
                "QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }"
                );
    ui->lineEdit->setStyleSheet("QLineEdit {background-color:white; color:black;}");
    ui->lineEdit_pos->setStyleSheet("QLineEdit {background-color:white; color:black;}");
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
                        );
    }

    prt[0] = ui->pushButton_i1;
    prt[1] = ui->pushButton_i2;
    for(int i=0; i<2; i++){
        prt[i]->setStyleSheet(
                        "QPushButton {background-color:white; color:black;   border-radius: 5px;  border: 2px; groove gray;border-style: outset;}"
                        );
    }

    QLabel *label_prt[8];
    label_prt[0] = ui->label_2;
    label_prt[1] = ui->label_3;
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

    serialport_= new QSerialPort();
    connect(serialport_, SIGNAL(readyRead()), this, SLOT(receieve_bytes()));

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
    serial_open_state_ = false;
    com_detect_timer_= new QTimer;
    com_detect_timer_->start(1000);
    connect(com_detect_timer_, SIGNAL(timeout()), this, SLOT(com_detect_timeout()));

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::com_detect_timeout()
{
    QStringList usart_list;
    if(!serial_open_state_){
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
//            ui->plainTextEdit->appendPlainText(QStringLiteral("串口更新"));
            ui->comboBox_com->clear();
            init_usart_list_.clear();
            for(int i=0; i<usart_list.size(); i++){
                ui->comboBox_com->addItem(usart_list.at(i));
                init_usart_list_.append(usart_list.at(i));
            }
        }
    }

}

void MainWindow::receieve_bytes(void)
{
    QByteArray temp = serialport_->readAll();
}

void MainWindow::on_pushButton_open_clicked()
{
    if(ui->pushButton_open->text() == QStringLiteral("打开串口")){
//        serialport_->setPortName(ui->comboBox_name->currentText());
//        serialport_->setBaudRate(ui->comboBox_baut->currentText().toInt());
//        serialport_->setDataBits(QSerialPort::Data8);
//        serialport_->setParity(QSerialPort::NoParity);
//        serialport_->setStopBits(QSerialPort::OneStop);
//        serialport_->setFlowControl(QSerialPort::NoFlowControl);
//        if(serialport_->open(QIODevice::ReadWrite)){
//            ui->comboBox_name->setDisabled(1);
//            ui->comboBox_baut->setDisabled(1);
//            ui->comboBox_channel->setDisabled(1);
//            ui->comboBox_number->setDisabled(1);

//            ui->pushButton_open->setText(QStringLiteral("关闭串口"));
//            ui->plainTextEdit->appendPlainText(QStringLiteral("串口")+serialport_->portName()+QStringLiteral("已连接"));
//        }
//        else{
//            QMessageBox::critical(this, tr("Error"), serialport_->errorString());
//        }
    }
    else{
//        if (serialport_->isOpen()){
//            serialport_->close();
//            ui->plainTextEdit->appendPlainText(QStringLiteral("串口")+serialport_->portName()+QStringLiteral("已关闭"));
//        }
//        ui->pushButton_open->setText(QStringLiteral("打开串口"));
//        ui->comboBox_name->setDisabled(0);
//        ui->comboBox_baut->setDisabled(0);
//        ui->comboBox_channel->setDisabled(0);
//        ui->comboBox_number->setDisabled(0);
    }
}

void MainWindow::on_pushButton_connect_clicked()
{
    if(ui->pushButton_connect->text() == "Connect"){
        ui->pushButton_connect->setText("Disconnect");
        serial_open_state_ = true;
    }
    else{
        ui->pushButton_connect->setText("Connect");
        serial_open_state_ = false;
    }
}
