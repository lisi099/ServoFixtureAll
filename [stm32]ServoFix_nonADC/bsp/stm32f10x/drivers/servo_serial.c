/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#include "servo_serial.h"
#include <rtthread.h>
#include "usart2.h"
#include "string.h"
#include "flash.h"
#include "board_info.h"
#include "menu_app.h"
#include "factory_data.h"

#define SERVO_DELAY_TIME	50
#define SERVO_DELAY_TIME_S	5
#define SAVE_DATA_SIZE		128
#define DATA_NUM_OF_PAGE 	(PAGE_SIZE/SAVE_DATA_SIZE)

uint8_t uart_write_datas[12];
uint8_t uart_read_datas[12];
volatile uint8_t receive_uart_data_flag = 0;
struct Servo_Data_Stru_ servoDataStru;

uint16_t 	servo_unique_address_id_set 	= 16;
uint16_t 	servo_unique_address_id 		= 16;
extern volatile uint8_t  write_read_busy_state_;
//--------------------------------------
uint8_t data_in_flash[PAGE_SIZE] = {0};


void save_servo_data_in_flash(uint8_t seq, uint16_t version)
{
    uint32_t page_start;
    uint32_t* data;
    uint32_t i = 0;

    struct Servo_Data_Stru_ servo_data;
    memcpy(&servo_data, &servoDataStru, sizeof(servo_data));
    servo_data.work_p12 = version;

    uint8_t num_page = seq / DATA_NUM_OF_PAGE;
    uint8_t seq_page = seq % DATA_NUM_OF_PAGE;

    page_start = FLASH_DATA1_ADDR + num_page * PAGE_SIZE;
    //read
    flash_read_n_byte(page_start, data_in_flash, sizeof(data_in_flash));
    //erase
    erase_flash_part(page_start, page_start + PAGE_SIZE);

    //data
    memcpy(&data_in_flash[seq_page * SAVE_DATA_SIZE], &servo_data, sizeof(servo_data));

    //program
    for(i = 0; i < PAGE_SIZE;)
    {
        data = (uint32_t*)&data_in_flash[i];
        flash_write_one_word(page_start + i,  *data);
        i += 4;
    }
}

void read_servo_data_in_flash(uint8_t seq)
{
    uint32_t page_start;
    struct Servo_Data_Stru_  *data = &servoDataStru;

    if(seq < 20)
    {
        uint8_t num_page = seq / DATA_NUM_OF_PAGE;
        uint8_t seq_page = seq % DATA_NUM_OF_PAGE;
        page_start = FLASH_DATA1_ADDR + num_page * PAGE_SIZE;
        //read
        flash_read_n_byte(page_start, data_in_flash, sizeof(data_in_flash));
        //cpy
        memcpy(data, &data_in_flash[seq_page * SAVE_DATA_SIZE], sizeof(servoDataStru));
    }
    else
    {
        get_factory_data(data, seq - 20);
    }

    Copy_Data_To_Show();
}

