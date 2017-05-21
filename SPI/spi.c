#include <AT91SAM7S256.h>
#include "spi.h"

void Spi_Init(void)
{
	*AT91C_PMC_PCER = 1<<AT91C_ID_SPI;
	*AT91C_PIOA_PDR = AT91C_PIO_PA11|AT91C_PIO_PA12|AT91C_PIO_PA13|AT91C_PIO_PA14;
	*AT91C_PIOA_ASR = AT91C_PIO_PA11|AT91C_PIO_PA12|AT91C_PIO_PA13|AT91C_PIO_PA14;

	// spi disable, reset
	*AT91C_SPI_CR = AT91C_SPI_SPIDIS|AT91C_SPI_SWRST;
	// master, direct to device, fixed CS, CS0 select, MFD disable   // local loopback
	*AT91C_SPI_MR = (1<<0)|((0x0E<<16)&AT91C_SPI_PCS)|AT91C_SPI_MODFDIS;  //|AT91C_SPI_LLB;	
	// NCPHA = 0, CPOL = 1 => SCK의 rising edge에서 data sample , 1MHz SCK
	// CSAAT=1 : 전송 완료 후 CS가 high로 변하지 않음 => SPI enable~disable에서 CS가 active 상태
	AT91C_SPI_CSR[0] = AT91C_SPI_CPOL|AT91C_SPI_CSAAT|((48<<8)&AT91C_SPI_SCBR);

	return;
}

unsigned char Spi_Data(unsigned char ucTxData)
{
	while( 0 == (AT91C_SPI_TDRE&(*AT91C_SPI_SR)));
	*AT91C_SPI_TDR = (ucTxData&AT91C_SPI_TD);
	while( 0 == (AT91C_SPI_RDRF&(*AT91C_SPI_SR)));

	return (unsigned char)((*AT91C_SPI_RDR)&0xFF);	
}

