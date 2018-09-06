#ifndef F_CPU
#define F_CPU   16000000UL
#endif

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <util/atomic.h>
#include <stddef.h>
#include <avr/power.h>
#include "ds18b20.h"
#include "segm.h"
#include "oled.h"

volatile uint8_t sec;
volatile uint8_t min;
volatile uint8_t hour;
volatile uint8_t alarm_h;
volatile uint8_t alarm_m;

#ifndef DS18B20_H_
#define DS18B20_H_

#define F_CPU 16000000UL

#include <stdio.h>
#include <util/delay.h>

#define port_termometr PORTB
#define ddr_termometr DDRB
#define pin_termometr PINB
#define termometr 0
void input_1wire();
void output_1wire();
void init_termometr();
void write_byte_termometr(uint8_t);
void writebit1();
void writebit0();
uint8_t readbit();
void read_rom_termometr(uint8_t *mas);
uint8_t read_byte_termometr();
void convert_tempr ();
void write_scrpad(uint8_t th, uint8_t tl, uint8_t config); 
void read_scrpad(uint8_t *data); 
void skip_rom();
void uart_put(uint8_t *str, uint8_t size);
void uart_init(void);

#endif /* DS18B20_H_ */

/* struct for displaying symbols on oled screen by dude24fromNV */
const uint8_t Waree8x8[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char  
        0x00, 0x2F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char !
        0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char "
        0x00, 0x0A, 0x1F, 0x0A, 0x1F, 0x0A, 0x00, 0x00,  // Code for char #
        0x02, 0x25, 0x7F, 0x39, 0x00, 0x00, 0x00, 0x00,  // Code for char $
        0x02, 0x05, 0x37, 0x0C, 0x1A, 0x25, 0x3C, 0x00,  // Code for char %
        0x18, 0x26, 0x25, 0x29, 0x10, 0x28, 0x00, 0x00,  // Code for char &
        0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char '
        0x00, 0x7E, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char (
        0x81, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char )
        0x00, 0x24, 0x18, 0x3C, 0x18, 0x24, 0x00, 0x00,  // Code for char *
        0x00, 0x08, 0x08, 0x3E, 0x08, 0x08, 0x00, 0x00,  // Code for char +
        0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char ,
        0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char -
        0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char .
        0x70, 0x0E, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char /
        0x0C, 0x33, 0x21, 0x33, 0x0C, 0x00, 0x00, 0x00,  // Code for char 0
        0x00, 0x21, 0x3F, 0x20, 0x00, 0x00, 0x00, 0x00,  // Code for char 1
        0x00, 0x31, 0x29, 0x27, 0x00, 0x00, 0x00, 0x00,  // Code for char 2
        0x00, 0x21, 0x25, 0x3B, 0x00, 0x00, 0x00, 0x00,  // Code for char 3
        0x18, 0x14, 0x12, 0x3F, 0x10, 0x00, 0x00, 0x00,  // Code for char 4
        0x00, 0x27, 0x25, 0x3D, 0x00, 0x00, 0x00, 0x00,  // Code for char 5
        0x00, 0x3E, 0x25, 0x25, 0x18, 0x00, 0x00, 0x00,  // Code for char 6
        0x00, 0x21, 0x19, 0x07, 0x00, 0x00, 0x00, 0x00,  // Code for char 7
        0x1A, 0x2D, 0x25, 0x1A, 0x00, 0x00, 0x00, 0x00,  // Code for char 8
        0x06, 0x29, 0x29, 0x3F, 0x00, 0x00, 0x00, 0x00,  // Code for char 9
        0x00, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char :
        0x00, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char ;
        0x00, 0x08, 0x18, 0x14, 0x14, 0x02, 0x00, 0x00,  // Code for char <
        0x00, 0x14, 0x14, 0x14, 0x14, 0x14, 0x00, 0x00,  // Code for char =
        0x00, 0x22, 0x14, 0x14, 0x14, 0x08, 0x00, 0x00,  // Code for char >
        0x00, 0x29, 0x05, 0x02, 0x00, 0x00, 0x00, 0x00,  // Code for char ?
        0x1C, 0x22, 0x59, 0x65, 0x65, 0x79, 0x3E, 0x00,  // Code for char @
        0x20, 0x1C, 0x13, 0x1E, 0x30, 0x00, 0x00, 0x00,  // Code for char A
        0x00, 0x3F, 0x25, 0x25, 0x1A, 0x00, 0x00, 0x00,  // Code for char B
        0x0C, 0x12, 0x21, 0x21, 0x21, 0x00, 0x00, 0x00,  // Code for char C
        0x00, 0x3F, 0x21, 0x21, 0x33, 0x0C, 0x00, 0x00,  // Code for char D
        0x00, 0x3F, 0x25, 0x25, 0x20, 0x00, 0x00, 0x00,  // Code for char E
        0x00, 0x3F, 0x05, 0x05, 0x00, 0x00, 0x00, 0x00,  // Code for char F
        0x0C, 0x12, 0x21, 0x29, 0x29, 0x18, 0x00, 0x00,  // Code for char G
        0x00, 0x3F, 0x04, 0x04, 0x3F, 0x00, 0x00, 0x00,  // Code for char H
        0x00, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char I
        0x80, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char J
        0x00, 0x3F, 0x0C, 0x12, 0x21, 0x00, 0x00, 0x00,  // Code for char K
        0x00, 0x3F, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00,  // Code for char L
        0x00, 0x3F, 0x0E, 0x18, 0x06, 0x3F, 0x00, 0x00,  // Code for char M
        0x00, 0x3F, 0x06, 0x18, 0x3F, 0x00, 0x00, 0x00,  // Code for char N
        0x0C, 0x12, 0x21, 0x21, 0x33, 0x1E, 0x00, 0x00,  // Code for char O
        0x00, 0x3F, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00,  // Code for char P
        0x0C, 0x12, 0x21, 0x21, 0x73, 0x1E, 0x00, 0x00,  // Code for char Q
        0x00, 0x3F, 0x09, 0x0D, 0x32, 0x00, 0x00, 0x00,  // Code for char R
        0x02, 0x25, 0x29, 0x29, 0x18, 0x00, 0x00, 0x00,  // Code for char S
        0x01, 0x01, 0x3F, 0x01, 0x01, 0x00, 0x00, 0x00,  // Code for char T
        0x0F, 0x30, 0x20, 0x20, 0x3F, 0x00, 0x00, 0x00,  // Code for char U
        0x03, 0x0C, 0x30, 0x1C, 0x03, 0x00, 0x00, 0x00,  // Code for char V
        0x03, 0x1C, 0x38, 0x07, 0x0F, 0x30, 0x1E, 0x01,  // Code for char W
        0x20, 0x13, 0x0C, 0x1A, 0x21, 0x00, 0x00, 0x00,  // Code for char X
        0x01, 0x02, 0x3C, 0x02, 0x01, 0x00, 0x00, 0x00,  // Code for char Y
        0x21, 0x31, 0x29, 0x27, 0x21, 0x00, 0x00, 0x00,  // Code for char Z
        0x3E, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char [
        0x07, 0x38, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char BackSlash
        0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char ]
        0x00, 0x02, 0x01, 0x01, 0x02, 0x00, 0x00, 0x00,  // Code for char ^
        0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00,  // Code for char _
        0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char `
        0x10, 0x2A, 0x2A, 0x3E, 0x00, 0x00, 0x00, 0x00,  // Code for char a
        0x2B, 0x14, 0x22, 0x36, 0x08, 0x00, 0x00, 0x00,  // Code for char b
        0x1C, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00,  // Code for char c
        0x1C, 0x22, 0x22, 0x3F, 0x00, 0x00, 0x00, 0x00,  // Code for char d
        0x1C, 0x2A, 0x2A, 0x2E, 0x00, 0x00, 0x00, 0x00,  // Code for char e
        0x02, 0x3F, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char f
        0x1C, 0xA2, 0xA2, 0xFE, 0x00, 0x00, 0x00, 0x00,  // Code for char g
        0x3B, 0x04, 0x02, 0x3E, 0x00, 0x00, 0x00, 0x00,  // Code for char h
        0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char i
        0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char j
        0x37, 0x08, 0x14, 0x22, 0x00, 0x00, 0x00, 0x00,  // Code for char k
        0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char l
        0x3A, 0x04, 0x02, 0x3E, 0x04, 0x02, 0x3E, 0x00,  // Code for char m
        0x3A, 0x04, 0x02, 0x3E, 0x00, 0x00, 0x00, 0x00,  // Code for char n
        0x1C, 0x22, 0x22, 0x3E, 0x00, 0x00, 0x00, 0x00,  // Code for char o
        0xEA, 0x14, 0x22, 0x36, 0x08, 0x00, 0x00, 0x00,  // Code for char p
        0x1C, 0x22, 0x22, 0xFE, 0x00, 0x00, 0x00, 0x00,  // Code for char q
        0x3A, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char r
        0x24, 0x2A, 0x2A, 0x12, 0x00, 0x00, 0x00, 0x00,  // Code for char s
        0x1F, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char t
        0x0E, 0x30, 0x20, 0x3E, 0x00, 0x00, 0x00, 0x00,  // Code for char u
        0x02, 0x1C, 0x30, 0x0E, 0x00, 0x00, 0x00, 0x00,  // Code for char v
        0x06, 0x38, 0x1C, 0x0E, 0x30, 0x0E, 0x00, 0x00,  // Code for char w
        0x20, 0x16, 0x1C, 0x22, 0x00, 0x00, 0x00, 0x00,  // Code for char x
        0x02, 0x9C, 0x70, 0x0E, 0x00, 0x00, 0x00, 0x00,  // Code for char y
        0x22, 0x32, 0x2A, 0x26, 0x00, 0x00, 0x00, 0x00,  // Code for char z
        0x00, 0x08, 0x77, 0x41, 0x00, 0x00, 0x00, 0x00,  // Code for char {
        0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char |
        0x00, 0x41, 0x77, 0x08, 0x00, 0x00, 0x00, 0x00,  // Code for char }
        0x00, 0x18, 0x08, 0x18, 0x10, 0x18, 0x00, 0x00,  // Code for char ~
        0x00, 0x00, 0x7F, 0x7F, 0x7F, 0x7F, 0x00, 0x00   // Code for char 
};

/* functions for work with oled by thodnev*/

#if !defined(OLED_NO_I2C)
/***** I2C-related logic *****/
uint8_t OLED_cmdbuffer[OLED_CMDBUFFER_LEN];

static uint8_t _i2c_cmd_init[] = {
	0x80, 0x8D, 0x80, 0x14	/* Enable charge pump	 */
	,0x80, 0xAF		/* Display on	      	 */
	,0x80, 0x81, 0x80, 0xFF /* Set brightness to 255 */
	,0x80, 0xA7		/* Enable inversion 	 */
};

static uint8_t _i2c_cmd_setpage[] = {
	0x80, 0x00, 0x80, 0x10, /* Set column cursor to 0 */
	0x80, 0xB0 /* Last nibble in 0xB0 defines page (0xB0..0xB7) */
};

static uint8_t _i2c_cmd_setbrightness[] = {
	0x80, 0x81, 0x80, 0xFF  /* Last byte is brightness level (0..255) */
};

static uint8_t _i2c_cmd_dataprefix[] = {0x40};

static uint8_t i2c_devaddr;
static uint8_t *i2c_prefix_ptr;
static uint8_t *i2c_prefix_count;
static uint8_t *i2c_data_ptr;
static uint16_t i2c_data_count;
static bool i2c_is_fastfail;
static void (*i2c_callback)(void *); /* called after transaction finish */
static void *i2c_callback_args;

/* States used in ISR FSM */
enum I2C_State_e {
	I2C_STATE_IDLE = 0,
	I2C_STATE_STOP,
	I2C_STATE_SLAVEADDR,
	I2C_STATE_WRITEPREFIX,
	I2C_STATE_WRITEBYTE
};
static enum I2C_State_e i2c_state = I2C_STATE_IDLE;


static void I2C_init(uint32_t hz_freq)
{
	i2c_state = I2C_STATE_IDLE;
	/* Enable the Two Wire Interface module */
	power_twi_enable();

	/* Select TWBR and TWPS based on frequency. Quite tricky, the main point */
	/* is that prescaler is a pow(4, TWPS)				 	 */
	/* TWBR * TWPS_prescaler value */
	uint32_t twbr = F_CPU / (2 * hz_freq) - 8;
	uint8_t twps;
	for (twps = 0; twps < 4; twps++) {
		if (twbr <= 255)
			break;
		twbr /= 4;
	}

	TWBR = (uint8_t)twbr;
	TWSR = (TWSR & 0xFC) | (twps & 0x03);

	TWCR = (1 << TWEN) | (1 << TWIE);
}


bool OLED_i2c_tx_shed(uint8_t addr, uint8_t *prefix, uint8_t prefix_len, uint8_t *bytes, uint16_t bytes_len, 
		      void (*end_cbk)(void *), void *cbk_args, bool fastfail)
{
	bool ret = false;
	/* No interrupts can occur while this block is executed */
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		if (i2c_state == I2C_STATE_IDLE) {
			i2c_prefix_ptr = prefix;
			i2c_prefix_count = prefix_len;
			i2c_data_ptr = bytes;
			i2c_data_count = bytes_len;
			i2c_is_fastfail = fastfail;
			i2c_callback = end_cbk;
			i2c_callback_args = cbk_args;
			/* Send START signal and initiating new transaction */
			i2c_state = I2C_STATE_SLAVEADDR;
			i2c_devaddr = (addr << 1);
			TWCR |= (1 << TWSTA) | (1 << TWINT);
			ret = true;
		}
	}
	return ret;
}

ISR(TWI_vect, ISR_BLOCK)
{
	switch(i2c_state) {
	case(I2C_STATE_IDLE):
	case(I2C_STATE_STOP):
		/* transfer stop and go to IDLE*/
		/* signal with callback that transaction is over */
		TWCR |= (1 << TWSTO) | (1 << TWINT);
		i2c_state = I2C_STATE_IDLE;
		(*i2c_callback)(i2c_callback_args);
		break;
	case(I2C_STATE_SLAVEADDR):
		// load value
		TWDR = i2c_devaddr;
		TWCR = (TWCR & ~(1 << TWSTA)) | (1 << TWINT);
		if ((NULL == i2c_prefix_ptr) && (NULL == i2c_data_ptr)) {
			i2c_state = I2C_STATE_STOP;
		} else if (NULL == i2c_prefix_ptr) {
			i2c_state = I2C_STATE_WRITEBYTE;
		} else {
			i2c_state = I2C_STATE_WRITEPREFIX;
		}
		break;
	case(I2C_STATE_WRITEPREFIX):
		// load next byte of prefix
		TWDR = *i2c_prefix_ptr++;
		i2c_prefix_count--;
		TWCR |= (1 << TWINT);
		if (!i2c_prefix_count) {
			i2c_state = (NULL == i2c_data_ptr) ? I2C_STATE_STOP : I2C_STATE_WRITEBYTE;
		}
		break;
	case(I2C_STATE_WRITEBYTE):
		// load next byte
		TWDR = *i2c_data_ptr++;
		i2c_data_count--;
		TWCR |= (1 << TWINT);
		if (!i2c_data_count)
			i2c_state = I2C_STATE_STOP;
		break;
	}
}

/* Callback which essentially does nothing */
static void OLED_cbk_empty(void *args)
{
	// empty callback
}

/* A dummy callback which simply unlocks the oled lock */
static void OLED_cbk_unlock(void *args)
{
	OLED *oled = args;
	OLED_unlock(oled);
}

/* Callbacks which are used to write each page */
static void OLED_cbk_writepage(void *args);
static void OLED_cbk_setwritepage(void *args);
/* Writes page. This is called after OLED_cbk_setwritepage */
static void OLED_cbk_writepage(void *args)
{
	OLED *oled = args;
	if (oled->cur_page >= oled->num_pages) {
		OLED_unlock(oled);
		return;
	}
	uint8_t *lineptr = &oled->frame_buffer[oled->cur_page * (uint16_t)oled->width];
	oled->cur_page++;
	while(!OLED_i2c_tx_shed(oled->i2c_addr, _i2c_cmd_dataprefix, OLED_ARR_SIZE(_i2c_cmd_dataprefix), 
				lineptr, oled->width,
				&OLED_cbk_setwritepage, oled, true)) {
		// nop
	}
}

/* Sets page index and calls OLED_cbk_writepage via callback */
static void OLED_cbk_setwritepage(void *args)
{
	OLED *oled = args;
	_i2c_cmd_setpage[OLED_ARR_SIZE(_i2c_cmd_setpage) - 1] = 0xB0 | oled->cur_page;
	while(!OLED_i2c_tx_shed(oled->i2c_addr, _i2c_cmd_setpage, 
                                OLED_ARR_SIZE(_i2c_cmd_setpage), NULL, 0,
				&OLED_cbk_writepage, oled, true)) {
		// nop
	}
}

void OLED_cmd_setbrightness(OLED *oled, uint8_t level)
{
	_i2c_cmd_setbrightness[OLED_ARR_SIZE(_i2c_cmd_setbrightness) - 1] = level;
	OLED_spinlock(oled);
	while(!OLED_i2c_tx_shed(oled->i2c_addr, _i2c_cmd_setbrightness, 
                                OLED_ARR_SIZE(_i2c_cmd_setbrightness), NULL, 0,
				&OLED_cbk_unlock, oled, true)) {
		// nop
	}
}

void OLED_refresh(OLED *oled)
{
	OLED_spinlock(oled);
	/* Code below is executed under lock */
	oled->cur_page = 0;
	OLED_cbk_setwritepage(oled);
	/* Lock is unlocked after series of callbacks, in the last one */
}
#endif // OLED_NO_I2C

/***** Display-related logic *****/
OLED_err __OLED_init(OLED *oled, uint8_t width, uint8_t height, uint8_t *frame_buffer, uint32_t i2c_freq_hz, uint8_t i2c_addr)
{
	oled->width = width;
	oled->height = height;
	oled->frame_buffer = frame_buffer;
	oled->busy_lock = 1;	/* Initially: 1 - unlocked */

	OLED_I2CWRAP(
		oled->i2c_addr = i2c_addr;
		oled->cur_page = 0;
		oled->num_pages = 8;

		I2C_init(i2c_freq_hz);
		
		if (!OLED_i2c_tx_shed(oled->i2c_addr, _i2c_cmd_init, OLED_ARR_SIZE(_i2c_cmd_init),
				      NULL, 0, OLED_cbk_empty, NULL, true)) {
			return OLED_EBUSY;
		}
	) // OLED_I2CWRAP

	return OLED_EOK;
}

OLED_err OLED_put_pixel(OLED *oled, uint8_t x, uint8_t y, bool pixel_state)
{
	if ((x >= oled->width) || (y >= oled->height))
		return OLED_EBOUNDS;
	OLED_put_pixel_(oled, x, y, pixel_state);	/* Use inline */
	return OLED_EOK;
}


OLED_err OLED_put_rectangle(OLED *oled, uint8_t x_from, uint8_t y_from, uint8_t x_to, uint8_t y_to, enum OLED_params params)
{
	if (params > (OLED_BLACK | OLED_FILL))
		return OLED_EPARAMS;
	bool pixel_color = (OLED_BLACK & params) != 0;
	bool is_fill = (OLED_FILL & params) != 0;

	/* Limit coordinates to display bounds */
	uint8_t size_errors = 0;
	uint8_t w_max = oled->width - 1;
	uint8_t h_max = oled->height - 1;
	if (x_from > w_max) {
		x_from = w_max;
		size_errors++;
	}
	if (x_to > w_max) {
		x_to = w_max;
		size_errors++;
	}
	if (y_from > h_max) {
		y_from = h_max;
		size_errors++;
	}
	if (y_to > h_max) {
		y_to = h_max;
		size_errors++;
	}
	/* If all coordinates are out of bounds */
	if (size_errors >= 4)
		return OLED_EBOUNDS;

	//OLED_WITH_SPINLOCK(oled) {
		/* Normalize coordinates */
		/* start_@ indicates coordinates of upper left corner  */
		/* stop_@ indicates coordinates of bottom right corner */
		uint8_t start_x = x_to < x_from ? x_to : x_from; /* x min */
		uint8_t start_y = y_to < y_from ? y_to : y_from; /* y min */
		uint8_t stop_x = x_to > x_from ? x_to : x_from;  /* x max */
		uint8_t stop_y = y_to > y_from ? y_to : y_from;  /* y max */

		if (is_fill) {
			/* Fill whole area */
			for (uint8_t x = start_x; x <= stop_x; x++) {
				for (uint8_t y = start_y; y <= stop_y; y++) {
					OLED_put_pixel_(oled, x, y, pixel_color);
				}
			}
		} else {
			/* Draw outer frame */
			for (uint8_t x = start_x; x <= stop_x; x++) {
				OLED_put_pixel_(oled, x, start_y, pixel_color);
				OLED_put_pixel_(oled, x, stop_y, pixel_color);
			}
			for (uint8_t y = start_y; y <= stop_y; y++) {
				OLED_put_pixel_(oled, start_x, y, pixel_color);
				OLED_put_pixel_(oled, stop_x, y, pixel_color);
			}
		}
	//}

	return OLED_EOK;
}

/*function for dicplaying letters by dude24fromNV*/
OLED_err OLED_put_letter(OLED *oled, char letter, uint8_t start_x, uint8_t start_y, bool invert)
{
        const uint8_t max_pos_x =120;
        const uint8_t max_pos_y = 55;
        /* Limit coordinates to display bounds */
        if ((start_y>max_pos_y) || (start_x>max_pos_x) || (start_y<0) || (start_x<0)) return OLED_EBOUNDS;

        uint8_t *paddr;
        uint16_t index;
        uint8_t shift = 0;
        if((letter<0x20) || (letter>0x7e)) {
                paddr = &Waree8x8[760];
        } else {
                index = (letter-32)*8;
                paddr = &Waree8x8[index];
        } 
        for (uint8_t x = start_x; x < (start_x+8); x++) {
                for (uint8_t y = start_y; y < (start_y+8); y++) {
                        if(!invert){
                                /*output letter*/
                                if((*paddr)&(0x01<<shift)){
                		        OLED_put_pixel_(oled, x, y, OLED_WHITE);
                                } else {
                                        OLED_put_pixel_(oled, x, y, OLED_BLACK );
                                }
                        } else {
                                /*negative of the drawing of the letter*/
                                if((*paddr)&(0x01<<shift)){
        		                OLED_put_pixel_(oled, x, y, OLED_BLACK);
                                } else {
                                        OLED_put_pixel_(oled, x, y, OLED_WHITE);
                                }
                        }
                        shift++;
                }
                shift = 0;
                paddr++;
        }
return OLED_EOK;
}


/*function for dicplaying strings by dude24fromNV*/
OLED_err OLED_put_string (OLED *oled, char *letters, uint8_t size, uint8_t start_x, uint8_t start_y, bool invert)
{
        size-=1; //delete from string \n
        const uint8_t max_pos_x =120;
        const uint8_t max_pos_y = 55;
        const uint8_t width_l = 8;
         /* Limit coordinates to display bounds */
        if ((start_y>max_pos_y) || (start_y<0) || (start_x>max_pos_x) || (start_x<0)) return OLED_EBOUNDS;
        
        /*output a string with transferring*/
        for( uint8_t i = 0; i < size; i++){
                if(start_x>120) {
                        start_y+=8;  
                        start_x=0;
                }
                OLED_put_letter(oled, *letters, start_x, start_y, invert);
                start_x+=8; 
                letters++;       
        }
return OLED_EOK;
}

ISR(TIMER1_OVF_vect)
{
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

int main(void)
{
	min = 0;
	hour = 0;
	alarm_h = 0;
	alarm_m = 0;
	int fl = 0;

	/*  Timer 1 initialization    */
	t1_init();
	int_ini();

	/* switch on interrupts by falling edge */
	EICRA |= (1<<ISC01) | (0 << ISC00) | (1 << ISC11);
	EIMSK |= (1 << INT0) | (1 << INT1);

	/* Setting clock buttons			 */
	DDRD |= (0 << 3) | (0 << 4) | (0 << 5);
	PORTD |= (1 << 3) | (1 << 4) | (1 << 5);
	DDRB = (1 << 0) | (1 << 1);
	PORTB |= (1 << 0)| (1 << 1);
    PORTB &= ~(1 << 2);

	bool byletter = false;

	sei();
	OLED oled;
	uint8_t fb[1024] = {0};
	OLED_init(&oled, 128, 64, fb, 200000, 0b0111100);

	/* Try to decrease frequency and see what happens without lock */
	OLED_WITH_SPINLOCK(&oled) {
		OLED_put_rectangle(&oled, 0, 0, 127, 63, OLED_FILL | 0);
		OLED_put_rectangle(&oled, 2, 2, 125, 61, OLED_FILL | 1);
		OLED_put_rectangle(&oled, 4, 4, 123, 57, 0);
	}
	if (byletter) OLED_refresh(&oled);

	bool color = OLED_BLACK;
    uint8_t scrpad[9];
    uint8_t tempr[1] = {0x00, 0x00}; 
    uint8_t temp = 0;

	while (1) {
		if ((PIND & (1 << 5)) == (0 << 5)) {
			sleep_ms(100);
			if ((PIND & (1 << 5)) == (0 << 5))
				fl++;
			if (fl > 2)
				fl = 0;	
		}
		if (fl == 0){
			if ((PIND & (1 << 4)) == (0 << 4)) {
				sleep_ms(100);
			if ((PIND & (1 << 4)) == (0 << 4))
				hour++;
			if (hour >= 24)
				hour = 0;
			}
			if ((PIND & (1 << 6)) == (0 << 6)) {
			sleep_ms(100);
			if ((PIND & (1 << 6)) == (0 << 6))
				min++;
			if (min >= 60)
				min = 0;	
			}
			OLED_refresh(&oled);
			OLED_put_string(&oled, "clock", 6, 45, 10, false);
			if (hour < 10){
				OLED_put_letter(&oled, '0', 40, 30, false);
				OLED_put_letter(&oled, hour + 48, 50, 30, false);
			}
			else {
				OLED_put_letter(&oled, (hour / 10) + 48, 40, 30, false);
				OLED_put_letter(&oled, (hour % 10) + 48, 50, 30, false);
			}
			OLED_put_letter(&oled, '.', 60, 30, false);
			if (min < 10){
				OLED_put_letter(&oled, '0', 70, 30, false);
				OLED_put_letter(&oled, min + 48, 80, 30, false);
			}
			else {
				OLED_put_letter(&oled, (min / 10) + 48, 70, 30, false);
				OLED_put_letter(&oled, (min % 10) + 48, 80, 30, false);
			}
			OLED_refresh(&oled);
		}
		if (fl == 1) {
			if ((PIND & (1 << 4)) == (0 << 4)) {
				sleep_ms(100);
			if ((PIND & (1 << 4)) == (0 << 4))
				alarm_h++;
			if (alarm_h >= 24)
				alarm_h = 0;
			}
			if ((PIND & (1 << 6)) == (0 << 6)) {
			sleep_ms(100);
			if ((PIND & (1 << 6)) == (0 << 6))
				alarm_m++;
			if (alarm_m >= 60)
				alarm_m = 0;	
			}
			OLED_refresh(&oled);
			OLED_put_string(&oled, "alarm", 6, 45, 10, false);
			if (alarm_h < 10){
				OLED_put_letter(&oled, '0', 40, 30, false);
				OLED_put_letter(&oled, alarm_h + 48, 50, 30, false);
			}
			else {
				OLED_put_letter(&oled, (alarm_h / 10) + 48, 40, 30, false);
				OLED_put_letter(&oled, (alarm_h % 10) + 48, 50, 30, false);
			}
			OLED_put_letter(&oled, '.', 60, 30, false);
			if (alarm_m < 10){
				OLED_put_letter(&oled, '0', 70, 30, false);
				OLED_put_letter(&oled, alarm_m + 48, 80, 30, false);
			}
			else {
				OLED_put_letter(&oled, (alarm_m / 10) + 48, 70, 30, false);
				OLED_put_letter(&oled, (alarm_m % 10) + 48, 80, 30, false);
			}
			OLED_refresh(&oled);
		}
		if (fl == 2) {
			skip_rom();
            convert_tempr ();
            init_termometr();
            skip_rom();
            read_scrpad(scrpad);
            temp = scrpad[0] >> 4;
            temp |= scrpad[1] << 4;
            char *num1 = malloc(sizeof(num1) * 3);
            char *num2 = malloc(sizeof(num2) * 3);
            itoa(temp / 10, num1, 10);
            itoa(temp % 10, num2, 10);
			OLED_refresh(&oled);
			OLED_put_string(&oled, "temperature", 12, 20, 10, false);
			OLED_put_string(&oled, num1, 5, 40, 30, false);
			OLED_put_string(&oled, num2, 5, 50, 30, false);
			OLED_put_string(&oled, " C", 3, 60, 30, false);
		}
		
		OLED_WITH_SPINLOCK(&oled);
		color = !color;
		OLED_refresh(&oled);

		/* Checking alarm time				*/
		if (((hour - alarm_h) < 1) && (min - alarm_m) < 2) {
			DDRB  |= (1 << 3);
			PORTB |= (1 << 3);
		}
		else
			PORTB &= ~(1 << 3);
	}
}
