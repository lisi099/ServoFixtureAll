#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QtSerialPort>
#include <QTimer>
#include "csysfile.h"
#include "csysparam.h"
#include "serial_send_receive.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();




private slots:
    void receieve_bytes(void);

    void receieve_bytes_update(void);

    void com_detect_timeout();

    void on_pushButton_open_clicked();

    void on_pushButton_connect_clicked();

    void  SetUI_NormSteeringEngineParam();

    void on_pushButton_save_clicked();

    void on_pushButton_lcdfirmware_clicked();

    void on_pushButton_lcdupgrade_clicked();

    void System_Ticks();
public:
    CSysParam  m_SysParam;
    uint32_t CRC32Software( uint32_t *pData, uint16_t Length );
    void Update_process(void);
    uint8_t data_prepare(void);


private:
    Ui::MainWindow *ui;
    QSerialPort *serialport_;
    QStringList init_usart_list_;

    QTimer  *com_detect_timer_;
    QTimer  *timer_1_;

    QByteArray      file_bin;
    QByteArray      requestData;
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
