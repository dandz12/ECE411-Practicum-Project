/*
 * Keypad_testing.c
 *
 * Created: 11/15/2016 5:49:11 PM
 * Author : dandiaz
 */ 

#define F_CPU 20000000UL

#include <avr/io.h>
#include <util/delay.h>

int getkey(void);
void blink(int);

int main(void)
{
	int num = 0;
	DDRC = 0b00111100;
	DDRB = 0b00000000;
	
	DDRD = 0b00000001;		//led
	PORTD = 0x00;
	
	PORTC = 0b00000011;		//output 5v and pull
	PORTB = 0b00110000;
	
	/* Replace with your application code */
	while (1)
	{

		if((((PINC & 0b00000011) << 2) | ((PINB & 0b00110000) >> 4)) != 0b00001111)
		{
			num = getkey();
			blink(num);
			num = 0;
		}

	}
}


int getkey(void)
{
	volatile int num = 0;
	
		_delay_ms(5);
		num = (((PINC & 0b00000011) << 2) | ((PINB & 0b00110000) >> 4));
		DDRB ^= 0x30;
		DDRC ^= 0x3F;
		_delay_ms(1);
		PORTB ^= 0x30;
		PORTC ^= 0x3F;
		
		_delay_ms(1);
		num |= (PINC << 2);
		
		
switch (num)
{
	case 0b11101110:
	num =1;
	break;
	case 0b11011110:
	num = 2;
	break;
	case 0b10111110:
	num = 3;
	break;
	case 0b11101101:
	num = 4;
	break;
	case 0b11011101:
	num = 5;
	break;
	case 0b10111101:
	num = 6;
	break;
	case 0b11101011:
	num = 7;
	break;
	case 0b11011011:
	num = 8;
	break;
	case 0b10111011:
	num = 9;
	break;
	case 0b11100111:
	num = 0;
	break;
	default:
	num = 0;
	break;
}

		
		_delay_ms(5);
		DDRB ^= 0x30;
		DDRC ^= 0x3F;
		_delay_ms(1);
		PORTB ^= 0x30;
		PORTC ^= 0x3F;
		
		_delay_ms(1);
	
	return num;
	
}

void blink(int num)
{
	for(int i = 0; i < num; i ++)
	{
		PORTD = 0x01;
		_delay_ms(10);
		PORTD = 0x00;
		_delay_ms(10);
		
	}
	
}
