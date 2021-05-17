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

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

    //��ʱ��TIM3��ʼ��
    TIM_TimeBaseStructure.TIM_Period = 72 - 1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //ʹ��ָ����TIM3�ж�,��������ж�

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

    TIM3->CNT = 0;
    TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx
    PWM_HIGH();
}

//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)   //���TIM3�����жϷ������
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);    //���TIMx�����жϱ�־
    }

    PWM_LOW();
    pwm_finish_flag = 1;
    TIM_Cmd(TIM3, DISABLE);  //ʹ��TIMx
}

//-----------------------end of file-------------------------------------------------------
