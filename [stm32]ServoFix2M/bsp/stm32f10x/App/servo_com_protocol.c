

#include "servo_com_protocol.h"

//Unsigned int servo_command_now ȫ�ֱ��� usb����������
//Unsigned int servo_state_now ȫ�ֱ��� usb����������

void servo_command_test_plus_upper()
{
	
}
void servo_command_test_plus_mid()
{
	
}
void servo_command_test_plus_lower()
{
	
}

void servo_command_read_work_parm()
{
	
}
void servo_command_read_debug_parm()
{
	
}
void servo_command_read_all_parm()
{
	
}
void servo_command_checksum_all_parm()
{
	
}	

void servo_command_write_work_parm()
{
	
}
void servo_command_write_degug_parm()
{
	
}
void servo_command_program_all_parm()
{
	
}

void servo_command_servo_reset_orginal()
{
	
}
void servo_command_feedback_all_parm()
{
	
}

unsigned int servo_command_protocol(unsigned int servo_command_now)//��ɷ��� OK/ERROR
{
	switch (servo_command_now)
	{
		case SERVO_COM_TEST_PULSE_UPPER:
//			servo_command_test_plus_upper();
			break;
		case SERVO_COM_READ_ALL_PARM:
//			servo_command_read_all_parm();
			break;
		default:
			break;	
	}
	return 0;	
}
