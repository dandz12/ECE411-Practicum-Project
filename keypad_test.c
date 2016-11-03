/*
 * Testing.c
 *
 * Created: 11/1/2016 2:45:16 PM
 * Author : dandiaz
 */ 

#include <avr/io.h>
#include <util/delay.h>

void blink(int);

int main(void)
{
	int num = 0;
	DDRB |= 1 << PINB0;				//set port B as output
	DDRD |= 0b11000000;				//set port D as output
	

	PORTD = 0b00110000;
	
    while (1) 
    {
		if(PIND != 0b11000000)		//detect if a key is pressed
		{
			_delay_ms(5);			//delay just in case for debouncing
			num = PIND;				//assign num the digit pressed
			
			DDRD ^= 0xF0;			//set d7/d6 as inputs and d4/d5 as outputs, this is XORing to flip bits
			_delay_ms(1);
			PORTD ^= 0xF0;			//set pins high, again using XOR set
			_delay_ms(1);
			
			num |= PIND;			//or and set number to what PinD is reading
			
			//determine the button pressed and call blink LED function
			if(num == 0b10100000)
				blink(5);
			if(num == 0b10010000)
				blink(4);
			if(num == 0b01100000)
				blink(2);
			if(num == 0b01010000)
				blink(1);
				
			//reset everything to its initial state for the next button press
			num = 0;
			DDRD ^= 0xF0;
			_delay_ms(1);
			PORTD ^= 0xF0;
			_delay_ms(5);			 
			
		}
    }
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
