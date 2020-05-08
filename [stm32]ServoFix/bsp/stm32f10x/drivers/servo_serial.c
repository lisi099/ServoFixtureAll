/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#include "servo_serial.h"
#include <rtthread.h>
#include "usart2.h"
#include "string.h"
#include "flash.h"
#include "board_info.h"

uint8_t uart_write_datas[12];
uint8_t uart_read_datas[12];
volatile uint8_t receive_uart_data_flag = 0;

struct Servo_Data_Stru_ servoDataStru;

uint8_t data_in_flash[PAGE_SIZE] ={0};
#define SERVO_DELAY_TIME	50

//-----------------------------------------------------------------------------------------
void uart_receive_data(uint8_t data)
{
	static  uint8_t uart_receive_state = 0;
	static 	uint8_t data_pos = 0;
	
	switch(uart_receive_state){
        case 0:
            if(data == 0xFA){ 
				uart_receive_state =1;
				data_pos = 0;
				uart_read_datas[data_pos++] =data;
			}
            break;
        case 1:
            uart_read_datas[data_pos++] =data;
            if(data_pos >= 12){
                uart_receive_state = 0;
//				if((uart_read_datas[11] ==0xFE) && (data_pos ==12) && (receive_uart_data_flag==0)){
				if((uart_read_datas[11] ==0xFE) && (data_pos ==12)){
					receive_uart_data_flag = 1;
				}
            }
            break;
        default:
            break;
	}
}

void uart_send_command(uint8_t id, uint8_t cmd, uint8_t state, uint8_t para_id1, uint16_t para1, uint8_t para_id2, uint16_t para2)
{
	uint8_t temp;
	
	uart_write_datas[0]		= 0xfa;
	uart_write_datas[1] 	= id;
	uart_write_datas[2] 	= cmd;
	uart_write_datas[3] 	= state;
	uart_write_datas[4] 	= para_id1;
	uart_write_datas[5] 	= (uint8_t)(para1 >>8);
	uart_write_datas[6]  	= (uint8_t)(para1);
	uart_write_datas[7] 	= para_id2;
	uart_write_datas[8] 	= (uint8_t)(para2 >>8);
	uart_write_datas[9] 	= (uint8_t)(para2);
//	temp = cmd +state +para_id1 +para_id2;//校验码不能超过250
	temp = cmd +state +para_id1;//校验码不能超过250
	uart_write_datas[10] = temp;
	uart_write_datas[11] = 0xfe;
	
	usart2_send_buff(uart_write_datas, sizeof(uart_write_datas));
	memset(uart_write_datas, 0, sizeof(uart_write_datas));
}


void uart_send_clear_command(void)
{
	uart_write_datas[0]		= 0xfa;
	uart_write_datas[1] 	= 0x55;
	uart_write_datas[2] 	= 0x55;
	uart_write_datas[3] 	= 0x55;
	uart_write_datas[4] 	= 0x55;
	uart_write_datas[5] 	= 0x55;
	uart_write_datas[6]  	= 0x55;
	uart_write_datas[7] 	= 0x55;
	uart_write_datas[8] 	= 0x55;
	uart_write_datas[9] 	= 0x55;
	uart_write_datas[10] 	= 0x55;
	uart_write_datas[11] 	= 0xfe;
	usart2_send_buff(uart_write_datas, sizeof(uart_write_datas));
	memset(uart_write_datas, 0, sizeof(uart_write_datas));
}

void uart_send_common_command(void)
{
    uart_write_datas[0]		= 0xfa;
    uart_write_datas[1] 	= 0x10;
    uart_write_datas[2] 	= 0x12;
    uart_write_datas[3] 	= 0x01;
    uart_write_datas[4] 	= 0x00;
    uart_write_datas[5] 	= 0x00;
    uart_write_datas[6]  	= 0x00;
    uart_write_datas[7] 	= 0x00;
    uart_write_datas[8] 	= 0x00;
    uart_write_datas[9] 	= 0x00;
    uart_write_datas[10] 	= 0x13;
    uart_write_datas[11] 	= 0xfe;
    usart2_send_buff(uart_write_datas, sizeof(uart_write_datas));
    memset(uart_write_datas, 0, sizeof(uart_write_datas));
}

void uart_send_p_command(void)
{
	uart_write_datas[0]		= 0xfa;
	uart_write_datas[1] 	= 0x10;
	uart_write_datas[2] 	= 0x10;
	uart_write_datas[3] 	= 0x01;
	uart_write_datas[4] 	= 0x00;
	uart_write_datas[5] 	= 0x00;
	uart_write_datas[6]  	= 0x00;
	uart_write_datas[7] 	= 0x00;
	uart_write_datas[8] 	= 0x00;
	uart_write_datas[9] 	= 0x00;
	uart_write_datas[10] 	= 0x11;
	uart_write_datas[11] 	= 0xfe;
	usart2_send_buff(uart_write_datas, sizeof(uart_write_datas));
	memset(uart_write_datas, 0, sizeof(uart_write_datas));
}

