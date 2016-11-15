#include <avr/io.h>
#include <util/delay.h>

#define ctrl PORTD
#define en 2							// enable signal
#define rs 0							// register select signal

void lcd_ini(void);						// Initialize LCD

void LCD_cmd(unsigned char cmd);		// Send 4 bits command (send 2 times to have 8 bits command)
void LCD_write(unsigned char data);		// Send 4 bits data (send 2 times to have 8 bits command)

void dis_cmd(char);						// Send full 8 bits command
void dis_data(char);					// Send full 8 bits data

void LCD_write_string(unsigned char *str);		// Display a string




void main()
{
	DDRD=0xFF;                                  // Set port D as output                                                                     
	lcd_ini();									// initialization of LCD
	LCD_write_string("hello");			// function to print string on LCD
	dis_cmd(0xC0);								// move to second line
	LCD_write_string("world");			// function to print string on LCD
	while(1);
}

void lcd_ini(void)
{
	dis_cmd(0x02); // to initialize LCD in 4-bit mode.
	dis_cmd(0x28); // to initialize LCD in 2 lines, 5X7 dots and 4bit mode.
	dis_cmd(0x01); // clear LCD
	dis_cmd(0x0E); // cursor ON
	dis_cmd(0x80); // —8 go to first line and –0 is for 0th position
	_delay_ms(30); // final delay to make sure that LCD is ready
	return;
}

void dis_cmd(char cmd_value)			
{
	char cmd_value1;
	cmd_value1 = cmd_value & 0xF0;          //Mask the lower 4 bits
	LCD_cmd(cmd_value1);					//Send the higher 4 bits
	cmd_value1 = ((cmd_value<<4) & 0xF0);   //Shift left 4 bits
	LCD_cmd(cmd_value1);					//Send the lower 4 bits
	_delay_ms(1);
}

void dis_data(char data_value)
{
	char data_value1;
	data_value1=data_value & 0xF0;			//Mask the lower 4 bits
	LCD_write(data_value1);					//Send the higher 4 bits
	data_value1=((data_value<<4) & 0xF0);	//Shift left 4 bits
	LCD_write(data_value1);					//Send the lower 4 bits
	_delay_ms(1);
}


void LCD_cmd(unsigned char cmd)				
{
	ctrl=cmd;								//output the 4 bit command
	ctrl&=~(1<<rs);							//resistor select to send instructor
	ctrl|=(1<<en);							//enable high
	_delay_ms(1);
	ctrl&=~(1<<en);							//enable low
	_delay_ms(40);
	return;
}

void LCD_write(unsigned char data)
{
	ctrl= data;								//output the 4 bit data
	ctrl|=(1<<rs);							//resistor select to send data
	ctrl|=(1<<en);							//enable high
	_delay_ms(1);
	ctrl&=~(1<<en);							//enable low
	_delay_ms(40);
	return ;
}

void LCD_write_string(unsigned char *str)             //store address value of the string in pointer *str
{
	int i=0;
	while(str[i]!='\0')                               // loop will go on till the NULL character in the string
	{
		dis_data(str[i]);							  // sending data on LCD byte by byte
		i++;
	}
	return;
}