#include "csysfile.h"
#include<QFile>
CSysFile::CSysFile()
{
}
CSysFile::~CSysFile()
{
}
/*
功能：写入信息
*/
bool  CSysFile::write( QString fileName,QString  str )
{
    QFile  file(fileName);
    bool bhr = file.open(QIODevice::WriteOnly);//|QIODevice::Text
    if( !bhr )
        return  false;

    file.write(str.toUtf8());
    file.close();
    return  true;
}
bool  CSysFile::write( QString fileName,char*str ,int  ilen)
{
    QFile  file(fileName);
    bool bhr = file.open(QIODevice::WriteOnly);//|QIODevice::Text
    if( !bhr )
        return  false;

    file.write(str,ilen);
    file.close();
    return  true;
}
/*
功能：读取信息
*/
bool  CSysFile::read( QString fileName,QString & str )
{
    QFile  file(fileName);
    bool bhr = file.open(QIODevice::ReadOnly);//|QIODevice::Text
    if( !bhr )
        return  false;

    QByteArray  t = file.readAll();
    str = QString(t);
    file.close();

    return  true;
}
bool  CSysFile::read( QString fileName, char*    str ,int &ilen )
{
    QFile  file(fileName);
    bool bhr = file.open(QIODevice::ReadOnly);//|QIODevice::Text
    if( !bhr )
        return  false;

    QByteArray  t = file.readAll();
    if( t.size()!=ilen )
        return false;

    ilen = t.size();
    memcpy(str,t.data(),ilen);
    file.close();

    return  true;
}
