#ifndef	__LCD_H__
#define	__LCD_H__

#define	LCD_BUS			(0x00FF0000)
#define	LCD_RS				(0x00000001)
#define	LCD_RW				(0x00000002)
#define	LCD_EN				(0x00000004)
#define	LCD_CAL_INST(x)	(   (LCD_BUS) & ((x)<<16)   )

#define 	LCD_INST_CLR	0x01
#define 	LCD_INST_HOME	0x02
#define 	LCD_INST_ENT	0x06
#define 	LCD_INST_DSP	0x0F
#define 	LCD_INST_CUR	0x14
#define 	LCD_INST_FUNC	0x38

void Lcd_Init();
void Lcd_Inst(unsigned int uiInst);
void Lcd_Data(unsigned int uiData);
void Lcd_Print(const unsigned char * ucString);

#endif	/* __LCD_H__ */
