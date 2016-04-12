#include <avr/io.h>
#include <util/delay.h>

#define DDR_LED  DDRB
#define PORT_LED PORTB
#define BIT_LED  5

int func1(int a, int b);
int func2(int a, int b);

int main(void)
{
	DDR_LED |= 1 << BIT_LED;

	int a = 2;
	a = func1(a,2);
	a = func2(a,2);

	while(1){
		PORT_LED |= 1 << BIT_LED;
		_delay_ms(500);
		PORT_LED &= ~(1 << BIT_LED);
		_delay_ms(500);
	}
}