uint8_t read_servo_data_in_flash_(uint8_t seq, struct Servo_Data_Stru_  *data)
{
    uint32_t page_start;
    if(seq < 20)
    {
        uint8_t num_page = seq / DATA_NUM_OF_PAGE;
        uint8_t seq_page = seq % DATA_NUM_OF_PAGE;
        page_start = FLASH_DATA1_ADDR + num_page * PAGE_SIZE;
        //read
        flash_read_n_byte(page_start, data_in_flash, sizeof(data_in_flash));
        //cpy
        memcpy(data, &data_in_flash[seq_page * SAVE_DATA_SIZE], sizeof(servoDataStru));
    }
		return 0;
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

//-----------------------------------------------------------------------------------------
void uart_receive_data(uint8_t data)
{
    static  uint8_t uart_receive_state = 0;
    static 	uint8_t data_pos = 0;

    switch(uart_receive_state)
    {
        case 0:
            if(data == 0xFA)
            {
                uart_receive_state = 1;
                data_pos = 0;
                uart_read_datas[data_pos++] = data;
            }

            break;

        case 1:
            uart_read_datas[data_pos++] = data;

            if(data_pos >= 12)
            {
                uart_receive_state = 0;

//				if((uart_read_datas[11] ==0xFE) && (data_pos ==12) && (receive_uart_data_flag==0)){
                if((uart_read_datas[11] == 0xFE) && (data_pos == 12))
                {
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
    uart_write_datas[5] 	= (uint8_t)(para1 >> 8);
    uart_write_datas[6]  	= (uint8_t)(para1);
    uart_write_datas[7] 	= para_id2;
    uart_write_datas[8] 	= (uint8_t)(para2 >> 8);
    uart_write_datas[9] 	= (uint8_t)(para2);
//	temp = cmd +state +para_id1 +para_id2;//校验码不能超过250
    temp = cmd + state + para_id1; //校验码不能超过250
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

void menu_combine_center(int16_t data)
{
    int16_t data_send;
	
    uart_send_clear_command();
    rt_thread_delay(SERVO_DELAY_TIME);
    data_send = 3100 + data * 28;
    uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, MENU_WORK_SERVO_PARM3, data_send, 0, 0);
    rt_thread_delay(SERVO_DELAY_TIME);

    data_send = 2048 + data * 28;
    uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, MENU_WORK_SERVO_PARM4, data_send, 0, 0);
    rt_thread_delay(SERVO_DELAY_TIME);

    data_send = 996 + data * 28;
    uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, MENU_WORK_SERVO_PARM5, data_send, 0, 0);
    rt_thread_delay(SERVO_DELAY_TIME);

    uart_send_p_command();
    rt_thread_delay(SERVO_DELAY_TIME);
		menu_combine_position(1500);
}

void menu_combine_position(uint16_t pos)
{
    uart_send_clear_command();
    rt_thread_delay(SERVO_DELAY_TIME);

    uart_send_command(servo_unique_address_id, 0x0A, 0x01, MENU_SERVO_RUN_POSITION_VALUE, pos, MENU_SERVO_RUN_SPEED_VALUE, 266);
    rt_thread_delay(SERVO_DELAY_TIME);

//    uart_send_p_command();
//    rt_thread_delay(SERVO_DELAY_TIME);
}

void send_work_param(uint8_t seq, struct Servo_Data_Stru_ *servo_data)
{
    int16_t work_param[16];
    int16_t temp_param;
    int16_t temp_param1;
    work_param[0] = MENU_WORK_SERVO_PARM0;
    work_param[1] = MENU_WORK_SERVO_PARM1;
    work_param[2] = MENU_WORK_SERVO_PARM2;
    work_param[3] = MENU_WORK_SERVO_PARM3;
    work_param[4] = MENU_WORK_SERVO_PARM4;
    work_param[5] = MENU_WORK_SERVO_PARM5;
    work_param[6] = MENU_WORK_SERVO_PARM6;
    work_param[7] = MENU_WORK_SERVO_PARM7;
    work_param[8] = MENU_WORK_SERVO_PARM8;
    work_param[9] = MENU_WORK_SERVO_PARM9;
    work_param[10] = MENU_WORK_SERVO_PARM10;
    work_param[11] = MENU_WORK_SERVO_PARM11;
    work_param[12] = MENU_WORK_SERVO_PARM12;
    work_param[13] = MENU_WORK_SERVO_PARM13;
    work_param[14] = MENU_WORK_SERVO_PARM14;
    work_param[15] = MENU_WORK_SERVO_PARM15;
    temp_param = *(work_param + seq);
    temp_param1 = *(&servo_data->work_p0 + seq);
    uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_WORK, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
}

void send_setup_param(uint8_t seq, struct Servo_Data_Stru_ *servo_data)
{
    int16_t setup_param[16];
    int16_t temp_param;
    int16_t temp_param1;
    setup_param[0] = MENU_SETUP_SERVO_PARM0;
    setup_param[1] = MENU_SETUP_SERVO_PARM1;
    setup_param[2] = MENU_SETUP_SERVO_PARM2;
    setup_param[3] = MENU_SETUP_SERVO_PARM3;
    setup_param[4] = MENU_SETUP_SERVO_PARM4;
    setup_param[5] = MENU_SETUP_SERVO_PARM5;
    setup_param[6] = MENU_SETUP_SERVO_PARM6;
    setup_param[7] = MENU_SETUP_SERVO_PARM7;
    setup_param[8] = MENU_SETUP_SERVO_PARM8;
    setup_param[9] = MENU_SETUP_SERVO_PARM9;
    setup_param[10] = MENU_SETUP_SERVO_PARM10;
    setup_param[11] = MENU_SETUP_SERVO_PARM11;
    setup_param[12] = MENU_SETUP_SERVO_PARM12;
    setup_param[13] = MENU_SETUP_SERVO_PARM13;
    setup_param[14] = MENU_SETUP_SERVO_PARM14;
    setup_param[15] = MENU_SETUP_SERVO_PARM15;
    temp_param = *(setup_param + seq);
    temp_param1 = *(&servo_data->set_p0 + seq);
    uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_SETUP, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
}

void send_debug_param(uint8_t seq, struct Servo_Data_Stru_ *servo_data)
{
    int16_t debug_param[11];
    int16_t temp_param;
    int16_t temp_param1;
    debug_param[0] = MENU_DEBUG_SERVO_PARM0;
    debug_param[1] = MENU_DEBUG_SERVO_PARM1;
    debug_param[2] = MENU_DEBUG_SERVO_PARM2;
    debug_param[3] = MENU_DEBUG_SERVO_PARM3;
    debug_param[4] = MENU_DEBUG_SERVO_PARM4;
    debug_param[5] = MENU_DEBUG_SERVO_PARM5;
    debug_param[6] = MENU_DEBUG_SERVO_PARM6;

    debug_param[7] = MENU_CONFIG_SERVO_PARM0;
    debug_param[8] = MENU_CONFIG_SERVO_PARM1;
    debug_param[9] = MENU_CONFIG_SERVO_PARM2;
    debug_param[10] = MENU_CONFIG_SERVO_PARM3;

    temp_param = *(debug_param + seq);
    temp_param1 = *(&servo_data->debug_p0 + seq);
	
		if(seq <7){
			uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_DEBUG, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
		}
		else{
			uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_CONFIG, SERVO_STATE_COM, temp_param, temp_param1, 0, 0);
		}
}

void menu_combine_prom_work_parm(void)
{
    uint8_t i = 0;
    struct Servo_Data_Stru_ servo_data;
    memcpy(&servo_data, &servoDataStru, sizeof(servo_data));
	
	while(write_read_busy_state_ == 1){
		rt_thread_delay(SERVO_DELAY_TIME);
	}
	write_read_busy_state_ = 1;

    uart_send_clear_command();
		rt_thread_delay(SERVO_DELAY_TIME);
	
    uart_send_p_command();//
		rt_thread_delay(SERVO_DELAY_TIME);
	
		uart_send_clear_command();//
    rt_thread_delay(SERVO_DELAY_TIME);
	
    uart_send_common_command();//
		rt_thread_delay(SERVO_DELAY_TIME);
	
		uart_send_clear_command();//
		rt_thread_delay(SERVO_DELAY_TIME);
    for(i = 0; i < 14; i++)
    {
        rt_thread_delay(SERVO_DELAY_TIME);
        send_work_param(i, &servo_data);
    }

    for(i = 0; i < 16; i++)
    {
        rt_thread_delay(SERVO_DELAY_TIME);
        send_setup_param(i, &servo_data);
    }


    rt_thread_delay(SERVO_DELAY_TIME);
    uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_DOWNLOAD, SERVO_STATE_COM, MENU_DOWMLOAD_WORK_PARM, 0, 0, 0);
    rt_thread_delay(SERVO_DELAY_TIME);

    for(i = 0; i < 11; i++)
    {
        rt_thread_delay(SERVO_DELAY_TIME);
        send_debug_param(i, &servo_data);
    }

    rt_thread_delay(SERVO_DELAY_TIME);
    uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_PARM_DOWNLOAD, SERVO_STATE_COM, MENU_DOWMLOAD_DEBUG_PARM, 0, 0, 0);
		
	write_read_busy_state_ = 0;
