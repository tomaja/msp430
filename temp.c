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
	WDTCTL = WDTPW + WDTHOLD;				// Остановка сторожевого таймера

//	int i = 456;
//	i = func(123, i);



	P1DIR = 0;								// Устанавливаем шину 1 в режим вывода сигналов
	P1OUT = 0;								// Обнуляем сигналы на шине 1
	P1DIR = BIT0 | BIT6;					// Устанавливаем линии 0 и 6 на шине 1 в режим вывода сигналов

	P1IFG = 0;								// Сбрасываем флаги прерываний от линий шины 1
	P1IE |= BIT3;							// Устанавливаем бит разрешения прерывания от линии 3 шины 1
	P1IES |= BIT3;							// Устанавливаем флаг генерации прерывания по заднему фронту сигнала
	P1REN |= BIT3;							// Установить подтяжку 3й линии шины 1

	TA0CCTL0 |= CCIE;						// Разрешаем прерывания от таймера
	TACCR0 = 65535;							// Устанавливаем верхнюю границу счета таймера
	TA0CTL |= MC_1 + TASSEL_2 + ID_1;				// Устанавливаем таймер в режим счета вверх


	__bis_SR_register(GIE);					// Установить бит GIE (разрешить прерывания) регистра статуса SR

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


#pragma vector=PORT1_VECTOR					// Устанавливаем обработчик прерывания от шины 1
__interrupt void Port1_ISR(void)
{
	P1IFG = 0;
//	if(MIDDLE == 2500)
//		MIDDLE = 20000;
//	MIDDLE /= 2;
	P1OUT ^= BIT6;
}


#pragma vector=TIMER0_A0_VECTOR					// Устанавливаем обработчик прерывания от таймера А
__interrupt void TimerA0_ISR(void)
{
//	TA0CCTL0 &= ~CCIFG;
//	if(MIDDLE == 2500)
//		MIDDLE = 20000;
//	MIDDLE /= 2;
	P1OUT ^= BIT6;
}

