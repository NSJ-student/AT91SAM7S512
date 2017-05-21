#include <project.h>

int main(void)
{
	unsigned char ucaDist[] = "000cm";
	unsigned int uiCm;
	volatile unsigned int uiCnt = 0;
	
	Lcd_Init();
	Ultra_Init();
	
	while (1)
	{
		uiCm = Ultra_Run();

		for(uiCnt = 0; uiCnt<0x5FFFF; uiCnt++);		// delay
		Lcd_Inst(LCD_INST_HOME);				// LCD 커서를 옮긴다
		
		if( 0 == uiCm )				// 결과 값이 0이면 에러
		{
			Lcd_Print("Error");		// 에러를 출력하고 재측정
			continue;
		}

		ucaDist[0] = '0' + (uiCm/100)%10;	// 거리값을 아스키 문자열로 변환
		ucaDist[1]	 = '0' + (uiCm%100)/10;
		ucaDist[2]	 = '0' + uiCm%10;
		
		Lcd_Print(ucaDist);				// 거리값 출력
	}
	
	return 0;
}

