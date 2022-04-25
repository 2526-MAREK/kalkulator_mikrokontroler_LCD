#include "GLOBAL.h"
#include "LCD_HD44780.h"

#define ILOSC_PRZYCISKOW 16
int array[4][4] = {
		{1,2,3,4},
		{5,6,7,8},
		{9,10,11,12},
		{13,14,15,16}
};

char calc[4][4] = {
		{7,8,9,'d'},
		{4,5,6,'o'},
		{1,2,3,'m'},
		{0,0,0,'r'}
};



char getKey(){
	for(unsigned int i=0; i<4; ++i){
		cbi(PORTC, i+4);
		for(unsigned int j = 0; j<4; ++j){
			if(bit_is_clear(PINC,j))
			{
				sbi(PORTC, i+4);
				_delay_ms(50);
				return calc[i][j];
			}
		}
		sbi(PORTC, i+4);
	}
	return 0;
}



int main()
{
	LCD_HD44780::init();

	/*while(1){
		LCD_HD44780::writeData(getKey());
		_delay_ms(50);
	}
	*/
	char str[20];
	char tmp;
	LCD_HD44780::goTo(0, 0);

	while (1) {
		LCD_HD44780::clear();

		tmp = getKey();
		sprintf(str,"%d", tmp);

		LCD_HD44780::writeText(str);
	}
	return 0;
}
