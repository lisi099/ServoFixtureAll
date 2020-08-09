/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#include "Menu.h"
#include "1602_iic_sw.h"

#define LOW_BYTE_NUM   8
#define LOW_BYTE_MAX  0xFF

void SelItemOfList(u8 index, char *s);

struct PAGE *pPage;
static uint8_t SelItem =0;
static u16 ListShow =0;

//--------------------------------------------
void LCD_Write_Str(u8 x, u8 y, char *data)
{
	put_chars(x, y, data);
}

void Lcd_Clr_Scr(void)
{
	lcd_clear();
}

void SetMainPage(struct PAGE *pMainPage)
{
    pPage =pMainPage;
}

//--------------------------------------------
int8_t Menu_GetSelItem(void)
{
    return SelItem;
}

void Menu_SetSelItem(u8 num)
{
    SelItem = num;
}

//--------------------------------------------
void ShowList(u8 min,u8 max)
{
    char str[10] ={ 0 };
    u8 i =0, index =0;
    Lcd_Clr_Scr();
	uint8_t u8_data;
    for (index=min;index<=max;index++)
    {
        LCD_Write_Str(i,1,pPage->pItem[index].pText);
        if(pPage->pItem[index].colum !=0){
			switch(pPage->pItem[index].type){
				case SHOW_NUM:
					u8_data = (uint8_t)pPage->pItem[index].data;
					sprintf(str, "%d", u8_data);
					break;
				default:
					break;
				}
            LCD_Write_Str(i, pPage->pItem[index].colum, (char *)str);
        }
        i++;
    }
    ListShow=(max<<LOW_BYTE_NUM)|min; //<记录当前显示的Item
}

void ShowPage(struct PAGE *pPage)
{
    char data[] = ">";
    Lcd_Clr_Scr();
    if(pPage->pItem ==0)
    {
        pPage->Function(KEY_Special);
        return; //<如果没有Item项则不显示Item，直接返回
    }


	ShowList(0, 1);  //
	SelItemOfList(0, data);
	pPage->Function(KEY_Special);
 
}

void ShowPage_Num( struct PAGE *pPage, uint8_t num)
{
    char data[] = ">";
    Lcd_Clr_Scr();
    if(pPage->pItem==0)
    {
        pPage->Function(KEY_Special);
        return;
    }
	if(num+1 == pPage->ItemNum){
		ShowList(num-1, num);  //
	}
	else{
		ShowList(num, num+1);  //
	}
	SelItemOfList(num, data);
	pPage->Function(KEY_Special);
}

void ShowParentPage(void)
{
    int i;
	if(pPage->pParent !=0){
		for(i =0; i< pPage->ItemNum; i++){
			pPage->pItem[i].state =0;
		}
		pPage =pPage->pParent;
		ShowPage(pPage);
	}
}

void ShowParentPage_Num(uint16_t num)
{
    int i;
    for(i =0; i< pPage->ItemNum; i++){
        pPage->pItem[i].state =0;
    }
    pPage =pPage->pParent;
    ShowPage_Num(pPage, num);
}

void ShowItemPage(void)
{
    char data[] ="+";
    //如果该项下没有页，这警告或返回
    if (pPage->pItem[Menu_GetSelItem()].pChildrenPage ==0)
    {
        if(pPage->pItem[Menu_GetSelItem()].colum !=0){
            if(pPage->pItem[Menu_GetSelItem()].state ==1){
                for(int i=0; i<pPage->ItemNum; i++){
                    pPage->pItem[i].state =0;
                }
                data[0] ='>';
            }
            else{
                for(int i=0; i<pPage->ItemNum; i++){
                    pPage->pItem[i].state =0;
                }
                pPage->pItem[Menu_GetSelItem()].state =1;
            }
            SelItemOfList(Menu_GetSelItem(), data);
        }
        return;
    }
    pPage = pPage->pItem[Menu_GetSelItem()].pChildrenPage; //获得菜单项(Item)对应的page
    ShowPage(pPage);
}

