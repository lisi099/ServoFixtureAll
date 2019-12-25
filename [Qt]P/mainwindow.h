#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QTextStream>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QWidget>
#include <QFont>
#include <QVector>
#include <QDebug>
#include <QColorDialog>

#include "serial_send_receive.h"

#include <QThread>
#include <inttypes.h>
#include <string>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void Update_process(void);
    uint8_t data_prepare(void);
    uint32_t CRC32Software( uint32_t *pData, uint16_t Length );

private slots:
    void System_Ticks();
    void Receieve_Bytes(void);
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QTimer         *timer;
    QByteArray      requestData;
    QByteArray      file_bin;
    QSerialPort    *my_serialport;

    serial_send_receive *data_send_receive;

    uint64_t        system_tick;
    uint8_t         system_state;
    uint8_t         system_package;

    uint8_t         update_send_receieve_finish;
    uint8_t         system_data[4];
    uint8_t         system_adress[3];
    uint32_t        adress;

    uint32_t        crc32_data[60000];
    uint16_t        crc32_length;
};




#endif // MAINWINDOW_H
