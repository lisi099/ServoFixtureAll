
#ifndef _MENU_COM_PROTOCOL_H_
#define _MENU_COM_PROTOCOL_H_

#include "stm32f10x.h"

#define		MENU_COM_SERVO_TEST_PULSE_UPPER     		10  //���ж�����������
#define		MENU_COM_SERVO_TEST_PULSE_MID       		11  //���ж�����м�����
#define		MENU_COM_SERVO_TEST_PULSE_LOWER     		12  //���ж������С����

#define 	MENU_COM_SERVO_READ_WORK_PARM       		20  //��ȡ�����������
#define  	MENU_COM_SERVO_READ_DEBUG_PARM      		21  //��ȡ������ò���
#define  	MENU_COM_SERVO_READ_ALL_PARM        		22  //��ȡ������в���
#define  	MENU_COM_SERVO_CHECKSUM_ALL_PARM      	23	//��ȡУ��������

#define  	MENU_COM_SERVO_WRITE_WORK_PARM       		30  //���ض����������
#define  	MENU_COM_SERVO_WRITE_DEBUG_PARM      		31  //���ض�����ò���
#define  	MENU_COM_SERVO_PROGRAM_ALL_PARM        	32 	//���� ��ȡ У��

#define  	MENU_COM_MEMORY_READ_WORK_PARM       		40  //��ȡ�ڴ湤������
#define  	MENU_COM_MEMORY_READ_DEBUG_PARM      		41  //��ȡ�ڴ����ò���
#define  	MENU_COM_MEMORY_READ_ALL_PARM        		42  //��ȡ�ڴ����в���
#define  	MENU_COM_MEMORY_CHECKSUM_ALL_PARM      	43	//��ȡУ���ڴ����

#define  	MENU_COM_MEMORY_WRITE_WORK_PARM       	50  //�����ڴ湤������
#define  	MENU_COM_MEMORY_WRITE_DEBUG_PARM      	51  //�����ڴ����ò���
#define  	MENU_COM_MEMORY_PROGRAM_ALL_PARM       	52	//���� ��ȡ У��

#define  	MENU_COM_SERVO_RESET_FACTORY            60	//��λ����ͻ�����
#define  	MENU_COM_SERVO_RESET_ORGINAL            61	//��λ�����������


unsigned int menu_disp_protocol(unsigned char key_value_now, unsigned char menu_disp_page_now);//��ɷ��� OK/ERROR	

#endif

