#ifndef EEPROM_H
#define EEPROM_H

#include "stm32f10x.h"

#define IIC_SDA_PORT    	GPIOB
#define IIC_SCL_PORT		GPIOB
#define IIC_SCL_PIN    		GPIO_Pin_14
#define IIC_SDA_PIN			GPIO_Pin_15

#define HIGH    	Bit_SET
#define LOW     	Bit_RESET
#define PAGE_SIZE	64

#define GetSDA()       (GPIO_ReadInputDataBit(IIC_SDA_PORT, IIC_SDA_PIN))
#define SetSDA(state)   GPIO_WriteBit(IIC_SDA_PORT,IIC_SDA_PIN,state)
#define SetSCL(state)   GPIO_WriteBit(IIC_SCL_PORT,IIC_SCL_PIN,state)

void IIC_EEInit(void);
void AT24CXX_Read(u32 ReadAddr,u8 *pBuffer,u16 NumToRead);
void AT24CXX_Write(u32 WriteAddr,u8 *pBuffer,u16 NumToWrite);

#endif
