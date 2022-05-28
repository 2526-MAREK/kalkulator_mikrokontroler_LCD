#include "LCD_HD44780.h"
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include <math.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "stack"

#define MAX_CHAR_CAN_DISPLAY_CALC 20
#define THE_NUMBER_OF_DIGITS_IN_THE_NUMBER 3

char array[4][4] = {  //tablica skonfigurowana pod klawiature kalkualtora 4x4:
{'7','4','1','0'},           // 789
{'8','5','2','c'},           // 456*
{'9','6','3','='},           //  123-
{'d','m','o','p'}            //  0C=+  macierz przyciskow podpieta pod port D: D0-7 kolejno: D0-3 wiersze maciearzy przyciskow, D4-7 kolumny macierzy przyiskow
};                            // gdzie d- dzielenie , m - mnozenie, o- odejmowanie, p - dodawanie, c - kasowanie (C)

char get_key()
{
	for(unsigned int i = 0; i<4; ++i){
		PORTD = PORTD & ~(1<<i+4); //ustawienie stanu niskiego

		for(unsigned int j = 0; j<4; ++j)
		{
			if(!(PIND & 1<<j))
			{
				PORTD = PORTD | 1<<i+4; //ustawienie stanu wysokiego na dnaym pinie
				return array[i][j];
			}
		}
		PORTD = PORTD | 1<<i+4; //ustawienie stanu wysokiego na dnaym pinie
	}
	return 0;
}

char calc_display[MAX_CHAR_CAN_DISPLAY_CALC];

void clear_display_calc()
{
	for(int i = 0; i < MAX_CHAR_CAN_DISPLAY_CALC; ++i)
		calc_display[i] = ' ';
}

char number_in_equaction[THE_NUMBER_OF_DIGITS_IN_THE_NUMBER];

void clear_number_in_equaction()
{
	for(int i = 0; i < THE_NUMBER_OF_DIGITS_IN_THE_NUMBER ; ++i)
		number_in_equaction[i] = ' ';
}


bool the_first_number_entered = false;
int number_of_save_char = 0;
int number_of_save_digits = 0;

void save_digit_to_number(char digit)
{
	if(the_first_number_entered){
		clear_display_calc();
		number_of_save_char = 0 ;
		number_of_save_digits = 0;
		clear_number_in_equaction();
	}
	the_first_number_entered = false;

	if(number_of_save_digits < THE_NUMBER_OF_DIGITS_IN_THE_NUMBER)
		number_in_equaction[number_of_save_digits++] = digit;
}

int first_number_in_equaction;
char operation_of_equaction;
int second_number_in_equaction;
int result_of_equaction;

void calculate_equation()
{
	switch(operation_of_equaction)
	{
		case '-':
		{
			result_of_equaction = first_number_in_equaction - second_number_in_equaction;
			break;
		}
		case '+':
		{
			result_of_equaction = first_number_in_equaction + second_number_in_equaction;
			break;
		}
		case '*':
		{
			result_of_equaction = first_number_in_equaction * second_number_in_equaction;
			break;
		}
		case '/':
		{
			result_of_equaction = first_number_in_equaction / second_number_in_equaction;
			break;
		}
	}
}

void clear_all_of_calc()
{
	number_of_save_char = 0;
	the_first_number_entered = false;
	number_of_save_digits = 0;
	clear_number_in_equaction();
	first_number_in_equaction = 0;
	operation_of_equaction = 0;
	second_number_in_equaction = 0;
	result_of_equaction = 0;
	clear_display_calc();
}

void entering_a_character()
{
	char tmp;

	tmp = get_key();

	if(number_of_save_char < MAX_CHAR_CAN_DISPLAY_CALC)
	{
		switch(tmp)
		{
		case '0':
		{
			save_digit_to_number('0');
			calc_display[number_of_save_char++]  = '0';
			break;
		}
		case '1':
		{
			save_digit_to_number('1');
			calc_display[number_of_save_char++]  = '1';
			break;
		}
		case '2':
		{
			save_digit_to_number('2');
			calc_display[number_of_save_char++]  = '2';
			break;
		}
		case '3':
		{
			save_digit_to_number('3');
			calc_display[number_of_save_char++]  = '3';
			break;
		}
		case '4':
		{
			save_digit_to_number('4');
			calc_display[number_of_save_char++]  = '4';
			break;
		}
		case '5':
		{
			save_digit_to_number('5');
			calc_display[number_of_save_char++]  = '5';
			break;

		}
		case '6':
		{
			save_digit_to_number('6');
			calc_display[number_of_save_char++]  = '6';
			break;
		}
		case '7':
		{
			save_digit_to_number('7');
			calc_display[number_of_save_char++]  = '7';
			break;
		}
		case '8':
		{
			save_digit_to_number('8');
			calc_display[number_of_save_char++]  = '8';
			break;
		}
		case '9':
		{
			save_digit_to_number('9');
			calc_display[number_of_save_char++]  = '9';
			break;
		}
		case 'o':
		{
			calc_display[number_of_save_char++]  = '-';
			first_number_in_equaction = atoi(number_in_equaction);
			operation_of_equaction = '-';
			the_first_number_entered = true;
			break;
		}

		case 'p':
		{
			calc_display[number_of_save_char++]  = '+';
			first_number_in_equaction = atoi(number_in_equaction);
			operation_of_equaction = '+';
			the_first_number_entered = true;
			break;
		}
		case 'd':
		{
			calc_display[number_of_save_char++]  = '/';
			first_number_in_equaction = atoi(number_in_equaction);
			operation_of_equaction = '/';
			the_first_number_entered = true;
			break;
		}
		case 'm':
		{
			calc_display[number_of_save_char++]  = '*';
			first_number_in_equaction = atoi(number_in_equaction);
			operation_of_equaction = '*';
			the_first_number_entered = true;
			break;
		}
		case 'c':
		{
			clear_all_of_calc();
			break;
		}
		case '=':
		{
			second_number_in_equaction = atoi(number_in_equaction);
			calculate_equation();
			sprintf(calc_display,"%d",result_of_equaction);
			break;
		}
		}
	}
}
int main()
{
	DDRA = 0xFF;
	DDRD = 0xF0;
	PORTD = 0xFF;

	LCD_HD44780::init();
	LCD_HD44780::goTo(0, 0);
	while (1) {
		entering_a_character();
		LCD_HD44780::writeText(calc_display);
		_delay_ms(1000);
		LCD_HD44780::clear();
	}
}
