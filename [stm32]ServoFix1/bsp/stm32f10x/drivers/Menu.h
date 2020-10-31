/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#ifndef _Menu_H_BAB
#define _Menu_H_BAB

#include "stm32f10x.h"
#include "1602_iic_sw.h"
#include "key.h"

#define KEY_Special 			255		//-这个保留用于特别事件

struct PAGE
{
    struct PAGE* pParent;
    void (*Function)(u8 key);
    struct Item* pItem;
    const u8 ItemNum;
};

typedef enum ShowType_
{
    SHOW_NUM = 0,
    SHOW_STRING,
    SHOW_STRING_VER,
    SHOW_NULL,
} ShowType;

struct Item
{
    char* pText;
    struct PAGE* pChildrenPage;
    u8 state;
    u8 colum;
    uint16_t data;
    ShowType type;
    uint16_t min;
    uint16_t max;
};

extern struct PAGE* pPage;

//----------------------functions---------------------
uint8_t is_need_update(void);

void Lcd_Clr_Scr(void);
void LCD_Write_Str(u8 x, u8 y, char* data);

void SetMainPage(struct PAGE* pMainPage);
uint8_t Menu_GetSelItem(void);
void Menu_SetSelItem(u8 num);

void ShowPage_Num(struct PAGE* pPage, uint8_t num);
void ShowParentPage_Num(uint16_t num);

void ShowItemPage_Num(u8 num);

void KeySelItem(u8 key);
void SelItemOfList(u8 index, char* s);

int find_version(int num);
#endif
