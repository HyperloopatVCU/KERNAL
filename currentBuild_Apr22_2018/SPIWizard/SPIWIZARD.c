/*
 * SPI device initialization and functional wizard
 *
 * Created: 3/30/2018 8:27:42 PM
 * Author : Karats
 */ 

#include "SPIWIZARD.h"

void spi_setup(char setupid) {
	DDR_SPI = setupid;
	SPCR = (1<<SPE)|((setupid == (SPI_MS)) ? ((1<<MSTR)|(1<<SPR0)) : 0);
}

#ifdef USE_INTERRUPTS

void (*spi_ptr)(void);

ISR(SPI_STC_vect) { /* SPI Serial Transfer Complete */
	(*spi_ptr)();
}

#else

uint8_t spi_block(uint8_t dat) {
	uint8_t timeout = 0xff;
	SPDR = dat;
	while (!(SPSR & (1<<SPIF)))
		if (!(timeout--)) return 0;
	return SPDR;
}

uint8_t spi_queue(uint8_t dat) {
	return -1;
}

#endif