#include <project.h>

int main(void)
{
	const unsigned int uiMel[7] = {OCT5_C, OCT5_D, OCT5_E, OCT5_F, OCT5_G, OCT5_A, OCT5_B};
	unsigned char ucKey;
	unsigned int uiCnt = 0;
	volatile unsigned int vuiCnt;
	int iDuty = DEF_DUTY; 
	
	Dbgu_Init();
	Pwm2_Init();
	
	while (1)
	{
		ucKey = Dbgu_Rx();		// UART로 음 정보 수신

		if( '1' <= ucKey )
		{
			if( ucKey <= '7')
			{
				uiCnt = ucKey - '1';	// 수신 신호 변환
			}
		}
		
		Pwm2_Run(uiMel[uiCnt]);	// 입력 음의 주파수 PWM 출력
		for(vuiCnt = 0; vuiCnt < 0x7FFFF; vuiCnt++);	// delay
		Pwm2_Stop();			// PWM 출력 정지
	}
	
	return 0;
}



