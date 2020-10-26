#include "csyscomm.h"
#include<qserialportinfo.h>
#include<iostream>
#include"mainui.h"
CSysComm::CSysComm()
{

}
CSysComm::~CSysComm()
{

}
//头协议
bool CSysComm::HeadEncode()
{
    unsigned char  outbuf[12] = {""};
    outbuf[0] = 0xFA;
    outbuf[1] = 0x55;
    outbuf[2] = 0x55;
    outbuf[3] = 0x55;
    outbuf[4] = 0x55;
    outbuf[5] = 0x55;
    outbuf[6] = 0x55;
    outbuf[7] = 0x55;
    outbuf[8] = 0x55;
    outbuf[9] = 0x55;
    outbuf[10] = 0x55;
    outbuf[11] = 0xFE;

    if( Send(outbuf,12) )
        return  true;
    else
        return false;
}
//校检位
char CSysComm::Xor(unsigned char *pBuf,int  iLen )
{
    unsigned char  cXor = 0;
    for( int  i = 0 ; i < iLen ;i++ )
        cXor += pBuf[i];

    return cXor;
}
//编码
bool CSysComm::Encode(unsigned char  iCMD,
                      unsigned char  iAddr,
                      unsigned char  iState,
                      unsigned char  iID1 ,
                      unsigned short iValue1,
                      unsigned char  iID2 ,
                      unsigned short iValue2)
{
    unsigned char  outbuf[12] = {""};
    outbuf[0] = 0xFA;
    outbuf[1] = iAddr;
    outbuf[2] = iCMD;
    outbuf[3] = iState;
    outbuf[4] = iID1;
    outbuf[5] = (unsigned char)(iValue1 >>8);
    outbuf[6] = (unsigned char)(iValue1 );
    outbuf[7] = iID2;
    outbuf[8] = (unsigned char)(iValue2 >>8);
    outbuf[9] = (unsigned char)(iValue2 );


    //注意校检的信息
    unsigned char  cXor = iCMD + iState + iID1;
    outbuf[10] = cXor;
    outbuf[11] = 0xFE;

    if( Send(outbuf,12) )
        return  true;
    else
        return false;
}
//编码
bool CSysComm::Decode( QByteArray InStr,
                       unsigned char &iCMD,
                       unsigned char &iAddr,
                       unsigned char &iState,
                       unsigned char &iID1 ,
                       unsigned short&iValue1,
                       unsigned char &iID2 ,
                       unsigned short&iValue2)
{
    if(InStr.size() != 12)
        return false;

    iAddr  = InStr.data()[1];
    iCMD   = InStr.data()[2];
    iState = InStr.data()[3];
    iID1    = InStr.data()[4];
    unsigned char   H_1 = InStr.data()[5];
    unsigned char   L_1 = InStr.data()[6];

    /*
    char2short  iv;
    iv.c2[1] = InStr.data()[5];
    iv.c2[0] = InStr.data()[6];
    iValue1 = iv.s1;*/

    iValue1 = H_1;
    iValue1 = (iValue1 << 8) + L_1;

    iID2 = InStr.data()[7];
    unsigned char   H_2 = InStr.data()[8];
    unsigned char   L_2 = InStr.data()[9];

    iValue2 = H_2;
    iValue2 = (iValue2 << 8) + L_2;
    return  true;
}
/*
功能：搜索系统中串口
*/
bool  CSysComm::QueryComm( std::vector<QString> & outComm )
{
    foreach (const QSerialPortInfo &qspinfo, QSerialPortInfo::availablePorts())
    {
        outComm.push_back(qspinfo.portName());
        //ui->comboBox->addItem(qspinfo.portName());
        //ui->textEdit->append("portname   " + qspinfo.portName());
        //ui->textEdit->append("description:   " + qspinfo.description());
        //ui->textEdit->append("manufacture    "+ qspinfo.manufacturer());
        //ui->textEdit->append("serialnumber   "+ qspinfo.serialNumber());
        //ui->textEdit->append("systemlocation "+qspinfo.serialNumber());
    }
    return  false;
}
/*
功能：连接串口
*/
bool  CSysComm::connect(QString  QStrComName ,int  iBaudRate , int   iData)
{
    if( m_comm.isOpen() )
        m_comm.close();

    //设置串口号
    m_comm.setPortName(QStrComName);

    //以读写模式打开串口
    bool  bhr = m_comm.open(QIODevice::ReadWrite);
    if( !bhr )
    {
        //qDebug<<QStrComName<<"打开失败!";
        return  false;
    }

    //设置串口波特率
    m_comm.setBaudRate(iBaudRate);
    //设置数据位

    if( iData == 5 )
        m_comm.setDataBits(QSerialPort::Data5);
    else if( iData == 6 )
        m_comm.setDataBits(QSerialPort::Data6);
    else if( iData == 7 )
        m_comm.setDataBits(QSerialPort::Data7);
    else if( iData == 8 )
        m_comm.setDataBits(QSerialPort::Data8);

    //设置流控制
    //m_comm.setFlowControl(QSerialPort::NoFlowControl);
    //设置校检位
    //m_comm.setParity(QSerialPort::NoParity);
    //设置停止位
    //m_comm.setStopBits(QSerialPort::OneStop);

    return  true;
}
/*
功能：断开端口
*/
bool  CSysComm::close()
{
    if( m_comm.isOpen() )
        m_comm.close();
    return  true;
}
/*
功能：发送数据
*/
bool  CSysComm::Send( QString str )
{
    if( !m_comm.isOpen() )
        return  false;

    QByteArray sendBuf = str.toLatin1();
    //convertStringToHex(str, sendBuf); //把QString 转换 为 hex
    qint64 ilen = m_comm.write(sendBuf);
    if( ilen > 0 )
        return true;

    if( m_comm.isOpen() )
        m_comm.close();
    return  false;
}
/*
功能：判断串口是否打开
*/
bool  CSysComm::IsOpen()
{
    if( !m_comm.isOpen() )
        return  false;

    return true;
}

/*
功能：发送数据
*/
bool  CSysComm::Send(unsigned char *buf , int len )
{
    if( !m_comm.isOpen() )
        return  false;

    qint64 ilen = m_comm.write((char*)buf,len);
    if( ilen > 0 )
        return true;

    if( m_comm.isOpen() )
        m_comm.close();
    return  false;
}
/*
功能：读取数据
*/
bool CSysComm::Read(QByteArray & hexData)
{
    hexData = m_comm.readAll();
    if( hexData.length() == 0 )
        return  false;

    return  true;
}