void ShowItemPage_Num(u8 num)
{
    pPage = pPage->pItem[Menu_GetSelItem()].pChildrenPage; //获得菜单项(Item)对应的page
    ShowPage_Num(pPage, num);
}

void SelPageItem(u8 ItemIndex)
{
///清除上次选中的
   if (SelItem<LOW_BYTE_NUM)
   {
        LCD_Write_Str(SelItem,0,(char*)" ");
   }
   else
   {
        LCD_Write_Str(SelItem-LOW_BYTE_NUM,4,(char*)" ");
   }
///选中这次要选中的
   if (ItemIndex<LOW_BYTE_NUM)
   {
        LCD_Write_Str(ItemIndex,0,(char*)">");
        SelItem=ItemIndex;
   }
   else
   {
        LCD_Write_Str(ItemIndex-LOW_BYTE_NUM,4,(char*)">");
        SelItem=ItemIndex;
   }
}

void SelItemOfListPara(u8 index, char *s)
{
    u8 max;
    u8 min;

    max=ListShow>>LOW_BYTE_NUM;
    min=ListShow&LOW_BYTE_MAX;

    if (index>max) //<超出最大当前显示的序号
    {
        LCD_Write_Str(Menu_GetSelItem()-min, pPage->pItem[index].colum, (char*)"    ");
        min+=1;
        max+=1;
        ListShow=(max<<LOW_BYTE_NUM)|min;
        LCD_Write_Str(index-min, pPage->pItem[index].colum, (char*)s);

    }
    else if(index>=min)//<在最小和最大序号之间
    {
        LCD_Write_Str(Menu_GetSelItem()-min, pPage->pItem[index].colum, (char*)"    ");
        LCD_Write_Str(index-min, pPage->pItem[index].colum, (char*)s);
    }
    else			//<低于最小当前显示最小序号
    {
        LCD_Write_Str(Menu_GetSelItem()-min, pPage->pItem[index].colum, (char*)"    ");
        min-=1;
        max-=1;
        ListShow=(max<<LOW_BYTE_NUM)|min;
        LCD_Write_Str(index-min, pPage->pItem[index].colum, (char*)s);
    }
    SelItem=index;
}

void SelItemOfList(u8 index, char *s)
{
    static uint8_t max;
    static uint8_t min;
	
	max = ListShow >> LOW_BYTE_NUM;
	min = ListShow & LOW_BYTE_MAX;
	
    if (index ==max+1) //<超出最大当前显示的序号
    {
        LCD_Write_Str(Menu_GetSelItem()-min,0," ");
        min +=1;
        max +=1;
        ShowList(min,max);
        ListShow =(max<<LOW_BYTE_NUM) | min;
        LCD_Write_Str(index-min,0,s);
    }
    else if(index == min || index == min+1)//<在最小和最大序号之间
    {
        LCD_Write_Str(Menu_GetSelItem()-min,0," ");
        LCD_Write_Str(index-min,0,(char*)s);
    }
    else if(index <min)//<低于最小当前显示最小序号
    {
        LCD_Write_Str(Menu_GetSelItem()-min,0," ");
        min-=1;
        max-=1;
        ShowList(min,max);
        ListShow=(max<<LOW_BYTE_NUM)|min;
        LCD_Write_Str(index-min,0,s);
    }
	else{
		
		min = pPage->ItemNum -2;
		max = pPage->ItemNum -1;
		ShowList(min,max);
		LCD_Write_Str(0,0," ");
		LCD_Write_Str(1,0,s);
	}
    SelItem =index;
}

void data_plus(int16_t *data, ShowType type, int16_t min, int16_t max)
{
	switch(type){
		case SHOW_NUM:
			break;
		default:
			break;
	}
	if(*data >max){
		*data = max;
	}
	else if(*data <min){
		*data = min;
	}
}

