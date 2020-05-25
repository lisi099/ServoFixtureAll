#ifndef BOARD_INFO_H
#define	BOARD_INFO_H


#define 	HARDWEAR_INFO   	1   
#define 	SOFTWEAR_INFO   	1

#define 	FLASH_START_ADDR 	0x08000000	
#define 	FLASH_END_ADDR 		0x08010000	
#define  	FLASH_IAP_ADDR  	0x08004000
#define  	FLASH_INFO_ADDR   0x08003C00 

#define  	FLASH_IMU_ADDR_START    0x0800FC00
#define  	FLASH_IMU_ADDR_END      0x08010000

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



#endif 
