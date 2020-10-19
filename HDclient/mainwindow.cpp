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
    QPushButton *prt[4];
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

    prt[0] = ui->pushButton_7;
    prt[1] = ui->pushButton_8;
    prt[2] = ui->pushButton_9;
    for(int i=0; i<3; i++){
        prt[i]->setStyleSheet(
                        "QPushButton {background-color:black; color:white;   border-radius: 5px;  border: 2px; groove gray;border-style: outset;}"
                        );
    }

    QLabel *label_prt[4];
    label_prt[0] = ui->label_2;
    label_prt[1] = ui->label_3;
    label_prt[2] = ui->label_4;
    label_prt[3] = ui->label_5;
    for(int i=0; i<4; i++){
        label_prt[i]->setStyleSheet(
                        "QLabel {background-color:white; color: black;}"
                        );
    }



}

MainWindow::~MainWindow()
{
    delete ui;
}
