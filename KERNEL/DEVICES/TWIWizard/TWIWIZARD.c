/*
 * TWI device initialization and functional wizard
 *
 * Created: 4/19/2018 5:12:04 PM
 *  Author: Karats
 */ 
#include "TWIWIZARD.h"

/* intermediary command codes */
#define RACK	0x10
#define WACK	0x12
#define RCONT	0x21
#define WCONT	0x22

/* status codes */
#define START		0x08
#define RESTART		0x10
#define SLAW_ACK	0x18
#define SLAW_NAK	0x20
#define WDATA_ACK	0x28
#define WDATA_NAK	0x30
#define SLA_ARB		0x38 /* ARB ERROR */
#define SLAR_ACK	0x40
#define SLAR_NAK	0x48
#define RDATA_ACK	0x50
#define RDATA_NAK	0x58
#define TWI_ENABLE	(1<<TWEN)|(1<<TWINT)

struct channel {
	volatile uint8_t Q[256];
	volatile uint8_t write;
	volatile uint8_t read;
};
static struct channel twicom = {{0}, 0, 0};

static struct {
	volatile uint8_t A : 1; // activity flag
} flag = {1};

void twi_sync() {
	while(flag.A == 0);
		/* NEEDS WATCHDOG */
}

uint8_t nstch(uint8_t expected) {
	return (TWSR != expected);
}

ISR(TWI_vect) {
	volatile static uint8_t command = 0;
	volatile static uint8_t counter = 0;
	volatile static uint8_t* data = NULL;

	if (counter) {
		switch (command) {
		case TWI_READ: case TWI_WRITE:
			if (nstch(START)) {
				/* RESTART OR ERROR */
			}
			break;
		case RACK:
			if (nstch(SLAR_ACK)) {
				/* ERROR */
			} break;
		case WACK:
			if (nstch(SLAW_ACK)) {
				/* ERROR */
			} break;
		case RCONT:
			if (nstch(RDATA_ACK)) {
				/* NACK OR ERROR */
			} break;
		case WCONT:
			if (nstch(WDATA_ACK)) {
				/* NACK OR ERROR */
			} break;
		}
		counter--;
		command += 0x10;
		TWDR = *data++;
	} else {
		if (twicom.read != twicom.write) {
			switch (command = twicom.Q[twicom.read++]) {
			case TWI_READ: case TWI_WRITE:
				TWDR = twicom.Q[twicom.read++];
				counter = twicom.Q[twicom.read++];
				data = *(uint8_t**)(&twicom.Q[twicom.read]);
				twicom.read += sizeof(uint8_t*);
				break;
			case TWI_END: default:
				TWCR = ((1<<TWSTO)|TWI_ENABLE);
				flag.A = 0;
				return;
			}
		}
	}
	TWCR = ((1<<TWIE)|TWI_ENABLE |
			((counter == 1) && RCONT)? (1<<TWEA) : 0);
}

void twi_queue(uint8_t addr, uint8_t *data, uint8_t length, uint8_t cmd) {
	if ((length == 0) && (cmd != TWI_END)) return;
	
	while (
		(uint8_t)(twicom.read - twicom.write - 1) <
		(uint8_t)((cmd == TWI_END) ? 1 : (1 + sizeof(uint8_t*)))
	);
		/* NEEDS WATCHDOG */
	
	twicom.Q[twicom.write++] = cmd;
	if (cmd != TWI_END) {
		twicom.Q[twicom.write++] = addr;
		twicom.Q[twicom.write++] = length;
		*((uint8_t**)(&twicom.Q[twicom.write])) = data;
		twicom.write += sizeof(uint8_t*);
	}
}

void twi_exec() {
	if (!flag.A) {
		flag.A = 1;
		TWCR = ((1<<TWSTA)|(1<<TWIE)|TWI_ENABLE);
	}
}