#define SAVE_DATA_SIZE		128
#define DATA_NUM_OF_PAGE 	(PAGE_SIZE/SAVE_DATA_SIZE)

void save_servo_data_in_flash(uint8_t seq)
{
	uint32_t page_start;
	uint32_t *data;
	uint32_t i =0;
	uint8_t num_page = seq /DATA_NUM_OF_PAGE;
	uint8_t seq_page = seq %DATA_NUM_OF_PAGE;
	
	page_start = FLASH_DATA1_ADDR + num_page*PAGE_SIZE;
	//read
	flash_read_n_byte(page_start, data_in_flash, sizeof(data_in_flash));
	//erase
	erase_flash_part(page_start, page_start +PAGE_SIZE);
	//data
	memcpy(&data_in_flash[seq_page*SAVE_DATA_SIZE], &servoDataStru, sizeof(servoDataStru));
	//program
	for(i=0; i< PAGE_SIZE; ){
		data = (uint32_t *)&data_in_flash[i];
		flash_write_one_word(page_start +i,  *data);
		i+=4;
	}
}

void read_servo_data_in_flash(uint8_t seq, struct Servo_Data_Stru_ *data)
{
	uint32_t page_start;
	uint8_t num_page = seq /DATA_NUM_OF_PAGE;
	uint8_t seq_page = seq %DATA_NUM_OF_PAGE;
	page_start = FLASH_DATA1_ADDR + num_page*PAGE_SIZE;
	//read
	flash_read_n_byte(page_start, data_in_flash, sizeof(data_in_flash));
	//cpy
	memcpy(data, &data_in_flash[seq_page*SAVE_DATA_SIZE], sizeof(servoDataStru));
}

void test_falsh_progrm(void)
{
//	uint8_t i =0;
//	
//	for(i=0; i<5; i++){
//		save_servo_data_in_flash(i);
//	}
//	for(i=0; i<5; i++){
//		read_servo_data_in_flash(i, &servoDataReadStru[i]);
//	}
}
/*---------------------------------------------------------------------------*/
uint16_t 	servo_unique_address_id_set 	= 16;
uint16_t 	servo_unique_address_id 		= 16;

void menu_combine_position(uint16_t pos)
{
	uart_send_clear_command();
	rt_thread_delay(SERVO_DELAY_TIME);

	uart_send_command(servo_unique_address_id, 0x0A, 0x01, 0x96, pos, 0x97, 266);
	rt_thread_delay(SERVO_DELAY_TIME);
	
	uart_send_p_command();
	rt_thread_delay(SERVO_DELAY_TIME);
}

