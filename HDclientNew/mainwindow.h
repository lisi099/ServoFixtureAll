#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QtSerialPort>
#include <QTimer>
#include "csysfile.h"
#include "csysparam.h"
#include "serial_send_receive.h"
#include "servo_protocol.h"
#include "lcd_protocol.h"
#include "QListWidget"

namespace Ui {
class MainWindow;
}

enum{
    IDLE = 0,
    READ_SERVO_DATA = 1,
    WRITE_SERVO_DATA,
    DEFAULT_SERVO_DATA,
    CONNECT_SERVO_DATA,
    DISCONECT_SERVO_DATA,

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void band_send(quint16 pwm, bool brod);

private slots:

    void receieve_bytes_update(void);

    void com_detect_timeout();

    void on_pushButton_open_clicked();

    void on_pushButton_connect_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_lcdfirmware_clicked();

    void on_pushButton_lcdupgrade_clicked();

    void System_Ticks();

    void lcd_Ticks();

    void on_pushButton_p1_clicked();

    void on_pushButton_p2_clicked();

    void on_pushButton_p3_clicked();

    void on_pushButton_p4_clicked();

    void on_pushButton_p5_clicked();

    void on_pushButton_p6_clicked();

    void on_pushButton_p7_clicked();

    void on_pushButton_p8_clicked();

    void on_pushButton_readData_clicked();

    void on_pushButton_writeData_clicked();

    void on_pushButton_Default_clicked();

    void items_callback(QListWidgetItem *item);

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
    QTimer  *timer_2_;

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

    servo_protocol *servoProtocl_;
    lcd_protocol *lcd_protocol_;

    int16_t operate_states_;
    quint64 ticks_;
    quint64 cmd_ticks_;
    quint64 time_out_set_;
    QByteArray lcd_data_;
    ui_data ui_data_;
};

#endif // MAINWINDOW_H
