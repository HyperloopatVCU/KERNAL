#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>

void ERR() {
	exit(EXIT_FAILURE);
}

/************************************************************************/
/* TWI interface                                                        */
/************************************************************************/
#define START		0x08	/* TWI START */
#define MT_SLA_ACK	0x18	/* TWI Master Transmit Slave Acknowledged */
#define MT_DAT_ACK	0x28	/* TWI Master Transmit Data Acknowledged */

#define READ		0x1		/* literal values for address R/W bits */
#define WRITE		0x0

struct {
	int length;
	char address;
	char* data;
	int err_stat;
} twistruct;

void (*twi_cont)(void);

ISR(TWI_vect) {
	(*twi_cont)();
}

void donothing(){};

void endtwi() {
	if ((TWSR & 0x0f8) != twistruct.err_stat) { ERR(); }
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	twi_cont = &donothing;
}

void twiloop() {
	if ((TWSR & 0x0f8) != twistruct.err_stat) { ERR(); }
	*(twistruct.data++) = TWDR;
	TWDR = *twistruct.data;
	
	if (twistruct.length-- == 0) twi_cont = &endtwi;
	if (twistruct.err_stat == START) twistruct.err_stat = MT_SLA_ACK;
	if (twistruct.err_stat == MT_SLA_ACK) twistruct.err_stat = MT_DAT_ACK;
	
	TWCR = (1<<TWINT)|(1<<TWEN);
}

void starttwi(char address, int length, char* data) {
	twistruct.address = address;
	twistruct.length = length+1;
	twistruct.data = data;
	
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	twistruct.err_stat = START;
	twi_cont = &twiloop;
}

/************************************************************************/
/* Main routine                                                         */
/************************************************************************/

#define WIZNET (0x8 << 1)

int main(void)
{
	char* data;
	void* buffer_area = malloc(256); /* 256-byte buffer area */
	
	starttwi(WIZNET|WRITE, 1, data);
	
    while (1) 
    {
    }
}