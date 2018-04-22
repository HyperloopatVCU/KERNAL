/*
 * TWI device initialization and functional wizard
 *
 * Created: 4/19/2018 5:12:04 PM
 *  Author: Karats
 */ 
#include "TWIWIZARD.h"

void wait() {
	while (!(TWCR & (1<<TWEN)));
}

uint8_t test_twsr(uint8_t cond) {
	return (TWSR & 0x0f8) != cond;
}

uint8_t mt_write_block(uint8_t address, uint8_t data) {
	TWCR = i((1<<TWSTA)|TWI_ENABLE);
	wait();
	if (test_twsr(START)) return -1;
	TWDR = address | 1; /*This makes it a WRITE condition */
	TWCR = i(TWI_ENABLE);
	wait();
	if (test_twsr(SLAW_ACK)) return -2;
	TWDR = data;
	TWCR = i(TWI_ENABLE);
	wait();
	if (test_twsr(WDATA_ACK)) return -3;
	TWCR = i((1<<TWSTO)|TWI_ENABLE);
	return 0;
}

uint8_t mt_read_block(uint8_t address, uint8_t* data) {
	(*data) = 0;
	TWCR = i((1<<TWSTA)|TWI_ENABLE);
	wait();
	if (test_twsr(START)) return -1;
	TWDR = address & 0x0fe;
	TWCR = i(TWI_ENABLE);
	wait();
	if (test_twsr(SLAR_ACK)) return -2;
	TWCR = i(TWI_ENABLE);
	wait();
	if (test_twsr(RDATA_ACK)) return -3;
	TWCR = i((1<<TWSTO)|TWI_ENABLE);
	return 0;
}