void data_minus(int16_t *data, ShowType type, int16_t min, int16_t max)
{
	switch(type){
		case SHOW_NUM:
			break;
		default:
			break;
	}
	
	if(*data >max){
		*data = max;
	}
	else if(*data <min){
		*data = min;
	}
}

void KeySelItem(u8 key)
{
    uint8_t index;
	uint8_t i;
    char data[] = ">";
    char str[10] ={0};
	
	if(pPage->pItem[Menu_GetSelItem()].state ==1){
		
	}
	else{
		if(key == KEY_UP_L || key == KEY_UP){
			if(Menu_GetSelItem() == 0){
				index =pPage->ItemNum -1;
			}
			else{
				index = Menu_GetSelItem() -1;
			}
			SelItemOfList(index, data);
		}
		else if(key == KEY_Down_L || key == KEY_Down){
			index =Menu_GetSelItem()+1;
			if(index>(pPage->ItemNum-1)){
				ShowPage(pPage);
			}
			else{
				SelItemOfList(index, data);
			}
		}
	}
	return;
	
	switch(key)
	{
		case KEY_UP_L:
			if(pPage->pItem[Menu_GetSelItem()].state ==1){
				index=Menu_GetSelItem();
				for(i=0; i<5; i++){
					data_plus(&pPage->pItem[index].data, pPage->pItem[index].type, pPage->pItem[index].min, pPage->pItem[index].max);
				}
				switch(pPage->pItem[index].type){
				case SHOW_NUM:
					sprintf(str, "%d", (uint8_t)pPage->pItem[index].data);
					break;
				default:
					break;
				}	
				SelItemOfListPara(index, str);
				break;
			}
			index=Menu_GetSelItem() -1;
			if(index<0){
				break;
			}
			SelItemOfList(index, data);
			break;
		case KEY_Down_L:
			if(pPage->pItem[Menu_GetSelItem()].state ==1){
				index=Menu_GetSelItem();
				for(i=0; i<5; i++){
					data_minus(&pPage->pItem[index].data, pPage->pItem[index].type, pPage->pItem[index].min, pPage->pItem[index].max);
				}
				switch(pPage->pItem[index].type){
				case SHOW_NUM:
					sprintf(str, "%d", (uint8_t)pPage->pItem[index].data);
					break;
				default:
					break;
				}
				SelItemOfListPara(index, str);
				break;
			}
			index=Menu_GetSelItem()+1;
			if(index>(pPage->ItemNum-1)){
				ShowPage(pPage);
				break;
			}
			SelItemOfList(index, data);
			break;
		case KEY_UP:
			//edit
			if(pPage->pItem[Menu_GetSelItem()].state ==1){
				index=Menu_GetSelItem();
				data_plus(&pPage->pItem[index].data, pPage->pItem[index].type, pPage->pItem[index].min, pPage->pItem[index].max);
				switch(pPage->pItem[index].type){
				case SHOW_NUM:
					sprintf(str, "%d", (uint8_t)pPage->pItem[index].data);
					break;
				default:
					break;
				}	
				SelItemOfListPara(index, str);
				break;
			}

			index =Menu_GetSelItem() -1;
			if(index < 0){
				index =pPage->ItemNum-1;
			}
			SelItemOfList(index, data);
			break;
		case KEY_Down:
			//edit
			if(pPage->pItem[Menu_GetSelItem()].state ==1){
				index=Menu_GetSelItem();
				data_minus(&pPage->pItem[index].data, pPage->pItem[index].type,pPage->pItem[index].min, pPage->pItem[index].max);
				switch(pPage->pItem[index].type){
				case SHOW_NUM:
					sprintf(str, "%d", (uint8_t)pPage->pItem[index].data);
					break;
				default:
					break;
				}
				SelItemOfListPara(index, str);
			}
			else{
				index =Menu_GetSelItem()+1;
				if(index>(pPage->ItemNum-1)){
					ShowPage(pPage);
					break;
				}
				SelItemOfList(index, data);
			}
			break;
	}
	return;
}
//------------------------end----------------------------------------------
