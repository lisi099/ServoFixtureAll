#include "mainui.h"
#include "ui_mainui.h"
#include "csysfile.h"
#include<QMessageBox>
#include<QFileDialog>
#include<QTime>
#include<QCoreApplication>
#include"CSysDefine.h"
#include<QDebug>
//#include<qwt_plot_zoomer.h>
#include<qscrollbar.h>
#include"ccrcthread.h"
void    MainUI::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 20);
}
//获得舵机ID
char    MainUI::GetID()
{
    QString  str = ui->comboBox_4->currentText();
    QByteArray  iv = str.toLatin1();
    int  iValue = 0;
    for( int i = 0 ; i < iv.size();i++ )
    {
        iValue += iv[i]*(i+1);
    }

    char  cv = iValue%255;
    return cv;
}
void  MainUI::timerEvent(QTimerEvent *event)
{
    qDebug() << "Timer ID:" << event->timerId();

    if( event->timerId() == m_TimerID )
    {
        CCOMMSteeringEngineTest  iParam = m_SysParam.m_COMMSteeringEngineTest;
        short  sNumber = iParam.m_NumberOfRuns.GetValue();

        if( m_SendCount == sNumber )
        {
            ui->pushButton_16->setEnabled(true);
            ui->pushButton_17->setEnabled(false);
            killTimer(m_TimerID);
        }
    }
}
void  MainUI::closeEvent(QCloseEvent *)
{
    m_RefComm->closeThread();
    m_CSendSlider->closeThread();
    int r = QMessageBox::question(this, QStringLiteral("提问"),QStringLiteral("是否保存参数...?"),QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok);
    if (QMessageBox::Ok == r)
    {
       OnSaveFileA();
    }
}
MainUI::MainUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainUI)
{
    ui->setupUi(this);

    m_CurComm = "";
    QString pathX=QDir::currentPath();
    pathX = pathX + QStringLiteral("/舵机控制系统");
    this->setWindowTitle(pathX);
    /*
    //QPalette  palette(this->palette());
    //palette.setColor(QPalette::Background,Qt::black);
    //this->setPalette(palette);
    */
    Qt::WindowFlags  flags = Qt::Dialog;
    flags |= Qt::WindowMinMaxButtonsHint;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    m_SendMark = false;
    QString path=QDir::currentPath();
    QString str = path+"/config.bat" ;
    QFile   file(str);
    bool bhr = file.open(QIODevice::ReadOnly | QIODevice::Text);//|QIODevice::Text
    if( bhr )
    {
        QByteArray  t = file.readAll();
        QString str = QString(t);
        file.close();
        qDebug()<<str;
        int iv = str.toInt();
        if( iv == 1124 )
           this->setFixedSize(iv,680);
        else
           this->setFixedSize(740,680);
    }
    else
    {
        this->setFixedSize(740,680);
    }

    m_TimerID = 0;
    m_bSendrecvData = false;
    m_RefTable = false;

    //刷新界面
    //设置界面-常规舵机参数
    SetUI_NormSteeringEngineParam();
    SetUI_COMMSteeringEngineParam();
    SetUI_SteeringEngineTest();
    SetUI_COMMSteeringEngineTest();
    SetUI_AdditionalVariable();

    //刷新提示
    //m_nTimerID = this->startTimer(1000);

    //循环运行
    m_crcSend = new crcSend(this);
    m_crcTestSend = new crcTestSend(this);
    m_CSendSlider = new CSendSlider(this);
    m_RefComm = new RefComm(this);

    sleep(1000);
    m_RefComm->start();
    m_CSendSlider->start();

    //波特率
    ui->comboBox_2->addItem(QString("1200"));
    ui->comboBox_2->addItem(QString("2400"));
    ui->comboBox_2->addItem(QString("4800"));
    ui->comboBox_2->addItem(QString("9600"));
    ui->comboBox_2->addItem(QString("19200"));
    ui->comboBox_2->addItem(QString("38400"));
    ui->comboBox_2->addItem(QString("57600"));
    ui->comboBox_2->addItem(QString("115200"));
    ui->comboBox_2->setCurrentIndex(7);

    //数据位
    ui->comboBox_3->addItem(QString("5"));
    ui->comboBox_3->addItem(QString("6"));
    ui->comboBox_3->addItem(QString("7"));
    ui->comboBox_3->addItem(QString("8"));
    ui->comboBox_3->setCurrentIndex(3);

    ui->comboBox_5->addItem(QStringLiteral("中文"));
    ui->comboBox_5->addItem(QStringLiteral("英文"));
    ui->comboBox_5->setCurrentIndex(0);

    //信号和槽绑定
    QObject::connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(OnOpenCOMM()));
    QObject::connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(OnOpenFileA()));
    QObject::connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(OnSaveFileA()));
    //常规舵机
    QObject::connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(OnNormRead()));
    QObject::connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(OnNormWirte()));
    QObject::connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(OnNormReStore()));
    //串口舵机
    QObject::connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(OnCOMMRead()));
    QObject::connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(OnCOMMWirte()));
    QObject::connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(OnCOMMReStore()));
    //舵机测试
    QObject::connect(ui->pushButton_18, SIGNAL(clicked()), this, SLOT(OnUp()));
    QObject::connect(ui->pushButton_19, SIGNAL(clicked()), this, SLOT(OnCenter()));
    QObject::connect(ui->pushButton_20, SIGNAL(clicked()), this, SLOT(OnDown()));
    QObject::connect(ui->pushButton_21, SIGNAL(clicked()), this, SLOT(OnQuery()));
    QObject::connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(OnSlider(int)));

    QObject::connect(ui->horizontalSlider, SIGNAL(sliderPressed()), this, SLOT(OnSlidDown()));
    QObject::connect(ui->horizontalSlider, SIGNAL(sliderReleased()), this, SLOT(OnSlidUp()));

    //串口舵机测试
    QObject::connect(ui->pushButton_16, SIGNAL(clicked()), this, SLOT(OnRun()));
    QObject::connect(ui->pushButton_17, SIGNAL(clicked()), this, SLOT(OnStop()));


    //串口舵机测试
    QObject::connect(ui->pushButton_22, SIGNAL(clicked()), this, SLOT(OnStratTest()));
    QObject::connect(ui->pushButton_23, SIGNAL(clicked()), this, SLOT(OnStopTest()));

    QObject::connect(ui->pushButton_10, SIGNAL(clicked()), this, SLOT(OnClear()));

    QObject::connect(ui->tabWidget,SIGNAL(currentChanged(int)),this, SLOT(OnSwitch(int)));
    QObject::connect(ui->comboBox_5,SIGNAL(currentIndexChanged(int)),this, SLOT(OnSwitchLang(int)));

    ui->tableWidget->setColumnWidth(0,200);
    ui->tableWidget->setColumnWidth(1,480);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);


//    ui->qwtPlot->setAxisScale(QwtPlot::xBottom, 0, 10000, 1000);
//    ui->qwtPlot->setAxisScale(QwtPlot::yLeft, 0, 100, 10);


//    QwtPlotZoomer *zoomer = new QwtPlotZoomer( ui->qwtPlot->canvas() );                     //选择图形局部放大
/*    zoomer->setRubberBandPen( QColor( Qt::blue ) );                          //勾选扩大区域边界颜色
    zoomer->setMousePattern( QwtEventPattern::MouseSelect2, Qt::RightButton, Qt::ControlModifier );//ctrl+右键==回复到原始状态
    zoomer->setMousePattern( QwtEventPattern::MouseSelect3, Qt::RightButton );  */     //右键==恢复到上一次扩大之前的状态

//    QwtPlotPanner *panner = new QwtPlotPanner( ui->qwtPlot->canvas() );              //默认的左键移动功能
//    panner->setMouseButton( Qt::LeftButton ,Qt::ControlModifier);                    //设置哪个按钮移动画布  如果不设置(注册掉当前行)按钮默认为左键为移动

    //QwtPlotMagnifier *magnifier = new QwtPlotMagnifier( ui->qwtPlot->canvas() );     //默认的滑轮及右键缩放功能  图形的整体缩放
    //magnifier->setMouseButton(Qt::LeftButton);                                       //设置哪个按钮与滑轮为缩放画布  如果不设置(注册掉当前行)按钮默认为滑轮以及右键为缩放

    //加入网格
//    QwtPlotGrid* grid = new QwtPlotGrid();
//    grid->setPen(QColor(222,222,222),1);
//    grid->attach(ui->qwtPlot);
    m_SendCount = 0;

    ui->lineEdit_23->hide();
    ui->lineEdit_28->hide();
    ui->lineEdit_25->hide();

    ui->label_36->hide();
    ui->label_34->hide();
    ui->label_39->hide();

    ui->label_36->hide();
    ui->label_34->hide();
    ui->label_39->hide();

    ui->label_47->setText(QStringLiteral("力度"));
    ui->label_75->setText(QStringLiteral("力度"));

    ui->label_73->setText(QStringLiteral("刚度"));
    ui->label_74->setText(QStringLiteral("刚度"));

    ui->label_72->setText(QStringLiteral("刹车"));
    ui->label_76->setText(QStringLiteral("刹车"));


}

