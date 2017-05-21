#ifndef	__PWM_H__
#define	__PWM_H__


////////////////////// PWM_MR

// CLKA 클럭 설정용
#define DIVA			0
#define PREA			8
// CLKB 클럭 설정용
#define DIVB			16
#define PREB			24

#define MAX_DUTY	800
#define DEF_DUTY		400

#define OCT5_C	2867
#define OCT5_C_	2706
#define OCT5_D	2554
#define OCT5_D_	2411
#define OCT5_E	2275
#define OCT5_F	2148
#define OCT5_F_	2027
#define OCT5_G	1913
#define OCT5_G_	1806
#define OCT5_A	1705
#define OCT5_A_	1609
#define OCT5_B	1519

void Pwm2_Init(void);
void Pwm2_Run(int iPeriod);
void Pwm2_Stop(void);

#endif	/* __PWM_H__ */


