

#ifndef _MENU_COM_PROTOCOL_H_
#define _MENU_COM_PROTOCOL_H_

#include "stm32f10x.h"

//���ָ����
#define		SERVO_COM_TEST_PULSE_UPPER     			210  //�������
#define 	SERVO_COM_TEST_PULSE_MID       			211  //�м�����
#define  	SERVO_COM_TEST_PULSE_LOWER     			212  //��С����

#define  	SERVO_COM_READ_WORK_PARM       			220  //��ȡ�����������
#define  	SERVO_COM_READ_DEBUG_PARM      			221  //��ȡ������ò���
#define  	SERVO_COM_READ_ALL_PARM        			222  //��ȡ������в���
#define  	SERVO_COM_CHECKSUM _ALL_PARM      	223	 //��ȡУ��������

#define  	SERVO_COM_WRITE_WORK_PARM       		230  //���ض����������
#define  	SERVO_COM_WRITE _DEBUG_PARM      		231  //���ض�����ò���
#define  	SERVO_COM_PROGRAM _ALL_PARM        	232  //���� ��ȡ У��

#define  	SERVO_COM_SERVO_RESET_ORGINAL       240	 //��λ�����������
#define  	SERVO_COM_FEEDBACK_ALL_PARM         270	 //����Һ���ڴ����

#endif
