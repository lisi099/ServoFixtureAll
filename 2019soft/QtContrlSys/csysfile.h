#ifndef CSYSFILE_H
#define CSYSFILE_H
#include<QString>
/*
   功能：文件存储和读取功能
*/
class CSysFile
{
public:
    CSysFile();
    ~CSysFile();
public:
    /*
    功能：写入信息
    */
    static bool  write( QString fileName,QString  str );
    static bool  write( QString fileName,char*    str ,int  ilen);
    /*
    功能：读取信息
    */
    static bool  read( QString filename,QString & str );
    static bool  read( QString filename, char*    str ,int &ilen );
};

#endif // CSYSFILE_H
