/******************** (C) COPYRIGHT 2016 ********************
 * 文件名  ：flash.c
 * 描述    ：flash 应用函数库
 *          
 * 实验平台：
 * 硬件连接：

***************************************************************/

#include "includes.h"
#include "flash.h"
#include "board_info.h"

//uint8_t page_write_data[1024]={1};
//uint8_t page_read_data[1024]={1};
//#define PAGE_START_TEST 	0x08003800
//#define PAGE_END_TEST 		0x08003C00
/****************************************************************
*Name:	
*Function:	
*Input:		
*Output:
*Other:		
****************************************************************/
//void page_write_data_test(void)
//{
//	uint32_t i;
//	uint32_t data;
//	uint32_t adress;
//	for (i=0; i<1024; i++)
//	{
//		page_write_data[i] +=2;
//	}
//	erase_flash_part(PAGE_START_TEST, PAGE_END_TEST);
//	adress = PAGE_START_TEST;
//	for(i=0; i<1024; i+=4)
//	{
//		data = BYTETOINT(page_write_data[i+3], page_write_data[i+2], page_write_data[i+1], page_write_data[i]);
//		flash_write_one_word(adress, data);
//		adress +=4;
//	}
//	flash_read_n_byte(PAGE_START_TEST, page_read_data, 1024);
//}
/****************************************************************
*Name:	
*Function:	
*Input:		
*Output:
*Other:		
****************************************************************/
int flash_read_n_byte(uint32_t ReadAddress, uint8_t *ReadBuf, int32_t ReadNum) 
{
		int DataNum = 0;
		while(DataNum < ReadNum) 
		{
			 *(ReadBuf + DataNum) = *(__IO uint8_t*) ReadAddress++;
			 DataNum++;
		}
		return DataNum;
}
/****************************************************************
*Name:		WriteFlashOneWord
*Function:	
*Input:		
*Output:	NULL 
*Other:		
****************************************************************/
uint8_t flash_write_one_word(uint32_t WriteAddress, uint32_t WriteData)
{
	  FLASH_Status FLASHStatus = FLASH_COMPLETE;   
		FLASH_UnlockBank1();
		FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR); 
		FLASHStatus = FLASH_ProgramWord(WriteAddress, WriteData);
		FLASH_LockBank1();
		if(FLASHStatus == FLASH_COMPLETE)
			return OK;
		else
			return ERR;
}

/*************************************************************
  Function   : IAP_FlashEease 
  Description: 
  Input      : none        
  return     : none    
*************************************************************/
uint8_t erase_flash_part(const uint32_t flash_start, const uint32_t flash_end)
{
	uint32_t NbrOfPage;
	uint32_t EraseCounter;
	FLASH_Status FLASHStatus;
	FLASH_Unlock();
	NbrOfPage = (flash_end - flash_start) / PAGE_SIZE;
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);       
	for(EraseCounter = 0; EraseCounter < NbrOfPage; EraseCounter++)
	{
		FLASHStatus = FLASH_ErasePage(flash_start + (PAGE_SIZE * EraseCounter));
		if(FLASHStatus != FLASH_COMPLETE)
		{
			FLASH_Lock();
			return ERR;
		}
	}
	FLASH_Lock();
	return OK;
}
/*************************************************************
  Function   : IAP_FlashEease 
  Description: 
  Input      : none        
  return     : none    
*************************************************************/
uint8_t erase_flash(void)
{
	return erase_flash_part(FLASH_IAP_ADDR, FLASH_END_ADDR);
}
/******************* (C) COPYRIGHT 2016*****END OF FILE****/
