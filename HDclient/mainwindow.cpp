#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(708, 425);
//    this->setStyleSheet("QPushButton{ background-color: rgb(0x1d, 0x1d, 0x1d); color: rgb(255, 255, 255);}");
    ui->pushButton_open->setStyleSheet(
                "QPushButton{background-color:white; color:black;   border-radius: 5px;  border: 2px; groove gray;border-style: outset;}"
                "QPushButton:hover{background-color:black; color: white;}"
                "QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }"
                );
    ui->pushButton_save->setStyleSheet(
                "QPushButton{background-color:white; color:black;   border-radius: 5px;  border: 2px; groove gray;border-style: outset;}"
                "QPushButton:hover{background-color:black; color: white;}"
                "QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }"
                );
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

    ui->textEdit->setStyleSheet(
                "QTextEdit {background-color:rgba(29, 29, 29, 100%); color:white;   border-radius: 5px;  border: 2px; groove gray;border-style: outset;}"
                );
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
    ui->pushButton_connect->setStyleSheet( "QPushButton {background-color:white; color:red;   border-radius: 5px;  border: 2px; groove gray;border-style: outset;}");

}

MainWindow::~MainWindow()
{
    delete ui;
}
