

#include "eeprom_driver.h"
#include "memory_com_protocol.h"

void memory_command_read_work_parm()
{
	
}
void memory_command_read_debug_parm()
{
	
}
void memory_command_read_all_parm()
{
	
}
void memory_command_checksum_all_parm()
{
	
}

void memory_command_write_work_parm()
{
	
}
void memory_command_write_degug_parm()
{
	
}
void memory_command_program_all_parm()
{
	
}

void memory_address_decode(unsigned int memory_page_address);// memory_page_address = memory_logic_address + memory_ parm_address;

//void memory_read_data(memory_logic_address, memory_parm_address);
//void memory_write_data(memory_logic_address, memory_parm_address, memory_parm_value);

unsigned int memory_command_protocol(unsigned int memory_command_now)//Íê³É·´À¡ OK/ERROR
{
	switch (memory_command_now)
	{
		case MEMORY_COM_READ_WORK_PARM:
			memory_command_read_work_parm();
			break;
		case MEMORY_COM_PROGRAM_ALL_PARM:
			memory_command_program_all_parm();
			break;
		default:
			break;	
	}
	return 0;
}

