/*
 * TWI device initialization and functional wizard
 *	GLOBAL DEFINITIONS
 * Created: 4/19/2018 5:12:36 PM
 *  Author: Karats
 */ 


#ifndef SPIWIZARD_H_
#define SPIWIZARD_H_

#include "../../base_includes.h"

#define TWI_END 0
#define TWI_READ 1
#define TWI_WRITE 2

void twi_queue(uint8_t, uint8_t*, uint8_t, uint8_t);
void twi_exec();

//uint8_t mt_write_block(uint8_t, uint8_t);
//uint8_t mt_read_block(uint8_t, uint8_t*);

#endif /* SPIWIZARD_H_ */