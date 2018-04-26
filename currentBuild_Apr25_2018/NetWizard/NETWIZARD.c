/*
 * Network device initialization and functional wizard
 *
 * Created: 3/30/2018 8:57:57 PM
 * Author : Karats
 ********************************************************
 * Application Notes for SPI usage:
 * 
 * setup MCU as SPI MASTER
 * SPE, MSTR, SPR0, (SPIE for interrupt usage)
 * 
 * /SS -> Output, High
 * SCK -> Output
 * MOSI -> Output
 * MISO -> Input
 * 
 * - set /SS to low to start data transfer.
 * - Wait for reception complete.
 * - set /SS to high after transmission finished.
 * 
 * Data structure (4 bytes, MSB first):
 * OP-code : ADDRH : ADDRL : DATA
 * 
 */ 

#include "NETWIZARD.h"

/************************************************************************/
/* SPI functions                                                        */
/************************************************************************/

uint8_t readby(uint16_t addr) {
	uint8_t msg[4] = {COM_READ, (uint8_t)(addr>>H), (uint8_t)addr, 0xff};
	spimaster_SS(msg, 4);
	return msg[3];
}

uint16_t readw(uint16_t addr) {
	uint16_t ret;
	ret = readby(addr)<<H;
	ret |= readby(addr+1);
	return ret;
}

void writeby(uint16_t addr, uint8_t by) {
	uint8_t msg[4] = {COM_WRITE, (uint8_t)(addr>>H), (uint8_t)addr, by};
	spimaster_SS(msg, 4);
}

void writes(uint16_t addr, const uint8_t *s, uint8_t length) {
	uint8_t i;
	for (i = 0; i < length; i++) {
		writeby(addr + i, s[i]);
	}
}

void writew(uint16_t addr, uint16_t w) {
	writeby(addr, (uint8_t)(w>>H));
	writeby(addr+1, (uint8_t)w);
}


/************************************************************************/
/* Socket functions                                                     */
/************************************************************************/

void disconnect() {
	writeby(S0 + S_CR, DISCON);
}

void closesocket() {
	writeby(S0 + S_CR, CLOSE);
}

uint8_t opensocket() {
	uint8_t retry = 10, c;

	opensock:
	if (!retry) return -1;
	writeby(S0 + S_CR, OPEN);
	for (c = 0xff; c > 0; c--);
	if (readby(S0 + S_SR) != SOCK_INIT) {
		writeby(S0 + S_CR, CLOSE);
		retry--;
		goto opensock;
	}
	return 0;
}

uint8_t establish_block() {
	/*wait for ESTABLISHED (blocking)*/
	uint32_t x;
	
	retry:  for (x = 0x100000; x; x--);
	
	switch (readby(S0 + S_SR)) {
		case SOCK_CLOSED: return -1;
		case SOCK_CLOSE_WAIT: return -2;
		case SOCK_ESTABLISHED: return 0; /*ESTABLISHED: return success*/
		default: goto retry;
	}
}

uint8_t connect_block(uint8_t* ip, uint16_t port) {
	writes(S0 + S_DIPR0, ip, 4);
	writew(S0 + S_DPORT0, port);
	
	writeby(S0 + S_CR, CONNECT);
	return establish_block();
}

uint8_t listen_block() {
	/*LISTEN*/
	writeby(S0 + S_CR, LISTEN);

	if (readby(S0 + S_SR) != SOCK_LISTEN) {
		return -2;
	}
	return establish_block();
}

/************************************************************************/
/* Port functions                                                       */
/************************************************************************/

void putby(uint8_t c) {
	uint16_t bwp = readw(S0 + S_TX_WR0);
	while (readw(S0 + S_TX_FSR0_RO) < TX_SIZE);

	writeby(bwp%TX_SIZE + TX_BASE, c);
	writew(S0 + S_TX_WR0, bwp + 1);
	writeby(S0 + S_CR, SEND);
}

uint16_t putline(uint8_t* buffer, uint16_t size) {
	uint16_t s, index;
	uint16_t bwp = readw(S0 + S_TX_WR0);

	while ((s = readw(S0 + S_TX_FSR0_RO)) < TX_SIZE);

	switch (readby(S0 + S_SR)) {
		case SOCK_CLOSED: return -1;
		case SOCK_CLOSE_WAIT: return -2;
	}

	for (index = 0; (index < s) && (index < size); index++) {
		writeby((bwp + index)%TX_SIZE + TX_BASE, buffer[index]);
	}
	
	writew(S0 + S_TX_WR0, bwp + index);
	writeby(S0 + S_CR, SEND);
	return index;
}

uint8_t getby() {
	uint8_t c = 0;
	uint16_t brp = readw(S0 + S_RX_RD0);
	if (readw(S0 + S_RX_RSR0_RO)) {
		writew(S0 + S_RX_RD0, brp + 1);
		writeby(S0 + S_CR, RECV);
		c = readby(brp%RX_SIZE + RX_BASE);
	}
	return c;
}

/************************************************************************/
/* Default setup functions                                              */
/************************************************************************/

void net_config_default(socker* sock) {
	writeby(MR, 0);

	writeby(IMR, 0); /*don't use interrupts at this time*/
	
	writew(RTR0, 4000); /*retry timeout in .1 microseconds*/
	
	writeby(RCR, 5); /*retry count*/
	
	writes(GAR0, sock->gateway, 4); /*gateway*/

	writeby(SHAR0, 'R'); /*MAC*/
	writeby(SHAR1, 'E');
	writeby(SHAR2, 'C');
	writeby(SHAR3, 'R');
	writeby(SHAR4, 'D');
	writeby(SHAR5, sock->ID);
	
	writes(SUBR0, sock->netmask, 4); /*net mask*/
	
	writeby(SIPR0, sock->gateway[0] & sock->netmask[0]); /*IP*/
	writeby(SIPR1, sock->gateway[1] & sock->netmask[1]);
	writeby(SIPR2, sock->gateway[2] & sock->netmask[2]);
	writeby(SIPR3, (sock->gateway[3] & sock->netmask[3]) | ((sock->ID + 1) & com(sock->netmask[3])));

	writeby(RMSR, 0x03);
	writeby(TMSR, 0x03);

	writeby(S0 + S_MR, sock->mode);

	writew(S0 + S_PORT0, sock->port);

	writew(S0 + S_MSSR0, 0x0218);

	writeby(S0 + S_TOS, 1);
	writeby(S0 + S_TTL, 0x80);
}