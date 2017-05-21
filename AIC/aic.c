#include <AT91SAM7S256.h>
#include "aic.h"

void Aic_Init(void)
{
	Lcd_Init();

	*AT91C_PMC_PCER = 1<<AT91C_ID_PIOA;		// PIOA 장치 활성화
	*AT91C_PIOA_PER = 1<<INT_PIN;			// 핀 Enable
	*AT91C_PIOA_OER = 1<<INT_PIN;			// Output Enable
	*AT91C_PIOA_IDR = 1<<INT_PIN;			// 인터럽트 핀 Disable
	*AT91C_PIOA_IFER = 1<<INT_PIN;			// 입력필터 Enable

	*AT91C_AIC_IDCR = 1<<AT91C_ID_PIOA;		// 인터럽트 핀 command  disable
	*AT91C_AIC_ICCR = 1<<AT91C_ID_PIOA;		// command register clear

	AT91C_AIC_SVR[AT91C_ID_PIOA] = (unsigned int)Handler;	// 핸들러 등록
	AT91C_AIC_SMR[AT91C_ID_PIOA] = 
		AT91C_AIC_SRCTYPE_POSITIVE_EDGE|AT91C_AIC_PRIOR_LOWEST;	// 동작모드 및 우선순위 결정

	*AT91C_PIOA_IER = 1<<INT_PIN;			// PIOA 중 15번 핀 인터럽트 활성화
	*AT91C_AIC_IECR = 1<<AT91C_ID_PIOA;		// PIOA 인터럽트 활성화

	return;
}

void Handler(void)
{
	volatile unsigned int uiIsr;
	volatile unsigned int uiCnt;

	uiIsr = *AT91C_PIOA_ISR;

	if( (uiIsr&(1<<INT_PIN)) != 0 )
	{
		Lcd_Data('A');
	}
	
	for(uiCnt=0; 1000000>uiCnt; ++uiCnt);	// 채터링 방지

	uiIsr = *AT91C_PIOA_ISR;	// ISR 레지스터 클리어
	*AT91C_AIC_EOICR = 0;	// End Code

	return;
}

