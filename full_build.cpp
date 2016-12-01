/*
 * Full_build.cpp
 *
 * Created: 11/21/2016 2:55:00 PM
 * Author : dandiaz
 */ 

#define F_CPU 1000000UL							// Defines CPU I/O speed
#define length 4

#define MOTOR_ON      PORTD |= (1 << PORTD3)		// Writes high to pin D3, used in PWM_Init() function
#define MOTOR_OFF     PORTD &= ~(1 << PORTD3)	// Writes low to pin D3, used in PWM_Init() function

#define ctrl PORTD				//4 bits data line = D7-D4 of MCU
#define en 0                    //enable signal = D0 of MCU
#define rs 2                    //register select signal = D2 of MCU


#include <avr/io.h>
#include <avr/interrupt.h>						// AVR interrupts header file
#include <util/delay.h>

//global definition of our code
int code[length] = {1, 2, 3, 4};

int getkey(void);
int testCode(int *);
void blink(int);
void unlock(void);
void PWM_Init(void);

void lcd_ini(void);						//initialize LCD
void LCD_cmd(unsigned char cmd);		//send 4bits command in 4bits mode (half of a command)
void LCD_write(unsigned char data);		//send 4bits data in 4bits mode (half of a 8 bit data)
void dis_cmd(char);						//send full 8bits command in 4bits mode
void dis_data(char);					//send full 8bits data in 4bits mode
void LCD_write_string(char *str);

int main(void)
{
	int codeIn[length];

	sei();					//enable interrupts
	//DDRD |= (1 << DDD3);
	DDRB |= (1 << DDB7);
	
	DDRD = 0xFF;					//Set Port D as output 
	
	//setup our ports
	DDRC = 0x3C;
	//DDRB = 0x00;
	PORTC = 0x03;		//output 5v and pull up
	PORTB = 0x30;		//output 5v and pull up
	
	lcd_ini();                  //Initialization of LCD
	_delay_ms(30);              // Wait 30ms to make sure that LCD is initialized
		
	LCD_write_string("Passcode please:");         // function to print string on LCD	
		
	/* Replace with your application code */
	while (1)
	{

		
		for(int i = 0; i < length; i ++)
		{
			codeIn[i] = getkey();

			/*
			if(codeIn[i] == -1);
				//change
			else if(codeIn[i] == -2)
				//reset				
				*/
		}
		
		if((testCode(codeIn) == 1))
		{
			unlock();
			PWM_Init();									// PWM (pulse width modulation) function

		}
		else
			PWM_Init();									// PWM (pulse width modulation) function
		
		
	}
}


int getkey(void)
{
	volatile int num = 0;
	
	//loop and wait for a button
	while(1)
	{
		
		if((((PINC & 0x03) << 2) | ((PINB & 0x30) >> 4)) != 0x0F)
		{
			_delay_ms(5);					//delay to debounce
			num = (((PINC & 0x03) << 2) | ((PINB & 0x30) >> 4));	//set the bitpattern for the first part of the keypad array
			
			DDRB ^= 0x30;					//change inputs
			DDRC ^= 0x3F;					//change inputs
			_delay_ms(1);					//delay to apply change
			
			PORTB ^= 0x30;					//change output high
			PORTC ^= 0x3F;					//change output high
			_delay_ms(1);					//delay to apply change
			
			num |= (PINC << 2);				//set the bitpattern for the second part of keypad array
			
			//determine what button was pushed based on bitpattern
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
				case 0b11010111:
					num = 0;
					break;
				case 0b11100111:	//*
					num = -1;
					break;
				case 0b10110111:	//#
					num = -2;
					break;
				//* should change passcode
				//#should reset
				default:	//default should be reset
					num = -2;
					break;
			}

			//set the outputs back to their original state
			_delay_ms(5);
			DDRB ^= 0x30;
			DDRC ^= 0x3F;
			_delay_ms(1);
			
			PORTB ^= 0x30;
			PORTC ^= 0x3F;
			_delay_ms(200);				//we need to delay so we don't get double input
			
			return num;
		}
	}
}

