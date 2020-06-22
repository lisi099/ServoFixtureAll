
#ifndef _KEY_CHECK_H_
#define _KEY_CHECK_H_

#include "stm32f10x.h"

# define KEY_BUTTON_OK                 						0
# define KEY_BUTTON_UP                 						1
# define KEY_BUTTON_DOWN              						2
# define KEY_BUTTON_ESC                 					3

#define KEY_NUM		4

typedef enum PressState_{
	NO_CLICK = 0,
	S_CLICK = 1,
	D_CLICK = 2,
	L_CLICK =3
}PressState;

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


PressState key_scan(uint8_t key);

#endif