void menu_combine_prom_work_parm(void)
{
	uint8_t i =0;
	int16_t temp_param;
	int16_t temp_param1;
	struct Servo_Data_Stru_ servo_data;
	memcpy(&servo_data, &servoDataStru, sizeof(servo_data));
	
	uart_send_clear_command();
    rt_thread_delay(SERVO_DELAY_TIME);
    uart_send_common_command();

	////////////////////////////////////////////////////////////////////////////////////////
	for(i=0; i<32; i++){
		rt_thread_delay(SERVO_DELAY_TIME);
		switch(i){
			//work param
			case 0:
				temp_param = MENU_WORK_SERVO_PARM0;
				temp_param1 = servo_data.work_p0;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 1:
				temp_param = MENU_WORK_SERVO_PARM1;
				temp_param1 = servo_data.work_p1;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 2:
				temp_param = MENU_WORK_SERVO_PARM2;
				temp_param1 = servo_data.work_p2;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 3:
				temp_param = MENU_WORK_SERVO_PARM3;
				temp_param1 = servo_data.work_p3;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 4:
				temp_param = MENU_WORK_SERVO_PARM4;
				temp_param1 = servo_data.work_p4;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 5:
				temp_param = MENU_WORK_SERVO_PARM5;
				temp_param1 = servo_data.work_p5;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 6:
				temp_param = MENU_WORK_SERVO_PARM6;
				temp_param1 = servo_data.work_p6;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 7:
				temp_param = MENU_WORK_SERVO_PARM7;
				temp_param1 = servo_data.work_p7;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 8:
				temp_param = MENU_WORK_SERVO_PARM8;
				temp_param1 = servo_data.work_p8;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 9:
				temp_param = MENU_WORK_SERVO_PARM9;
				temp_param1 = servo_data.work_p9;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 10:
				temp_param = MENU_WORK_SERVO_PARM10;
				temp_param1 = servo_data.work_p10;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 11:
				temp_param = MENU_WORK_SERVO_PARM11;
				temp_param1 = servo_data.work_p11;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 12:
				temp_param = MENU_WORK_SERVO_PARM12;
				temp_param1 = servo_data.work_p12;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 13:
				temp_param = MENU_WORK_SERVO_PARM13;
				temp_param1 = servo_data.work_p13;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 14:
				temp_param = MENU_WORK_SERVO_PARM14;
				temp_param1 = servo_data.work_p14;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 15:
				temp_param = MENU_WORK_SERVO_PARM15;
				temp_param1 = servo_data.work_p15;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			//setup param
			case 16:
				temp_param = MENU_SETUP_SERVO_PARM0;
				temp_param1 = servo_data.set_p0;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 17:
				temp_param = MENU_SETUP_SERVO_PARM1;
				temp_param1 = servo_data.set_p1;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 18:
				temp_param = MENU_SETUP_SERVO_PARM2;
				temp_param1 = servo_data.set_p2;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 19:
				temp_param = MENU_SETUP_SERVO_PARM3;
				temp_param1 = servo_data.set_p3;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 20:
				temp_param = MENU_SETUP_SERVO_PARM4;
				temp_param1 = servo_data.set_p4;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 21:
				temp_param = MENU_SETUP_SERVO_PARM5;
				temp_param1 = servo_data.set_p5;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 22:
				temp_param = MENU_SETUP_SERVO_PARM6;
				temp_param1 = servo_data.set_p6;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 23:
				temp_param = MENU_SETUP_SERVO_PARM7;
				temp_param1 = servo_data.set_p7;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 24:
				temp_param = MENU_SETUP_SERVO_PARM8;
				temp_param1 = servo_data.set_p8;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 25:
				temp_param = MENU_SETUP_SERVO_PARM9;
				temp_param1 = servo_data.set_p9;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 26:
				temp_param = MENU_SETUP_SERVO_PARM10;
				temp_param1 = servo_data.set_p10;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 27:
				temp_param = MENU_SETUP_SERVO_PARM11;
				temp_param1 = servo_data.set_p11;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 28:
				temp_param = MENU_SETUP_SERVO_PARM12;
				temp_param1 = servo_data.set_p12;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 29:
				temp_param = MENU_SETUP_SERVO_PARM13;
				temp_param1 = servo_data.set_p13;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 30:
				temp_param = MENU_SETUP_SERVO_PARM14;
				temp_param1 = servo_data.set_p14;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 31:
				temp_param = MENU_SETUP_SERVO_PARM15;
				temp_param1 = servo_data.set_p15;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			default:
				break;
		}
	}
	rt_thread_delay(SERVO_DELAY_TIME);
	uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_DOWNLOAD, SERVO_STATE_COM, MENU_DOWMLOAD_WORK_PARM, 0, 0, 0);
	rt_thread_delay(500);
	for(i=0; i<11; i++){
		rt_thread_delay(SERVO_DELAY_TIME);
		switch(i){
			case 0:
				temp_param = MENU_DEBUG_SERVO_PARM0;
				temp_param1 = servo_data.debug_p0;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_DEBUG, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 1:
				temp_param = MENU_DEBUG_SERVO_PARM1;
				temp_param1 = servo_data.debug_p1;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_DEBUG, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 2:
				temp_param = MENU_DEBUG_SERVO_PARM2;
				temp_param1 = servo_data.debug_p2;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_DEBUG, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 3:
				temp_param = MENU_DEBUG_SERVO_PARM3;
				temp_param1 = servo_data.debug_p3;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_DEBUG, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 4:
				temp_param = MENU_DEBUG_SERVO_PARM4;
				temp_param1 = servo_data.debug_p4;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_DEBUG, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 5:
				temp_param = MENU_DEBUG_SERVO_PARM5;
				temp_param1 = servo_data.debug_p5;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_DEBUG, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 6:
				temp_param = MENU_DEBUG_SERVO_PARM6;
				temp_param1 = servo_data.debug_p6;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_DEBUG, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 7:
				temp_param = MENU_DEBUG_SERVO_PARM7;
				temp_param1 = servo_data.debug_p7;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_DEBUG, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 8:
				temp_param = MENU_DEBUG_SERVO_PARM8;
				temp_param1 = servo_data.debug_p8;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_DEBUG, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 9:
				temp_param = MENU_DEBUG_SERVO_PARM9;
				temp_param1 = servo_data.debug_p9;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_DEBUG, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 10:
				temp_param = MENU_DEBUG_SERVO_PARM10;
				temp_param1 = servo_data.debug_p10;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_DEBUG, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
		}
	}
	rt_thread_delay(SERVO_DELAY_TIME);
	uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_DOWNLOAD, SERVO_STATE_COM, MENU_DOWMLOAD_DEBUG_PARM, 0, 0, 0);
    rt_thread_delay(50);
    uart_send_p_command();
	rt_thread_delay(5);
}

