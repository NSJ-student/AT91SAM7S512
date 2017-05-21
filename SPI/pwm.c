#include <AT91SAM7S256.h>
#include "pwm.h"

void Pwm2_Init(void)
{
	*AT91C_PMC_PCER = 1<<AT91C_ID_PWMC;		// PWM 장치에 클럭 공급
	*AT91C_PIOA_PPUDR = AT91C_PA25_PWM2;		// 25번 핀의 pull-up off
	
	*AT91C_PIOA_PDR 	= AT91C_PA25_PWM2;		// 25번 핀의 PIO기능 off
	*AT91C_PIOA_BSR 	= AT91C_PA25_PWM2;		// 25번 핀의 PWM2 기능 on
	// CLKA, CLKB 를 200kHz로 설정
	*AT91C_PWMC_MR = (30<<DIVA)|(30<<DIVB)|(3<<PREA)|(3<<PREB);
	// CLKA 클럭을 사용, left aligned, low level start, CUPD modify period cycle
	*AT91C_PWMC_CH2_CMR = (5)&(~AT91C_PWMC_CALG)&(~AT91C_PWMC_CPOL)|(AT91C_PWMC_CPD);

	*AT91C_PWMC_CH2_CPRDR = OCT5_C;		// PWM2 주기를 결정
	*AT91C_PWMC_CH2_CDTYR = (unsigned int)(OCT5_C/2);		// PWM2 듀티비를 결정
	//*AT91C_PWMC_ENA = AT91C_PWMC_CHID2;	// PWM2 시작
	
	return;
}

void Pwm2_Run(int iPeriod)
{
	*AT91C_PWMC_DIS = AT91C_PWMC_CHID2;	// PWM2 정지

	*AT91C_PWMC_CH2_CPRDR = iPeriod;				// PWM2 주기를 결정
	*AT91C_PWMC_CH2_CDTYR = (unsigned int)(iPeriod/2);	// PWM2 듀티비를 결정

	*AT91C_PWMC_ENA = AT91C_PWMC_CHID2;	// PWM2 시작
	
	return;
}

void Pwm2_Stop(void)
{
	*AT91C_PWMC_DIS = AT91C_PWMC_CHID2;	// PWM2 정지

	return;
}



