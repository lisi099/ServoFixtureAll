/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#ifndef _Menu_H_BAB
#define _Menu_H_BAB

#include "stm32f10x.h"
#include "Lcd1602.h"
#include "key.h"

#define KEY_Special 	255	///<这个保留用于特别事件

void Menu_Show(void);

#define DISPLAY_MODE_2_COLUMN   (2) //2列显示
#define DISPLAY_MODE_0_COLUMN   (0) //全屏显示
#define DISPLAY_MODE_1_COLUMN   (1) //1列显示

struct PAGE
{
    struct PAGE *pParent;
    void (*Function)(u8 key);
    struct Item *pItem;
    const u8 ItemNum;
    const u8 DisplayMode;
};

typedef enum ShowType_{
	SHOW_BOOL =0,
	SHOW_U8,
	SHOW_8,
	SHOW_U16,
	SHOW_16,
	
}ShowType;

struct Item
{
    char *pText;
    struct PAGE *pChildrenPage;
    u8 state;
    u8 colum;
    int16_t data;
	ShowType type;
};


extern struct PAGE *pPage;

void SetMainPage(struct PAGE *pMainPage);
void ShowMenu( struct PAGE *pPage);
void ShowPage( struct PAGE *pPage);
void ShowPage_Num( struct PAGE *pPage, uint8_t num);
void ShowParentPage(void);
void ShowParentPage_Num(uint16_t num);
void ShowItemPage(void);
void SelPageItem(u8 ItemIndex);
u8 Menu_GetSelItem(void);

void GetShowLst(u8 *pOutMin,u8 *pOutMax);

void KeySelItem(u8 key);
void SelItemOfListPara(u8 index, char *s);
void SelItemOfList(u8 index, char *s);

void LCD_Write_Str(u8 x, u8 y, char *data);
void Lcd_Clr_Scr(void);



#endif 
