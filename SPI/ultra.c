#include <AT91SAM7S256.h>
#include "ultra.h"

static volatile unsigned int uiCm;
static volatile unsigned int uiState = INIT_OFF;

static void Timer_ISR(void);

static void UTimer_Init(void)
{
// 타이머 카운터 0 사용을 위한 PMC 활성화(AT91C_ID_TC0)
	*AT91C_PMC_PCER = 1<<AT91C_ID_TC0;

// 1.시작 : 타이머 클럭 비활성화 ------------------
// 타이머 클럭 비활성화(TC_CCR 설정)
	*AT91C_TC0_CCR = AT91C_TC_CLKDIS;

// 2. 시작 : 타이머 인터럽트 비활성화 -------------
// 타이머 인터럽트 비활성화(TC_IDR 설정)
// 인터럽트 상태 정보 초기화(TC_SR 읽기)
	*AT91C_TC0_IDR = 0xFFFFFFFF;
	*AT91C_TC0_SR;	// 레지스터 읽기만 한다

// 분주비 128, 비교 방식 레지스터 설정(TC_CMR, DIV4_CLOCK, AT91C_TC_CPCTRG)
// MCKR divided by 128 => 12, 0.25us(TC_RC 설정)
	*AT91C_TC0_CMR = AT91C_TC_CLKS_TIMER_DIV4_CLOCK|AT91C_TC_CPCTRG;

// 타이머 카운터 0 인터럽트 비활성화(AIC_IDCR, AT91C_ID_TC0)
	*AT91C_AIC_IDCR = 1<<AT91C_ID_TC0;

// 3. 시작 : 타이머 카운터 0 인터럽트 비활성화 ------
// 타이머 카운터 0 인터럽트 핸들러 등록(AIC_SVR[AT91C_ID_TC0], timer_handler)
// 타이머 카운터 0 인터럽트 모드 설정(AIC_SMR[AT91C_ID_TC0], AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL, AT91C_AIC_PRIOR_LOWEST)
// 타이머 카운터 0 인터럽트 클리어(AIC_ICCR, AT91C_ID_TC0)
	AT91C_AIC_SVR[AT91C_ID_TC0] = (unsigned int)Timer_ISR;	// 인터럽트 함수의 주소(벡터라 부른다)
	AT91C_AIC_SMR[AT91C_ID_TC0] = AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL|AT91C_AIC_PRIOR_LOWEST;
	*AT91C_AIC_ICCR = 1<<AT91C_ID_TC0;		// 인터럽트가 호출된 내역을 삭제 (시작하자마자 인터럽트가 걸리지 않도록)

// TC_RC 값 비교 타이머 인터럽트 활성화(TC_IER, AT91C_TC_CPCS)
	*AT91C_TC0_IER = AT91C_TC_CPCS;
	*AT91C_TC0_RC = 22;				// 1cm 전진하면 인터럽트가 발생(58us 간격)

// 2. 끝 : 타이머 인터럽트 비활성화 ---------------

// 타이머 카운터 0 인터럽트 활성화(AIC_IECR, AT91C_ID_TC0)
	*AT91C_AIC_IECR =1<< AT91C_ID_TC0;	

// 3. 끝 : 타이머 카운터 0 인터럽트 비활성화 -------

// 타이머 클럭 활성화(TC_CCR, AT91C_TC_CLKEN)
	// *AT91C_TC0_CCR = AT91C_TC_CLKEN;

// 1. 끝 : 타이머 클럭 비활성화 ------------------

// 타이머 시작(TC_CCR, AT91C_TC_SWTRG)
	// *AT91C_TC0_CCR = AT91C_TC_SWTRG;

}

static void Timer_ISR(void)	// 소리가 1cm 전진하면 발생하는 인터럽트
{
// 인터럽트 상태 정보 초기화(TC_SR 읽기)
	*AT91C_TC0_SR;	// 레지스터 읽기만 한다

	++uiCm;		// 물체와의 거리측정
}


static void UTrigger_Init(void)		// 초음파 센서 trigger 초기화
{ 
	*AT91C_PMC_PCER 	= 1<<AT91C_ID_PIOA;	// PIOA의 클럭 공급 on
	*AT91C_PIOA_PER 		= UTRI_PIN;			// 초음파 trigger 핀 활성화
	*AT91C_PIOA_PPUDR 	= UTRI_PIN;			// PIOA pull-up off(스위치를 사용하는데 주로 사용)
	*AT91C_PIOA_OER 		= UTRI_PIN;			// 출력 enable
	*AT91C_PIOA_CODR 	= UTRI_PIN;			// PIOA를 clear
	
	return;
}

static void Echo_Init(void)		// 초음파 센서 echo 초기화
{ 
	*AT91C_PMC_PCER 	= 1<<AT91C_ID_PIOA;	// PIOA의 클럭 공급 on
	*AT91C_PIOA_PER 		= ECHO_PIN;			// 초음파 echo 핀 활성화
	*AT91C_PIOA_PPUDR 	= ECHO_PIN;			// PIOA pull-up off(스위치를 사용하는데 주로 사용)
	
	return;
}

static void UTrigger(void)			// 초음파 센서 trigger 신호 출력
{
	volatile unsigned int uiCnt;
	
	// trigger pulse
	*AT91C_PIOA_CODR 	= UTRI_PIN;			// PIOA를 clear
	for(uiCnt = 0; uiCnt<1000; uiCnt++);	// delay
	*AT91C_PIOA_SODR 	= UTRI_PIN;			// PIOA를 set
	for(uiCnt = 0; uiCnt<1000; uiCnt++);	// delay
	*AT91C_PIOA_CODR 	= UTRI_PIN;			// PIOA를 clear
	
	return;
}

unsigned int Ultra_Run(void)
{
	if( INIT_OFF == uiState)
	{
		return 0;
	}

	uiCm = 0;						// 거리 초기화
	*AT91C_TC0_CCR = AT91C_TC_CLKEN;	// timer/counter clock enable
	
	UTrigger();		// trigger 신호 발사
	
	// echo 신호 : polling방식 검출
	while( 0 == (ECHO_PIN&(*AT91C_PIOA_PDSR)) );	// pin 입력이 low인 상태에서 대기
	*AT91C_TC0_CCR = AT91C_TC_SWTRG;				// timer/counter 시작

	while( 0 != (ECHO_PIN&(*AT91C_PIOA_PDSR)) );	// pin 입력이 high인 상태에서 대기
	*AT91C_TC0_CCR = AT91C_TC_CLKDIS;				// timer/counter clock disable

	
	return uiCm;
}

void Ultra_Init(void)
{
	UTimer_Init();			// 거리 측정을 위한 타이머 초기화
	UTrigger_Init();		// 초음파 센서 trigger 신호를 주기위한 핀 초기화
	Echo_Init();			// echo 신호를 받기위한 핀 초기화
	
	uiState = INIT_ON;

	return;
}

