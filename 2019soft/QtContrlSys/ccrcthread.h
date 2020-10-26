#ifndef CCRCTHREAD_H
#define CCRCTHREAD_H
#include<QThread>
class MainUI;

class CSendSlider:public QThread
{
public:
    CSendSlider(MainUI   *MainUI);
    void closeThread();
public:
    MainUI   *m_MainUI;
    int      m_iV;
protected:
    //运行
    virtual  void run();
private:
    volatile bool isStop;
};

class crcSend:public QThread
{
public:
    crcSend(MainUI   *MainUI);
    void closeThread();
public:
    MainUI   *m_MainUI;
protected:
    //运行
    virtual  void run();
private:
    volatile bool isStop;
};
//刷新串口
class RefComm:public QThread
{
public:
    RefComm(MainUI   *MainUI);
    void closeThread();
public:
    MainUI   *m_MainUI;
protected:
    //运行
    virtual  void run();
private:
    volatile bool isStop;
};
class crcTestSend:public QThread
{
public:
    crcTestSend(MainUI   *MainUI);
    void closeThread();
public:
    MainUI   *m_MainUI;
protected:
    //运行
    virtual  void run();
private:
    volatile bool isStop;
};
#endif // CCRCTHREAD_H
