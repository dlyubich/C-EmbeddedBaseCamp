#define   F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>
#include <limits.h>
#include <util/delay.h>

#define a 22695477 
#define c 0
unsigned long b = 1;

/**
 * soft_delay() - wastes CPU time crunching cycle to achieve delay
 * @N:	number of outer "while" steps.
 *
 * This is very inefficient in terms of CPU and energy usage.
 * Better way is to use timer to count time and put CPU into sleep mode to save
 * energy.
 * But soft delays are useful for very precise timings (i.e. software USB
 * implementation, 1-Wire interface, etc.)
 * See <util/delay.h> for alternative implementation
 */

static void soft_delay(volatile uint16_t N)
{                           
	volatile uint8_t inner = 0xFF;
	while (N--) {
		while (inner--);
	}
}

uint16_t Rand(void) 
{
	static uint32_t Z;
	
	Z = b = (a * b + c) % ULONG_MAX;
	
	return (uint16_t)Z;
}

void pwm_duty(int n)
{
	switch(n) {
		case 15:
			OCR2A = 255;
			break;
		case 14:
			OCR2A = 238;
			break;
		case 13:
			OCR2A = 221;
			break;
		case 12:
			OCR2A = 204;
			break;
		case 11:
			OCR2A = 187;
			break;
		case 10:
			OCR2A = 170;
			break;
		case 9:
			OCR2A = 153;
			break;
		case 8:
			OCR2A = 136;
			break;
		case 7:
			OCR2A = 119;
			break;
		case 6:
			OCR2A = 102;
			break;
		case 5:
			OCR2A = 85;
			break;
		case 4:
			OCR2A = 68;
			break;
		case 3:
			OCR2A = 51;
			break;
		case 2:
			OCR2A = 34;
			break;
		case 1:
			OCR2A = 17;
			break;
		case 0:
			OCR2A = 0;
			break;
	}
}

int main(void)
{
	DDRB |= 1 << 3;		
	PORTB |= 1 << 3;	
	TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20);
  	TCCR2B = _BV(CS22);
	uint16_t r;

	while(1) {
		r = Rand() % 65535;
		if (r > 50461) {
   			pwm_duty(15);
		} else if (r > 39976) {
    		pwm_duty(14);
		} else if (r > 34078) {
   			 pwm_duty(13);
		} else if (r > 30801) {
    		pwm_duty(12);
		} else if (r > 27524) {
    		pwm_duty(11);
		} else if (r > 24247) {
    		pwm_duty(10);
		} else if (r > 20971) {
    		pwm_duty(9);
		} else if (r > 17694) {
    		pwm_duty(8);
		} else if (r > 14417) {
    		pwm_duty(7);
		} else if (r > 11140) {
    		pwm_duty(6);
		} else if (r > 7863) {
    		pwm_duty(5);
		} else if (r > 4587) {
    		pwm_duty(4);
		} else if (r > 1965) {
    		pwm_duty(3);
		} else if (r > 1310) {
    		pwm_duty(2);
		} else if (r > 654) {
    		pwm_duty(1);
		} else {
    		pwm_duty(0);
	}
		soft_delay(50);	
	}
	return 0;
}
