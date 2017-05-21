#ifndef	__DBGU_H__
#define	__DBGU_H__


#define	BAUD		115200
#define	DBGU_CD	(MCK/(16*BAUD))


void Dbgu_Init(void);
unsigned char Dbgu_Rx(void);
void Dbgu_Tx(unsigned char cData);


#endif	/* __DBGU_H__ */
