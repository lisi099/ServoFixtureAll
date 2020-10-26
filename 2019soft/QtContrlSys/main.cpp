#include "mainui.h"
#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //QTranslator *translator = new QTranslator;
    //translator->load("chinese.qm");
    //a.installTranslator(translator);

    MainUI w;
    w.show();

    return a.exec();
}