//    rt_thread_delay(SERVO_DELAY_TIME);
//    uart_send_p_command();
//    rt_thread_delay(SERVO_DELAY_TIME_S);
}


void read_work_param(uint8_t seq)
{
    int16_t work_param[16];
    int16_t temp_param;
    work_param[0] = MENU_FB_SERVO_WORK_PARM0;
    work_param[1] = MENU_FB_SERVO_WORK_PARM1;
    work_param[2] = MENU_FB_SERVO_WORK_PARM2;
    work_param[3] = MENU_FB_SERVO_WORK_PARM3;
    work_param[4] = MENU_FB_SERVO_WORK_PARM4;
    work_param[5] = MENU_FB_SERVO_WORK_PARM5;
    work_param[6] = MENU_FB_SERVO_WORK_PARM6;
    work_param[7] = MENU_FB_SERVO_WORK_PARM7;
    work_param[8] = MENU_FB_SERVO_WORK_PARM8;
    work_param[9] = MENU_FB_SERVO_WORK_PARM9;
    work_param[10] = MENU_FB_SERVO_WORK_PARM10;
    work_param[11] = MENU_FB_SERVO_WORK_PARM11;
    work_param[12] = MENU_FB_SERVO_WORK_PARM12;
    work_param[13] = MENU_FB_SERVO_WORK_PARM13;
    work_param[14] = MENU_FB_SERVO_WORK_PARM14;
    work_param[15] = MENU_FB_SERVO_WORK_PARM15;
    temp_param = *(work_param + seq);
    uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
}

