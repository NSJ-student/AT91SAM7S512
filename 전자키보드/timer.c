#include <AT91SAM7S256.h>
#include "timer.h"

volatile static unsigned int uiMSec;	// ╢ы╦╔ ╪р╫╨(.c фдЕюо)©м ╟Цд║╟еЁ╙ ╪Жа╓гоаЖ ╦Ьго╣╣╥о гя╢ы

void Timer_ISR(void);

// Timer.c

// 0╧Ь е╦юл╦с а╒╣н╬Н: TC0 // 1╧Ь е╦юл╦с а╒╣н╬Н: TC1 // 2╧Ь е╦юл╦с а╒╣н╬Н: TC2
// ©╧: #define TC0_CCR    ((volatile unsigned int *)0xFFFA0000)
// юнем╥╢ем ╟Э╥ц : AT91C_BASE_AIC
// 0╧Ь е╦юл╦с ╟Э╥ц : AT91C_BASE_TC0
// PMC ╟Э╥ц   : AT91C_BASE_PMC

void Timer_Init(void)
{
// е╦юл╦с д╚©Нем 0 ╩Г©Кю╩ ю╖гя PMC х╟╪╨х╜(AT91C_ID_TC0)
	*AT91C_PMC_PCER = 1<<AT91C_ID_TC0;

// 1.╫цюш : е╦юл╦с е╛╥╟ ╨Ях╟╪╨х╜ ------------------
// е╦юл╦с е╛╥╟ ╨Ях╟╪╨х╜(TC_CCR ╪Ёа╓)
	*AT91C_TC0_CCR = AT91C_TC_CLKDIS;

// 2. ╫цюш : е╦юл╦с юнем╥╢ф╝ ╨Ях╟╪╨х╜ -------------
// е╦юл╦с юнем╥╢ф╝ ╨Ях╟╪╨х╜(TC_IDR ╪Ёа╓)
// юнем╥╢ф╝ ╩Себ а╓╨╦ цй╠Бх╜(TC_SR юп╠Б)
	*AT91C_TC0_IDR = 0xFFFFFFFF;
	*AT91C_TC0_SR;	// ╥╧аЖ╫╨ем юп╠Б╦╦ гя╢ы

// ╨паж╨Я 128, ╨Я╠Ё ╧Ф╫д ╥╧аЖ╫╨ем ╪Ёа╓(TC_CMR, DIV4_CLOCK, AT91C_TC_CPCTRG)
// MCKR divided by 128 => 12, 0.25us(TC_RC ╪Ёа╓)
	*AT91C_TC0_CMR = AT91C_TC_CLKS_TIMER_DIV4_CLOCK|AT91C_TC_CPCTRG;
	
// е╦юл╦с д╚©Нем 0 юнем╥╢ф╝ ╨Ях╟╪╨х╜(AIC_IDCR, AT91C_ID_TC0)
	*AT91C_AIC_IDCR = 1<<AT91C_ID_TC0;	

// 3. ╫цюш : е╦юл╦с д╚©Нем 0 юнем╥╢ф╝ ╨Ях╟╪╨х╜ ------
// е╦юл╦с д╚©Нем 0 юнем╥╢ф╝ гз╣И╥╞ ╣Н╥о(AIC_SVR[AT91C_ID_TC0], timer_handler)
// е╦юл╦с д╚©Нем 0 юнем╥╢ф╝ ╦П╣Е ╪Ёа╓(AIC_SMR[AT91C_ID_TC0], AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL, AT91C_AIC_PRIOR_LOWEST)
// е╦юл╦с д╚©Нем 0 юнем╥╢ф╝ е╛╦╝╬Н(AIC_ICCR, AT91C_ID_TC0)
	AT91C_AIC_SVR[AT91C_ID_TC0] = (unsigned int)Timer_ISR;
	AT91C_AIC_SMR[AT91C_ID_TC0] = AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL|AT91C_AIC_PRIOR_LOWEST;
	*AT91C_AIC_ICCR = 1<<AT91C_ID_TC0;		// юнем╥╢ф╝╟║ хёцБ╣х Ё╩©╙ю╩ ╩Ха╕ (╫цюшгоюз╦╤юз юнем╥╢ф╝╟║ ╟и╦╝аЖ ╬й╣╣╥о)

// TC_RC ╟╙ ╨Я╠Ё е╦юл╦с юнем╥╢ф╝ х╟╪╨х╜(TC_IER, AT91C_TC_CPCS)
	*AT91C_TC0_IER = AT91C_TC_CPCS;
	*AT91C_TC0_RC = 375;

// 2. Ё║ : е╦юл╦с юнем╥╢ф╝ ╨Ях╟╪╨х╜ ---------------

// е╦юл╦с д╚©Нем 0 юнем╥╢ф╝ х╟╪╨х╜(AIC_IECR, AT91C_ID_TC0)
	*AT91C_AIC_IECR =1<< AT91C_ID_TC0;	

// 3. Ё║ : е╦юл╦с д╚©Нем 0 юнем╥╢ф╝ ╨Ях╟╪╨х╜ -------

// е╦юл╦с е╛╥╟ х╟╪╨х╜(TC_CCR, AT91C_TC_CLKEN)
	*AT91C_TC0_CCR = AT91C_TC_CLKEN;

// 1. Ё║ : е╦юл╦с е╛╥╟ ╨Ях╟╪╨х╜ ------------------

// е╦юл╦с ╫цюш(TC_CCR, AT91C_TC_SWTRG)
	*AT91C_TC0_CCR = AT91C_TC_SWTRG;

}

void Timer_ISR(void)
{
// юнем╥╢ф╝ ╩Себ а╓╨╦ цй╠Бх╜(TC_SR юп╠Б)
	*AT91C_TC0_SR;	// ╥╧аЖ╫╨ем юп╠Б╦╦ гя╢ы

	++uiMSec;
}

void ms_delay(unsigned int uiMS)	// е╦юл╦с д╚©Нем interrupt╦╕ юл©Кгь╪╜ uiMS╦╦е╜ аЖ©╛╫це╟╢б гт╪Ж
{
	uiMSec = 0;

	while(uiMSec < uiMS);	// interrupt╟║ uiMS ╧Ь ╧ъ╩Щгр╣©╬х аЖ©╛╣х╢ы

	return;
}
