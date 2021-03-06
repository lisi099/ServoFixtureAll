/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#ifndef _COMMON_FIFO_
#define _COMMON_FIFO_

#include "stm32f10x.h"

#define 	GET_PACKAGE 	1
#define 	NO_PACKAGE  	0


struct Common_Fifo_Stru
{
    int16_t Head;
    int16_t Tail;
    int16_t Total;
    int16_t Fifo_Size;
    int16_t Data_Size;
    uint8_t *Data_Ptr;
};


void common_fifo_init(struct Common_Fifo_Stru *fifo_stru, int16_t fifo_size, int16_t data_size, void *ptr);
void common_fifo_insert_package(struct Common_Fifo_Stru *fifo_stru, void *data);
uint8_t common_fifo_get_package(struct Common_Fifo_Stru *fifo_stru, void *data);



#endif

