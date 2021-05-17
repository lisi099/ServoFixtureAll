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
#include "tai_pwm.h"
#include "string.h"

#define PWM_HIGH() 				GPIO_SetBits(GPIOA, GPIO_Pin_2)
#define PWM_LOW()					GPIO_ResetBits(GPIOA, GPIO_Pin_2)

volatile uint8_t 					pwm_finish_flag = 0;
extern rt_mutex_t servo_mutex;

void TIM3_Int_Init(uint16_t psc);// 500 ~ 2500

void produce_pwm(uint16_t pwm) //0.5ms ~ 2.5ms
{
    rt_thread_delay(10);
    rt_mutex_take(servo_mutex, RT_WAITING_FOREVER);
    usart2_init_pwm();
    PWM_LOW();
    rt_thread_delay(3);
    pwm_finish_flag = 0;
    TIM3_Int_Init(pwm);

    while(1)
    {
        if(pwm_finish_flag)
        {
            break;
        }

        rt_thread_delay(3);
    }

    rt_thread_delay(10);
    usart2_init_rx(115200);
    rt_mutex_release(servo_mutex);
}

void produce_pwm_count(uint16_t pwm, uint8_t count) //0.5ms ~ 2.5ms
{
    uint8_t p_count = 0;
    rt_thread_delay(10);
    rt_mutex_take(servo_mutex, RT_WAITING_FOREVER);

    usart2_init_pwm();
    PWM_LOW();
    rt_thread_delay(12);
    pwm_finish_flag = 0;
    TIM3_Int_Init(pwm);

    while(1)
    {
        if(pwm_finish_flag)
        {
            p_count ++;
            if(p_count >= count)
            {
                break;
            }
            rt_thread_delay(12);
            pwm_finish_flag = 0;
            TIM3_Int_Init(pwm);
        }
        rt_thread_delay(1);
    }
    rt_thread_delay(10);
	PWM_HIGH();
	rt_thread_delay(50);
    usart2_init_rx(115200);
    rt_mutex_release(servo_mutex);
}


void TIM3_Int_Init(uint16_t psc)// 500 ~ 2500
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

    //定时器TIM3初始化
    TIM_TimeBaseStructure.TIM_Period = 72 - 1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //使能指定的TIM3中断,允许更新中断

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

    TIM3->CNT = 0;
    TIM_Cmd(TIM3, ENABLE);  //使能TIMx
    PWM_HIGH();
}

//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)   //检查TIM3更新中断发生与否
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);    //清除TIMx更新中断标志
    }

    PWM_LOW();
    pwm_finish_flag = 1;
    TIM_Cmd(TIM3, DISABLE);  //使能TIMx
}

//-----------------------end of file-------------------------------------------------------