void read_setup_param(uint8_t seq)
{
    int16_t setup_param[16];
    int16_t temp_param;
    setup_param[0] = MENU_FB_SERVO_SETUP_PARM0;
    setup_param[1] = MENU_FB_SERVO_SETUP_PARM1;
    setup_param[2] = MENU_FB_SERVO_SETUP_PARM2;
    setup_param[3] = MENU_FB_SERVO_SETUP_PARM3;
    setup_param[4] = MENU_FB_SERVO_SETUP_PARM4;
    setup_param[5] = MENU_FB_SERVO_SETUP_PARM5;
    setup_param[6] = MENU_FB_SERVO_SETUP_PARM6;
    setup_param[7] = MENU_FB_SERVO_SETUP_PARM7;
    setup_param[8] = MENU_FB_SERVO_SETUP_PARM8;
    setup_param[9] = MENU_FB_SERVO_SETUP_PARM9;
    setup_param[10] = MENU_FB_SERVO_SETUP_PARM10;
    setup_param[11] = MENU_FB_SERVO_SETUP_PARM11;
    setup_param[12] = MENU_FB_SERVO_SETUP_PARM12;
    setup_param[13] = MENU_FB_SERVO_SETUP_PARM13;
    setup_param[14] = MENU_FB_SERVO_SETUP_PARM14;
    setup_param[15] = MENU_FB_SERVO_SETUP_PARM15;
    temp_param = *(setup_param + seq);
    uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
}

void read_debug_param(uint8_t seq)
{
    int16_t debug_param[7];
    int16_t temp_param;
    debug_param[0] = MENU_FB_SERVO_DEBUG_PARM0;
    debug_param[1] = MENU_FB_SERVO_DEBUG_PARM1;
    debug_param[2] = MENU_FB_SERVO_DEBUG_PARM2;
    debug_param[3] = MENU_FB_SERVO_DEBUG_PARM3;
    debug_param[4] = MENU_FB_SERVO_DEBUG_PARM4;
    debug_param[5] = MENU_FB_SERVO_DEBUG_PARM5;
    debug_param[6] = MENU_FB_SERVO_DEBUG_PARM6;
    temp_param = *(debug_param + seq);
    uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
}

