/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#include "Menu.h"
#include "1602_iic_sw.h"
#include <rtthread.h>
#include "servo_serial.h"
#include "factory_data.h"

#define LOW_BYTE_NUM   8
#define LOW_BYTE_MAX  0xFF

extern struct rt_messagequeue key_mq;
struct PAGE* pPage;
static uint8_t SelItem = 0;
static u16 ListShow = 0;

#define BLINK_TIME1 	50

void SelItemOfList(u8 index, char* s);

extern struct Servo_Data_Stru_ servoDataStru;
//--------------------------------------------
int is_version_taiwan(const uint8_t * data)
{
	if(data[96] != '8'){
		return 0;
	}
	if(data[97] > '5' || data[97] < '0'){
		return 0;
	}
	return 1;
}

int find_version(int num)
{
    int i;
    int total = get_total_num();

    for(i = 0; i < total; i++)
    {
        if(num == get_ver_num(i))
        {
            return i;
        }
    }
    return 100;
}

uint8_t is_need_update(void)
{
    uint8_t buf[2];
    uint8_t distribtor;
    buf[0] = servoDataStru.work_p12 / 1000 % 10;
    buf[1] = servoDataStru.work_p12 / 100 % 10;
    distribtor = buf[0] * 10 + buf[1];

    if(find_version(distribtor) == 100)
    {
        return 1;
    }

    return 0;
}

void LCD_Write_Str(u8 x, u8 y, char* data)
{
    put_chars(x, y, data);
}

void Lcd_Clr_Scr(void)
{
    lcd_clear();
}

void SetMainPage(struct PAGE* pMainPage)
{
    pPage = pMainPage;
}

//--------------------------------------------
uint8_t Menu_GetSelItem(void)
{
    return SelItem;
}

void Menu_SetSelItem(u8 num)
{
    SelItem = num;
}
extern volatile uint8_t is_tai_servo_;
//--------------------------------------------
void ShowList(u8 min, u8 max)
{
    char str[16] = { 0 };
    u8 i = 0, index = 0;
    Lcd_Clr_Scr();
    uint8_t u8_data;
    int distribtor, costormer;

    uint8_t buf[2];

    for(index = min; index <= max; index++)
    {
        LCD_Write_Str(i, 1, pPage->pItem[index].pText);

        //显示编辑值
        if(pPage->pItem[index].colum != 0)
        {
            switch(pPage->pItem[index].type)
            {
                case SHOW_NUM:
                    u8_data = (uint8_t)pPage->pItem[index].data;
                    sprintf(str, "%d", u8_data);
					LCD_Write_Str(i, pPage->pItem[index].colum, (char*)str);
                    break;
                case SHOW_STRING:
                    if(pPage->pItem[index].data) sprintf(str, "%s", "Y");
                    else sprintf(str, "%s", "N");
					LCD_Write_Str(i, pPage->pItem[index].colum, (char*)str);
                    break;
                case SHOW_STRING_VER:
						buf[0] = pPage->pItem[index].data / 10000 % 10 + 1;
						if(!is_tai_servo_){
							sprintf(str, "v1.%d ", buf[0]);
						}
						else{
							sprintf(str, "v1.%d ", buf[0]);
						}

						buf[0] = pPage->pItem[index].data / 1000 % 10;
						buf[1] = pPage->pItem[index].data / 100 % 10;
						distribtor = buf[0] * 10 + buf[1];

						if(find_version(distribtor) == 100)
						{
							sprintf(&str[5], "%s-", "XXX-XXX");
						}
						else
						{
							sprintf(&str[5], "%s-", get_ver_char(find_version(distribtor)));
						}

						buf[0] = pPage->pItem[index].data / 10 % 10;
						buf[1] = pPage->pItem[index].data / 1 % 10;
						costormer = buf[0] * 10 + buf[1];

						if(costormer >= 21)
						{
							sprintf(&str[13], "00");
						}
						else if(costormer >= 10)
						{
							sprintf(&str[13], "%d", costormer);
						}
						else
						{
							sprintf(&str[13], "0%d", costormer);
						}
					
					LCD_Write_Str(i, pPage->pItem[index].colum, (char*)str);
                    break;
                default:
                    break;
            }

            
        }

        i++;
    }

    ListShow = (max << LOW_BYTE_NUM) | min; //<记录当前显示的Item
}


void ShowPage_Num(struct PAGE* pPage, uint8_t num)
{
    char data[] = ">";
    Lcd_Clr_Scr();

    if(pPage->pItem == 0)
    {
        pPage->Function(KEY_Special);
        return;
    }

    if(num + 1 == pPage->ItemNum)
    {
        ShowList(num - 1, num);
    }
    else
    {
        ShowList(num, num + 1);
    }

    SelItemOfList(num, data);
    pPage->Function(KEY_Special);
}


void ShowParentPage_Num(uint16_t num)
{
    if(pPage->pParent == 0)
    {
        return;
    }

    pPage = pPage->pParent;
    ShowPage_Num(pPage, num);
}

void BlinkEdit(char *str, uint8_t state)
{
    uint8_t min;
    min = ListShow & LOW_BYTE_MAX;

    if(state)
    {
        if(Menu_GetSelItem() == min)
        {
            LCD_Write_Str(0, pPage->pItem[Menu_GetSelItem()].colum, str);
        }
        else
        {
            LCD_Write_Str(1, pPage->pItem[Menu_GetSelItem()].colum, str);
        }
    }
    else
    {
        if(Menu_GetSelItem() == min)
        {
            LCD_Write_Str(0, pPage->pItem[Menu_GetSelItem()].colum, "        ");
        }
        else
        {
            LCD_Write_Str(1, pPage->pItem[Menu_GetSelItem()].colum, "        ");
        }
    }
}

