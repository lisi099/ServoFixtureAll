#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QtSerialPort>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void receieve_bytes(void);


private slots:

    void com_detect_timeout();

    void on_pushButton_open_clicked();

    void on_pushButton_connect_clicked();



private:
    Ui::MainWindow *ui;
    QSerialPort *serialport_;
    QStringList init_usart_list_;
    bool serial_open_state_;

    QTimer  *com_detect_timer_;
};

#endif // MAINWINDOW_H