void menu_combine_prom_work_clear_parm(void)
{
	uint8_t i =0;
	int16_t temp_param;
	int16_t temp_param1;
	struct Servo_Data_Stru_ servo_data;
	memcpy(&servo_data, &servoDataStru, sizeof(servo_data));
	
    uart_send_clear_command();
    rt_thread_delay(SERVO_DELAY_TIME);
    uart_send_common_command();
	////////////////////////////////////////////////////////////////////////////////////////
	for(i=0; i<32; i++){
		rt_thread_delay(SERVO_DELAY_TIME);
		switch(i){
			//work param
			case 0:
				temp_param = MENU_WORK_SERVO_PARM0;
				temp_param1 = servo_data.work_p0;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 1:
				temp_param = MENU_WORK_SERVO_PARM1;
				temp_param1 = servo_data.work_p1;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 2:
				temp_param = MENU_WORK_SERVO_PARM2;
				temp_param1 = servo_data.work_p2;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 3:
				temp_param = MENU_WORK_SERVO_PARM3;
				temp_param1 = servo_data.work_p3;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 4:
				temp_param = MENU_WORK_SERVO_PARM4;
				temp_param1 = servo_data.work_p4;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 5:
				temp_param = MENU_WORK_SERVO_PARM5;
				temp_param1 = servo_data.work_p5;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 6:
				temp_param = MENU_WORK_SERVO_PARM6;
				temp_param1 = servo_data.work_p6;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 7:
				temp_param = MENU_WORK_SERVO_PARM7;
				temp_param1 = servo_data.work_p7;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 8:
				temp_param = MENU_WORK_SERVO_PARM8;
				temp_param1 = servo_data.work_p8;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 9:
				temp_param = MENU_WORK_SERVO_PARM9;
				temp_param1 = servo_data.work_p9;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 10:
				temp_param = MENU_WORK_SERVO_PARM10;
				temp_param1 = servo_data.work_p10;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 11:
				temp_param = MENU_WORK_SERVO_PARM11;
				temp_param1 = servo_data.work_p11;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 12:
				temp_param = MENU_WORK_SERVO_PARM12;
				temp_param1 = servo_data.work_p12;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 13:
				temp_param = MENU_WORK_SERVO_PARM13;
				temp_param1 = 0;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 14:
				temp_param = MENU_WORK_SERVO_PARM14;
				temp_param1 = servo_data.work_p14;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 15:
				temp_param = MENU_WORK_SERVO_PARM15;
				temp_param1 = servo_data.work_p15;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			//setup param
			case 16:
				temp_param = MENU_SETUP_SERVO_PARM0;
				temp_param1 = servo_data.set_p0;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 17:
				temp_param = MENU_SETUP_SERVO_PARM1;
				temp_param1 = servo_data.set_p1;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 18:
				temp_param = MENU_SETUP_SERVO_PARM2;
				temp_param1 = servo_data.set_p2;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 19:
				temp_param = MENU_SETUP_SERVO_PARM3;
				temp_param1 = servo_data.set_p3;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 20:
				temp_param = MENU_SETUP_SERVO_PARM4;
				temp_param1 = servo_data.set_p4;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 21:
				temp_param = MENU_SETUP_SERVO_PARM5;
				temp_param1 = servo_data.set_p5;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 22:
				temp_param = MENU_SETUP_SERVO_PARM6;
				temp_param1 = servo_data.set_p6;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 23:
				temp_param = MENU_SETUP_SERVO_PARM7;
				temp_param1 = servo_data.set_p7;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 24:
				temp_param = MENU_SETUP_SERVO_PARM8;
				temp_param1 = servo_data.set_p8;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 25:
				temp_param = MENU_SETUP_SERVO_PARM9;
				temp_param1 = servo_data.set_p9;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 26:
				temp_param = MENU_SETUP_SERVO_PARM10;
				temp_param1 = servo_data.set_p10;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 27:
				temp_param = MENU_SETUP_SERVO_PARM11;
				temp_param1 = servo_data.set_p11;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 28:
				temp_param = MENU_SETUP_SERVO_PARM12;
				temp_param1 = servo_data.set_p12;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 29:
				temp_param = MENU_SETUP_SERVO_PARM13;
				temp_param1 = servo_data.set_p13;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 30:
				temp_param = MENU_SETUP_SERVO_PARM14;
				temp_param1 = servo_data.set_p14;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 31:
				temp_param = MENU_SETUP_SERVO_PARM15;
				temp_param1 = servo_data.set_p15;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			default:
				break;
		}
	}
	rt_thread_delay(SERVO_DELAY_TIME);
	uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_DOWNLOAD, SERVO_STATE_COM, MENU_DOWMLOAD_WORK_PARM, 0, 0, 0);
	rt_thread_delay(500);
	for(i=0; i<11; i++){
		rt_thread_delay(SERVO_DELAY_TIME);
		switch(i){
			case 0:
				temp_param = MENU_DEBUG_SERVO_PARM0;
				temp_param1 = servo_data.debug_p0;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_DEBUG, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 1:
				temp_param = MENU_DEBUG_SERVO_PARM1;
				temp_param1 = servo_data.debug_p1;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_DEBUG, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 2:
				temp_param = MENU_DEBUG_SERVO_PARM2;
				temp_param1 = servo_data.debug_p2;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_DEBUG, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 3:
				temp_param = MENU_DEBUG_SERVO_PARM3;
				temp_param1 = servo_data.debug_p3;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_DEBUG, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 4:
				temp_param = MENU_DEBUG_SERVO_PARM4;
				temp_param1 = servo_data.debug_p4;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_DEBUG, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 5:
				temp_param = MENU_DEBUG_SERVO_PARM5;
				temp_param1 = servo_data.debug_p5;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_DEBUG, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 6:
				temp_param = MENU_DEBUG_SERVO_PARM6;
				temp_param1 = servo_data.debug_p6;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_DEBUG, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 7:
				temp_param = MENU_DEBUG_SERVO_PARM7;
				temp_param1 = servo_data.debug_p7;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_DEBUG, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 8:
				temp_param = MENU_DEBUG_SERVO_PARM8;
				temp_param1 = servo_data.debug_p8;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_DEBUG, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 9:
				temp_param = MENU_DEBUG_SERVO_PARM9;
				temp_param1 = servo_data.debug_p9;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_DEBUG, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
			case 10:
				temp_param = MENU_DEBUG_SERVO_PARM10;
				temp_param1 = servo_data.debug_p10;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_DEBUG, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
				break;
		}
	}
	rt_thread_delay(SERVO_DELAY_TIME);
	uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_DOWNLOAD, SERVO_STATE_COM, MENU_DOWMLOAD_DEBUG_PARM, 0, 0, 0);
    rt_thread_delay(50);
    uart_send_p_command();
	rt_thread_delay(5);
}

