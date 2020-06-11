/******************** (C) COPYRIGHT 2016 ********************
º

***************************************************************/
#ifndef __KEY_H__
#define __KEY_H__

#include "stm32f10x.h"

/*****KEY******/
#define KEY_NUM		4

typedef enum KeyState_{
	WAIT_TRIG = 0,
	TRIGGER,
	ANTI_SHAKE,
	CLICK,
	WAILT_T,
	TRIGGER_2,
	ANTI_SHAKE_2,
	DLICK
}KeyState;

typedef enum PressState_{
	NO_CLICK = 0,
	S_CLICK = 1,
	D_CLICK = 2,
	L_CLICK =3
}PressState;


PressState key_run1(uint8_t key);
void key_new_init(void);

#define KEY_NONE	0
#define KEY_UP_L	42
#define KEY_Down_L	44
#define KEY_UP		38
#define KEY_Down	40
#define KEY_Return	27
#define KEY_Ok 		13

#endif