void read_config_param(uint8_t seq)
{
    int16_t config_param[4];
    int16_t temp_param;
    config_param[0] = MENU_FB_SERVO_CONFIG_PARM0;
    config_param[1] = MENU_FB_SERVO_CONFIG_PARM1;
    config_param[2] = MENU_FB_SERVO_CONFIG_PARM2;
    config_param[3] = MENU_FB_SERVO_CONFIG_PARM3;
    temp_param = *(config_param + seq);
    uart_send_command(servo_unique_address_id, SERVO_COMMAND_SERVO_FB, SERVO_STATE_COM, temp_param, 0, 0, 0);
}

extern volatile uint16_t current_servo_version_;
extern volatile uint8_t connect_servo_state_;
uint8_t menu_combine_fb_work_parm(void)
{
    uint8_t i = 0;
    uint8_t time_count;
    uint8_t chech_sum;
    int16_t* buff;
    struct Servo_Data_Stru_ servo_data;
	while(write_read_busy_state_ == 1){
		rt_thread_delay(SERVO_DELAY_TIME);
	}
	write_read_busy_state_ = 1;

    uart_send_clear_command();
    rt_thread_delay(SERVO_DELAY_TIME);
    uart_send_common_command();

    for(i = 0; i < 43; i++)
    {
        receive_uart_data_flag = 0;
        rt_thread_delay(SERVO_DELAY_TIME);

        if(i < 16)
        {
            read_work_param(i);
        }
        else if(i < 32)
        {
            read_setup_param(i - 16);
        }
        else if(i < 39)
        {
            read_debug_param(i - 32);
        }
        else if(i < 43)
        {
            read_config_param(i - 39);
        }

        //wait for feedback datas
        time_count = 0;

        while(receive_uart_data_flag == 0)
        {
            time_count++;

            if(time_count > 200) //200ms
            {
				write_read_busy_state_ = 0;
                return 0; //erro
            }

            rt_thread_delay(1); //0.001 s
        }

        //process datas
        receive_uart_data_flag = 0;
        chech_sum = uart_read_datas[2] + uart_read_datas[3] + uart_read_datas[4];

        if(chech_sum != uart_read_datas[10])
        {
			write_read_busy_state_ = 0;
            return 0;
        }

        if(uart_read_datas[3] != SERVO_STATE_FB)
        {
			write_read_busy_state_ = 0;
            return 0;
        }

        if(i < 16)
        {
            buff = &servo_data.work_p0 + i;
            *buff = (uart_read_datas[9] | (uart_read_datas[8] << 8));
        }
        else if(i < 32)
        {
            buff = &servo_data.set_p0 + i - 16;
            *buff = (uart_read_datas[9] | (uart_read_datas[8] << 8));
        }
        else if(i < 39)
        {
            buff = &servo_data.debug_p0 + i - 32;
            *buff = (uart_read_datas[9] | (uart_read_datas[8] << 8));
        }
        else if(i < 43)
        {
            buff = &servo_data.debug_p7 + i - 39;
            *buff = (uart_read_datas[9] | (uart_read_datas[8] << 8));
        }
    }

    memcpy(&servoDataStru, &servo_data, sizeof(servo_data));
    Copy_Data_To_Show();
    rt_thread_delay(SERVO_DELAY_TIME_S);
	current_servo_version_ = servoDataStru.work_p12;
	connect_servo_state_ = 1;
	write_read_busy_state_ = 0;
    return 1;
}

