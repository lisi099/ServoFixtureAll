#ifndef _EEPROM_H_
#define _EEPROM_H_

#include "stm32f10x.h"

void eeprom_init(void);
void eeprom_read(u32 ReadAddr,u8 *pBuffer,u16 NumToRead);
void eeprom_write(u32 WriteAddr,u8 *pBuffer,u16 NumToWrite);

#endif
