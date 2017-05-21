#include <project.h>

unsigned char GetASCh(unsigned int uiNum, unsigned char ucNum);
void Print_Temp(unsigned int uiTemp);
void Print_All(unsigned char ucHTemp, unsigned char ucLTemp);

int main(void)
{	
	unsigned char ucHTemp = 0;
	unsigned char ucLTemp = 0;
	unsigned int uiValue = 0;

	Lcd_Init();
	Timer_Init();
	Spi_Init();

	while (1)
	{
		// 타겟 온도
		*AT91C_SPI_CR = AT91C_SPI_SPIEN;
		ms_delay(1);
		ucLTemp = Spi_Data(0xA0);	// 명령 신호
		ms_delay(10);
		ucLTemp = Spi_Data(0x22);	// 하위 1 byte
		ms_delay(10);
		ucHTemp = Spi_Data(0x22);	// 상위 1 byte
		*AT91C_SPI_CR = AT91C_SPI_SPIDIS;

		Lcd_Inst(0x80 | 0x00);
		Print_All(ucHTemp, ucLTemp);

		ms_delay(100);

		// 주변 온도
		*AT91C_SPI_CR = AT91C_SPI_SPIEN;
		ms_delay(1);
		ucLTemp = Spi_Data(0xA1);
		ms_delay(10);
		ucLTemp = Spi_Data(0x22);
		ms_delay(10);
		ucHTemp = Spi_Data(0x22);
		*AT91C_SPI_CR = AT91C_SPI_SPIDIS;

		Lcd_Inst(0x80 | 0x40);
		Print_All(ucHTemp, ucLTemp);

		ms_delay(1000);
	}
	
	return 0;
}

void Print_All(unsigned char ucHTemp, unsigned char ucLTemp)
{
	unsigned int uiValue;
	
	uiValue = ucHTemp<<8;
	uiValue = uiValue|ucLTemp;
	
	Print_Temp(uiValue/100);
	Lcd_Data('.');
	Print_Temp(uiValue%100);
	
	Lcd_Print("/ ");
	Lcd_Data(GetASCh(uiValue, 4));
	Lcd_Data(GetASCh(uiValue, 3));
	Lcd_Data(GetASCh(uiValue, 2));
	Lcd_Data(GetASCh(uiValue, 1));

	return;
}
	
void Print_Temp(unsigned int uiTemp)
{
	unsigned char ucTen = (uiTemp/10)%10;
	unsigned char ucOne = uiTemp%10;

	Lcd_Data(ucTen + '0');
	Lcd_Data(ucOne + '0');

	return;
}

unsigned char GetASCh(unsigned int uiNum, unsigned char ucNum)
{
	unsigned int tmp = uiNum;

	tmp = (uiNum>>(4*(ucNum-1)))&0x000F;

	if(tmp >= 10)
		return 'A'+tmp-10;
	else
		return '0'+tmp;
}


