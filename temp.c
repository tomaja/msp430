/*
 * main.c
 */
#include "msp430.h"


int func (int val1, int val2)
{
	return val1 + val2;
}



unsigned long MIDDLE = 10000;
void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;				// ��������� ����������� �������

//	int i = 456;
//	i = func(123, i);



	P1DIR = 0;								// ������������� ���� 1 � ����� ������ ��������
	P1OUT = 0;								// �������� ������� �� ���� 1
	P1DIR = BIT0 | BIT6;					// ������������� ����� 0 � 6 �� ���� 1 � ����� ������ ��������

	P1IFG = 0;								// ���������� ����� ���������� �� ����� ���� 1
	P1IE |= BIT3;							// ������������� ��� ���������� ���������� �� ����� 3 ���� 1
	P1IES |= BIT3;							// ������������� ���� ��������� ���������� �� ������� ������ �������
	P1REN |= BIT3;							// ���������� �������� 3� ����� ���� 1

	TA0CCTL0 |= CCIE;						// ��������� ���������� �� �������
	TACCR0 = 65535;							// ������������� ������� ������� ����� �������
	TA0CTL |= MC_1 + TASSEL_2 + ID_1;				// ������������� ������ � ����� ����� �����


	__bis_SR_register(GIE);					// ���������� ��� GIE (��������� ����������) �������� ������� SR

	volatile long Counter = 0;
	while(1)
	{
		++Counter;
		P1OUT |= (Counter < MIDDLE);
//		P1OUT |= (!(Counter < MIDDLE)) << 6;
		if(Counter == 2 * MIDDLE)
			Counter = 0;
	}
}


#pragma vector=PORT1_VECTOR					// ������������� ���������� ���������� �� ���� 1
__interrupt void Port1_ISR(void)
{
	P1IFG = 0;
//	if(MIDDLE == 2500)
//		MIDDLE = 20000;
//	MIDDLE /= 2;
	P1OUT ^= BIT6;
}


#pragma vector=TIMER0_A0_VECTOR					// ������������� ���������� ���������� �� ������� �
__interrupt void TimerA0_ISR(void)
{
//	TA0CCTL0 &= ~CCIFG;
//	if(MIDDLE == 2500)
//		MIDDLE = 20000;
//	MIDDLE /= 2;
	P1OUT ^= BIT6;
}

