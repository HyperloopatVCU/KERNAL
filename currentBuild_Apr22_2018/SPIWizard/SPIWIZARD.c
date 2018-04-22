/*
 * SPI device initialization and functional wizard
 *
 * Created: 3/30/2018 8:27:42 PM
 * Author : Karats
 */ 

#include "SPIWIZARD.h"

void (*spi_ptr)(void);

/**
struct {
	uint8_t queue : 1;
} spi_flag = {0};
**/

ISR(SPI_STC_vect) { /* SPI Serial Transfer Complete */
	(*spi_ptr)();
}

void spi_setup(char dir) {
	DDR_SPI = dir;
	SPCR = (1<<SPE)|((dir == (SPI_MS)) ? ((1<<MSTR)|(1<<SPR0)) : 0);
}

void SS_S(uint8_t sig) {
  if (sig != 0) {
    P_SPI |= 1<<P_SS;
  }
  else {
    P_SPI &= (0xff^(1<<P_SS));
  }
}

void blockcheck(void) {
	/*if (spi_flag.queue & 1) {
    logint(spi_flag.queue);
    logger(" ");
		spi_flag.queue = 0;
		SPCR &= !(1<<SPIE);
    loghex(SPCR);
    logger("\n\t");
	}*/
}

uint8_t spi_block(uint8_t dat) {
	SPDR = dat;
	while (!(SPSR & (1<<SPIF)));
  return SPDR;
}

uint8_t spi_queue(uint8_t dat) {
	return -1;
}