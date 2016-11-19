#define F_CPU 1000000UL 
#include <avr/io.h>
#include <util/delay.h>

#define ctrl PORTD				//4 bits data line = D7-D4 of MCU
#define en 0                    //enable signal = D0 of MCU
#define rs 2                    //register select signal = D2 of MCU
								//read write signal = GND

void lcd_ini(void);						//initialize LCD
void LCD_cmd(unsigned char cmd);		//send 4bits command in 4bits mode (half of a command)
void LCD_write(unsigned char data);		//send 4bits data in 4bits mode (half of a 8 bit data)
void dis_cmd(char);						//send full 8bits command in 4bits mode
void dis_data(char);					//send full 8bits data in 4bits mode	
void LCD_write_string(char *str);

int main()
{
	DDRD=0xFF;					//Set Port D as output                                 
	
	lcd_ini();                  //Initialization of LCD
	_delay_ms(30);              // Wait 30ms to make sure that LCD is initialized
	
	LCD_write_string("Hello World 15");         // function to print string on LCD
	while(1);
	return 0;
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
