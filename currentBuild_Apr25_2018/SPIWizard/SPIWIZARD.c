/*
 * SPI device initialization and functional wizard
 *
 * Created: 3/30/2018 8:27:42 PM
 * Author : Karats
 */ 

#include "SPIWIZARD.h"

void spimaster_SS(uint8_t* data, uint8_t length) {
  uint8_t timeout = 0xff;
  if (!length) return;
  if (!(DDRB & DDB3)) {
    DDRB = (1<<DDB5)|(1<<DDB3)|(1<<DDB2);
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
  }
  CLR(PINB, PINB2);
  while (length--) {
    SPDR = *data;
    while (!(SPSR & (1<<SPIF)))
      if (!(timeout--)) {
        goto ex;
      }
    *(data++) = SPDR;
  }
ex: SET(PINB, PINB2);
}