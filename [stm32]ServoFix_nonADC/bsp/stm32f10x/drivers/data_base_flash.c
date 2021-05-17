/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */
#include <rtthread.h>
#include <stm32f10x.h>
#include "data_base_flash.h"
#include "flash.h"
#include <string.h>
#include "board_info.h"
#include "tai_servo.h"


#define SAVE_DATA_SIZE		128
#define DATA_NUM_OF_PAGE 	(PAGE_SIZE/SAVE_DATA_SIZE)

extern volatile uint8_t is_tai_servo_;
extern struct Servo_Data_Stru_ servoDataStru;
extern void Copy_Data_To_Show(void);

uint8_t taiwan_servo_data[128];
uint8_t data_in_flash[PAGE_SIZE] = {0};


void save_servo_data_in_flash(uint8_t seq, uint16_t version)
{
    uint32_t page_start;
    uint32_t* data;
    uint32_t i = 0;

    struct Servo_Data_Stru_ servo_data;

    uint8_t* data_ptr = get_taiwan_write_data();
    if(is_tai_servo_ == 0)
    {
        memcpy(&servo_data, &servoDataStru, sizeof(servo_data));
        servo_data.work_p12 = version;
    }
    else
    {
        memcpy(&taiwan_servo_data, data_ptr, sizeof(taiwan_servo_data));
        taiwan_servo_data[98] = version / 10 % 10 + 0x30;
        taiwan_servo_data[99] = version % 10 + 0x30;
    }

    uint8_t num_page = seq / DATA_NUM_OF_PAGE;
    uint8_t seq_page = seq % DATA_NUM_OF_PAGE;

    page_start = FLASH_DATA1_ADDR + num_page * PAGE_SIZE;
    //read
    flash_read_n_byte(page_start, data_in_flash, sizeof(data_in_flash));
    //erase
    erase_flash_part(page_start, page_start + PAGE_SIZE);
    //data
    if(is_tai_servo_ == 0)
    {
        memcpy(&data_in_flash[seq_page * SAVE_DATA_SIZE], &servo_data, sizeof(servo_data));
    }
    else
    {
        memcpy(&data_in_flash[seq_page * SAVE_DATA_SIZE], taiwan_servo_data, sizeof(taiwan_servo_data));
    }
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
    uint8_t* data_ptr = get_taiwan_read_data();
    uint8_t* data_w_ptr = get_taiwan_write_data();

    if(seq < 20)
    {
        uint8_t num_page = seq / DATA_NUM_OF_PAGE;
        uint8_t seq_page = seq % DATA_NUM_OF_PAGE;
        page_start = FLASH_DATA1_ADDR + num_page * PAGE_SIZE;
        //read
        flash_read_n_byte(page_start, data_in_flash, sizeof(data_in_flash));
        //cpy
        if(is_tai_servo_ == 0)
        {
            memcpy(data, &data_in_flash[seq_page * SAVE_DATA_SIZE], sizeof(servoDataStru));
        }
        else
        {
            memcpy(data_ptr, &data_in_flash[seq_page * SAVE_DATA_SIZE], 128);
        }
    }
    else
    {
        if(is_tai_servo_ == 0)
        {
            get_factory_data(data, seq - 20);
        }
        else
        {
            memcpy(data_w_ptr, data_ptr, 129);
            get_tai_factory_data(data_w_ptr, seq - 20);
			memcpy(data_ptr, data_w_ptr, 129);
        }
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

uint8_t read_servo_data_in_flash_taiwan(uint8_t seq, uint8_t* const data)
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
        memcpy(data, &data_in_flash[seq_page * SAVE_DATA_SIZE], 128);
    }
    return 0;
}

//-----------------------end of file-------------------------------------------------------
