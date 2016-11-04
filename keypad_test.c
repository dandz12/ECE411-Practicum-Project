/*
 * Testing.c
 *
 * Created: 11/1/2016 2:45:16 PM
 * Author : dandiaz
 */ 

#include <avr/io.h>
#include <util/delay.h>

void blink(int);
int getKey(void);

int main(void)
{
	int key = 0;
	int passCode[4];				//sets and array of int's 4 elements long, this will be our pass code
	DDRB |= 1 << PINB0;				//set port B as output for LED
	DDRC |= 0xF0;					//set port C as output
	PORTC = 0x0F;					//active pull up resistor
	
    while (1) 
    {
		if(PINC != 0xF0)
			blink(getKey());
    }
}

//this function will flip bits on port C and return the value of the key pressed
int getKey(void)
{
			int num = 0;
			
			//flip bits and set num
			_delay_ms(5);			//delay just in case for debouncing
			num = PINC;				//assign num the digit pressed
			DDRC ^= 0xFF;			//XOR and set to flip port direction
			_delay_ms(1);
			PORTC ^= 0xFF;			//set pins high, again using XOR set
			_delay_ms(1);
			num |= PINC;			//or and set number to what PinD is reading
			
			//determine the button pressed and call blink LED function
			if(num == 0x88)
				num = 1;
			if(num == 0x48)
				num = 2;
			if(num == 0x28)
				num = 3;
			if(num == 0x84)
				num = 4;
			if(num == 0x44)
				num = 5;
			if(num == 0x24)
				num = 6;
			if(num == 0x82)
				num = 7;
			if(num == 0x42)
				num = 8;
			if(num == 0x22)
				num = 9;
			if(num == 0x41)
				num = 0;
				
			//reset everything to its initial state for the next button press
			DDRC ^= 0xFF;
			_delay_ms(1);
			PORTC ^= 0xFF;
			_delay_ms(5);			 	
			
			return num;
}

//simply take in number and blink that amount
void blink(int number)
{
	for(int i = 0; i < number; i++)
	{
		PORTB ^= 1 << PINB0;
		_delay_ms(500);
		PORTB ^= 1 << PINB0;
		_delay_ms(500);
		
	}
}
