#include <avr/io.h>
#include <util/delay.h>

#define DDR_LED  DDRB
#define PORT_LED PORTB
#define BIT_LED  5


int main(void)
{
	DDR_LED |= 1 << BIT_LED;

	while(1){
		PORT_LED |= 1 << BIT_LED;
		_delay_ms(200);
		PORT_LED &= ~(1 << BIT_LED);
		_delay_ms(200);
	}
}
