// Functions for ST7032I

// Write a string to LCD
void SendStr(uint8 *ptString)
{
	while((*ptString)!='\0')
	{
		SendByte(iDat, *ptString++);
	}
}

// Move Cursor or display
void Move(uint8 dir)
{
	SendByte(iCmd, dir);
}

// Goto specific location
void Gotoxy(uint8 Row, uint8 Col)
{
	switch (Row)
	{
		#if defined(LCD_L2)
		case 2:
			SendByte(iCmd, LCD_L2 + Col); break;
		#endif
		#if defined(LCD_L3)
		case 3:
			SendByte(iCmd, LCD_L3 + Col); break;
		#endif
		#if defined(LCD_L4)
		case 4:
			SendByte(iCmd, LCD_L4 + Col); break;
		#endif
		default:
			SendByte(iCmd, LCD_L1 + Col);
	}
}
// Clear LCD Screen
void LCD_CLS(void)
{
	SendByte(iCmd, 0x01);
	DelayMs(5);
	// 2ms delay is Necessary after sending LCD_CLS command !!!
}

// Wait some time and clear screen
void wait_and_clear(void)
{
	DelayMs(1500);
	LCD_CLS();
}

// Tile LCD Screen with one data
void Tile(uint8 tData)
{
	uint8 k;
	SendByte(iCmd, LCD_L1);
	for (k = 0; k < LCD_CHAR; k++)
	{
		switch (k)
		{
			#if defined(LCD_L2)
				case LCD_COL:
				SendByte(iCmd, LCD_L2); break;
			#endif
			#if defined(LCD_L3)
				case LCD_COL*2:
				SendByte(iCmd, LCD_L3); break;
			#endif
			#if defined(LCD_L4)
				case LCD_COL*3:
				SendByte(iCmd, LCD_L4); break;
			#endif
			default:
			break;
		}
		SendByte(iDat, tData);
	}
}
// Fill CGRAM with array CGRAM[]
void FillCGRAM(void)
{
	uint8 i;
	SendByte(iCmd, 0x38);
	DelayUs(30);
	SendByte(iCmd, LCD_CGRAM_ADDR);
	for (i = 0; i < LCD_CGMAX; i++)
	{
		SendByte(iDat, pgm_read_byte(CGRAM + i));
	}
}

// Show All patterns in CGRAM
void ShowCGRAM(void)
{
	uint8 i,k;
	
	for (i = 0; i < 8; i++)
	{
		SendByte(iCmd, LCD_OFF);
		SendByte(iCmd, LCD_L1);
		for (k = 0; k < LCD_CHAR; k++)
		{
			switch (k)
			{
				#if defined(LCD_L2)
					case LCD_COL:
					SendByte(iCmd, LCD_L2); break;
				#endif
				#if defined(LCD_L3)
					case LCD_COL*2:
					SendByte(iCmd, LCD_L3); break;
				#endif
				#if defined(LCD_L4)
					case LCD_COL*3:
					SendByte(iCmd, LCD_L4); break;
				#endif
				default:
				break;
			}
			SendByte(iDat, i);
		}
		SendByte(iCmd, LCD_ON);
		wait_and_clear();
	}
}
// Call built-in Characters
void CallBuiltinChar(void)
{
	uint8 i, k;
	for (i = 0; i < LCD_COL; i += LCD_ROW)
	{
		SendByte(iCmd, LCD_L1);
		for (k = 0; k < LCD_CHAR; k++)
		{
			#if defined(LCD_L2)
			switch (k)
			{
				case LCD_COL:
				SendByte(iCmd, LCD_L2); break;
				#if defined(LCD_L3)
				case LCD_COL*2:
				SendByte(iCmd, LCD_L3); break;
				#endif
				#if defined(LCD_L4)
				case LCD_COL*3:
				SendByte(iCmd, LCD_L4); break;
				#endif
				default:
				break;
			}
			#endif
			SendByte(iDat, k + LCD_COL*i);
		}
		wait_and_clear();
	}
}
// LCD initialize procedure
void LCD_Initial(void)
{
	LCD_Reset();
	
	SendByte(iCmd, 0x38);
	DelayUs(30);
	SendByte(iCmd, 0x39);	// Function Set
	DelayUs(30);
	SendByte(iCmd, 0x14);	// Internal OSC Frequency
	DelayUs(30);
	SendByte(iCmd, 0x78);	// Contrast Adjustment (Lower 4 bits)
	DelayUs(30);
	SendByte(iCmd, 0x57);	// Power/Icon/Contrast Control
	// (Icon off/Booster On/C5C4=11)
	DelayUs(30);
	SendByte(iCmd, 0x6A);	// Follower control
	DelayMs(200);
	
	SendByte(iCmd, LCD_ON);
	DelayUs(50);
	LCD_CLS();
	SendByte(iCmd, LCD_ENTRY);	// Entry Mode Set
}