MainUI::~MainUI()
{
    delete ui;
}
//清除数据
void    MainUI::OnClear()
{
    y_weizhi.clear();
    x_weizhi.clear();
    y_wendu.clear();
    x_wendu.clear();
    y_dianya.clear();
    x_dianya.clear();
    y_dianliu.clear();
    x_dianliu.clear();

//    curve_dianya.setSamples(x_dianya,y_dianya);
//    curve_wendu.setSamples(x_wendu,y_wendu);
//    curve_dianliu.setSamples(x_dianliu,y_dianliu);
//    curve_weizhi.setSamples(x_weizhi,y_weizhi);
//    ui->qwtPlot->replot();
}
//设置提示信息
void    MainUI::SetInfo(QString str)
{
    QDateTime itime  = QDateTime::currentDateTime();
    QString  strTime = itime.toString("yyyy-MM-dd hh:mm:ss:zzz");

    int  index = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(index + 1);


    QTableWidgetItem *item1 = new QTableWidgetItem(strTime);   //加入图标和字符串
    ui->tableWidget->setItem(index, 0, item1);

    QTableWidgetItem *item2 = new QTableWidgetItem(str);   //加入图标和字符串
    ui->tableWidget->setItem(index, 1, item2);

    QScrollBar  *ph = ui->tableWidget->verticalScrollBar();
    ph->setValue(index++);
}
//获得界面-常规舵机参数
void    MainUI::GetUI_NormSteeringEngineParam()
{
    short  fvalue = ui->lineEdit_14->text().toInt();
    m_SysParam.m_NormSteeringEngineParam.m_UpperPulseWidthlimit.SetValue(fvalue);

    fvalue = ui->lineEdit_15->text().toInt();
    m_SysParam.m_NormSteeringEngineParam.m_MiddlePulseWidthlimit.SetValue(fvalue);

    fvalue = ui->lineEdit_16->text().toInt();
    m_SysParam.m_NormSteeringEngineParam.m_DownPulseWidthlimit.SetValue(fvalue);

    fvalue = ui->lineEdit_17->text().toInt();
    m_SysParam.m_NormSteeringEngineParam.m_StartingVoltage.SetValue(fvalue);

    fvalue = ui->lineEdit_18->text().toInt();
    m_SysParam.m_NormSteeringEngineParam.m_DrivingFrequency.SetValue(fvalue);

    fvalue = ui->lineEdit_19->text().toInt();
    m_SysParam.m_NormSteeringEngineParam.m_DeadZone.SetValue(fvalue);

    //
    Qt::CheckState  bState = ui->checkBox->checkState();
    if( bState == Qt::Checked )
        fvalue = 1;
    else
        fvalue = 0;
    m_SysParam.m_NormSteeringEngineParam.m_Self_lockingSetting.SetValue(fvalue);

    bState = ui->checkBox_2->checkState();
    if( bState == Qt::Checked )
        fvalue = 100;
    else
        fvalue = 0;
    m_SysParam.m_NormSteeringEngineParam.m_Locked_rotorProtection.SetValue(fvalue);

    bState = ui->checkBox_3->checkState();
    if( bState == Qt::Checked )
        fvalue = 1;
    else
        fvalue = 0;
    m_SysParam.m_NormSteeringEngineParam.m_SignalReset.SetValue(fvalue);

    bState = ui->checkBox_4->checkState();
    if( bState == Qt::Checked )
        fvalue = 1;
    else
        fvalue = 0;
    m_SysParam.m_NormSteeringEngineParam.m_SteeringGearDirection.SetValue(fvalue);

    fvalue = ui->lineEdit_20->text().toInt();
    m_SysParam.m_NormSteeringEngineParam.m_ProtectionTime.SetValue(fvalue);

    fvalue = ui->lineEdit_21->text().toInt();
    m_SysParam.m_NormSteeringEngineParam.m_ProtectionOutput.SetValue(fvalue);

    fvalue = ui->lineEdit_30->text().toInt();
    m_SysParam.m_NormSteeringEngineParam.m_UpperLimitOfAngle.SetValue(fvalue);


    fvalue = ui->lineEdit_31->text().toInt();
    m_SysParam.m_NormSteeringEngineParam.m_AngularMidpoint.SetValue(fvalue);

    fvalue = ui->lineEdit_32->text().toInt();
    m_SysParam.m_NormSteeringEngineParam.m_LowerAngleLimit.SetValue(fvalue);

    fvalue = ui->lineEdit_33->text().toInt();
    m_SysParam.m_NormSteeringEngineParam.m_MaximumOutput.SetValue(fvalue);

    fvalue = ui->lineEdit_35->text().toInt();
    m_SysParam.m_NormSteeringEngineParam.m_FrequencySetting.SetValue(fvalue);

    fvalue = ui->lineEdit_99->text().toInt();
    m_SysParam.m_SysLoseParam.servo_position_pid_parm_p_set.SetValue(fvalue);

    fvalue = ui->lineEdit_101->text().toInt();
    m_SysParam.m_SysLoseParam.servo_speed_pid_parm_p_set.SetValue(fvalue);

    fvalue = ui->lineEdit_100->text().toInt();
    m_SysParam.m_SysLoseParam.servo_speed_run_sample_k_set.SetValue(fvalue);

}
//设置界面-常规舵机参数
void    MainUI::SetUI_NormSteeringEngineParam()
{
    QString str;
    short fvalue = m_SysParam.m_NormSteeringEngineParam.m_UpperPulseWidthlimit.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_14->setText(str);
    qDebug() <<"----" << fvalue;

    fvalue = m_SysParam.m_NormSteeringEngineParam.m_MiddlePulseWidthlimit.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_15->setText(str);

    fvalue = m_SysParam.m_NormSteeringEngineParam.m_DownPulseWidthlimit.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_16->setText(str);

    fvalue = m_SysParam.m_NormSteeringEngineParam.m_StartingVoltage.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_17->setText(str);

    fvalue = m_SysParam.m_NormSteeringEngineParam.m_DrivingFrequency.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_18->setText(str);

    fvalue = m_SysParam.m_NormSteeringEngineParam.m_DeadZone.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_19->setText(str);

    //
    fvalue = m_SysParam.m_NormSteeringEngineParam.m_Self_lockingSetting.GetValue();
    if(fvalue != 0)
        ui->checkBox->setCheckState(Qt::Checked);
    else
        ui->checkBox->setCheckState(Qt::Unchecked);

    fvalue = m_SysParam.m_NormSteeringEngineParam.m_Locked_rotorProtection.GetValue();
    if(fvalue != 0)
        ui->checkBox_2->setCheckState(Qt::Checked);
    else
        ui->checkBox_2->setCheckState(Qt::Unchecked);

    fvalue = m_SysParam.m_NormSteeringEngineParam.m_SignalReset.GetValue();
    if(fvalue != 0)
        ui->checkBox_3->setCheckState(Qt::Checked);
    else
        ui->checkBox_3->setCheckState(Qt::Unchecked);

    fvalue = m_SysParam.m_NormSteeringEngineParam.m_SteeringGearDirection.GetValue();
    if(fvalue != 0)
        ui->checkBox_4->setCheckState(Qt::Checked);
    else
        ui->checkBox_4->setCheckState(Qt::Unchecked);

    fvalue = m_SysParam.m_NormSteeringEngineParam.m_ProtectionTime.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_20->setText(str);

    fvalue = m_SysParam.m_NormSteeringEngineParam.m_ProtectionOutput.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_21->setText(str);

    fvalue = m_SysParam.m_NormSteeringEngineParam.m_UpperLimitOfAngle.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_30->setText(str);

    fvalue = m_SysParam.m_NormSteeringEngineParam.m_AngularMidpoint.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_31->setText(str);

    fvalue = m_SysParam.m_NormSteeringEngineParam.m_LowerAngleLimit.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_32->setText(str);

    fvalue = m_SysParam.m_NormSteeringEngineParam.m_MaximumOutput.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_33->setText(str);

    fvalue = m_SysParam.m_NormSteeringEngineParam.m_FrequencySetting.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_35->setText(str);

    fvalue =m_SysParam.m_SysLoseParam.servo_position_pid_parm_p_set.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_99->setText(str);

    fvalue =m_SysParam.m_SysLoseParam.servo_speed_pid_parm_p_set.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_101->setText(str);

    fvalue =m_SysParam.m_SysLoseParam.servo_speed_run_sample_k_set.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_100->setText(str);
}
//获得界面-串口舵机参数
void    MainUI::GetUI_COMMSteeringEngineParam()
{
    short  fvalue = ui->lineEdit_36->text().toInt();
    m_SysParam.m_COMMSteeringEngineParam.m_UpperPulseWidthlimit.SetValue(fvalue);

    fvalue = ui->lineEdit_46->text().toInt();
    m_SysParam.m_COMMSteeringEngineParam.m_MiddlePulseWidthlimit.SetValue(fvalue);

    fvalue = ui->lineEdit_40->text().toInt();
    m_SysParam.m_COMMSteeringEngineParam.m_DownPulseWidthlimit.SetValue(fvalue);

    fvalue = ui->lineEdit_45->text().toInt();
    m_SysParam.m_COMMSteeringEngineParam.m_StartingVoltage.SetValue(fvalue);

    fvalue = ui->lineEdit_41->text().toInt();
    m_SysParam.m_COMMSteeringEngineParam.m_DrivingFrequency.SetValue(fvalue);

    fvalue = ui->lineEdit_44->text().toInt();
    m_SysParam.m_COMMSteeringEngineParam.m_DeadZone.SetValue(fvalue);

    //
    Qt::CheckState  bState = ui->checkBox_9->checkState();
    if( bState == Qt::Checked )
        fvalue = 1;
    else
        fvalue = 0;
    m_SysParam.m_COMMSteeringEngineParam.m_Self_lockingSetting.SetValue(fvalue);

    bState = ui->checkBox_12->checkState();
    if( bState == Qt::Checked )
        fvalue = 100;
    else
        fvalue = 0;
    m_SysParam.m_COMMSteeringEngineParam.m_Locked_rotorProtection.SetValue(fvalue);

    bState = ui->checkBox_11->checkState();
    if( bState == Qt::Checked )
        fvalue = 1;
    else
        fvalue = 0;
    m_SysParam.m_COMMSteeringEngineParam.m_SignalReset.SetValue(fvalue);

    bState = ui->checkBox_10->checkState();
    if( bState == Qt::Checked )
        fvalue = 1;
    else
        fvalue = 0;
    m_SysParam.m_COMMSteeringEngineParam.m_SteeringGearDirection.SetValue(fvalue);

    fvalue = ui->lineEdit_49->text().toInt();
    m_SysParam.m_COMMSteeringEngineParam.m_ProtectionTime.SetValue(fvalue);

    fvalue = ui->lineEdit_48->text().toInt();
    m_SysParam.m_COMMSteeringEngineParam.m_ProtectionOutput.SetValue(fvalue);

    fvalue = ui->lineEdit_43->text().toInt();
    m_SysParam.m_COMMSteeringEngineParam.m_UpperLimitOfAngle.SetValue(fvalue);


    fvalue = ui->lineEdit_42->text().toInt();
    m_SysParam.m_COMMSteeringEngineParam.m_AngularMidpoint.SetValue(fvalue);

    fvalue = ui->lineEdit_39->text().toInt();
    m_SysParam.m_COMMSteeringEngineParam.m_LowerAngleLimit.SetValue(fvalue);

    fvalue = ui->lineEdit_47->text().toInt();
    m_SysParam.m_COMMSteeringEngineParam.m_MaximumOutput.SetValue(fvalue);

    fvalue = ui->lineEdit_37->text().toInt();
    m_SysParam.m_COMMSteeringEngineParam.m_FrequencySetting.SetValue(fvalue);

    fvalue = ui->lineEdit_50->text().toInt();
    m_SysParam.m_COMMSteeringEngineParam.m_CurID.SetValue(fvalue);

    fvalue = ui->lineEdit_51->text().toInt();
    m_SysParam.m_COMMSteeringEngineParam.m_NewID.SetValue(fvalue);

    fvalue = ui->lineEdit_104->text().toInt();
    m_SysParam.m_SysLoseParam.servo_position_pid_parm_p_set.SetValue(fvalue);

    fvalue = ui->lineEdit_102->text().toInt();
    m_SysParam.m_SysLoseParam.servo_speed_pid_parm_p_set.SetValue(fvalue);

    fvalue = ui->lineEdit_103->text().toInt();
    m_SysParam.m_SysLoseParam.servo_speed_run_sample_k_set.SetValue(fvalue);

}
//设置界面-串口舵机参数
void    MainUI::SetUI_COMMSteeringEngineParam()
{
    QString str;
    short fvalue =m_SysParam.m_COMMSteeringEngineParam.m_UpperPulseWidthlimit.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_36->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineParam.m_MiddlePulseWidthlimit.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_46->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineParam.m_DownPulseWidthlimit.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_40->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineParam.m_StartingVoltage.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_45->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineParam.m_DrivingFrequency.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_41->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineParam.m_DeadZone.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_44->setText(str);

    //
    fvalue =m_SysParam.m_COMMSteeringEngineParam.m_Self_lockingSetting.GetValue();
    if( fvalue != 0 )
        ui->checkBox_9->setCheckState(Qt::Checked);
    else
        ui->checkBox_9->setCheckState(Qt::Unchecked);

    fvalue =m_SysParam.m_COMMSteeringEngineParam.m_Locked_rotorProtection.GetValue();
    if( fvalue != 0 )
        ui->checkBox_12->setCheckState(Qt::Checked);
    else
        ui->checkBox_12->setCheckState(Qt::Unchecked);

    fvalue = m_SysParam.m_COMMSteeringEngineParam.m_SignalReset.GetValue();
    if( fvalue != 0 )
        ui->checkBox_11->setCheckState(Qt::Checked);
    else
        ui->checkBox_11->setCheckState(Qt::Unchecked);

    fvalue =m_SysParam.m_COMMSteeringEngineParam.m_SteeringGearDirection.GetValue();
    if( fvalue != 0 )
        ui->checkBox_10->setCheckState(Qt::Checked);
    else
        ui->checkBox_10->setCheckState(Qt::Unchecked);

    fvalue = m_SysParam.m_COMMSteeringEngineParam.m_ProtectionTime.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_49->setText(str);

    fvalue = m_SysParam.m_COMMSteeringEngineParam.m_ProtectionOutput.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_48->setText(str);

    fvalue = m_SysParam.m_COMMSteeringEngineParam.m_UpperLimitOfAngle.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_43->setText(str);

    fvalue = m_SysParam.m_COMMSteeringEngineParam.m_AngularMidpoint.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_42->setText(str);

    fvalue = m_SysParam.m_COMMSteeringEngineParam.m_LowerAngleLimit.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_39->setText(str);

    fvalue = m_SysParam.m_COMMSteeringEngineParam.m_MaximumOutput.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_47->setText(str);

    fvalue = m_SysParam.m_COMMSteeringEngineParam.m_FrequencySetting.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_37->setText(str);

    fvalue = m_SysParam.m_COMMSteeringEngineParam.m_CurID.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_50->setText(str);


    fvalue = m_SysParam.m_COMMSteeringEngineParam.m_NewID.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_51->setText(str);

    fvalue =m_SysParam.m_SysLoseParam.servo_position_pid_parm_p_set.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_104->setText(str);

    fvalue =m_SysParam.m_SysLoseParam.servo_speed_pid_parm_p_set.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_102->setText(str);

    fvalue =m_SysParam.m_SysLoseParam.servo_speed_run_sample_k_set.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_103->setText(str);
}
//获得界面-舵机测试
void    MainUI::GetUI_SteeringEngineTest()
{
    short fvalue = ui->lineEdit_86->text().toInt();
    m_SysParam.m_NormSteeringEngineTest.m_Value.SetValue(fvalue);

    fvalue = ui->lineEdit_87->text().toInt();
    m_SysParam.m_NormSteeringEngineTest.m_MaxValue.SetValue(fvalue);


    fvalue = ui->lineEdit_88->text().toInt();
    m_SysParam.m_NormSteeringEngineTest.m_MiddleValue.SetValue(fvalue);

    fvalue = ui->lineEdit_89->text().toInt();
    m_SysParam.m_NormSteeringEngineTest.m_MinValue.SetValue(fvalue);


    fvalue = ui->lineEdit_90->text().toInt();
    m_SysParam.m_NormSteeringEngineTest.m_Position.SetValue(fvalue);


    fvalue = ui->lineEdit_93->text().toInt();
    m_SysParam.m_NormSteeringEngineTest.m_Temperature.SetValue(fvalue);


    fvalue = ui->lineEdit_91->text().toInt();
    m_SysParam.m_NormSteeringEngineTest.m_Voltage.SetValue(fvalue);


    fvalue = ui->lineEdit_92->text().toInt();
    m_SysParam.m_NormSteeringEngineTest.m_ElectricWave.SetValue(fvalue);
}
//设置界面-舵机测试
void    MainUI::SetUI_SteeringEngineTest()
{
    QString  str;
    short fvalue =m_SysParam.m_NormSteeringEngineTest.m_Value.GetValue();
    //str.sprintf("%d",fvalue);
    //ui->lineEdit_86->setText(str);
    //ui->horizontalSlider->setValue(fvalue);

    fvalue =m_SysParam.m_NormSteeringEngineTest.m_MaxValue.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_87->setText(str);

    fvalue =m_SysParam.m_NormSteeringEngineTest.m_MiddleValue.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_88->setText(str);

    fvalue =m_SysParam.m_NormSteeringEngineTest.m_MinValue.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_89->setText(str);

    fvalue =m_SysParam.m_NormSteeringEngineTest.m_Position.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_90->setText(str);

    fvalue =m_SysParam.m_NormSteeringEngineTest.m_Temperature.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_93->setText(str);

    fvalue =m_SysParam.m_NormSteeringEngineTest.m_Voltage.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_91->setText(str);

    fvalue =m_SysParam.m_NormSteeringEngineTest.m_ElectricWave.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_92->setText(str);
}

