#ifndef CSYSCOMM_H
#define CSYSCOMM_H
#include<QString>
#include<QDialog>
#include<QtSerialPort/qserialport.h>
#include<vector>
using  namespace std;
#include<csysparam.h>
/*
   功能：根据串口和私有协议，实现和下位机通信设计
*/
class MainUI;
class CSysComm
{
public:
    CSysComm();
    ~CSysComm();
public:
    //头协议
    bool HeadEncode();
    //校检位
    char Xor(unsigned char *pBuf,int  iLen );
    //编码
    bool Encode( unsigned char  iCMD,
                        unsigned char  iAddr,
                        unsigned char  iState,
                        unsigned char  iID1 ,
                        unsigned short iValue1,
                        unsigned char  iID2 ,
                        unsigned short iValue2);
    //编码
    bool Decode( QByteArray InStr,
                        unsigned char &iCMD,
                        unsigned char &iAddr,
                        unsigned char &iState,
                        unsigned char & iID1 ,
                        unsigned short&iValue1,
                        unsigned char & iID2 ,
                        unsigned short&iValue2
                         );
public:
    /*
    功能：搜索系统中串口
    */
    bool  QueryComm(std::vector<QString> & outComm );
    /*
    功能：连接串口
    */
    bool  connect(QString  QStrComName,int  iBaudRate , int  iData);
    /*
    功能：断开端口
    */
    bool  close();
    /*
    功能：发送数据
    */
    bool  Send( QString str );
    /*
    功能：发送数据
    */
    bool  Send(unsigned char *buf , int ilen );
    /*
    功能：读取数据
    */
    bool  Read(QByteArray & hexData);
    /*
    功能：判断串口是否打开
    */
    bool  IsOpen();
public:
    QSerialPort  m_comm;
};

#endif // CSYSCOMM_H






























































