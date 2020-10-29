#ifndef MAINUI_H
#define MAINUI_H

#include <QDialog>
#include<csyscomm.h>
#include<csysfile.h>
#include<csysparam.h>
#include<QThread>
#include<QTime>
#include<QTranslator>
namespace Ui {
class MainUI;
}

//#pragma execution_character_set("utf-8")

union char2short
{
    char  c2[2];
    short s1;
};
struct servo_params{
    qint16 data[43];
};
#include<qmath.h>
#include <QVector>
//#include <qwt_plot.h>
//#include <qwt_plot_curve.h>
//#include <qwt_plot_magnifier.h>
//#include <qwt_plot_panner.h>
//#include <qwt_legend.h>
//#include <qwt_point_data.h>
//#include <qwt_symbol.h>
//#include<qwt_plot_grid.h>
//#include<qwt_plot_curve.h>
#include<QMutex>
class crcSend;
class crcTestSend;
class RefComm;
class CSendSlider;
#define  delyTime  1
class MainUI : public QDialog
{
    Q_OBJECT

public:
    explicit MainUI(QWidget *parent = 0);
    ~MainUI();

public:
    Ui::MainUI *ui;
public:
    friend  class   crcSend;
    friend  class   crcTestSend;
    virtual void    closeEvent(QCloseEvent *);
    virtual void    timerEvent(QTimerEvent *);
public:
    void    sleep(unsigned int msec);
public:
    //获得舵机ID
    char    GetID();
public:
    //翻译对象
    QTranslator              translator;
    //发送收到数据TRUE 发送 FALSE 收到
    bool                     m_bSendrecvData;
    //通信对象
    CSysComm                 m_SysComm;
    //系统参数
    CSysParam                m_SysParam;
    //循环运行
    crcSend                  *m_crcSend;
    //循环测试
    crcTestSend              *m_crcTestSend;
    //刷新窗口
    RefComm                  *m_RefComm;
    //刷新窗口
    CSendSlider              *m_CSendSlider;
    //发送次数
    int                      m_SendCount;
    //ID
    int                      m_TimerID;
    //刷新在Table
    bool                     m_RefTable;
    //标志控制
    bool                     m_SendMark;
    //当前串口号
    QString                  m_CurComm;
    //
    int                      m_iBytecount;
    //
    int                      m_iDecodeCount;
protected:
    //数据缓存
    QByteArray               m_outData;
protected:
    //设置提示信息
    void    SetInfo(QString str);
    //获得界面-常规舵机参数
    void    GetUI_NormSteeringEngineParam();
    //设置界面-常规舵机参数
    void    SetUI_NormSteeringEngineParam();

    //获得界面-串口舵机参数
    void    GetUI_COMMSteeringEngineParam();
    //设置界面-串口舵机参数
    void    SetUI_COMMSteeringEngineParam();

    //获得界面-舵机测试
    void    GetUI_SteeringEngineTest();
    //设置界面-舵机测试
    void    SetUI_SteeringEngineTest();

    //获得界面-串口舵机测试
    void    GetUI_COMMSteeringEngineTest();
    //设置界面-串口舵机测试
    void    SetUI_COMMSteeringEngineTest();

    //获得界面-附加信息
    void    GetUI_AdditionalVariable();
    //设置界面-附加信息
    void    SetUI_AdditionalVariable();
private:
    //数据分包
    void    SegPacket( QByteArray InStr , std::vector<QByteArray> &outStrs );
    //解析数据包
    void    DecodePacket(QByteArray InStr);
     //解析舵机反馈数据
    void    AnalyzeTheFeedbackDataOfSteeringGear(unsigned char IDByte1,short IDValue1,unsigned char IDByte2,short IDValue2);
public slots:
    //开始-测试
    void        OnStratTest();
    //停止-测试
    void        OnStopTest();
    //串口接收数据
    void        receiveInfo();
    //语言切换
    void        OnSwitchLang(int Index);
    //打开串口信息
    void        OnOpenCOMM();
    //打开文件
    void        OnOpenFileA();
    //保存文件
    void        OnSaveFileA();
    //常规舵机读取-ok
    void        OnNormRead();
    //常规舵机下载-ok
    void        OnNormWirte();
    //常规舵机复位-ok
    void        OnNormReStore();
    //串口舵机读取-ok
    void        OnCOMMRead();
    //串口舵机下载-ok
    void        OnCOMMWirte();
    //串口舵机复位-ok
    void        OnCOMMReStore();
    //上-ok
    void        OnUp();
    //中-ok
    void        OnCenter();
    //下-ok
    void        OnDown();
    //反馈-ok
    void        OnQuery();
    //滑动条-ok
    void        OnSlider(int  ivalue);
    //运行-ok
    void        OnRun();
    //停止-ok
    void        OnStop();
    //切换tab
    void        OnSwitch(int iTab);
    //清除数据
    void        OnClear();
    //s_S
    void        OnSlidDown();
    //s_e
    void        OnSlidUp();
public:
    //发送舵机位置速度
    void        OnSendPoistion( char  iID, short sPos,short  sSpeed );
    //常规舵机下发界面配置指令
    void        OnNormSendUI();
    //串口舵机下发界面配置指令
    void        OnCOMMSendUI();
public:
    //发送等待
    bool        Waiting();
    //刷新英文
    void        RefEnglish();
    //刷新中文
    void        RefChinese();
public:
    //刷新位置
    void        Refweizhi(int iv);
    //刷新温度
    void        Refwendu(int iv);
    //刷新电压
    void        Refdianya(int iv);
    //刷新电流
    void        Refdianliu(int  iv);
public:
    //中位指令
    void        CenterPosition( int  id );
    //掉电指令
    void        DownCommand( int  id );
public:
//     QwtPlotCurve curve_weizhi;
//     QwtPlotCurve curve_wendu;
//     QwtPlotCurve curve_dianya;
//     QwtPlotCurve curve_dianliu;
     QVector<double> y_weizhi,x_weizhi;
     QVector<double> y_wendu,x_wendu;
     QVector<double> y_dianya,x_dianya;
     QVector<double> y_dianliu,x_dianliu;
};
#endif // MAINUI_H
