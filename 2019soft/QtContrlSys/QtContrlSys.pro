#-------------------------------------------------
#
# Project created by QtCreator 2019-10-17T09:11:40
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtContrlSys
TEMPLATE = app


SOURCES += main.cpp\
        mainui.cpp \
    csyscomm.cpp \
    csysfile.cpp \
    csysparam.cpp \
    ChildCommand.cpp \
    CDecodePacket.cpp \
    ccrcthread.cpp

HEADERS  += mainui.h \
    csyscomm.h \
    csysfile.h \
    csysparam.h \
    CSysDefine.h \
    ccrcthread.h

FORMS    += mainui.ui
TRANSLATIONS = english_EN_2.ts

#LIBS += -L”C:\Users\ubtech\Desktop\2019soft\lib” -lqwt
#INCLUDEPATH += "C:\Users\ubtech\Desktop\2019soft\qwt-6.1.2\qwt-6.1.2\src"
#include (C:\Users\ubtech\Desktop\2019soft\qwt-6.1.2\qwt-6.1.2\qwt.prf)