uint8_t connect_detect(void)
{
    uint8_t time_count;
	
	while(write_read_busy_state_ == 1){
		rt_thread_delay(SERVO_DELAY_TIME);
	}
	write_read_busy_state_ = 1;
	
	receive_uart_data_flag = 0;
	
    uart_send_clear_command();
    rt_thread_delay(SERVO_DELAY_TIME);
	
	uart_send_common_command();
	rt_thread_delay(SERVO_DELAY_TIME);
	
	read_work_param(0);
	rt_thread_delay(SERVO_DELAY_TIME);
	
	read_work_param(1);
	while(receive_uart_data_flag == 0)
	{
		time_count++;
		if(time_count > 200) //200ms
		{
			write_read_busy_state_ = 0;
			return 0; //erro
		}
		rt_thread_delay(1); //0.001 s
	}
	
	write_read_busy_state_ = 0;
	return 1;
}

uint8_t menu_combine_verify_work_parm(void)
{
    uint8_t i = 0;
    uint16_t j = 0;
    int16_t* buff;
    uint8_t* buff1, *buff2;
    uint8_t time_count;
    uint8_t chech_sum;
    struct Servo_Data_Stru_ servo_data;
	while(write_read_busy_state_ == 1){
		rt_thread_delay(SERVO_DELAY_TIME);
	}
	write_read_busy_state_ = 1;

    uart_send_clear_command();
    rt_thread_delay(SERVO_DELAY_TIME);
    uart_send_common_command();

    for(i = 0; i < 43; i++)
    {
        receive_uart_data_flag = 0;
        rt_thread_delay(SERVO_DELAY_TIME);

        if(i < 16)
        {
            read_work_param(i);
        }
        else if(i < 32)
        {
            read_setup_param(i - 16);
        }
        else if(i < 39)
        {
            read_debug_param(i - 32);
        }
        else if(i < 43)
        {
            read_config_param(i - 39);
        }

        //wait for feedback datas
        time_count = 0;

        while(receive_uart_data_flag == 0)
        {
            time_count++;

            if(time_count > 200) //200ms
            {
				write_read_busy_state_ = 0;
                return 0; //erro
            }

            rt_thread_delay(1); //0.001 s
        }

        //process datas
        receive_uart_data_flag = 0;
        chech_sum = uart_read_datas[2] + uart_read_datas[3] + uart_read_datas[4];

        if(chech_sum != uart_read_datas[10])
        {
			write_read_busy_state_ = 0;
            return 0;
        }

        if(uart_read_datas[3] != SERVO_STATE_FB)
        {
			write_read_busy_state_ = 0;
            return 0;
        }

        if(i < 16)
        {
            buff = &servo_data.work_p0 + i;
            *buff = (uart_read_datas[9] | (uart_read_datas[8] << 8));
        }
        else if(i < 32)
        {
            buff = &servo_data.set_p0 + i - 16;
            *buff = (uart_read_datas[9] | (uart_read_datas[8] << 8));
        }
        else if(i < 39)
        {
            buff = &servo_data.debug_p0 + i - 32;
            *buff = (uart_read_datas[9] | (uart_read_datas[8] << 8));
        }
        else if(i < 43)
        {
            buff = &servo_data.debug_p7 + i - 39;
            *buff = (uart_read_datas[9] | (uart_read_datas[8] << 8));
        }
    }
	servoDataStru.work_p14 = servo_data.work_p14;
	servoDataStru.work_p15 = servo_data.work_p15;
	servoDataStru.set_p6 = servo_data.set_p6;
    buff1 = (uint8_t*)(&servo_data);
    buff2 = (uint8_t*)(&servoDataStru);

    for(j = 0; j < sizeof(servo_data); j++)
    {
        if(buff1[j] != buff2[j])
        {
			write_read_busy_state_ = 0;
            return 0;
        }
    }

    rt_thread_delay(SERVO_DELAY_TIME);
    uart_send_p_command();
    rt_thread_delay(SERVO_DELAY_TIME_S);
	write_read_busy_state_ = 0;
    return 1;
}
/******************************************************/
