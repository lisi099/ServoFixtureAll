/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 * 2013-07-12     aozima       update for auto initial.
 */
#include <rtthread.h>

void thread_disp_key_check(void* parameter);


int rt_application_init(void)
{
    static rt_thread_t tid1 = RT_NULL;
	
	tid1 = rt_thread_create("thread_disp_key_check", thread_disp_key_check, RT_NULL, 512, 19, 5);
	if (tid1 != RT_NULL) rt_thread_startup(tid1);
	
	
	
	
	
	
    return 0;
}

/*@}*/