//获得界面-串口舵机测试
void    MainUI::GetUI_COMMSteeringEngineTest()
{
   short fvalue = ui->lineEdit_52->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_ID[0].SetValue(fvalue);

   fvalue = ui->lineEdit_53->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_ID[1].SetValue(fvalue);

   fvalue = ui->lineEdit_54->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_ID[2].SetValue(fvalue);

   fvalue = ui->lineEdit_55->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_ID[3].SetValue(fvalue);

   fvalue = ui->lineEdit_56->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_ID[4].SetValue(fvalue);

   fvalue = ui->lineEdit_57->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_ID[5].SetValue(fvalue);

   fvalue = ui->lineEdit_58->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_ID[6].SetValue(fvalue);

   fvalue = ui->lineEdit_59->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_ID[7].SetValue(fvalue);


   ///
   fvalue = ui->lineEdit_60->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_Position1[0].SetValue(fvalue);

   fvalue = ui->lineEdit_61->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_Position1[1].SetValue(fvalue);

   fvalue = ui->lineEdit_62->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_Position1[2].SetValue(fvalue);

   fvalue = ui->lineEdit_63->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_Position1[3].SetValue(fvalue);

   fvalue = ui->lineEdit_64->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_Position1[4].SetValue(fvalue);

   fvalue = ui->lineEdit_65->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_Position1[5].SetValue(fvalue);

   fvalue = ui->lineEdit_66->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_Position1[6].SetValue(fvalue);

   fvalue = ui->lineEdit_67->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_Position1[7].SetValue(fvalue);

   ///
   fvalue = ui->lineEdit_68->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_Position2[0].SetValue(fvalue);

   fvalue = ui->lineEdit_69->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_Position2[1].SetValue(fvalue);

   fvalue = ui->lineEdit_70->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_Position2[2].SetValue(fvalue);

   fvalue = ui->lineEdit_71->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_Position2[3].SetValue(fvalue);

   fvalue = ui->lineEdit_72->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_Position2[4].SetValue(fvalue);

   fvalue = ui->lineEdit_73->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_Position2[5].SetValue(fvalue);

   fvalue = ui->lineEdit_74->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_Position2[6].SetValue(fvalue);

   fvalue = ui->lineEdit_75->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_Position2[7].SetValue(fvalue);

   ///
   fvalue = ui->lineEdit_76->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_Speed[0].SetValue(fvalue);

   fvalue = ui->lineEdit_77->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_Speed[1].SetValue(fvalue);

   fvalue = ui->lineEdit_78->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_Speed[2].SetValue(fvalue);

   fvalue = ui->lineEdit_79->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_Speed[3].SetValue(fvalue);

   fvalue = ui->lineEdit_80->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_Speed[4].SetValue(fvalue);

   fvalue = ui->lineEdit_81->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_Speed[5].SetValue(fvalue);

   fvalue = ui->lineEdit_82->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_Speed[6].SetValue(fvalue);

   fvalue = ui->lineEdit_83->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_Speed[7].SetValue(fvalue);


   fvalue = ui->lineEdit_84->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_StopTime.SetValue(fvalue);

   fvalue = ui->lineEdit_85->text().toInt();
   m_SysParam.m_COMMSteeringEngineTest.m_NumberOfRuns.SetValue(fvalue);
}
//设置界面-串口舵机测试
void    MainUI::SetUI_COMMSteeringEngineTest()
{
    QString  str;
    short fvalue =m_SysParam.m_COMMSteeringEngineTest.m_ID[0].GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_52->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineTest.m_ID[1].GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_53->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineTest.m_ID[2].GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_54->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineTest.m_ID[3].GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_55->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineTest.m_ID[4].GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_56->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineTest.m_ID[5].GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_57->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineTest.m_ID[6].GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_58->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineTest.m_ID[7].GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_59->setText(str);


    ///
    fvalue =m_SysParam.m_COMMSteeringEngineTest.m_Position1[0].GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_60->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineTest.m_Position1[1].GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_61->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineTest.m_Position1[2].GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_62->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineTest.m_Position1[3].GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_63->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineTest.m_Position1[4].GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_64->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineTest.m_Position1[5].GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_65->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineTest.m_Position1[6].GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_66->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineTest.m_Position1[7].GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_67->setText(str);

    ///
    fvalue = m_SysParam.m_COMMSteeringEngineTest.m_Position2[0].GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_68->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineTest.m_Position2[1].GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_69->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineTest.m_Position2[2].GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_70->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineTest.m_Position2[3].GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_71->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineTest.m_Position2[4].GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_72->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineTest.m_Position2[5].GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_73->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineTest.m_Position2[6].GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_74->setText(str);

    fvalue = m_SysParam.m_COMMSteeringEngineTest.m_Position2[7].GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_75->setText(str);

    ///
    fvalue =m_SysParam.m_COMMSteeringEngineTest.m_Speed[0].GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_76->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineTest.m_Speed[1].GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_77->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineTest.m_Speed[2].GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_78->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineTest.m_Speed[3].GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_79->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineTest.m_Speed[4].GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_80->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineTest.m_Speed[5].GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_81->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineTest.m_Speed[6].GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_82->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineTest.m_Speed[7].GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_83->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineTest.m_StopTime.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_84->setText(str);

    fvalue =m_SysParam.m_COMMSteeringEngineTest.m_NumberOfRuns.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_85->setText(str);


}
//获得界面-附加信息
void    MainUI::GetUI_AdditionalVariable()
{
    short fvalue = ui->lineEdit->text().toInt();
    m_SysParam.m_AdditionalVariable.m_MotorDirection.SetValue(fvalue);

    fvalue = ui->lineEdit_2->text().toInt();
    m_SysParam.m_AdditionalVariable.m_SignalTolerance.SetValue(fvalue);

    fvalue = ui->lineEdit_3->text().toInt();
    m_SysParam.m_AdditionalVariable.m_ProgramVersion.SetValue(fvalue);

    fvalue = ui->lineEdit_4->text().toInt();
    m_SysParam.m_AdditionalVariable.m_PowerDownEnable.SetValue(fvalue);

    fvalue = ui->lineEdit_5->text().toInt();
    m_SysParam.m_AdditionalVariable.m_SteeringGearID.SetValue(fvalue);

    fvalue = ui->lineEdit_6->text().toInt();
    m_SysParam.m_AdditionalVariable.m_UniversalID.SetValue(fvalue);

    fvalue = ui->lineEdit_7->text().toInt();
    m_SysParam.m_AdditionalVariable.m_ResetPosition.SetValue(fvalue);

    fvalue = ui->lineEdit_8->text().toInt();
    m_SysParam.m_AdditionalVariable.m_ResetTime.SetValue(fvalue);

    fvalue = ui->lineEdit_9->text().toInt();
    m_SysParam.m_AdditionalVariable.m_ResetStepSize.SetValue(fvalue);

    fvalue = ui->lineEdit_10->text().toInt();
    m_SysParam.m_AdditionalVariable.m_StartTime.SetValue(fvalue);

    fvalue = ui->lineEdit_11->text().toInt();
    m_SysParam.m_AdditionalVariable.m_StartingStepSize.SetValue(fvalue);

    fvalue = ui->lineEdit_12->text().toInt();
    m_SysParam.m_AdditionalVariable.m_BlockingTime.SetValue(fvalue);

    fvalue = ui->lineEdit_13->text().toInt();
    m_SysParam.m_AdditionalVariable.m_Power_onReset.SetValue(fvalue);

    //fvalue = ui->lineEdit_23->text().toInt();
    //m_SysParam.m_SysLoseParam.servo_position_pid_parm_p_set.SetValue(fvalue);

    fvalue = ui->lineEdit_24->text().toInt();
    m_SysParam.m_SysLoseParam.position_pid_speed_parm_radio.SetValue(fvalue);

    //fvalue = ui->lineEdit_25->text().toInt();
    //m_SysParam.m_SysLoseParam.servo_speed_run_sample_k_set.SetValue(fvalue);

    fvalue = ui->lineEdit_26->text().toInt();
    m_SysParam.m_SysLoseParam.servo_position_sample_ov_time_set.SetValue(fvalue);

    fvalue = ui->lineEdit_27->text().toInt();
    m_SysParam.m_SysLoseParam.servo_speed_sample_ov_time_set.SetValue(fvalue);

    //fvalue = ui->lineEdit_28->text().toInt();
    //m_SysParam.m_SysLoseParam.servo_speed_pid_parm_p_set.SetValue(fvalue);

    fvalue = ui->lineEdit_29->text().toInt();
    m_SysParam.m_SysLoseParam.servo_speed_pid_parm_p_radio.SetValue(fvalue);


    fvalue = ui->lineEdit_34->text().toInt();
    m_SysParam.m_SysLoseParam.servo_work_mode_lock_flag_set.SetValue(fvalue);

    fvalue = ui->lineEdit_38->text().toInt();
    m_SysParam.m_SysLoseParam.servo_work_mode_now.SetValue(fvalue);


    fvalue = ui->lineEdit_96->text().toInt();
    m_SysParam.m_SysLoseParam.servo_pram_config_set.SetValue(fvalue);


    fvalue = ui->lineEdit_97->text().toInt();
    m_SysParam.m_SysLoseParam.servo_command_fb_flag.SetValue(fvalue);

    fvalue = ui->lineEdit_98->text().toInt();
    m_SysParam.m_SysLoseParam.servo_protect_pwm_cmpt.SetValue(fvalue);
}
//设置界面-附加信息
void    MainUI::SetUI_AdditionalVariable()
{
    QString  str;
    short fvalue = m_SysParam.m_AdditionalVariable.m_MotorDirection.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit->setText(str);

    fvalue = m_SysParam.m_AdditionalVariable.m_SignalTolerance.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_2->setText(str);

    fvalue = m_SysParam.m_AdditionalVariable.m_ProgramVersion.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_3->setText(str);
    ui->lineEdit_22->setText(str);

    fvalue = m_SysParam.m_AdditionalVariable.m_PowerDownEnable.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_4->setText(str);

    fvalue = m_SysParam.m_AdditionalVariable.m_SteeringGearID.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_5->setText(str);

    fvalue = m_SysParam.m_AdditionalVariable.m_UniversalID.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_6->setText(str);

    fvalue = m_SysParam.m_AdditionalVariable.m_ResetPosition.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_7->setText(str);

    fvalue = m_SysParam.m_AdditionalVariable.m_ResetTime.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_8->setText(str);

    fvalue = m_SysParam.m_AdditionalVariable.m_ResetStepSize.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_9->setText(str);

    fvalue = m_SysParam.m_AdditionalVariable.m_StartTime.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_10->setText(str);

    fvalue =m_SysParam.m_AdditionalVariable.m_StartingStepSize.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_11->setText(str);

    fvalue =  m_SysParam.m_AdditionalVariable.m_BlockingTime.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_12->setText(str);

    fvalue = m_SysParam.m_AdditionalVariable.m_Power_onReset.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_13->setText(str);

    ///
    //fvalue =m_SysParam.m_SysLoseParam.servo_position_pid_parm_p_set.GetValue();
    //str.sprintf("%d",fvalue);
    //ui->lineEdit_23->setText(str);

    fvalue =m_SysParam.m_SysLoseParam.position_pid_speed_parm_radio.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_24->setText(str);


    //fvalue =m_SysParam.m_SysLoseParam.servo_speed_run_sample_k_set.GetValue();
    //str.sprintf("%d",fvalue);
    //ui->lineEdit_25->setText(str);

    fvalue =m_SysParam.m_SysLoseParam.servo_position_sample_ov_time_set.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_26->setText(str);

    fvalue =m_SysParam.m_SysLoseParam.servo_speed_sample_ov_time_set.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_27->setText(str);

    //fvalue =m_SysParam.m_SysLoseParam.servo_speed_pid_parm_p_set.GetValue();
    //str.sprintf("%d",fvalue);
    //ui->lineEdit_28->setText(str);

    fvalue =m_SysParam.m_SysLoseParam.servo_speed_pid_parm_p_radio.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_29->setText(str);

    fvalue =m_SysParam.m_SysLoseParam.servo_work_mode_lock_flag_set.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_34->setText(str);

    fvalue =m_SysParam.m_SysLoseParam.servo_work_mode_now.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_38->setText(str);

    fvalue =m_SysParam.m_SysLoseParam.servo_pram_config_set.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_96->setText(str);

    fvalue =m_SysParam.m_SysLoseParam.servo_command_fb_flag.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_97->setText(str);

    fvalue =m_SysParam.m_SysLoseParam.servo_protect_pwm_cmpt.GetValue();
    str.sprintf("%d",fvalue);
    ui->lineEdit_98->setText(str);
}
//数据分包
void  MainUI::SegPacket( QByteArray InStr , std::vector<QByteArray> &outStrs )
{
    //表示一个包
    unsigned char cValue = InStr.data()[0];
    if(InStr.size() == 12 && cValue == 0xFA )
    {
        outStrs.push_back(InStr);

        QString str;
        str.sprintf("outStrs %d",outStrs.size());
        qDebug(str.toLatin1().data());

        return  ;
    }

    int  icount = InStr.size();
    for( int i = 0 ; i < icount ;i++ )
    {
        cValue = InStr.data()[i];
        if( cValue == 0xFA )
        {
           int iend = i+11;
           if( iend <= icount - 1 )
           {
               cValue = InStr.data()[iend];
               if( cValue == 0xFE )
               {
                   outStrs.push_back(InStr.mid(i,12));
                   i = iend;
               }
           }
        }
    }
}
//解析数据包
void  MainUI::DecodePacket(QByteArray InStr)
{
    unsigned char  iCmd, iAddr,iState;
    unsigned char  iID1, iID2;
    unsigned short iValue1,iValue2;
    bool bhr = m_SysComm.Decode( InStr,iCmd,iAddr,iState,iID1,iValue1,iID2,iValue2 );
    if( !bhr || iState == 1 )
        return;

    QString str;
    str.sprintf("iCmd %d --iAddr %d iID1 %d iValue1 %d  ",iCmd,iAddr,iID1,iValue1);
    qDebug(str.toLatin1().data());

    switch ( iCmd )
    {
    case SERVO_COMMAND_SERVO_COM_PACKAGE://舵机数据包命令
    {
        //SteeringGearPacketParsing(iID1,iValue1,iID2,iValue2);
        break;
    }
    case SERVO_COMMAND_SERVO_FB:        //舵机反馈数据
    {
        AnalyzeTheFeedbackDataOfSteeringGear(iID1,iValue1,iID2,iValue2);
        break;
    }
    default:
        break;
    }

}
//刷新英文
void    MainUI::RefEnglish()
{
    ui->comboBox_5->setItemText(0,QStringLiteral("Chinese"));
    ui->comboBox_5->setItemText(1,QStringLiteral("English"));

    int icount = ui->tableWidget->rowCount();
    for( int  i = 0 ; i < icount ;i++ )
    {
        QTableWidgetItem *iItem = ui->tableWidget->item(i,1);
        QString  str = iItem->text();
//        str = QStringLiteral(str.toUtf8().data());
        iItem->setText(str);
    }
    ui->pushButton_10->setText("clear data");

    ui->checkBox->setText(QStringLiteral("Lose PPM Lock"));
    ui->checkBox_2->setText(QStringLiteral("Plug Protect"));
    ui->checkBox_3->setText(QStringLiteral("Reset Signal"));
    ui->checkBox_4->setText(QStringLiteral("Sevro Direction"));

    ui->checkBox_9->setText(QStringLiteral("Lose PPM Lock"));
    ui->checkBox_10->setText(QStringLiteral("Sevro Direction"));
    ui->checkBox_11->setText(QStringLiteral("Reset Signal"));
    ui->checkBox_12->setText(QStringLiteral("Plug Protect"));

    ui->label_45->setText(QStringLiteral("CP Protect"));

    QString pathX=QDir::currentPath();
    pathX = pathX + QStringLiteral("/AZSMART Sevro Config");
    this->setWindowTitle(pathX);

    ui->label_47->setText(QStringLiteral("Strensity"));
    ui->label_75->setText(QStringLiteral("Strensity"));

    ui->label_73->setText(QStringLiteral("Solidity"));
    ui->label_74->setText(QStringLiteral("Solidity"));

    ui->label_72->setText(QStringLiteral("Vehicle"));
    ui->label_76->setText(QStringLiteral("Vehicle"));
}
//刷新中文
void    MainUI::RefChinese()
{
    QString pathX=QDir::currentPath();
    pathX = pathX + QStringLiteral("/舵机控制系统");
    this->setWindowTitle(pathX);

    ui->pushButton_10->setText("清除数据");
    ui->comboBox_5->setItemText(0,QStringLiteral("中文"));
    ui->comboBox_5->setItemText(1,QStringLiteral("英文"));
    ui->label_45->setText(QStringLiteral("保护对比"));

    ui->label_47->setText(QStringLiteral("力度"));
    ui->label_75->setText(QStringLiteral("力度"));

    ui->label_73->setText(QStringLiteral("刚度"));
    ui->label_74->setText(QStringLiteral("刚度"));

    ui->label_72->setText(QStringLiteral("刹车"));
    ui->label_76->setText(QStringLiteral("刹车"));

    int icount = ui->tableWidget->rowCount();
    for( int  i = 0 ; i < icount ;i++ )
    {
        QTableWidgetItem *iItem = ui->tableWidget->item(i,1);
        QString  str = iItem->text();
        if( str == QStringLiteral("Conventional steering gear configuration read parameters") )
            str = QStringLiteral("常规舵机配置读取参数");
        if( str == QStringLiteral("General steering gear configuration download parameters") )
            str = QStringLiteral("常规舵机配置下载参数");
        if( str == QStringLiteral("Conventional steering gear configuration reset parameters") )
            str = QStringLiteral("常规舵机配置复位参数");
        if( str == QStringLiteral("Serial port servo configuration read parameters") )
            str = QStringLiteral("串口舵机配置读取参数");
        if( str == QStringLiteral("Serial port steering gear configuration download parameters") )
            str = QStringLiteral("串口舵机配置下载参数");
        if( str == QStringLiteral("Serial port servo configuration servo reset") )
            str = QStringLiteral("串口舵机配置舵机复位");

        if( str == QStringLiteral("Conventional steering gear configuration read parameters complete") )
            str = QStringLiteral("常规舵机配置读取参数完成");
        if( str == QStringLiteral("General steering gear configuration download parameters complete") )
            str = QStringLiteral("常规舵机配置下载参数完成");
        if( str == QStringLiteral("Conventional steering gear configuration reset parameters complete") )
            str = QStringLiteral("常规舵机配置复位参数完成");
        if( str == QStringLiteral("Serial port servo configuration read parameters complete") )
            str = QStringLiteral("串口舵机配置读取参数完成");
        if( str == QStringLiteral("Serial port steering gear configuration download parameters complete") )
            str = QStringLiteral("串口舵机配置下载参数完成");
        if( str == QStringLiteral("Serial port servo configuration servo reset complete") )
            str = QStringLiteral("串口舵机配置舵机复位完成");

        if( str == QStringLiteral("Save system files to files"))
            str = QStringLiteral("将系统文件存入文件");
        if( str == QStringLiteral("Open the file import system") )
            str = QStringLiteral("打开文件导入系统");
        if( str == QStringLiteral("Closed serial port successfully") )
            str = QStringLiteral("关闭串口成功");
        if( str == QStringLiteral("Serial port closed successfully...") )
            str = QStringLiteral("关闭串口成功...");
        if( str == QStringLiteral("Failed to open serial port") )
            str = QStringLiteral("打开串口失败");
        if( str == QStringLiteral("Failed to open serial port...") )
            str = QStringLiteral("打开串口失败...");
        if( str == QStringLiteral("Serial port opened successfully...") )
            str = QStringLiteral("打开串口成功...");
        if( str == QStringLiteral("The serial port was opened successfully") )
            str = QStringLiteral("打开串口成功");
        iItem->setText(str);
    }

    ui->checkBox->setText(QStringLiteral("自锁设置"));
    ui->checkBox_2->setText(QStringLiteral("堵转保护"));
    ui->checkBox_3->setText(QStringLiteral("信号复位"));
    ui->checkBox_4->setText(QStringLiteral("舵机方向"));

    ui->checkBox_9->setText(QStringLiteral("自锁设置"));
    ui->checkBox_10->setText(QStringLiteral("舵机方向"));
    ui->checkBox_11->setText(QStringLiteral("信号复位"));
    ui->checkBox_12->setText(QStringLiteral("堵转保护"));
}
//语言切换
void   MainUI::OnSwitchLang(int Index)
{
    setWindowTitle(QStringLiteral("舵机控制系统"));

    if( Index == 0 )
    {
        qApp->removeTranslator(&translator);
        ui->retranslateUi(this);
        RefChinese();
    }
    else if( Index == 1 )
    {
        QString path=QDir::currentPath();
        QString strName = path+"/english_EN_3.qm" ;
        if ( translator.load(strName))
        {
           qApp->installTranslator(&translator);
           ui->retranslateUi(this);
           RefEnglish();
        }
    }

}
//开始
void   MainUI::OnStratTest()
{
    if( !m_SysComm.IsOpen() )
    {
        QMessageBox message(QMessageBox::NoIcon, QStringLiteral("提示"), QStringLiteral("\n 串口没有打开..."));
        message.exec();
        return;
    }

    ui->pushButton_22->setEnabled( false );
    ui->pushButton_23->setEnabled( true );
    ui->lineEdit_94->setEnabled(false);
    ui->checkBox_5->setEnabled(false);
    ui->checkBox_6->setEnabled(false);
    ui->checkBox_7->setEnabled(false);
    ui->checkBox_8->setEnabled(false);

    //curve_weizhi.detach();
    //curve_wendu.detach();
    //curve_dianya.detach();
    //curve_dianliu.detach();
    OnClear();
    m_RefTable = true;
    m_crcTestSend->start();
}
//停止
void   MainUI::OnStopTest()
{
   m_crcTestSend->closeThread();
   ui->pushButton_22->setEnabled( true );
   ui->pushButton_23->setEnabled( false );
   ui->lineEdit_94->setEnabled(true);
   ui->checkBox_5->setEnabled(true);
   ui->checkBox_6->setEnabled(true);
   ui->checkBox_7->setEnabled(true);
   ui->checkBox_8->setEnabled(true);
   m_RefTable = false;
   /*
   for(int  i = 0 ; i < 10 ;i++)
   {
       //刷新位置
       Refweizhi(i);
       //刷新温度
       Refwendu(i*2);
       //刷新电压
       Refdianya(i*3);
       //刷新电流
       Refdianliu(i*4);
   }
   ui->qwtPlot->replot();*/
}
void  MainUI::receiveInfo()
{
    //获取数据
    QByteArray  outData;
    if( !m_SysComm.Read( outData ) )
        return ;

    m_outData.append(outData);
    if( m_outData.size() >= 12 )
    {
        QString str;
        str.sprintf("%d",m_outData.size());
        qDebug(str.toLatin1().data());

        //对数据包进行分包检测
        std::vector<QByteArray>  singleStrs;
        SegPacket(m_outData,singleStrs);


        str.sprintf("single %d",singleStrs.size());
        qDebug(str.toLatin1().data());

        int  icount = singleStrs.size();
        for( int i = 0 ; i < icount;i++ )
            DecodePacket(singleStrs[i]);

        m_outData.clear();
    }
}
//打开串口信息
void  MainUI::OnOpenCOMM()
{
   QString  str= ui->pushButton->text();
   if( str == QStringLiteral("打开") )
   {
       QString  strCOMM =  ui->comboBox->currentText();
       str = ui->comboBox_2->currentText();
       int      iBaudRate = str.toInt();
       str = ui->comboBox_3->currentText();
       int      iData = str.toInt();

       if( m_SysComm.connect(strCOMM,iBaudRate,iData))
       {
           //消息绑定
          QObject::connect(&m_SysComm.m_comm,SIGNAL(readyRead()), this, SLOT(receiveInfo()));
          ui->pushButton->setText(QStringLiteral("关闭"));

          QPixmap pixmap("..\\img\\red.png");//设定图片
          ui->pushButton->setIcon(pixmap);

          SetInfo(QStringLiteral("打开串口成功"));
          QMessageBox message(QMessageBox::NoIcon, QStringLiteral("提示"), QStringLiteral("\n 打开串口成功..."));
          message.exec();

          m_CurComm = strCOMM;
          //m_RefComm->closeThread();
       }
       else
       {
           m_CurComm = "";
           SetInfo(QStringLiteral("打开串口失败"));
           QMessageBox message(QMessageBox::NoIcon, QStringLiteral("提示"), QStringLiteral("\n 打开串口失败..."));
           message.exec();
       }

   }
   else
   {
       m_CurComm = "";
       m_SysComm.close();
       QPixmap pixmap("..\\img\\screen.png");//设定图片
       ui->pushButton->setIcon(pixmap);
       QMessageBox message(QMessageBox::NoIcon, QStringLiteral("提示"), QStringLiteral("\n 关闭串口成功..."));
       message.exec();
       ui->pushButton->setText(QStringLiteral("打开"));
       SetInfo(QStringLiteral("关闭串口成功"));

       //m_RefComm->start();
   }
}
//打开文件
void  MainUI::OnOpenFileA()
{
    QFileDialog  fDlg(this);
    //英文
    if( ui->comboBox_5->currentIndex() == 1 )
        fDlg.setOption(QFileDialog::DontUseNativeDialog);

    fDlg.setAcceptMode(QFileDialog::AcceptOpen);
    int  ihr = fDlg.exec();
    if( ihr != 1 )
        return ;

    QStringList  str = fDlg.selectedFiles();
    if( str.size() == 0 )
       return ;

    QString fileName = str[0];
    if( fileName.isEmpty() )
       return ;

    int     iSize = sizeof(m_SysParam);
    char    *buf = new char[iSize+1];
    bool    bhr = CSysFile::read(fileName,buf,iSize);
    if( bhr ){
        memcpy((char*)&m_SysParam,buf,iSize);
        qDebug() << "open success";
    }
    else{
        qDebug() << "open fail";
    }
    SetUI_NormSteeringEngineParam();
    SetUI_COMMSteeringEngineParam();
    SetUI_AdditionalVariable();

    fileName = fileName + QStringLiteral("(舵机控制系统)");
    this->setWindowTitle(fileName);

    SetInfo(QStringLiteral("打开文件导入系统"));
}
//保存文件
void  MainUI::OnSaveFileA()
{
    QFileDialog  fDlg(this);
    //英文
    if( ui->comboBox_5->currentIndex() == 1 )
        fDlg.setOption(QFileDialog::DontUseNativeDialog);

    fDlg.setAcceptMode(QFileDialog::AcceptSave);
    int  ihr = fDlg.exec();
    if( ihr != 1 )
        return ;

    QStringList  str = fDlg.selectedFiles();
    if( str.size() == 0 )
       return ;

    QString fileName = str[0];
    if(!fileName.contains(".bat"))
        fileName += ".bat";

    GetUI_NormSteeringEngineParam();
    GetUI_COMMSteeringEngineParam();
    GetUI_AdditionalVariable();
    int  iSize = sizeof(m_SysParam);
    char *buf = new char[iSize+1];
    memcpy(buf,(char*)&m_SysParam,iSize);
    CSysFile::write(fileName,buf,iSize);

    fileName = fileName + QStringLiteral("(舵机控制系统)");
    this->setWindowTitle(fileName);

    SetInfo(QStringLiteral("将系统文件存入文件"));
}
//常规舵机读取
void  MainUI::OnNormRead()
{
    if( !m_SysComm.IsOpen() )
    {
        QMessageBox message(QMessageBox::NoIcon, QStringLiteral("提示"), QStringLiteral("\n 串口没有打开..."));
        message.exec();
        return;
    }
    if( m_SendMark )
    {
        QMessageBox message(QMessageBox::NoIcon, QStringLiteral("提示"), QStringLiteral("\n 常规舵机配置正在读取参数..."));
        message.exec();
        return ;
    }

    m_SendMark = true;

    SetInfo(QStringLiteral("常规舵机配置读取参数"));

    char  cID = GetID();

    GetUI_AdditionalVariable();
    GetUI_NormSteeringEngineParam();
    m_SysParam.m_NormSteeringEngineParam.SetValue(-1);
    m_SysParam.m_AdditionalVariable.SetValue(-1);
    m_SysParam.m_SysLoseParam.SetValue(-1);
    SetUI_AdditionalVariable();
    SetUI_NormSteeringEngineParam();

    //反馈
    m_SysComm.HeadEncode();
    sleep(delyTime);

    for(int i = 81;i <= 115;i++)
    {
        m_SysComm.Encode(SERVO_COMMAND_SERVO_FB, cID,SERVO_STATE_COM, i,0,0, 0);
        m_bSendrecvData = true;
        sleep(delyTime);
        Waiting();
    }

    m_SysComm.HeadEncode();
    sleep(delyTime);

    for(int i = 120;i <= 126;i++)
    {
        m_SysComm.Encode(SERVO_COMMAND_SERVO_FB, cID,SERVO_STATE_COM, i,0,0, 0);
        m_bSendrecvData = true;
        sleep(delyTime);
        Waiting();
    }

    m_SysComm.Encode(SERVO_COMMAND_SERVO_FB, cID,SERVO_STATE_COM, MENU_FB_SERVO_CONFIG_PARM0,0,0, 0);
    m_bSendrecvData = true;
    sleep(delyTime);
    Waiting();

    m_SysComm.Encode(SERVO_COMMAND_SERVO_FB, cID,SERVO_STATE_COM, MENU_FB_SERVO_CONFIG_PARM1,0,0, 0);
    sleep(delyTime);
    m_bSendrecvData = true;
    Waiting();

    m_SysComm.Encode(SERVO_COMMAND_SERVO_FB, cID,SERVO_STATE_COM, MENU_FB_SERVO_CONFIG_PARM2,0,0, 0);
    sleep(delyTime);
    m_bSendrecvData = true;
    Waiting();

    m_SysComm.Encode(SERVO_COMMAND_SERVO_FB, cID,SERVO_STATE_COM, MENU_FB_SERVO_CONFIG_PARM3,0,0, 0);
    m_bSendrecvData = true;
    sleep(delyTime);
    Waiting();

    SetInfo(QStringLiteral("常规舵机配置读取参数完成"));
    m_SendMark = false;
}
//常规舵机下载
void  MainUI::OnNormWirte()
{
    if( !m_SysComm.IsOpen() )
    {
        QMessageBox message(QMessageBox::NoIcon, QStringLiteral("提示"), QStringLiteral("\n 串口没有打开..."));
        message.exec();
        return;
    }
    if( m_SendMark )
    {
        QMessageBox message(QMessageBox::NoIcon, QStringLiteral("提示"), QStringLiteral("\n 常规舵机配置正在下载参数..."));
        message.exec();
        return ;
    }

    m_SendMark = true;
    SetInfo(QStringLiteral("常规舵机配置下载参数"));

    m_SysComm.HeadEncode();
    sleep(delyTime);
    //下发界面参数及默认参数
    OnNormSendUI();

    SetInfo(QStringLiteral("常规舵机配置下载参数完成"));
    m_SendMark = false;
}
//常规舵机复位
void  MainUI::OnNormReStore()
{
    if( !m_SysComm.IsOpen() )
    {
        QMessageBox message(QMessageBox::NoIcon, QStringLiteral("提示"), QStringLiteral("\n 串口没有打开..."));
        message.exec();
        return;
    }
    if( m_SendMark )
    {
        QMessageBox message(QMessageBox::NoIcon, QStringLiteral("提示"), QStringLiteral("\n 常规舵机配置正在复位参数..."));
        message.exec();
        return ;
    }

    m_SendMark = true;
    SetInfo(QStringLiteral("常规舵机配置复位参数"));

    ui->lineEdit_4->setText("0");

    m_SysComm.HeadEncode();
    sleep(delyTime);
    //下发界面参数及默认参数
    OnNormSendUI();

    SetInfo(QStringLiteral("常规舵机配置复位参数完成"));

     m_SendMark = false;
}
//串口舵机读取
void   MainUI::OnCOMMRead()
{
    if( !m_SysComm.IsOpen() )
    {
        QMessageBox message(QMessageBox::NoIcon, QStringLiteral("提示"), QStringLiteral("\n 串口没有打开..."));
        message.exec();
        return;
    }
    if( m_SendMark )
    {
        QMessageBox message(QMessageBox::NoIcon, QStringLiteral("提示"), QStringLiteral("\n 串口舵机配置正在读取参数..."));
        message.exec();
        return ;
    }
     m_SendMark = true;
    SetInfo(QStringLiteral("串口舵机配置读取参数"));

    //将界面数据设置到舵机
    //GetUI_COMMSteeringEngineParam();
    GetUI_AdditionalVariable();
    GetUI_COMMSteeringEngineParam();
    m_SysParam.m_COMMSteeringEngineParam.SetValue(-1);
    m_SysParam.m_AdditionalVariable.SetValue(-1);
    m_SysParam.m_SysLoseParam.SetValue(-1);
    SetUI_AdditionalVariable();
    SetUI_COMMSteeringEngineParam();

    //当前舵机ID
    char  cID = m_SysParam.m_COMMSteeringEngineParam.m_CurID.GetValue();

    //反馈
    m_SysComm.HeadEncode();
    sleep(delyTime);

    for(int i = 81;i <= 115;i++)
    {
        m_SysComm.Encode(SERVO_COMMAND_SERVO_FB, cID,SERVO_STATE_COM, i,0,0, 0);
        m_bSendrecvData = true;
        sleep(delyTime);
        Waiting();
    }

    m_SysComm.HeadEncode();
    sleep(delyTime);

    for(int i = 120;i <= 126;i++)
    {
        m_SysComm.Encode(SERVO_COMMAND_SERVO_FB, cID,SERVO_STATE_COM, i,0,0, 0);
        m_bSendrecvData = true;
        sleep(delyTime);
        Waiting();
    }

    m_SysComm.Encode(SERVO_COMMAND_SERVO_FB, cID,SERVO_STATE_COM, MENU_FB_SERVO_CONFIG_PARM0,0,0, 0);
    m_bSendrecvData = true;
    sleep(delyTime);
    Waiting();

    m_SysComm.Encode(SERVO_COMMAND_SERVO_FB, cID,SERVO_STATE_COM, MENU_FB_SERVO_CONFIG_PARM1,0,0, 0);
    m_bSendrecvData = true;
    sleep(delyTime);
    Waiting();

    m_SysComm.Encode(SERVO_COMMAND_SERVO_FB, cID,SERVO_STATE_COM, MENU_FB_SERVO_CONFIG_PARM2,0,0, 0);
    m_bSendrecvData = true;
    sleep(delyTime);
    Waiting();

    m_SysComm.Encode(SERVO_COMMAND_SERVO_FB, cID,SERVO_STATE_COM, MENU_FB_SERVO_CONFIG_PARM3,0,0, 0);
    m_bSendrecvData = true;
    sleep(delyTime);
    Waiting();

    SetInfo(QStringLiteral("串口舵机配置读取参数完成"));
     m_SendMark = false;
}
//串口舵机下载
void  MainUI::OnCOMMWirte()
{
    if( !m_SysComm.IsOpen() )
    {
        QMessageBox message(QMessageBox::NoIcon, QStringLiteral("提示"), QStringLiteral("\n 串口没有打开..."));
        message.exec();
        return;
    }
    if( m_SendMark )
    {
        QMessageBox message(QMessageBox::NoIcon, QStringLiteral("提示"), QStringLiteral("\n 串口舵机配置正在下载参数..."));
        message.exec();
        return ;
    }
     m_SendMark = true;
    SetInfo(QStringLiteral("串口舵机配置下载参数"));

    //将界面数据设置到舵机
    GetUI_COMMSteeringEngineParam();

    //当前舵机ID
    char  cID = m_SysParam.m_COMMSteeringEngineParam.m_CurID.GetValue();


    m_SysComm.HeadEncode();
    sleep(delyTime);

    //下发界面参数
    OnCOMMSendUI();

    //新ID不为0，ID更改命令
    char  cNewID = m_SysParam.m_COMMSteeringEngineParam.m_NewID.GetValue();
    if( cNewID != 0 )
    {
        m_SysComm.HeadEncode();
        sleep(delyTime);

        m_SysComm.Encode(SERVO_COMMAND_SERVO_ADDRESS_SET, 252,SERVO_STATE_COM, MENU_CONFIG_SERVO_UNIQUE_ADDRESS_SET,cID,0,cNewID);
        sleep(delyTime);
    }
    SetInfo(QStringLiteral("串口舵机配置下载参数完成"));
     m_SendMark = false;
}
//串口舵机复位
void  MainUI::OnCOMMReStore()
{
    if( !m_SysComm.IsOpen() )
    {
        QMessageBox message(QMessageBox::NoIcon, QStringLiteral("提示"), QStringLiteral("\n 串口没有打开..."));
        message.exec();
        return;
    }
    if( m_SendMark )
    {
        QMessageBox message(QMessageBox::NoIcon, QStringLiteral("提示"), QStringLiteral("\n 串口舵机配置正在舵机复位..."));
        message.exec();
        return ;
    }

    m_SendMark = true;
    SetInfo(QStringLiteral("串口舵机配置舵机复位"));

    ui->lineEdit_4->setText("0");

    m_SysComm.HeadEncode();
    sleep(delyTime);
    //下发界面参数及默认参数
    OnCOMMSendUI();

    SetInfo(QStringLiteral("串口舵机配置舵机复位完成"));
    m_SendMark = false;
}
//上
void  MainUI::OnUp()
{
    if( !m_SysComm.IsOpen() )
    {
        QMessageBox message(QMessageBox::NoIcon, QStringLiteral("提示"), QStringLiteral("\n 串口没有打开..."));
        message.exec();
        return;
    }

    GetUI_AdditionalVariable();
    GetUI_SteeringEngineTest();
    char  cID = GetID();

    m_SysComm.HeadEncode();
    sleep(delyTime);

    char2short  ispeed;
    ispeed.c2[1] = m_SysParam.m_AdditionalVariable.m_StartTime.GetValue();
    ispeed.c2[0] = m_SysParam.m_AdditionalVariable.m_StartingStepSize.GetValue();
    short  sPos = m_SysParam.m_NormSteeringEngineTest.m_MaxValue.GetValue();
    OnSendPoistion(cID,sPos,ispeed.s1);
    //m_ErrorInfoManger.AddValue("shang error",ui->comboBox_6,ui->comboBox_5);
}
//中
void  MainUI::OnCenter()
{
    if( !m_SysComm.IsOpen() )
    {
        QMessageBox message(QMessageBox::NoIcon, QStringLiteral("提示"), QStringLiteral("\n 串口没有打开..."));
        message.exec();
        return;
    }

    GetUI_AdditionalVariable();
    GetUI_SteeringEngineTest();
    char  cID = GetID();

    m_SysComm.HeadEncode();
    sleep(delyTime);

    char2short  ispeed;
    ispeed.c2[1] = m_SysParam.m_AdditionalVariable.m_StartTime.GetValue();
    ispeed.c2[0] = m_SysParam.m_AdditionalVariable.m_StartingStepSize.GetValue();
    short  sPos = m_SysParam.m_NormSteeringEngineTest.m_MiddleValue.GetValue();
    OnSendPoistion(cID,sPos,ispeed.s1);
}
//下
void  MainUI::OnDown()
{
    if( !m_SysComm.IsOpen() )
    {
        QMessageBox message(QMessageBox::NoIcon, QStringLiteral("提示"), QStringLiteral("\n 串口没有打开..."));
        message.exec();
        return;
    }

    GetUI_AdditionalVariable();
    GetUI_SteeringEngineTest();
    char  cID = GetID();

    m_SysComm.HeadEncode();
    sleep(delyTime);

    char2short  ispeed;
    ispeed.c2[1] = m_SysParam.m_AdditionalVariable.m_StartTime.GetValue();
    ispeed.c2[0] = m_SysParam.m_AdditionalVariable.m_StartingStepSize.GetValue();
    short  sPos = m_SysParam.m_NormSteeringEngineTest.m_MinValue.GetValue();
    OnSendPoistion(cID,sPos,ispeed.s1);
}
//反馈
void  MainUI::OnQuery()
{
    if( !m_SysComm.IsOpen() )
    {
        QMessageBox message(QMessageBox::NoIcon, QStringLiteral("提示"), QStringLiteral("\n 串口没有打开..."));
        message.exec();
        return;
    }

    char  cID = GetID();

    m_RefTable = false;
    OnStopTest();

    m_SysComm.HeadEncode();
    sleep(delyTime);

    m_SysComm.Encode(SERVO_COMMAND_SERVO_FB, cID,SERVO_STATE_COM, MENU_FB_SERVO_STATE_PARM1,0,0, 0);
    sleep(delyTime);

    m_SysComm.Encode(SERVO_COMMAND_SERVO_FB, cID,SERVO_STATE_COM, MENU_FB_SERVO_STATE_PARM2,0,0, 0);
    sleep(delyTime);

    m_SysComm.Encode(SERVO_COMMAND_SERVO_FB, cID,SERVO_STATE_COM, MENU_FB_SERVO_STATE_PARM3,0,0, 0);
    sleep(delyTime);

    m_SysComm.Encode(SERVO_COMMAND_SERVO_FB, cID,SERVO_STATE_COM, MENU_FB_SERVO_STATE_PARM4,0,0, 0);
    sleep(delyTime);

    m_SysComm.Encode(SERVO_COMMAND_SERVO_FB, cID,SERVO_STATE_COM, MENU_FB_SERVO_STATE_PARM5,0,0, 0);
    sleep(delyTime);
}
//s_S
void    MainUI::OnSlidDown()
{
    if( !m_SysComm.IsOpen() )
    {
        //QMessageBox message(QMessageBox::NoIcon, QStringLiteral("提示"), QStringLiteral("\n 串口没有打开..."));
       // message.exec();
        return;
    }

    GetUI_AdditionalVariable();

    //m_CSendSlider->closeThread();

}
//s_e
void    MainUI::OnSlidUp()
{
    //m_CSendSlider->closeThread();
}
//滑动条
void  MainUI::OnSlider(int  ivalue)
{
    m_CSendSlider->m_iV = ivalue;
    QString str;
    str.sprintf("%d",ivalue);
    ui->lineEdit_86->setText(str);
    GetUI_AdditionalVariable();
 }
