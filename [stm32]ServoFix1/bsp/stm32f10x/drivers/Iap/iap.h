#ifndef __IAP_H
#define	__IAP_H

#include "stm32f10x.h"


#define LOCK_ON 	0
#define LOCK_OFF 	1

#define ERASE_ER 	0
#define ERASE_OK 	1

#define CMD_REQUEST 0xC1
#define CMD_UNLOCK 	0xC3
#define CMD_UPDATE 	0xC5
#define CMD_FINISH 	0xC7
#define CMD_ERASE 	0xC9

#define CMD_READ 		0xB1

struct Iap_Status_Stru
{
    uint8_t Lock_flag;
    uint8_t Erase_flag;
    uint8_t Iap_Cmd;
    uint8_t Data[7];
};

void IAP_JumpToApplication(void);
void iap_update(void);
uint8_t iap_data_receieve(void);
uint8_t get_update_status(void);
#endif