int testCode(int *codeIn)
{
	int result = 1;
	
	for(int i = 0; i < length; i ++)
	{
		if((code[i] =! codeIn[i]))
			result = 0;		
	}
	
	return result;
	
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

ISR(TIMER1_COMPA_vect)							// Interrupt service routine for OCRnA (output compare register A)
{												// On portion of the PWM cycle
	MOTOR_ON;									// Macro turns MOTOR on, defined above
}

ISR(TIMER1_COMPB_vect)							// Interrupt service routine for OCRnB (output compare register B)
{												// Off portion of the PWM cycle
	MOTOR_OFF;									// Macro turns MOTOR off, defined above
}

void PWM_Init(void)								// Function for PWM (pulse width modulation)
{
	TCCR1B |= (1 << CS10) | (1 << WGM12);		// Sets CS(clock set) prescaler to zero and WGM (wave generation mode) bits in TCCR1A (timer/counter control register)
	TIMSK1 |= (1 << OCIE1A) | (1 << OCIE1B);		// Sets OCIE1x (output compare interrupt enable) register in TIMSK1 (timer mask) register
	OCR1A =	20000;								// Sets PWM period to 20ms (50Hz) by writing to OCR1A (output compare) register with TOP value of counter
	OCR1B = 3000;								// Extend arm by writing longer pulse width to OCR1B (output compare) register
	_delay_ms(500);								// Delay to give arm enough time to flip switch before retracting
	OCR1B = 1000;								// Retract arm by writing shorter pulse width to OCR1B (output compare) register
}

void lcd_ini(void) //initialize LCD
{
	dis_cmd(0x02); //0x02 = set LCD in 4 bits mode
	_delay_ms(1);
	dis_cmd(0x28); //0x28 = 2 lines, 5x7 dots, 4 bits mode
	_delay_ms(1);
	dis_cmd(0x01); //0x01 = clear LCD
	_delay_ms(1);
	dis_cmd(0x0E); //0x0E = cursor ON
	_delay_ms(1);
	dis_cmd(0x80); //0x80 = first lines, 0th position
	_delay_ms(1);
	return;
}

void dis_cmd(char cmd_value)	//send full 8bits command in 4bits mode
{
	char cmd_value1;
	cmd_value1 = cmd_value & 0xF0;          //mask lower nibble
	//because PA4-PA7 pins are used.
	LCD_cmd(cmd_value1);					//send to LCD, use LCD_cmd
	cmd_value1 = ((cmd_value<<4) & 0xF0);   //shift 4-bit and mask the others 4
	LCD_cmd(cmd_value1);					//send to LCD, use LCD_cmd
}
void dis_data(char data_value)  //send full 8bits data in 4bits mode
{
	char data_value1;
	data_value1=data_value & 0xF0;			//mask lower nibble
	LCD_write(data_value1);					//send to LCD, use LCD_write
	data_value1=((data_value<<4) & 0xF0);	//shift 4bit and mask the others 4
	LCD_write(data_value1);					//send to LCD, use LCD_write
}

void LCD_cmd(unsigned char cmd)  //send 4bits command in 4bits mode (half of a command)
{
	ctrl=cmd;					 //output the data
	ctrl&=~(1<<rs);				 //rs = high (sending command)
	ctrl|=(1<<en);				 //create falling edge in enable (set to high -> low)
	_delay_ms(1);
	ctrl&=~(1<<en);
	_delay_ms(40);
	return;
}

void LCD_write(unsigned char data) //send 4bits data in 4bits mode (half of a 8 bit data)
{
	ctrl= data;					//output the data
	ctrl|=(1<<rs);				//rs = low (sendding datas)
	ctrl|=(1<<en);				//create falling edge in enable (set to high -> low)
	_delay_ms(1);
	ctrl&=~(1<<en);
	_delay_ms(40);
	return ;
}

void LCD_write_string(char *str)             //store address value of the string in pointer *str
{
	int i=0;
	while(str[i]!='\0')                               // loop will go on till the NULL character in the string
	{
		dis_data(str[i]);                            // sending data on LCD byte by byte
		i++;
	}
	return;
}

void unlock(void)
{
	PORTB ^= (1 << DDB7);
	_delay_ms(2000);
	PORTB ^= (1 << DDB7);
	_delay_ms(500);
	
}