//运行
void  MainUI::OnRun()
{
    if( !m_SysComm.IsOpen() )
    {
        QMessageBox message(QMessageBox::NoIcon, QStringLiteral("提示"), QStringLiteral("\n 串口没有打开..."));
        message.exec();
        return;
    }

    GetUI_COMMSteeringEngineTest();
    m_crcSend->start();
    ui->pushButton_16->setEnabled(false);
    ui->pushButton_17->setEnabled(true);

    m_TimerID = startTimer(1000);
}
//停止
void  MainUI::OnStop()
{
    m_crcSend->closeThread();
    ui->pushButton_16->setEnabled(true);
    ui->pushButton_17->setEnabled(false);

    killTimer(m_TimerID);
}
//切换tab
void MainUI::OnSwitch(int iTab)
{
    switch(iTab)
    {
    case 0:
    {
        OnStop();
        OnStopTest();
        break;
    }
    case 1:
    {
        OnStop();
        OnStopTest();
        break;
    }
    case 2:
    {
        OnStop();
        break;
    }
    case 3:
    {

        OnStopTest();
        break;
    }
    }
}
//刷新位置
void        MainUI::Refweizhi(int iv)
{
    if( !m_RefTable )
        return;

    y_weizhi.push_back(iv);

    int  iTime = ui->lineEdit_94->text().toInt();
    int  icount = y_weizhi.size();
    x_weizhi.push_back(icount*iTime);

    //平滑曲线
//    curve_weizhi.attach(ui->qwtPlot);//把曲线附加到plot上
//    curve_weizhi.setSamples(x_weizhi,y_weizhi);
    //curve_weizhi.setSymbol(new QwtSymbol(QwtSymbol::Cross, Qt::NoBrush, QPen(Qt::black), QSize(5, 5) ) );
    //curve_weizhi.setStyle(QwtPlotCurve::Dots);//设置曲线上是点还是线，默认是线，所以此行可不加
//    curve_weizhi.setCurveAttribute(QwtPlotCurve::Fitted, true);//使曲线更光滑，不加这句曲线会很硬朗，有折点
//    curve_weizhi.setPen(QPen(Qt::red));//设置画笔
}
//刷新温度
void        MainUI::Refwendu(int iv)
{
    if( !m_RefTable )
        return;

    y_wendu.push_back(iv);

    int  iTime = ui->lineEdit_94->text().toInt();
    int  icount = y_wendu.size();
    x_wendu.push_back(icount*iTime);

    //平滑曲线
//    curve_wendu.attach(ui->qwtPlot);//把曲线附加到plot上
//    curve_wendu.setSamples(x_wendu,y_wendu);
    //curve_wendu.setSymbol(new QwtSymbol(QwtSymbol::Cross, Qt::NoBrush, QPen(Qt::black), QSize(5, 5) ) );
    //curve_wendu.setStyle(QwtPlotCurve::Dots);//设置曲线上是点还是线，默认是线，所以此行可不加
//    curve_wendu.setCurveAttribute(QwtPlotCurve::Fitted, true);//使曲线更光滑，不加这句曲线会很硬朗，有折点
//    curve_wendu.setPen(QPen(Qt::green));//设置画笔
}
//刷新电压
void        MainUI::Refdianya(int iv)
{
    if( !m_RefTable )
        return;

    y_dianya.push_back(iv);

    int  iTime = ui->lineEdit_94->text().toInt();
    int  icount = y_dianya.size();
    x_dianya.push_back(icount*iTime);

    //平滑曲线
//    curve_dianya.attach(ui->qwtPlot);//把曲线附加到plot上
//    curve_dianya.setSamples(x_dianya,y_dianya);
    //curve_dianya.setSymbol(new QwtSymbol(QwtSymbol::Cross, Qt::NoBrush, QPen(Qt::black), QSize(5, 5) ) );
    //curve_dianya.setStyle(QwtPlotCurve::Dots);//设置曲线上是点还是线，默认是线，所以此行可不加
//    curve_dianya.setCurveAttribute(QwtPlotCurve::Fitted, true);//使曲线更光滑，不加这句曲线会很硬朗，有折点
//    curve_dianya.setPen(QPen(Qt::yellow));//设置画笔
}
//刷新电流
void        MainUI::Refdianliu(int  iv)
{
    if( !m_RefTable )
        return;

    y_dianliu.push_back(iv);

    int  iTime = ui->lineEdit_94->text().toInt();
    int  icount = y_dianliu.size();
    x_dianliu.push_back(icount*iTime);

    //平滑曲线
//    curve_dianliu.attach(ui->qwtPlot);//把曲线附加到plot上
//    curve_dianliu.setSamples(x_dianliu,y_dianliu);
    //curve_dianliu.setSymbol(new QwtSymbol(QwtSymbol::Cross, Qt::NoBrush, QPen(Qt::black), QSize(5, 5) ) );
    //curve_dianliu.setStyle(QwtPlotCurve::Dots);//设置曲线上是点还是线，默认是线，所以此行可不加
//    curve_dianliu.setCurveAttribute(QwtPlotCurve::Fitted, true);//使曲线更光滑，不加这句曲线会很硬朗，有折点
//    curve_dianliu.setPen(QPen(Qt::black));//设置画笔
}
