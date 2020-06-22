
#ifndef _HOST_COM_PROTOCOL_
#define _HOST_COM_PROTOCOL_

#include "stm32f10x.h"

#define		HOST_COM_SERVO_TEST_PULSE_UPPER     				110  //�������
#define  	HOST_COM_SERVO_TEST_PULSE_MID       				111  //�м�����
#define  	HOST_COM_SERVO_TEST_PULSE_LOWER     				112  //��С����

#define  	HOST_COM_SERVO_READ_WORK_PARM       				120  //��ȡ�����������
#define  	HOST_COM_SERVO_READ_DEBUG_PARM      				121  //��ȡ������ò���
#define  	HOST_COM_SERVO_READ_ALL_PARM        				122  //��ȡ������в���
#define  	HOST_COM_SERVO_CHECKSUM _ALL_PARM      			123	 //��ȡУ��������

#define  	HOST_COM_SERVO_WRITE_WORK_PARM       				130  //���ض����������
#define  	HOST_COM_SERVO_WRITE _DEBUG_PARM      			131  //���ض�����ò���
#define  	HOST_COM_SERVO_PROGRAM _ALL_PARM        		132  //���� ��ȡ У��
#define  	HOST_COM_MEMORY_READ_WORK_PARM       				140  //��ȡ�ڴ湤������
#define  	HOST_COM_MEMORY_READ_DEBUG_PARM      				141  //��ȡ�ڴ����ò���
#define  	HOST_COM_MEMORY_READ_ALL_PARM        				142  //��ȡ�ڴ����в���
#define  	HOST_COM_MEMORY_CHECKSUM _ALL_PARM      		143	 //��ȡУ���ڴ����

#define  	HOST_COM_MEMORY_WRITE_WORK_PARM       			150  //�����ڴ湤������
#define  	HOST_COM_MEMORY_WRITE _DEBUG_PARM      			151  //�����ڴ����ò���
#define  	HOST_COM_MEMORY_PROGRAM _ALL_PARM       		152	 //���� ��ȡ У��

#define  	HOST_COM_SERVO_RESET_FACTORY                160	 //��λ����ͻ�����
#define  	HOST_COM_SERVO_RESET_ORGINAL                161	 //��λ�����������

#define  	HOST_COM_FEEDBACK_ALL_PARM                  170	 //����Һ���ڴ����


#endif