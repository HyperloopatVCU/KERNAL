/*
 * SPI device initialization and functional wizard
 *
 * Created: 3/30/2018 8:27:42 PM
 * Author : Karats
 */

#include "SPIWIZARD.h"

#define SPI_TRIG 1

struct channel {
	volatile uint8_t Q[256];
	volatile uint8_t write;
	volatile uint8_t read;
};
static struct channel comm = {{0}, 0, 0}, data = {{0}, 0, 0};

static struct {
	volatile uint8_t A : 1; // activity flag
} flag = {1};

static void (*trig)() = NULL;

void spi_sync() {
	while(flag.A == 0);
		/* NEEDS WATCHDOG */
}

ISR(SPI_STC_vect) {
	volatile static uint8_t command = 0;
	volatile static uint16_t addr = 0;
	volatile static uint16_t counter = 0;

	if (counter) {
		switch (--counter & 3) {
		case 3:
			SPDR = (uint8_t)(addr>>H);
			break;
		case 2:
			SPDR = (uint8_t)addr;
			addr++;
			break;
		case 1:
			SPDR = data.Q[data.read];
			break;
		case 0:
			data.Q[data.read++] = SPDR;
			SET(PINB, PINB2);
		begn:	if (counter) {
				CLR(PINB, PINB2);
				SPDR = command;
			} else goto q;
		}
	} else {
	q:	if (comm.read != comm.write) {
			flag.A = 0;
			switch(command = comm.Q[comm.read++]) {
			case 0:
				flag.A = 1;
				cli();
				if (trig != NULL) trig();
				return;
			default:
				counter = (comm.Q[comm.read++])<<2;
				addr = *((uint16_t*)(&comm.Q[comm.read]));
				comm.read += 2;
				goto begn;
			}
		}
		flag.A = 1;
	}
}

static void spi_wait_for_command_length(uint8_t length) {
	uint16_t timeout = 0xffff;

	while ((uint8_t)(comm.read - comm.write - 1) < length) {
		/* NEEDS WATCHDOG */
		if (!timeout--) kill(EQUEUE);
	}
}

void spi_init(void (*T)()) {
	trig = T;
	sei();
	if (DDRB != ((1<<DDB5)|(1<<DDB3)|(1<<DDB2))) {
		DDRB = (1<<DDB5)|(1<<DDB3)|(1<<DDB2);
		SPCR = (1<<SPIE)|(1<<SPE)|(1<<MSTR);
	}
	SET(PINB, PINB2);
}

uint8_t spi_read(int8_t relative) {
	return data.Q[(uint8_t)(data.read + relative)];
}

uint8_t spi_pushdata(const uint8_t *s, uint8_t length) {
	while (length) {
		while((uint8_t)(data.read - data.write - 1) < 1);
			/* NEEDS WATCHDOG */
		length--;
		data.Q[data.write++] = *(s++);
	}
	return length;
}

void spi_halt() {
	spi_wait_for_command_length(1);
	comm.Q[comm.write++] = 0;
}

void spi_process(uint8_t command, uint8_t length, uint16_t startaddr,
		uint8_t generate_trigger)
{
	if (length == 0) return;
	
	spi_wait_for_command_length(4);
	
	comm.Q[comm.write++] = command;
	comm.Q[comm.write++] = length;
	*(uint16_t*)(comm.Q+comm.write) = startaddr;
	comm.write += 2;
	if (generate_trigger) spi_halt();
}

void spi_exec() {
	if (flag.A) {
		flag.A = 0;
		SPDR = 0;
	}
}