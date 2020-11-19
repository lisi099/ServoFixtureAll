#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QtSerialPort>
#include <QTimer>
#include "csysfile.h"
#include "csysparam.h"

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

    void com_detect_timeout();

    void on_pushButton_open_clicked();

    void on_pushButton_connect_clicked();

    void  SetUI_NormSteeringEngineParam();

    void on_pushButton_save_clicked();

public:
    CSysParam  m_SysParam;



private:
    Ui::MainWindow *ui;
    QSerialPort *serialport_;
    QStringList init_usart_list_;
    bool serial_open_state_;

    QTimer  *com_detect_timer_;
};

#endif // MAINWINDOW_H