uint8_t menu_combine_fb_work_parm(void)
{
	uint8_t i =0;
	uint8_t time_count;
	int16_t temp_param;
	uint8_t chech_sum;
	struct Servo_Data_Stru_ servo_data;
	
    uart_send_clear_command();
    rt_thread_delay(SERVO_DELAY_TIME);
    uart_send_common_command();
	////////////////////////////////////////////////////////////////////////////////////////
	for(i=0; i<43; i++){
		receive_uart_data_flag =0;
		rt_thread_delay(SERVO_DELAY_TIME);
		switch(i){
			//work param
			case 0:
				temp_param = MENU_FB_SERVO_WORK_PARM0;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, MENU_FB_SERVO_WORK_PARM0, 0, 0, 0);
				break;
			case 1:
				temp_param = MENU_FB_SERVO_WORK_PARM1;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, MENU_FB_SERVO_WORK_PARM1, 0, 0, 0);
				break;
			case 2:
				temp_param = MENU_FB_SERVO_WORK_PARM2;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, MENU_FB_SERVO_WORK_PARM2, 0, 0, 0);
				break;
			case 3:
				temp_param = MENU_FB_SERVO_WORK_PARM3;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, MENU_FB_SERVO_WORK_PARM3, 0, 0, 0);
				break;
			case 4:
				temp_param = MENU_FB_SERVO_WORK_PARM4;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, MENU_FB_SERVO_WORK_PARM4, 0, 0, 0);
				break;
			case 5:
				temp_param = MENU_FB_SERVO_WORK_PARM5;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, MENU_FB_SERVO_WORK_PARM5, 0, 0, 0);
				break;
			case 6:
				temp_param = MENU_FB_SERVO_WORK_PARM6;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, MENU_FB_SERVO_WORK_PARM6, 0, 0, 0);
				break;
			case 7:
				temp_param = MENU_FB_SERVO_WORK_PARM7;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, MENU_FB_SERVO_WORK_PARM7, 0, 0, 0);
				break;
			case 8:
				temp_param = MENU_FB_SERVO_WORK_PARM8;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, MENU_FB_SERVO_WORK_PARM8, 0, 0, 0);
				break;
			case 9:
				temp_param = MENU_FB_SERVO_WORK_PARM9;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, MENU_FB_SERVO_WORK_PARM9, 0, 0, 0);
				break;
			case 10:
				temp_param = MENU_FB_SERVO_WORK_PARM10;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, MENU_FB_SERVO_WORK_PARM10, 0, 0, 0);
				break;
			case 11:
				temp_param = MENU_FB_SERVO_WORK_PARM11;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, MENU_FB_SERVO_WORK_PARM11, 0, 0, 0);
				break;
			case 12:
				temp_param = MENU_FB_SERVO_WORK_PARM12;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, MENU_FB_SERVO_WORK_PARM12, 0, 0, 0);
				break;
			case 13:
				temp_param = MENU_FB_SERVO_WORK_PARM13;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, MENU_FB_SERVO_WORK_PARM13, 0, 0, 0);
				break;
			case 14:
				temp_param = MENU_FB_SERVO_WORK_PARM14;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, MENU_FB_SERVO_WORK_PARM14, 0, 0, 0);
				break;
			case 15:
				temp_param = MENU_FB_SERVO_WORK_PARM15;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, MENU_FB_SERVO_WORK_PARM15, 0, 0, 0);
				break;
			//setup param
			case 16:
				temp_param = MENU_FB_SERVO_SETUP_PARM0;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 17:
				temp_param = MENU_FB_SERVO_SETUP_PARM1;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 18:
				temp_param = MENU_FB_SERVO_SETUP_PARM2;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 19:
				temp_param = MENU_FB_SERVO_SETUP_PARM3;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 20:
				temp_param = MENU_FB_SERVO_SETUP_PARM4;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 21:
				temp_param = MENU_FB_SERVO_SETUP_PARM5;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 22:
				temp_param = MENU_FB_SERVO_SETUP_PARM6;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 23:
				temp_param = MENU_FB_SERVO_SETUP_PARM7;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 24:
				temp_param = MENU_FB_SERVO_SETUP_PARM8;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 25:
				temp_param = MENU_FB_SERVO_SETUP_PARM9;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 26:
				temp_param = MENU_FB_SERVO_SETUP_PARM10;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 27:
				temp_param = MENU_FB_SERVO_SETUP_PARM11;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 28:
				temp_param = MENU_FB_SERVO_SETUP_PARM12;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 29:
				temp_param = MENU_FB_SERVO_SETUP_PARM13;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 30:
				temp_param = MENU_FB_SERVO_SETUP_PARM14;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 31:
				temp_param = MENU_FB_SERVO_SETUP_PARM15;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 32:
				temp_param = MENU_FB_SERVO_DEBUG_PARM0;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 33:
				temp_param = MENU_FB_SERVO_DEBUG_PARM1;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 34:
				temp_param = MENU_FB_SERVO_DEBUG_PARM2;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 35:
				temp_param = MENU_FB_SERVO_DEBUG_PARM3;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 36:
				temp_param = MENU_FB_SERVO_DEBUG_PARM4;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 37:
				temp_param = MENU_FB_SERVO_DEBUG_PARM5;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 38:
				temp_param = MENU_FB_SERVO_DEBUG_PARM6;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 39:
				temp_param = MENU_FB_SERVO_CONFIG_PARM0;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 40:
				temp_param = MENU_FB_SERVO_CONFIG_PARM1;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 41:
				temp_param = MENU_FB_SERVO_CONFIG_PARM2;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 42:
				temp_param = MENU_FB_SERVO_CONFIG_PARM3;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			default:
				break;
		}
		//wait for feedback datas
		time_count = 0;
		while(receive_uart_data_flag == 0)
		{
			time_count++;
			if(time_count >200){ //200ms
				return 0; //erro
			}
			rt_thread_delay(1); //0.001 s
		}
		//process datas
		receive_uart_data_flag =0;
		chech_sum = uart_read_datas[2] + uart_read_datas[3] + uart_read_datas[4];
		if(chech_sum != uart_read_datas[10]){
			return 0;
		}
		if(uart_read_datas[3] != SERVO_STATE_FB){
			return 0;
		}
		if(uart_read_datas[4] != temp_param){
			return 0;
		}
		switch(i){
			case 0:
				servo_data.work_p0 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 1:
				servo_data.work_p1 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 2:
				servo_data.work_p2 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 3:
				servo_data.work_p3 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 4:
				servo_data.work_p4 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 5:
				servo_data.work_p5 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 6:
				servo_data.work_p6 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 7:
				servo_data.work_p7 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 8:
				servo_data.work_p8 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 9:
				servo_data.work_p9 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 10:
				servo_data.work_p10 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 11:
				servo_data.work_p11 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 12:
				servo_data.work_p12 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 13:
				servo_data.work_p13 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 14:
				servo_data.work_p14 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 15:
				servo_data.work_p15 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 16:
				servo_data.set_p0 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 17:
				servo_data.set_p1 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 18:
				servo_data.set_p2 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 19:
				servo_data.set_p3 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 20:
				servo_data.set_p4 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 21:
				servo_data.set_p5 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 22:
				servo_data.set_p6 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 23:
				servo_data.set_p7 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 24:
				servo_data.set_p8 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 25:
				servo_data.set_p9 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 26:
				servo_data.set_p10 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 27:
				servo_data.set_p11 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 28:
				servo_data.set_p12 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 29:
				servo_data.set_p13 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 30:
				servo_data.set_p14 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 31:
				servo_data.set_p15 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 32:
				servo_data.debug_p0 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 33:
				servo_data.debug_p1 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 34:
				servo_data.debug_p2 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 35:
				servo_data.debug_p3 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 36:
				servo_data.debug_p4 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 37:
				servo_data.debug_p5 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 38:
				servo_data.debug_p6 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 39:
				servo_data.debug_p7 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 40:
				servo_data.debug_p8 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 41:
				servo_data.debug_p9 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 42:
				servo_data.debug_p10 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			default:
				break;
		}
	}
	memcpy(&servoDataStru, &servo_data, sizeof(servo_data));
	rt_thread_delay(5);
	return 1;
}


