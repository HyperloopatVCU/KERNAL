/*
 * SPI device initialization and functional wizard
 *  GLOBAL DEFINITIONS
 * Created: 3/30/2018 11:44:57 PM
 *  Author: Karats
 */ 

#ifndef SPIWIZARD_H_
#define SPIWIZARD_H_

#include "../base_includes.h"
//
//#define OUT 1
//#define IN  0
//
//#define DDR_SPI DDRB
//#define DD_SCK  DDB5
//#define DD_MISO DDB4
//#define DD_MOSI DDB3
//#define DD_SS   DDB2
//
//#define P_SPI   PINB
//#define P_SCK   PINB5
//#define P_MISO  PINB4
//#define P_MOSI  PINB3
//#define P_SS    PINB2
//
///* SETUP ID'S */
//#define SPI_MS  (1<<DDB3)|(1<<DDB5)
//#define SPI_SL  (1<<DDB4)
//
///* general-purpose */
//
//void spi_setup(char);
//
///* blocking prototypes */
//uint8_t spi_block(uint8_t);

void spimaster_SS(uint8_t*, uint8_t);

#endif /* SPIWIZARD_H_ */