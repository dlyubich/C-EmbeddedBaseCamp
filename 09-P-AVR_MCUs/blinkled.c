#define   F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>
#include <limits.h>
#include <util/delay.h>

uint16_t __seed = 1;
#define my_rand() ({__seed = 5 * __seed + 3;})

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

int main(void)
{
	DDRB |= 1 << 3;		
	PORTB |= 1 << 3;	
	TCCR2A = _BV(COM2A1) |_BV(COM2B1) | _BV(WGM20);
  	TCCR2B = _BV(CS22);
	uint16_t r;

	while(1) {
		r = my_rand();
		if (r > 50461) {
   			OCR2A = 240 | r;     // (15 << 4) | r; 
		} else if (r > 39976) {
    		OCR2A = 223 | r;     // (14 << 4) | r; 
		} else if (r > 34078) {
   			OCR2A = 206 | r;     // (13 << 4) | r; 
		} else if (r > 30801) {
    		OCR2A = 191 | r;     // (12 << 4) | r; 
		} else if (r > 27524) {
    		OCR2A = 176 | r;    
		} else if (r > 24247) {
    		OCR2A = 161 | r;
		} else if (r > 20971) {
    		OCR2A = 146 | r;
		} else if (r > 17694) {
    		OCR2A = 131 | r;
		} else if (r > 14417) {
    		OCR2A = 116 | r;
		} else if (r > 11140) {
    		OCR2A = 101 | r;
		} else if (r > 7863) {
    		OCR2A = 86 | r;
		} else if (r > 4587) {
    		OCR2A = 71 | r; 
		} else if (r > 1965) {
    		OCR2A = 56 | r; 
		} else if (r > 1310) {
    		OCR2A = 41 | r;
		} else if (r > 654) {
    		OCR2A = 26 | r;
		} else {
    		OCR2A = 0;
	}
		soft_delay(50);	
	}
	return 0;
}
