/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#ifndef BOARD_INFO_H
#define	BOARD_INFO_H


#if defined (STM32F10X_MD) || defined (STM32F10X_MD_VL)
    #define PAGE_SIZE                         (0x400)    // 1 Kbyte
    #define FLASH_SIZE                        (0x20000)  // 128 KBytes
#elif defined STM32F10X_CL
    #define PAGE_SIZE                         (0x800)    // 2 Kbytes
    #define FLASH_SIZE                        (0x40000)  // 256 KBytes
#elif defined STM32F10X_HD || defined (STM32F10X_HD_VL)
    #define PAGE_SIZE                         (0x800)    // 2 Kbytes
    #define FLASH_SIZE                        (0x80000)  // 512 KBytes
#elif defined STM32F10X_XL
    #define PAGE_SIZE                         (0x800)    // 2 Kbytes
    #define FLASH_SIZE                        (0x100000) // 1 MByte
#else
    #error "Please select first the STM32 device to be used (in stm32f10x.h)"
#endif


#define 	HARDWEAR_INFO   	1
#define 	SOFTWEAR_INFO   	2

#define 	FLASH_START_ADDR 	0x08000000 // 0K
#define  	FLASH_IAP_ADDR  	0x08001800 // 6K
//#define  	FLASH_DATA1_ADDR  0x0800BC00 // 47K
#define  	FLASH_DATA1_ADDR  0x0800F000 // 60K
#define  	FLASH_INFO_ADDR  	0x0800FC00 // 63K
#define 	FLASH_END_ADDR 		0x08010000 // 64K

#endif
