/******************** (C) COPYRIGHT 2016 ********************

***************************************************************/
#include "ServoAdc.h"

#define  ADC1_DR_Address    ((u32)0x4001244C)

volatile uint16_t  Adc_Convert_Value[NUM_FILTER][NUM_INFRARED];     //ÓÃÀ´´æ·ÅADC×ª»»½á¹û£¬Ò²ÊÇDMAµÄÄ¿±êµØÖ·


void adc_gpio_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  //infrared for  wall distance
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void adc_init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    ADC_DeInit(ADC1);
    ADC_InitStructure.ADC_Mode 			= ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode 		= ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode 	= ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv 	= ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign 		= ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel 		= NUM_INFRARED;
    ADC_Init(ADC1, &ADC_InitStructure);       

    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr =  (u32)&ADC1->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = 	(u32)Adc_Convert_Value;
    DMA_InitStructure.DMA_DIR = 		DMA_DIR_PeripheralSRC;          
    DMA_InitStructure.DMA_BufferSize = 		NUM_FILTER * NUM_INFRARED;     
    DMA_InitStructure.DMA_PeripheralInc = 	DMA_PeripheralInc_Disable;  
    DMA_InitStructure.DMA_MemoryInc = 		DMA_MemoryInc_Enable;        
    DMA_InitStructure.DMA_PeripheralDataSize = 	DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = 	DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = 		DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = 		DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = 		DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel1, ENABLE);
    DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);

    RCC_ADCCLKConfig(RCC_PCLK2_Div8);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_239Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_239Cycles5 );

    ADC_Cmd(ADC1, ENABLE);
    ADC_DMACmd(ADC1, ENABLE);           	        //Ê¹ÄÜÖ¸¶¨µÄADC1
    ADC_ResetCalibration(ADC1);          	        //¸´Î»Ö¸¶¨µÄADC1µÄÐ£×¼¼Ä´æÆ÷
    while(ADC_GetResetCalibrationStatus(ADC1));         //»ñÈ¡ADC1¸´Î»Ð£×¼¼Ä´æÆ÷µÄ×´Ì¬,ÉèÖÃ×´Ì¬ÔòµÈ´ý
    ADC_StartCalibration(ADC1);                	        //¿ªÊ¼Ö¸¶¨ADC1µÄÐ£×¼×´Ì¬
    while(ADC_GetCalibrationStatus(ADC1));              //»ñÈ¡Ö¸¶¨ADC1µÄÐ£×¼³ÌÐò,ÉèÖÃ×´Ì¬ÔòµÈ´ý
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void adc_nvic_init(void)
{
    NVIC_InitTypeDef NVIC_InitTypeDefstructure;
    NVIC_InitTypeDefstructure.NVIC_IRQChannel                   =DMA1_Channel1_IRQn;
    NVIC_InitTypeDefstructure.NVIC_IRQChannelCmd                =ENABLE;
    NVIC_InitTypeDefstructure.NVIC_IRQChannelPreemptionPriority =3;
    NVIC_InitTypeDefstructure.NVIC_IRQChannelSubPriority        =2;
    NVIC_Init(&NVIC_InitTypeDefstructure);
}

void adc_configration(void)
{
  adc_gpio_init();
  adc_init();
  //adc_nvic_init();
}

void DMA1_Channel1_IRQHandler(void)
{
   if(DMA_GetFlagStatus(DMA1_FLAG_TC1)!=RESET)
   {
     DMA_ClearFlag(DMA1_FLAG_TC1);    //clear the flag
   }
   else if (DMA_GetFlagStatus(DMA1_FLAG_GL1)!=RESET)
   {
     DMA_ClearFlag(DMA1_FLAG_GL1);    //clear the flag
   }
   else if (DMA_GetFlagStatus(DMA1_FLAG_HT1)!=RESET)
   {
     DMA_ClearFlag(DMA1_FLAG_HT1);    //clear the flag
   }
   else if (DMA_GetFlagStatus(DMA1_FLAG_TE1)!=RESET)
   {
     DMA_ClearFlag(DMA1_FLAG_TE1);    //clear the flag
   }
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
