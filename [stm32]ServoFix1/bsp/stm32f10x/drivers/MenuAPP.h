/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#ifndef _MenuAPP_H_BAB
#define _MenuAPP_H_BAB

#include "Menu.h"

extern struct PAGE mainPage;

void Edit_CallBack(u8 key);
void Common_CallBack(u8 key);
void Common_CallBack_rmb(u8 key);
void Load_CallBack(u8 key);

void mainPageCallBack(u8 key);
void Setting_CallBack(u8 key);
void Save_CallBack(u8 key);
void clear_CallBack(u8 key);

void para_500_callBack(u8 key);
void para_1000_callBack(u8 key);
void para_1500_callBack(u8 key);
void para_2000_callBack(u8 key);
void para_2500_callBack(u8 key);

void para_01_callBack(u8 key);
void para_02_callBack(u8 key);
void para_03_callBack(u8 key);

void read_CallBack(u8 key);
void program_CallBack(u8 key);
void download_CallBack(u8 key);
void verify_CallBack(u8 key);

void ble_CallBack(u8 key);
void info_CallBack(u8 key);
void none_CallBack(u8 key);

void usb_online_CallBack_B9600(u8 key);
void usb_online_CallBack_B38400(u8 key);
void usb_online_CallBack_B19200(u8 key);
void usb_online_CallBack_B115200(u8 key);

void usb_program_CallBack(u8 key);

void Load_servo_parameter(uint8_t data);

void copy_data_to_read_menu(void);
void copy_data_to_write_menu(void);
void copy_data_to_stru(void);
#endif
