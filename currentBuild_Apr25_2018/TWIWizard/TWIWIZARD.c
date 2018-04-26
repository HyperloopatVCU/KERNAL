/*
 * TWI device initialization and functional wizard
 *
 * Created: 4/19/2018 5:12:04 PM
 *  Author: Karats
 */ 
#include "TWIWIZARD.h"

static volatile uint8_t r = 0;
static volatile uint8_t *dat = NULL;
static volatile uint8_t cnt = 0;

ISR(TWI_vect) {
	if (r == 0) r--;
	else if (r < 0) *(dat++) = TWDR;
	else if (r > 0) TWDR = *(dat++);

	if (cnt) {
		TWCR = ((1<<TWIE)|TWI_ENABLE);
		if (--cnt) TWCR |= (1<<TWEA);
		return;
	}

	TWCR = ((1<<TWSTO)|TWI_ENABLE);
	flag.TWIhold = 0;
}

void twimaster(uint8_t address, uint8_t *data, uint8_t length) {
	if ((!length) || flag.TWIhold) return;
	flag.TWIhold = 1;
	dat = data;
	cnt = length;
	r = address & 1;

	TWCR = ((1<<TWSTA)|TWI_ENABLE);

	while (!(TWCR & (1<<TWEN)));

	TWDR = address;
	TWCR = ((1<<TWIE)|TWI_ENABLE);
}
//
//void wait() {
	//while (!(TWCR & (1<<TWEN)));
//}
//
//uint8_t test_twsr(uint8_t cond) {
	//return (TWSR & 0x0f8) != cond;
//}
//
//uint8_t mt_write_block(uint8_t address, uint8_t data) {
	//TWCR = ((1<<TWSTA)|TWI_ENABLE);
	//wait();
	//if (test_twsr(START)) return -1;
	//TWDR = address | 1; /*This makes it a WRITE condition */
	//TWCR = (TWI_ENABLE);
	//wait();
	//if (test_twsr(SLAW_ACK)) return -2;
	//TWDR = data;
	//TWCR = (TWI_ENABLE);
	//wait();
	//if (test_twsr(WDATA_ACK)) return -3;
	//TWCR = ((1<<TWSTO)|TWI_ENABLE);
	//return 0;
//}
//
//uint8_t mt_read_block(uint8_t address, uint8_t* data) {
	//(*data) = 0;
	//TWCR = ((1<<TWSTA)|TWI_ENABLE);
	//wait();
	//if (test_twsr(START)) return -1;
	//TWDR = address & 0x0fe;
	//TWCR = (TWI_ENABLE);
	//wait();
	//if (test_twsr(SLAR_ACK)) return -2;
	//TWCR = (TWI_ENABLE);
	//wait();
	//if (test_twsr(RDATA_ACK)) return -3;
	//TWCR = ((1<<TWSTO)|TWI_ENABLE);
	//return 0;
//}