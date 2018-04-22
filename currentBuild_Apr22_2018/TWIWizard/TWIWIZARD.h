/*
 * TWI device initialization and functional wizard
 *	GLOBAL DEFINITIONS
 * Created: 4/19/2018 5:12:36 PM
 *  Author: Karats
 */ 


#ifndef SPIWIZARD_H_
#define SPIWIZARD_H_

#include "../base_includes.h"

#define START		0x08
#define RESTART		0x10
#define SLAW_ACK	0x18
#define SLAW_NAK	0x20
#define WDATA_ACK	0x28
#define WDATA_NAK	0x30
#define SLA_ARB		0x38
#define SLAR_ACK	0x40
#define SLAR_NAK	0x48
#define RDATA_ACK	0x50
#define RDATA_NAK	0x58
#define TWI_ENABLE (1<<TWEN)|(1<<TWINT)

uint8_t mt_write_block(uint8_t, uint8_t);
uint8_t mt_read_block(uint8_t, uint8_t*);

#endif /* SPIWIZARD_H_ */