#include <avr/io.h>
#include <util/delay.h>

#define ctrl PORTD
#define en 2                         // enable signal
#define rs 0                     // register select signal

void LCD_cmd(unsigned char cmd);
void lcd_ini(void);
void LCD_write(unsigned char data);
void dis_cmd(char);
void dis_data(char);
void LCD_write_string(unsigned char *str);

void main()
{
	DDRD=0xFF;                                  // setting the port c                                                                       // setting for port D
	
	lcd_ini();                                 // initialization of LCD
	_delay_ms(30);                        // delay of 50 mili seconds
	dis_data('b');

	while(1);
}

void lcd_ini(void)
{
	dis_cmd(0x02); // to initialize LCD in 4-bit mode.
	_delay_ms(1);
	dis_cmd(0x28); //to initialize LCD in 2 lines, 5X7 dots and 4bit mode.
	_delay_ms(1);
	dis_cmd(0x01);                                 // clear LCD
	_delay_ms(1);
	dis_cmd(0x0E);                        // cursor ON
	_delay_ms(1);
	dis_cmd(0x80);                     // —8 go to first line and –0 is for 0th position
	_delay_ms(1);
	return;
}

void dis_cmd(char cmd_value)
{
	char cmd_value1;
	cmd_value1 = cmd_value & 0xF0;          //mask lower nibble
	//because PA4-PA7 pins are used.
	LCD_cmd(cmd_value1);               // send to LCD
	cmd_value1 = ((cmd_value<<4) & 0xF0);     //shift 4-bit and
	//mask
	LCD_cmd(cmd_value1);               // send to LCD
}
void dis_data(char data_value)
{
	char data_value1;
	data_value1=data_value & 0xF0;
	LCD_write(data_value1);
	data_value1=((data_value<<4) & 0xF0);
	LCD_write(data_value1);
}

void LCD_cmd(unsigned char cmd)
{
	ctrl=cmd;
	ctrl&=~(1<<rs);
	ctrl|=(1<<en);
	_delay_ms(1);
	ctrl&=~(1<<en);
	_delay_ms(40);
	return;
}

void LCD_write(unsigned char data)
{
	ctrl= data;
	ctrl|=(1<<rs);
	ctrl|=(1<<en);
	_delay_ms(1);
	ctrl&=~(1<<en);
	_delay_ms(40);
	return ;
}

void LCD_write_string(unsigned char *str)             //store address value of the string in pointer *str
{
	int i=0;
	while(str[i]!='\0')                               // loop will go on till the NULL character in the string
	{
		dis_data(str[i]);                            // sending data on LCD byte by byte
		i++;
	}
	return;
}
