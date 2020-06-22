#include "servo_adc.h"
#include <rtthread.h>

void thread_servo_motor_check(void* parameter)
{
	servo_adc_init();
	while(1)
	{
		rt_thread_delay(RT_TICK_PER_SECOND/50);
	}
}