void GetShowString(char *str, uint16_t *data)
{
    switch(pPage->pItem[Menu_GetSelItem()].type)
    {
        case SHOW_NUM:
            sprintf(str, "%d ", *data);
            break;

        case SHOW_STRING:
            if(*data)
            {
                *data = 1;
                sprintf(str, "%s", "Y");
            }
            else
            {
                sprintf(str, "%s", "N");
            }

        default:
            break;
    }
}

void ShowItemPage_Num(u8 num) //编辑参数值首先到这个函数
{
    uint16_t data_old;

    uint8_t rec_buff[2];
    uint8_t time_count;
    char str[10] = {0};

    if(pPage->pItem[Menu_GetSelItem()].pChildrenPage != 0)
    {
        pPage = pPage->pItem[Menu_GetSelItem()].pChildrenPage; //获得菜单项(Item)对应的page
        ShowPage_Num(pPage, num);
        return;
    }

    if(pPage->pItem[Menu_GetSelItem()].type == SHOW_NULL)
    {
        return;
    }

    if(pPage->pItem[Menu_GetSelItem()].type == SHOW_STRING_VER)
    {
        return;
    }

    data_old = pPage->pItem[Menu_GetSelItem()].data;
    GetShowString(str, &data_old);

    // blink on
    while(1)
    {
        while(!rt_mq_recv(&key_mq, &rec_buff, 2, 0))
        {
            if(rec_buff[0] == 0) //KEY_Up
            {
                if(data_old < pPage->pItem[Menu_GetSelItem()].max)
                {
                    data_old ++;
                }

                GetShowString(str, &data_old);
            }
            else if(rec_buff[0] == 1) //KEY_Down
            {
                if(data_old > pPage->pItem[Menu_GetSelItem()].min)
                {
                    data_old --;
                }

                GetShowString(str, &data_old);
            }
            else if(rec_buff[0] == 3) //ok
            {
                pPage->pItem[Menu_GetSelItem()].data = data_old;
                GetShowString(str, &data_old);
                BlinkEdit(str, 1);
                return;
            }
            else if(rec_buff[0] == 2) //KEY_Return
            {
                GetShowString(str, &pPage->pItem[Menu_GetSelItem()].data );
                BlinkEdit(str, 1);
                return;
            }
        }

        GetShowString(str, &data_old);
        time_count ++;

        if(time_count > BLINK_TIME1)
        {
            time_count = 0;
        }

        if(time_count > BLINK_TIME1 / 2)
        {
            BlinkEdit(str, 1);
        }
        else
        {
            BlinkEdit(str, 0);
        }

    }
}


void SelItemOfList(u8 index, char* s)
{
    static uint8_t max;
    static uint8_t min;

    max = ListShow >> LOW_BYTE_NUM;
    min = ListShow & LOW_BYTE_MAX;

    if(index == max + 1) //<超出最大当前显示的序号
    {
        LCD_Write_Str(Menu_GetSelItem() - min, 0, " ");
        min += 1;
        max += 1;
        ShowList(min, max);
        ListShow = (max << LOW_BYTE_NUM) | min;
        LCD_Write_Str(index - min, 0, s);
    }
    else if(index == min || index == min + 1) //<在最小和最大序号之间
    {
        LCD_Write_Str(Menu_GetSelItem() - min, 0, " ");
        LCD_Write_Str(index - min, 0, (char*)s);
    }
    else if(index < min) //<低于最小当前显示最小序号
    {
        LCD_Write_Str(Menu_GetSelItem() - min, 0, " ");
        min -= 1;
        max -= 1;
        ShowList(min, max);
        ListShow = (max << LOW_BYTE_NUM) | min;
        LCD_Write_Str(index - min, 0, s);
    }
    else //特殊情形，按键向上到顶时处理
    {
        min = pPage->ItemNum - 2;
        max = pPage->ItemNum - 1;
        ShowList(min, max);
        LCD_Write_Str(0, 0, " ");
        LCD_Write_Str(1, 0, s);
    }

    SelItem = index;
}

void KeySelItem(u8 key)
{
    uint8_t index;
    char data[] = ">";

    if(pPage->pItem[Menu_GetSelItem()].state == 1) //edit param state
    {
        return;
    }

    if(key == KEY_UP_L || key == KEY_UP)
    {
        if(Menu_GetSelItem() == 0)
        {
            index = pPage->ItemNum - 1;
        }
        else
        {
            index = Menu_GetSelItem() - 1;
        }

        SelItemOfList(index, data);
    }
    else if(key == KEY_Down_L || key == KEY_Down)
    {
        index = Menu_GetSelItem() + 1;

        if(index > (pPage->ItemNum - 1))
        {
            ShowPage_Num(pPage, 0);
        }
        else
        {
            SelItemOfList(index, data);
        }
    }

    return;
}

void data_plus(int16_t* data)
{
    if(*data < 100)
        (*data) ++;
}

void data_minus(int16_t* data)
{
    if(*data > 0)
        (*data) --;
}
//------------------------end----------------------------------------------
