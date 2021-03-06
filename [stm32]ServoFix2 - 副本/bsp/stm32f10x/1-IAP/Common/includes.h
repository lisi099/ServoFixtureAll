/******************** (C) COPYRIGHT 2016 ********************
* Filename:
* Description:
* Platform:
* Other info:
* Author         Date          Notes
xiangbin.huang   2017.02.21    The first version template
***************************************************************/
#ifndef INCLUDES_H
#define INCLUDES_H

#define     YES                             0
#define     NO                              1

#define     RTN_OK                          0   
#define     RTN_ERR                         1  

#define     OK                              0
#define     ERR                             1

#define     RECEVIEVE_DOING                 0
#define     RECEVIEVE_DONE                  1

#define     PASS                            0
#define     FAIL                            1

#define     DOING                           0
#define     DONE                            1

#define     TIMEOUT                         1   
#define     NOTTIMEOUT                      0   

#define     HIGH_LEVEL                      1   
#define     LOW_LEVEL                       0  

#define     KEY_UP                          0   
#define     KEY_SHORT_PRESS                 2   
#define     KEY_LONG_PRESS                  1   
#define     KEY_DOWN                        3   
  
                                     
#define SETBIT(Data, Bit)                   ((Data)|=(1<<Bit))      
#define CLEARBIT(Data, Bit)                 ((Data)&=~(1<<Bit))     
#define READBIT(Data, Bit)                  ((Data>>Bit)%2)         
#define REVERBIT(Data,Bit)                  ((Data)^=(1<<Bit))      

                                  
#define GETHHBYTE(Data)                     ((u8)(Data>>24))       
#define GETHLBYTE(Data)                     ((u8)(Data>>16)&0xff)  
#define GETLHBYTE(Data)                     ((u8)(Data>>8)&0xff)   
#define GETLLBYTE(Data)                     ((u8)(Data&0xff))      

#define GETHBYTE(Data)                      ((u8)(Data>>8)&0xff)   
#define GETLBYTE(Data)                      ((u8)(Data&0xff))    

#define BYTETOINT(Byte0,Byte1,Byte2,Byte3)  ((Byte0<<24)|(Byte1<<16)|(Byte2<<8)|Byte3)                          
#define BYTETOSHORT(Byte0,Byte1)            ((Byte0<<8)|Byte1) 




#endif 



