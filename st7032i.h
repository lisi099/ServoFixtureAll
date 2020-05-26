// ------------------------- Head file for st7032i.c ---------------------------
#ifndef __ST7032I__
#define __ST7032I__

// ---------------------- Constant Definition ----------------------------------
#define LCD_HOME	0x02		// LCD Return home
#define LCD_ENTRY 	0x06		// Set LCD Entry Mode
#define LCD_C2L		0x10		// Move Cursor to the left
#define LCD_C2R		0x14		// Move Cursor to the right
#define LCD_D2L		0x18		// Move display to the left
#define LCD_D2R		0x1C		// Move display to the right

#define LCD_ON		0x0E		// Turn on LCD and Cursor
#define LCD_OFF		0x08		// Turn off LCD

#define LCD_CGRAM_ADDR	0x40	// Start address of LCD CGRAM
#define LCD_CGMAX 	64			// Max CGRAM bytes

#define LCD_COL 	16
#define LCD_ROW 	2
#define LCD_CHAR	LCD_COL*LCD_ROW

#define LCD_L1		0x80
#define LCD_L2		0xC0
//#define LCD_L3		0x90
//#define LCD_L4		0xD0

FlashByte CGRAM[LCD_CGMAX] =
{
	// Make eight patterns of 8*5 font
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,		// 1.Dsiplay All
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,		// 2.White Board
	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,		// 3.Virtical 1
	0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,		// 4.Virtical 2
	0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,		// 5.Horizon 1
	0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,		// 6.Horizon 2	
	0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,		// 7.Stars
	0xFF,0x11,0x11,0x11,0x11,0x11,0x11,0xFF,		// 8.Rectangle
};
// -----------------------------------------------------------------------------

// --------------------------- Function Prototypes -----------------------------
// Write a string to LCD
void SendStr(uint8 *ptString);
// Move Cursor or display
void Move(uint8 dir);
// Goto specific location
void Gotoxy(uint8 Row, uint8 Col);
// Tile LCD Screen with one data
void Tile(uint8 tData);
// Clear LCD Screen
void LCD_CLS(void);
// Wait some time and clear screen
void wait_and_clear(void);
// Fill CGRAM with array CGRAM[]
void FillCGRAM(void);
// Show all patterns in CGRAM
void ShowCGRAM(void);
// Call built-in Charactors
void CallBuiltinChar(void);
// LCD initialization procedure
void LCD_Initial(void);
// -----------------------------------------------------------------------------
#include "st7032i.c"
#endif
