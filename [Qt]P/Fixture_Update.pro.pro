#-------------------------------------------------
#
# Project created by QtCreator 2017-04-30T20:09:22
#
#-------------------------------------------------
QT       += core gui
QT       += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IMU_Update
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serial_send.cpp \
    serial_receive.cpp \
    serial_send_receive.cpp

HEADERS  += mainwindow.h \
    serial_send.h \
    serial_receive.h \
    serial_send_receive.h

FORMS    += mainwindow.ui

RC_FILE =  IMU_Update.rc