uint8_t menu_combine_verify_work_parm(void)
{
	uint8_t i =0;
	uint16_t j=0;
	uint8_t *buff1, *buff2;
	uint8_t time_count;
	int16_t temp_param;
	uint8_t chech_sum;
	struct Servo_Data_Stru_ servo_data;
	
    uart_send_clear_command();
    rt_thread_delay(SERVO_DELAY_TIME);
    uart_send_common_command();
	////////////////////////////////////////////////////////////////////////////////////////
	for(i=0; i<43; i++){
		receive_uart_data_flag =0;
		rt_thread_delay(SERVO_DELAY_TIME);
		switch(i){
			//work param
			case 0:
				temp_param = MENU_FB_SERVO_WORK_PARM0;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, MENU_FB_SERVO_WORK_PARM0, 0, 0, 0);
				break;
			case 1:
				temp_param = MENU_FB_SERVO_WORK_PARM1;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, MENU_FB_SERVO_WORK_PARM1, 0, 0, 0);
				break;
			case 2:
				temp_param = MENU_FB_SERVO_WORK_PARM2;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, MENU_FB_SERVO_WORK_PARM2, 0, 0, 0);
				break;
			case 3:
				temp_param = MENU_FB_SERVO_WORK_PARM3;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, MENU_FB_SERVO_WORK_PARM3, 0, 0, 0);
				break;
			case 4:
				temp_param = MENU_FB_SERVO_WORK_PARM4;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, MENU_FB_SERVO_WORK_PARM4, 0, 0, 0);
				break;
			case 5:
				temp_param = MENU_FB_SERVO_WORK_PARM5;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, MENU_FB_SERVO_WORK_PARM5, 0, 0, 0);
				break;
			case 6:
				temp_param = MENU_FB_SERVO_WORK_PARM6;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, MENU_FB_SERVO_WORK_PARM6, 0, 0, 0);
				break;
			case 7:
				temp_param = MENU_FB_SERVO_WORK_PARM7;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, MENU_FB_SERVO_WORK_PARM7, 0, 0, 0);
				break;
			case 8:
				temp_param = MENU_FB_SERVO_WORK_PARM8;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, MENU_FB_SERVO_WORK_PARM8, 0, 0, 0);
				break;
			case 9:
				temp_param = MENU_FB_SERVO_WORK_PARM9;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, MENU_FB_SERVO_WORK_PARM9, 0, 0, 0);
				break;
			case 10:
				temp_param = MENU_FB_SERVO_WORK_PARM10;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, MENU_FB_SERVO_WORK_PARM10, 0, 0, 0);
				break;
			case 11:
				temp_param = MENU_FB_SERVO_WORK_PARM11;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, MENU_FB_SERVO_WORK_PARM11, 0, 0, 0);
				break;
			case 12:
				temp_param = MENU_FB_SERVO_WORK_PARM12;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, MENU_FB_SERVO_WORK_PARM12, 0, 0, 0);
				break;
			case 13:
				temp_param = MENU_FB_SERVO_WORK_PARM13;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, MENU_FB_SERVO_WORK_PARM13, 0, 0, 0);
				break;
			case 14:
				temp_param = MENU_FB_SERVO_WORK_PARM14;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, MENU_FB_SERVO_WORK_PARM14, 0, 0, 0);
				break;
			case 15:
				temp_param = MENU_FB_SERVO_WORK_PARM15;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, MENU_FB_SERVO_WORK_PARM15, 0, 0, 0);
				break;
			//setup param
			case 16:
				temp_param = MENU_FB_SERVO_SETUP_PARM0;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 17:
				temp_param = MENU_FB_SERVO_SETUP_PARM1;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 18:
				temp_param = MENU_FB_SERVO_SETUP_PARM2;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 19:
				temp_param = MENU_FB_SERVO_SETUP_PARM3;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 20:
				temp_param = MENU_FB_SERVO_SETUP_PARM4;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 21:
				temp_param = MENU_FB_SERVO_SETUP_PARM5;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 22:
				temp_param = MENU_FB_SERVO_SETUP_PARM6;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 23:
				temp_param = MENU_FB_SERVO_SETUP_PARM7;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 24:
				temp_param = MENU_FB_SERVO_SETUP_PARM8;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 25:
				temp_param = MENU_FB_SERVO_SETUP_PARM9;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 26:
				temp_param = MENU_FB_SERVO_SETUP_PARM10;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 27:
				temp_param = MENU_FB_SERVO_SETUP_PARM11;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 28:
				temp_param = MENU_FB_SERVO_SETUP_PARM12;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 29:
				temp_param = MENU_FB_SERVO_SETUP_PARM13;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 30:
				temp_param = MENU_FB_SERVO_SETUP_PARM14;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 31:
				temp_param = MENU_FB_SERVO_SETUP_PARM15;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 32:
				temp_param = MENU_FB_SERVO_DEBUG_PARM0;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 33:
				temp_param = MENU_FB_SERVO_DEBUG_PARM1;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 34:
				temp_param = MENU_FB_SERVO_DEBUG_PARM2;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 35:
				temp_param = MENU_FB_SERVO_DEBUG_PARM3;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 36:
				temp_param = MENU_FB_SERVO_DEBUG_PARM4;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 37:
				temp_param = MENU_FB_SERVO_DEBUG_PARM5;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 38:
				temp_param = MENU_FB_SERVO_DEBUG_PARM6;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 39:
				temp_param = MENU_FB_SERVO_CONFIG_PARM0;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 40:
				temp_param = MENU_FB_SERVO_CONFIG_PARM1;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 41:
				temp_param = MENU_FB_SERVO_CONFIG_PARM2;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			case 42:
				temp_param = MENU_FB_SERVO_CONFIG_PARM3;
				uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
				break;
			default:
				break;
		}
		//wait for feedback datas
		time_count = 0;
		while(receive_uart_data_flag == 0)
		{
			time_count++;
			if(time_count >200){ //200ms
				return 0; //erro
			}
			rt_thread_delay(1); //0.001 s
		}
		//process datas
		receive_uart_data_flag =0;
		chech_sum = uart_read_datas[2] + uart_read_datas[3] + uart_read_datas[4];
		if(chech_sum != uart_read_datas[10]){
			return 0;
		}
		if(uart_read_datas[3] != SERVO_STATE_FB){
			return 0;
		}
		if(uart_read_datas[4] != temp_param){
			return 0;
		}
		switch(i){
			case 0:
				servo_data.work_p0 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 1:
				servo_data.work_p1 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 2:
				servo_data.work_p2 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 3:
				servo_data.work_p3 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 4:
				servo_data.work_p4 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 5:
				servo_data.work_p5 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 6:
				servo_data.work_p6 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 7:
				servo_data.work_p7 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 8:
				servo_data.work_p8 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 9:
				servo_data.work_p9 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 10:
				servo_data.work_p10 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 11:
				servo_data.work_p11 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 12:
				servo_data.work_p12 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 13:
				servo_data.work_p13 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 14:
				servo_data.work_p14 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 15:
				servo_data.work_p15 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 16:
				servo_data.set_p0 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 17:
				servo_data.set_p1 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 18:
				servo_data.set_p2 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 19:
				servo_data.set_p3 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 20:
				servo_data.set_p4 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 21:
				servo_data.set_p5 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 22:
				servo_data.set_p6 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 23:
				servo_data.set_p7 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 24:
				servo_data.set_p8 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 25:
				servo_data.set_p9 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 26:
				servo_data.set_p10 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 27:
				servo_data.set_p11 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 28:
				servo_data.set_p12 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 29:
				servo_data.set_p13 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 30:
				servo_data.set_p14 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 31:
				servo_data.set_p15 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 32:
				servo_data.debug_p0 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 33:
				servo_data.debug_p1 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 34:
				servo_data.debug_p2 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 35:
				servo_data.debug_p3 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 36:
				servo_data.debug_p4 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 37:
				servo_data.debug_p5 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 38:
				servo_data.debug_p6 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 39:
				servo_data.debug_p7 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 40:
				servo_data.debug_p8 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 41:
				servo_data.debug_p9 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			case 42:
				servo_data.debug_p10 =  (uart_read_datas[9] | (uart_read_datas[8]<<8));
				break;
			default:
				break;
		}
	}
	buff1 = (uint8_t *)(&servo_data);
	buff2 = (uint8_t *)(&servoDataStru);
	for(j=0; j<sizeof(servo_data); j++){
		if(buff1[j] != buff2[j]){
			return 0;
		}
	}
    rt_thread_delay(50);
    uart_send_p_command();
	rt_thread_delay(5);
	return 1;
}
/******************************************************/
