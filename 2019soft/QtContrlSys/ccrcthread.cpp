#include "ccrcthread.h"
#include "mainui.h"
#include "ui_mainui.h"
#include "CSysDefine.h"
#include<QTime>
#include<QDebug>
#include<QCoreApplication>


CSendSlider::CSendSlider(MainUI   *MainUI)
{
   m_MainUI = MainUI;
   m_iV = -1;
}
void CSendSlider::closeThread()
{
    isStop = true;

}
void CSendSlider::run()
{
    isStop = false;
    int   ivalue = m_iV;
    while( !isStop )
    {
        if( ivalue != m_iV )
        {
            ivalue = m_iV;
            if( !m_MainUI->m_SysComm.IsOpen() )
            {
                m_MainUI->sleep(delyTime);
                continue;
            }

            m_MainUI->m_SysComm.HeadEncode();
            m_MainUI->sleep(delyTime);

            char2short  ispeed;
            ispeed.c2[1] = m_MainUI->m_SysParam.m_AdditionalVariable.m_StartTime.GetValue();
            ispeed.c2[0] = m_MainUI->m_SysParam.m_AdditionalVariable.m_StartingStepSize.GetValue();

            char  cID = m_MainUI->GetID();
            m_MainUI->OnSendPoistion(cID,ivalue,ispeed.s1);
        }
        else
        {
            m_MainUI->sleep(50);
        }

    }

}



crcSend::crcSend(MainUI   *MainUI)
{
   m_MainUI = MainUI;
}
void crcSend::closeThread()
{
    isStop = true;

}
void crcSend::run()
{
    isStop = false;

    CCOMMSteeringEngineTest  iParam = m_MainUI->m_SysParam.m_COMMSteeringEngineTest;
    short  sTime = iParam.m_StopTime.GetValue();
    short  sNumber = iParam.m_NumberOfRuns.GetValue();

    char2short  iV;
    iV.c2[1] = 1;
    m_MainUI->m_SendCount = 0;

    m_MainUI->m_SysComm.HeadEncode();
    m_MainUI->sleep(delyTime);
    for( int  i = 0 ; i < sNumber ;i++ )
    {
        m_MainUI->m_SendCount++;

        for(int j = 0 ; j < 8;j++)
        {
            iV.c2[0] = iParam.m_Speed[j].GetValue();
            m_MainUI->OnSendPoistion( iParam.m_ID[j].GetValue(),iParam.m_Position1[j].GetValue(),iV.s1 );
            m_MainUI->sleep(sTime);

            //跳出来
            if( isStop )
                break;
        }

        for(int j = 0 ; j < 8;j++)
        {
            m_MainUI->OnSendPoistion( iParam.m_ID[j].GetValue(),iParam.m_Position2[j].GetValue(), iParam.m_Speed[j].GetValue());
            m_MainUI->sleep(sTime);

            //跳出来
            if( isStop )
                break;
        }
        //跳出来
        if( isStop )
            break;
    }

    //m_MainUI->ui->pushButton_16->setEnabled(true);
    //m_MainUI->ui->pushButton_17->setEnabled(false);

}



RefComm::RefComm(MainUI   *MainUI)
{
   m_MainUI = MainUI;
}
void RefComm::closeThread()
{
    isStop = true;

}
void RefComm::run()
{
    isStop = false;
    std::vector<QString>  HisComm;
    m_MainUI->ui->comboBox->clear();
    while( !isStop )
    {
        std::vector<QString>  outComm;
        m_MainUI->m_SysComm.QueryComm(outComm);
        int  icount = outComm.size();

        bool   bhr = false;
        for( int  i = 0 ; i < icount ;i++ )
        {
            if( m_MainUI->m_CurComm == outComm[i] || m_MainUI->m_CurComm.isEmpty())
            {
                bhr = true;
            }
            int  j = 0;
            int  iHisCount = HisComm.size();
            for( j = 0; j < iHisCount ;j++ )
            {
                if( HisComm[j] == outComm[i] )
                {
                    break;
                }
            }
            if( iHisCount == 0 )
            {
                m_MainUI->ui->comboBox->addItem(outComm[i]);
            }
            else
            {
                if( iHisCount == j )
                m_MainUI->ui->comboBox->addItem(outComm[i]);
            }
        }

        if( !bhr )
        {
            m_MainUI->ui->comboBox->clear();
            m_MainUI->m_CurComm = "";
            m_MainUI->m_SysComm.close();
            QPixmap pixmap("..\\img\\screen.png");//设定图片
            m_MainUI->ui->pushButton->setIcon(pixmap);
            m_MainUI->ui->pushButton->setText(tr("打开"));
        }

        HisComm = outComm;
        m_MainUI->sleep(100);
    }
}

crcTestSend::crcTestSend(MainUI   *MainUI)
{
   m_MainUI = MainUI;
}
void crcTestSend::closeThread()
{
    isStop = true;

}
void crcTestSend::run()
{
    isStop = false;
    char  cID = m_MainUI->GetID();
    int  iTime = m_MainUI->ui->lineEdit_94->text().toInt();
    while (!isStop)
    {
        m_MainUI->m_SysComm.HeadEncode();
        m_MainUI->sleep(delyTime);

        if( m_MainUI->ui->checkBox_5->isChecked() )
        {
            m_MainUI->m_SysComm.Encode(SERVO_COMMAND_SERVO_FB, cID,SERVO_STATE_COM, MENU_FB_SERVO_STATE_PARM1,0,0, 0);
            m_MainUI->sleep(delyTime);
        }

        if( m_MainUI->ui->checkBox_6->isChecked() )
        {
            m_MainUI->m_SysComm.Encode(SERVO_COMMAND_SERVO_FB, cID,SERVO_STATE_COM, MENU_FB_SERVO_STATE_PARM4,0,0, 0);
            m_MainUI->sleep(delyTime);
        }

        if( m_MainUI->ui->checkBox_7->isChecked() )
        {
            m_MainUI->m_SysComm.Encode(SERVO_COMMAND_SERVO_FB, cID,SERVO_STATE_COM, MENU_FB_SERVO_STATE_PARM5,0,0, 0);
            m_MainUI->sleep(delyTime);
        }

        if( m_MainUI->ui->checkBox_8->isChecked() )
        {
            m_MainUI->m_SysComm.Encode(SERVO_COMMAND_SERVO_FB, cID,SERVO_STATE_COM, MENU_FB_SERVO_STATE_PARM3,0,0, 0);
            m_MainUI->sleep(delyTime);
        }

        m_MainUI->sleep(iTime);
    }

}
