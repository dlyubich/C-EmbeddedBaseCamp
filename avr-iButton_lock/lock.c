#include "onewire.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <util/atomic.h>
#include <stdlib.h>

#define 	F_CPU   16000000UL
#include <util/delay.h>

static int fl;

/* Indicates transfer & reception completion */
#define BUFFER_LEN (64)
static const uint32_t uart_baudrate = 19200;	/* Baud rate (Baud / second) */

/* Value to be placed in UBRR register. See datasheet for more */
static const uint16_t ubrr_val = (F_CPU / (16 * uart_baudrate)) - 1;
/* Read and write buffers */
static uint8_t	rdbuff[BUFFER_LEN] = {'\0'},
		wrbuff[BUFFER_LEN] = {'\0'};
static uint8_t rdind = 0, wrind = 0;
/* Indicates transfer & reception completion */
volatile bool txcflag = true;
volatile bool rxcflag = false;

/* USART RX Complete interrupt handler */
ISR(USART_RX_vect, ISR_BLOCK)
{
	/* Buffer will contain the last N = <buffer_len> chars read */
	rdbuff[rdind] = UDR0;

	if ('\n' == rdbuff[rdind]) {
		rdbuff[rdind] = '\0';
		rxcflag = true;
		rdind = 0;
	} else {
		rxcflag = false;
		rdind++;
	}

	if (rdind >= BUFFER_LEN)
		rdind = 0;
}

/* USART TX Complete interrupt handler */
ISR(USART_TX_vect, ISR_BLOCK)
{
	/* When data register is empty and after the shift register contents */
	/* are already transfered, this interrupt is raised */
	UCSR0B &= ~(1 << TXCIE0);
}

/* USART Data Register Empty interrupt handler */
ISR(USART_UDRE_vect, ISR_BLOCK)
{
	if (('\0' == wrbuff[wrind]) || txcflag) {
		/* If nothing to transmit - disable this interrupt and exit */
		UCSR0B &= ~(1 << UDRIE0);
		txcflag = true;
		return;
	}

	UDR0 = wrbuff[wrind++];	

	/* Really we don't need this as long as every string ends with '\0' */
	if (wrind >= BUFFER_LEN)
		wrind = 0;
}

static void uart_init(void)
{
	/* To use USART module, we need to power it on first */
	power_usart0_enable();

	/* Configure prescaler */
	UBRR0L = ubrr_val & 0xFF; /* Lower byte */
	UBRR0H = (ubrr_val >> 8) & 0xFF;   /* Higher byte */
	/* Or we could use UBRR0 (16-bit defined) instead */

	/* Set operation mode */
	/* Asynchronous mode, even parity, 2 stop bits, 8 data bits */
	UCSR0C = (1 << UPM01) | (1 << USBS0) | (1 << UCSZ00) | (1 << UCSZ01);

	/* Continue setup & enable USART in one operation */
	/* RX & TX complete, Data reg empty interrupts enabled */
	UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
}


static void uart_put(char *str)
{
	/* If buffer contents have not been transfered yet -- put MCU to sleep */
	while(!txcflag)
		sleep_cpu();

	/* No interrupts can occur while this block is executed */
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		for (uint8_t i = 0; i < BUFFER_LEN; i++) {
			wrbuff[i] = str[i];
			if ('\0' == str[i])
				break;
		}
		wrind = 0;
		txcflag = false; /* programmatic transfer complete flag */
		/* Enable transmitter interrupts */
		UCSR0B |= (1 << TXCIE0) | (1 << UDRIE0);
	}
}

static bool atomic_str_eq(char *str1, char *str2)
{
	bool res = true;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		for (uint8_t i = 0; i < BUFFER_LEN; i++) {
			if (str1[i] != str2[i]) {
				res = false;
				break;
			}
			if ('\0' == str1[i])
				break;
		}
	}
	return res;
}

void soft_delay_us(uint16_t time)
{
	while (time--) {
		_delay_us(1.0);
	}
}

void line_low(void)
{
	PORTB &= ~(1 << 0);
	DDRB |= (1 << 0);
}

void line_release(void)
{
	DDRB &= ~(1 << 0);
	PORTB |= (1 << 0);
}

bool line_read(void)
{
	uint8_t val = PINB & 0x01;
	return val;
}

void put(uint8_t byte)
{
	PORTB |= 1 << 3;
	soft_delay_us(20);
	PORTB &= ~(1 << 3);
	for (int i=0; i<8; i++) {
		if (byte & (1 << i))
			PORTB |= 1 << 3;
		else
			PORTB &= ~(1 << 3);
		soft_delay_us(20);
	}
}

void write_eeprom(uint8_t *ibutton_id, uint8_t *num_keys)
{
	uint16_t addr = *num_keys * 8;
    eeprom_write_block((const void*)&ibutton_id, (const void*)addr, (uint8_t)8);
    (*num_keys)++;
}

/*returns 0 if key isn't in eeprom memory */
uint8_t check_eeprom(uint8_t* id, uint8_t num_keys)
{	if (num_keys <= 0)	
		return 0;
	
	uint8_t eeprom_id[8];
	uint8_t ret = 0;
	for(uint8_t i=0; i<num_keys; i++){
		uint16_t cur_addr = i*8;
		eeprom_read_block((void *)eeprom_id, (void *)cur_addr,(uint8_t) 8);
		for(uint8_t j=0; j<8; j++){
			if(eeprom_id[j] != id[j]){
				ret = 0;
				break ;
			}
		}
		ret = 1;
	}
	return ret;
}

int main()
{
	/* We use internal pullup resitor for 1-wire line */
	DDRB = (1 << 1);
	PORTB |= (1 << 0);
	DDRD |= (0 << 4);
	PORTD |= (1 << 4);

	ow_Pin pin;
	ow_Pin_init(&pin, &line_low, &line_release, &line_read, &soft_delay_us, 5, 60, 60, 5);
	ow_err err;
	fl = 0;

	uart_init();
	sei();

	uint8_t ibutton_id[8];

	//eeprom_write_byte((uint8_t *) 1023,(uint8_t) 0);
	uint8_t key_number = eeprom_read_byte((uint8_t *) 1023);

	uint8_t crc;
	while (1) {

		if (fl == 0)
		{	
			PORTB &= ~ (1 << 1);
			err = ow_cmd_readrom(&pin, ibutton_id, &crc, true, false);
			if (err == OW_EOK && check_eeprom(ibutton_id, key_number)){
				uart_put("\n\nOpen\n\n");
			}
		}
		else {
			PORTB |= (1 << 1);
			err = ow_cmd_readrom(&pin, ibutton_id, &crc, true, false);
			if (err == OW_EOK){
				if (!check_eeprom(ibutton_id, key_number)){
					write_eeprom(ibutton_id, &key_number);
					eeprom_write_byte((uint8_t *) 1023, key_number);
					uart_put("\n\nKey added\n\n");
			}
			else
				uart_put("\n\nKey already exists\n\n");
			}
		}
		if ((PIND & (1 << 4)) == (0 << 4)){
			soft_delay_us(100);
			if ((PIND & (1 << 4)) == (0 << 4))
				fl ^= 1;
		}
	}
}
