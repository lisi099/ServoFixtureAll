/******************** (C) COPYRIGHT 2016 ********************


***************************************************************/
#include "Menu.h"
#include "Lcd1602.h"

#define LOW_BYTE_NUM   8
#define LOW_4BYTE_MAX  0xFF

//保存选中的菜单项变量
static u8 SelItem =0;
static u16 ListShow=0x00;

struct PAGE *pPage;

void SelItemOfList(u8 index, char *s);


void LCD_Write_Str(u8 x, u8 y, char *data)
{
//	Lcd_Puts(y, x, data);
}

void Lcd_Clr_Scr(void)
{
//	Lcd_Write_Command( 0x01,1);       //显示清屏
}


void SetMainPage(struct PAGE *pMainPage)
{
    pPage =pMainPage;
}
/**
获得当前选中的菜单项
@return 返回菜单序号
*/
u8 Menu_GetSelItem(void)
{
    return SelItem;
}

extern  struct PAGE Save_Page;

void ShowList(u8 min,u8 max)
{
    char str[10] ={ 0 };
    u8 i =0, index =0;
    Lcd_Clr_Scr();
	uint8_t u8_data;
	int8_t i8_data;
	uint16_t u16_data;
	int16_t i16_data;
    for (index=min;index<=max;index++)
    {
        LCD_Write_Str(i,1,pPage->pItem[index].pText);
        if(pPage->pItem[index].colum !=0){
			switch(pPage->pItem[index].type){
				case SHOW_BOOL:
					u8_data = (uint8_t)pPage->pItem[index].data;
					sprintf(str, "%d", u8_data);
					break;
				case SHOW_U8:
					u8_data = (uint8_t)pPage->pItem[index].data;
					if(pPage == &Save_Page){
						if(u8_data >9){
							sprintf(str, "%d", u8_data);
						}
						else{
							str[0]='0';
							sprintf(&str[1], "%d", u8_data);
						}
					}
					else{
						sprintf(str, "%d", u8_data);
					}
//					if(u8_data >99){
//						sprintf(&str[2], "%d", u8_data);
//					}
//					else if (u8_data >9){
//						sprintf(&str[3], "%d", u8_data);
//					}
//					else{
//						sprintf(&str[4], "%d", u8_data);
//					}
					break;
				case SHOW_8:
					i8_data = (int8_t)pPage->pItem[index].data;
					sprintf(str, "%d", i8_data);
//					if(i8_data >=0){
//						if(i8_data >99){
//							sprintf(&str[2], "%d", i8_data);
//						}
//						else if (i8_data >9){
//							sprintf(&str[3], "%d", i8_data);
//						}
//						else{
//							sprintf(&str[4], "%d", i8_data);
//						}
//					}
//					else{
//						if(i8_data <-99){
//							sprintf(&str[1], "%d", i8_data);
//						}
//						else if (i8_data <-9){
//							sprintf(&str[2], "%d", i8_data);
//						}
//						else{
//							sprintf(&str[3], "%d", i8_data);
//						}
//					}
					break;
				case SHOW_U16:
					u16_data = (uint16_t)pPage->pItem[index].data;
					sprintf(str, "%d", u16_data);
//					if(u16_data >999){
//						sprintf(&str[1], "%d", u16_data);
//					}
//					else if(u16_data >99){
//						sprintf(&str[2], "%d", u16_data);
//					}
//					else if (u16_data >9){
//						sprintf(&str[3], "%d", u16_data);
//					}
//					else{
//						sprintf(&str[4], "%d", u16_data);
//					}
					break;
				case SHOW_16:
					i16_data = (int16_t)pPage->pItem[index].data;
					sprintf(str, "%d", i16_data);
//					if(i16_data >=0){
//						if(i16_data >999){
//							sprintf(&str[1], "%d", i16_data);
//						}
//						else if(i16_data >99){
//							sprintf(&str[2], "%d", i16_data);
//						}
//						else if (i16_data >9){
//							sprintf(&str[3], "%d", i16_data);
//						}
//						else{
//							sprintf(&str[4], "%d", i16_data);
//						}
//					}
//					else{
//						if(i16_data <-999){
//							sprintf(&str[0], "%d", i16_data);
//						}
//						else if(i16_data <-99){
//							sprintf(&str[1], "%d", i16_data);
//						}
//						else if (i16_data <-9){
//							sprintf(&str[2], "%d", i16_data);
//						}
//						else{
//							sprintf(&str[3], "%d", i16_data);
//						}
//					}
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
/**
页显示

1.当这个页有项目(Item)时：显示Item并同时选中Item 0  	\n
2.没有时:会调用该Page的回调函数并传入KEY_Special 参数	\n
@param pPage 指向一个page
*/
void ShowPage( struct PAGE *pPage)
{
//    s8 i;
    char data[] = ">";
    Lcd_Clr_Scr();
    if(pPage->pItem==0)
    {
        pPage->Function(KEY_Special);
        return; //<如果没有Item项则不显示Item，直接返回
    }

    if(pPage->DisplayMode==DISPLAY_MODE_1_COLUMN) //一列显示
    {
        ShowList(0, 1);  //
        SelItemOfList(0, data);
        pPage->Function(KEY_Special);
    }
//    else if(pPage->DisplayMode==DISPLAY_MODE_2_COLUMN) //两列显示
//    {
//        ///取出page中的Item并显示
//        for (i=0;i<pPage->ItemNum;i++)
//        {
//            if (i<2)
//            {
//                LCD_Write_Str(i,1,pPage->pItem[i].pText);
//            }
//            else
//            {
//                LCD_Write_Str(i-2,5,pPage->pItem[i].pText);
//            }

//        }
//        SelPageItem(0);//<选中Item 0
//        pPage->Function(KEY_Special);
//    }
}
void ShowPage_Num( struct PAGE *pPage, uint8_t num)
{
//    s8 i;
    char data[] = ">";
    Lcd_Clr_Scr();
    if(pPage->pItem==0)
    {
        pPage->Function(KEY_Special);
        return; //<如果没有Item项则不显示Item，直接返回
    }

    if(pPage->DisplayMode==DISPLAY_MODE_1_COLUMN) //一列显示
    {
        ShowList(num, num+1);  //
        SelItemOfList(num, data);
        pPage->Function(KEY_Special);
    }
}
/**
显示父页(ParentPage)
*/
void ShowParentPage(void)
{
    int i;
    for(i =0; i< pPage->ItemNum; i++){
        pPage->pItem[i].state =0;
    }
    pPage =pPage->pParent;
    ShowPage(pPage);
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

/**
显示项目(Item)下对应的页(Page)
*/
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

/**
选择page中的Item项
@param ItemIndex page中Item的索引号 0~7
*/
void SelPageItem(u8 ItemIndex)
{
	///检查是否有错误调用
#if MENU_DEBUG

	if (ItemIndex>=8)
	{
		LCD_Write_Str(0,0,"设置菜单项溢出");
		return;
	}
#endif

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
    min=ListShow&LOW_4BYTE_MAX;

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
    u8 max;
    u8 min;

    max=ListShow>>LOW_BYTE_NUM;
    min=ListShow&LOW_4BYTE_MAX;

    if (index>max) //<超出最大当前显示的序号
    {

        LCD_Write_Str(Menu_GetSelItem()-min,0,(char*)" ");

        min+=1;
        max+=1;
        ShowList(min,max);
        ListShow=(max<<LOW_BYTE_NUM)|min;

        LCD_Write_Str(index-min,0,(char*)s);

    }
    else if(index>=min)//<在最小和最大序号之间
    {
        LCD_Write_Str(Menu_GetSelItem()-min,0,(char*)" ");
        LCD_Write_Str(index-min,0,(char*)s);
    }
    else					//<低于最小当前显示最小序号
    {
        LCD_Write_Str(Menu_GetSelItem()-min,0,(char*)" ");
        min-=1;
        max-=1;
        ShowList(min,max);
        ListShow=(max<<LOW_BYTE_NUM)|min;

        LCD_Write_Str(index-min,0,(char*)s);
    }
    SelItem=index;
}

void data_plus(int16_t *data, ShowType type)
{
	uint8_t 	*t8;
	int8_t 		*it8;
	uint16_t 	*t16;
	int16_t 	*it16;
	switch(type){
		case SHOW_BOOL:
			if((uint8_t)(*data) == 0){
				*data = 1;
			}
			else{
				*data = 0;
			}
			break;
		case SHOW_U8:
			t8 = (uint8_t *)data;
			*t8 +=1;
			break;
		case SHOW_8:
			it8 = (int8_t *)data;
			*it8 +=1;
			break;
		case SHOW_U16:
			t16 = (uint16_t *)data;
			if(*t16 <9999){
				*t16 +=1;
			}
			break;
		case SHOW_16:
			it16 = (int16_t *)data;
			if(*it16 <9999 && *it16 >-9999){
				*it16 +=1;
			}
			break;
		default:
			break;
	}
}

void data_minus(int16_t *data, ShowType type)
{
	uint8_t 	*t8;
	int8_t 		*it8;
	uint16_t 	*t16;
	int16_t 	*it16;
	switch(type){
		case SHOW_BOOL:
			if((uint8_t)(*data) == 0){
				*data = 1;
			}
			else{
				*data = 0;
			}
			break;
		case SHOW_U8:
			t8 = (uint8_t *)data;
			*t8 -=1;
			break;
		case SHOW_8:
			it8 = (int8_t *)data;
			*it8 -=1;
			break;
		case SHOW_U16:
			t16 = (uint16_t *)data;
			if(*t16 >0){
				*t16 -=1;
			}
			break;
		case SHOW_16:
			it16 = (int16_t *)data;
			if(*it16 > -9999 ){
				*it16 -=1;
			}
			break;
		default:
			break;
	}
}

void KeySelItem(u8 key)
{
    s8 index;
	uint8_t i;
    char data[] = ">";
    char str[10] ={0};
    if (pPage->DisplayMode==DISPLAY_MODE_1_COLUMN)//<如果是使用列表方式
    {
        switch(key)
        {
			case KEY_UP_L:
				if(pPage->pItem[Menu_GetSelItem()].state ==1){
					index=Menu_GetSelItem();
					for(i=0; i<3; i++){
						data_plus(&pPage->pItem[index].data, pPage->pItem[index].type);
					}
					switch(pPage->pItem[index].type){
					case SHOW_BOOL:
						sprintf(str, "%d", (uint8_t)pPage->pItem[index].data);
						break;
					case SHOW_U8:
						sprintf(str, "%d", (uint8_t)pPage->pItem[index].data);
						break;
					case SHOW_8:
						sprintf(str, "%d", (int8_t)pPage->pItem[index].data);
						break;
					case SHOW_U16:
						sprintf(str, "%d", (uint16_t)pPage->pItem[index].data);
						break;
					case SHOW_16:
						sprintf(str, "%d", (int16_t)pPage->pItem[index].data);
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
					for(i=0; i<3; i++){
						data_minus(&pPage->pItem[index].data, pPage->pItem[index].type);
					}
					switch(pPage->pItem[index].type){
					case SHOW_BOOL:
						sprintf(str, "%d", (uint8_t)pPage->pItem[index].data);
						break;
					case SHOW_U8:
						sprintf(str, "%d", (uint8_t)pPage->pItem[index].data);
						break;
					case SHOW_8:
						sprintf(str, "%d", (int8_t)pPage->pItem[index].data);
						break;
					case SHOW_U16:
						sprintf(str, "%d", (uint16_t)pPage->pItem[index].data);
						break;
					case SHOW_16:
						sprintf(str, "%d", (int16_t)pPage->pItem[index].data);
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
					data_plus(&pPage->pItem[index].data, pPage->pItem[index].type);
					switch(pPage->pItem[index].type){
					case SHOW_BOOL:
						sprintf(str, "%d", (uint8_t)pPage->pItem[index].data);
						break;
					case SHOW_U8:
						sprintf(str, "%d", (uint8_t)pPage->pItem[index].data);
						break;
					case SHOW_8:
						sprintf(str, "%d", (int8_t)pPage->pItem[index].data);
						break;
					case SHOW_U16:
						sprintf(str, "%d", (uint16_t)pPage->pItem[index].data);
						break;
					case SHOW_16:
						sprintf(str, "%d", (int16_t)pPage->pItem[index].data);
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
            case KEY_Down:
                //edit
                if(pPage->pItem[Menu_GetSelItem()].state ==1){
                    index=Menu_GetSelItem();
                    data_minus(&pPage->pItem[index].data, pPage->pItem[index].type);
					switch(pPage->pItem[index].type){
					case SHOW_BOOL:
						sprintf(str, "%d", (uint8_t)pPage->pItem[index].data);
						break;
					case SHOW_U8:
						sprintf(str, "%d", (uint8_t)pPage->pItem[index].data);
						break;
					case SHOW_8:
						sprintf(str, "%d", (int8_t)pPage->pItem[index].data);
						break;
					case SHOW_U16:
						sprintf(str, "%d", (uint16_t)pPage->pItem[index].data);
						break;
					case SHOW_16:
						sprintf(str, "%d", (int16_t)pPage->pItem[index].data);
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
        }
        return;
    }
    switch(key)
    {
        case KEY_UP:
            index=Menu_GetSelItem()-1;
            if(index<0) index=pPage->ItemNum-1;
            SelPageItem(index);
            break;
        case KEY_Down:
            index=Menu_GetSelItem()+1;
            if(index>(pPage->ItemNum-1)) index=0;
            SelPageItem(index);
            break;
    }
}

