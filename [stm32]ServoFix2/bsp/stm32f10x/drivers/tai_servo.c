/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#include "tai_servo.h"
#include <rtthread.h>
#include "usart2.h"
#include "string.h"


/*
115200 8 N 1
Header(0x22) id(0x32)  size addr0 addr1 data0 data1 data2 data3 checksum end(0xed) header data checksum end
Header(0x22) id(0x32)  size addr0 addr1 data0 data1 data2 data3 checksum end(0xed) header(0xda) data checksum end
*/


uint8_t chech_sum_xor(const uint8_t *data, const uint8_t size)
{
	uint8_t sum = data[0];
	for(int i=1; i<size; i++){
		sum^= data[i];
	}
	return sum;
}

void test_send_data(void)
{
	uint8_t data[11];
	data[0] = 0x22;
	data[1] = 0x32;
	data[2] = 0x00;
	///addr
	data[3] = 0x00;
	data[4] = 0x00;
	///data
	data[5] = 0x00;
	data[6] = 0x00;
	data[7] = 0x00;
	data[8] = 0x00;
	///check sum
	data[9] = chech_sum_xor(data, 9);
	///end
	data[10] = 0xed;
	usart2_send_buff(data, sizeof(data));
}






/******************************************************/
