#-------------------------------------------------
#
# Project created by QtCreator 2020-10-12T11:08:44
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SRT

TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    dataprocess.cpp \
    csysfile.cpp \
    csysparam.cpp \
    serial_receive.cpp \
    serial_send.cpp \
    serial_send_receive.cpp \
    servo_protocol.cpp

HEADERS  += mainwindow.h \
    dataprocess.h \
    csysfile.h \
    csysparam.h \
    serial_receive.h \
    serial_send.h \
    serial_send_receive.h \
    servo_protocol.h \
    lcd_protocol.h \
    include/gaugecircle.h

FORMS    += mainwindow.ui

DISTFILES +=
INCLUDEPATH += include

RESOURCES += \
    pic.qrc
RC_ICONS = logo.ico

CONFIG(debug, debug|release){
LIBS += -L$$PWD/sdk/ -lqucd
} else {
LIBS += -L$$PWD/sdk/ -lquc
}

