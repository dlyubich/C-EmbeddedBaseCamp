#ifndef F_CPU
#define F_CPU   16000000UL
#endif

#include "segm.h"
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>

volatile uint8_t sec;
volatile uint8_t min;
volatile uint8_t hour;
volatile uint8_t alarm_h;
volatile uint8_t alarm_m;
static int point;
static int fl;
uint8_t alarm_arr[] = {0, 0, 0, 0};
 

static void soft_delay(volatile uint8_t N)
{
	/* If volatile is not used, AVR-GCC will optimize this stuff out     */
        /* making our function completely empty                              */
	volatile uint8_t inner = 0x01;
	while (N--) {
		while (inner--);
	}
}

/** Timer2 Interrupt (on overflow), see datasheet
 * For vectors, refer to <avr/iom328p.h>
 * For more on interrupts handling with AVR-GCC see
 * https://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
 */
ISR(TIMER1_OVF_vect)
{
	point ^= 1;
	sec++;
	if (sec >= 60){   
		min++;
		sec = 0;
	}
	if (min >= 60){
		hour++;
		min = 0;
	}
	if (hour >= 24){
		hour=0;
	}
}

ISR(TIMER2_OVF_vect, ISR_BLOCK)
{
	TCCR2B &= ~((1 << CS22) | (1 << CS21) | (1 << CS20)); /* stop timer */
	/* It's often required to manually reset interrupt flag */
         //to avoid infinite processing of it.                  
        /* not on AVRs (unless OCB bit set)                     */
        /* 	TIFR2 &= ~TOV2;    
                                  */
}

void sleep_ms(uint16_t ms_val)
{
	/* Set Power-Save sleep mode */
	/* https://www.nongnu.org/avr-libc/user-manual/group__avr__sleep.html */
	set_sleep_mode(SLEEP_MODE_IDLE);
	cli();		/* Disable interrupts -- as memory barrier */
	sleep_enable();	/* Set SE (sleep enable bit) */
	sei();  	/* Enable interrupts. We want to wake up, don't we? */
	TIMSK2 |= (1 << TOIE2); /* Enable Timer2 Overflow interrupt by mask */
	while (ms_val--) {
		/* Count 1 ms from TCNT2 to 0xFF (up direction) */
		TCNT2 = (uint8_t)(0xFF - (F_CPU / 128) / 1000);
		/* Enable Timer2 */
		TCCR2B =  (1 << CS22) | (1 << CS20); /* f = Fclk_io / 128, start timer */
		sleep_cpu();	/* Put MCU to sleep */
		/* This is executed after wakeup */
	}
	sleep_disable();	/* Disable sleeps for safety */		
}

static struct segm_Port PB = {
	.DDR = &DDRB,
	.PIN = &PINB,
	.PORT = &PORTB,
};

static struct segm_Display display = {
	.SHCP = {.port = &PB, .pin = 0},
	.STCP = {.port = &PB, .pin = 1},
	.DS   = {.port = &PB, .pin = 2},
	.delay_func = &_delay_loop_1,	/* 3 cycles / loop, busy wait */
	.sleep_ms_func = &sleep_ms,	/* 3 cycles / loop, busy wait */
	.is_comm_anode = false		/* We have common cathode display */
};

 /*Initializing an Interrupt (seconds account)*/
void t1_init()
{
	/* Adjust the divisor (CLK/256)*/
	TCCR1A = 0x00;
	TCCR1B = (1<<CS12)|(0<<CS11)|(0<<CS10);
	/* Counting to 62500 (1 sec))*/
	TCNT1 = 0;
	OCR1B = 62500;
	/*Enable timer overflow interrupt*/
	 TIMSK1 |= (1 << TOIE1);
	/* set the general interrupt enable bit*/
	sei();
}

void int_ini(void)
{
	/* Enable interrupts INT0 on a falling edge */
	EICRA |= (1<<ISC01) | (0 << ISC00) | (1 << ISC11);
	/* allow external interrupts INT0 */
	EIMSK |= (1<<INT0) | (1 << INT1);
}

/* setting an alarm minutes */
ISR(INT1_vect)
{
	fl ^= 1;
}

int main(void)
{
	segm_init(&display);
	min = 0;
	hour = 0;
	alarm_h = 0;
	alarm_m = 0;
	uint8_t arr[] = {0, 0, 0, 0};

	/*  Timer 1 initialization    */
	t1_init();
	int_ini();

	/* switch on interrupts by falling edge */
	EICRA |= (1<<ISC01) | (0 << ISC00) | (1 << ISC11);
	EIMSK |= (1 << INT0) | (1 << INT1);
	while (1) {
		
		if (fl == 1) {
			BIN2BCD(alarm_arr+2, alarm_m);
			BIN2BCD(alarm_arr, alarm_h);
			for (int i = 0; i < 4; i++) {
				alarm_arr[i] = segm_sym_table[alarm_arr[i]];
			}
			if (point == 1)
				alarm_arr[1] |= (1 << 7);
			else
				alarm_arr[1] &= ~(1 << 7);	
			segm_indicate4(&display, alarm_arr);
		}
		else {
			BIN2BCD(arr+2, min);
			BIN2BCD(arr, hour);
			for (int i = 0; i < 4; i++) {
				arr[i] = segm_sym_table[arr[i]];
			}
			if (point == 1)
				arr[1] |= (1 << 7);
			else
				arr[1] &= ~(1 << 7);
			segm_indicate4(&display, arr);
		}

		/* Setting clock buttons			 */
		DDRD |= (0 << 2) | (0 << 3) | (0 << 4) | (0 << 5);
		PORTD |= (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);
		
		if (fl != 1){
		/* Setting hours manually			 */
			if ((PIND & (1 << 4)) == (0 << 4)) {
			sleep_ms(100);
			if ((PIND & (1 << 4)) == (0 << 4))
				hour++;
			if (hour >= 24)
				hour = 0;
			}
		
		/* Setting minutes manually			 */
			if ((PIND & (1 << 5)) == (0 << 5)) {
			sleep_ms(100);
			if ((PIND & (1 << 5)) == (0 << 5))
				min++;
			if (min >= 60)
				min = 0;	
			}
		}
		else {
			if ((PIND & (1 << 4)) == (0 << 4)) {
			sleep_ms(100);
			if ((PIND & (1 << 4)) == (0 << 4))
				alarm_h++;
			if (alarm_h >= 24)
				alarm_h = 0;
			}
		

			if ((PIND & (1 << 5)) == (0 << 5)) {
			sleep_ms(100);
			if ((PIND & (1 << 5)) == (0 << 5))
				alarm_m++;
			if (alarm_m >= 60)
				alarm_m = 0;	
			}
		}
		/* Checking alarm time				*/
		if (((hour - alarm_h) < 1) && (min - alarm_m) < 5) {
			DDRB  |= (1 << 3);
			PORTB |= (1 << 3);
		}
		else
			PORTB &= ~(1 << 3);
	}
}
