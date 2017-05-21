#include <project.h>

int main()
{
	Lcd_Init();
	Dbgu_Init();
	
	while (1)
	{
		// DBGU에서 수신된 문자를 LCD에 출력
		//Lcd_Data(Dbgu_Rx());
	}
	
	return 0;
}


