#include <AT91SAM7S256.h>
#include "lcd.h"

void Lcd_Init()
{
	volatile unsigned int uiCnt;

	//*PIO_PER = LCD_BUS|LCD_RS|LCD_RW|LCD_EN;
	*AT91C_PIOA_PER = LCD_BUS|LCD_RS|LCD_RW|LCD_EN;			// PIOA Enable
	//AT91C_BASE_PIOA->PIO_PER = LCD_BUS|LCD_RS|LCD_RW|LCD_EN;
	
	*AT91C_PIOA_OER = LCD_BUS|LCD_RS|LCD_RW|LCD_EN;			// PIOA Output Enable
	*AT91C_PIOA_PPUDR = LCD_BUS|LCD_RS|LCD_RW|LCD_EN;			// PIOA Pull-up disable
	
	for(uiCnt = 0; uiCnt < 10000; uiCnt++);	// LCD 부팅 대기

	// LCD 초기화
	Lcd_Inst(LCD_INST_FUNC);		// data length 8bit(DL = 1), 2 line(N = 1), 5x8 font(F = 0)
	Lcd_Inst(LCD_INST_DSP);		// display on(D = 1), cursor on(C = 1), blink on(B = 1)
	Lcd_Inst(LCD_INST_ENT);		// increase mode(I/D = 1), shift off(S = 0)
	Lcd_Inst(LCD_INST_CUR);		// cursor home
	Lcd_Inst(LCD_INST_CLR);		// clear display
	Lcd_Inst(LCD_INST_HOME);	// cursor home
	
	for(uiCnt = 0; uiCnt < 5000; uiCnt++);	// delay
	
	return;
}

void Lcd_Inst(unsigned int uiInst)
{
	volatile unsigned int uiCnt;
	// all init, RS = low
	*AT91C_PIOA_CODR = LCD_BUS|LCD_RS|LCD_RW|LCD_EN;
	*AT91C_PIOA_SODR = LCD_CAL_INST(uiInst);
	
	for(uiCnt = 0; uiCnt < 10; uiCnt++);
	
	*AT91C_PIOA_SODR = LCD_EN;		// EN = high
	
	for(uiCnt = 0; uiCnt < 300; uiCnt++);	// BUS = data
									// BUS = valid data
	*AT91C_PIOA_CODR = LCD_EN;
	for(uiCnt = 0; uiCnt < 5000; uiCnt++);	// EN = low
	
	return;
}

void Lcd_Data(unsigned int uiData)
{
	volatile unsigned int uiCnt;
	// all init
	*AT91C_PIOA_CODR = LCD_BUS|LCD_RS|LCD_RW|LCD_EN;
	*AT91C_PIOA_SODR = LCD_RS;		// RS = high
	*AT91C_PIOA_SODR = LCD_CAL_INST(uiData);
	
	for(uiCnt = 0; uiCnt < 10; uiCnt++);	
	
	*AT91C_PIOA_SODR = LCD_EN;		// EN = high
	
	for(uiCnt = 0; uiCnt < 100; uiCnt++);	// BUS = data
									// BUS = valid data
	*AT91C_PIOA_CODR = LCD_EN;
	for(uiCnt = 0; uiCnt < 10; uiCnt++);	// EN = low
	
	return;
}

void Lcd_Print(const unsigned char * ucString)
{
	while( 0 != *ucString )
	{
		Lcd_Data(*ucString);
		++ucString;
	}

	return;
}

