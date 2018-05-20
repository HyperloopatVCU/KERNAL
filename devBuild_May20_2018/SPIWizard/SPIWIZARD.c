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
static struct channel com = {{0}, 0, 0}, dat = {{0}, 0, 0};

static struct {
	volatile uint8_t A : 1; // command finished
} flag = {1};

static void (*trig)() = NULL;

void spi_sync() {
	while(flag.A == 0);
	//whistle(0x09f8, 2);
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
			SPDR = dat.Q[dat.read];
			break;
		case 0:
			dat.Q[dat.read++] = SPDR;
			SET(PINB, PINB2);
		begn:	if (counter) {
				CLR(PINB, PINB2);
				SPDR = command;
			} else goto q;
		}
	} else {
	q:	if (com.read != com.write) {
			flag.A = 0;
			switch(command = com.Q[com.read++]) {
			case 0:
				flag.A = 1;
				cli();
				if (trig != NULL) trig();
				return;
			default:
				counter = (com.Q[com.read++])<<2;
				addr = *((uint16_t*)(&com.Q[com.read]));
				com.read += 2;
				goto begn;
			}
		}
		flag.A = 1;
	}
}

static void spi_wait_for_command_length(uint8_t length) {
	uint16_t timeout = 0xffff;

	while ((uint8_t)(com.read - com.write - 1) < length) {	/* NEEDS WATCHDOG */
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
	return dat.Q[(uint8_t)(dat.read + relative)];
}

uint8_t spi_pushdata(const uint8_t *s, uint8_t length) {
	while (length) {
		while((uint8_t)(dat.read - dat.write - 1) < 1); /* NEEDS WATCHDOG */
		length--;
		dat.Q[dat.write++] = *(s++);
	}
	return length;
}

void spi_process(uint8_t command, uint8_t length, uint16_t startaddr, uint8_t generate_trigger) {
	if (length == 0) return;
	
	spi_wait_for_command_length(4);
	
	com.Q[com.write++] = command;
	com.Q[com.write++] = length;
	*(uint16_t*)(com.Q+com.write) = startaddr;
	com.write += 2;
	if (generate_trigger) spi_halt();
}

void spi_halt() {
	spi_wait_for_command_length(1);
	com.Q[com.write++] = 0;
}

void spi_exec() {
	if (flag.A) {
		flag.A = 0;
		SPDR = 0;
	}
}