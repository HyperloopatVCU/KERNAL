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

void write(uint16_t addr, uint8_t dat) {
	uint8_t lim = 10;
	if (!(SPCR & (1<<MSTR))) spi_setup(SPI_MS);
	SS_S(L);
	SS_S(L);
	while(spi_block(COM_WRITE)) {
		SS_S(H);
		if (!lim--) return;
		SS_S(L);
	}
	spi_block(addr>>H);
	spi_block(addr);
	spi_block(dat);
	SS_S(H);
	SS_S(H);
}

uint8_t read(uint16_t addr) {
	uint8_t dat, lim = 10;
	if (!(SPCR & (1<<MSTR))) spi_setup(SPI_MS);
	SS_S(L);
	SS_S(L);
	while(spi_block(COM_READ)) {
		SS_S(H);
		if (!lim--) return 0;
		SS_S(L);
	}
	spi_block(addr>>H);
	spi_block(addr);
	dat = spi_block(0xff);
	SS_S(H);
	SS_S(H);
	return dat;
}

/************************************************************************/
/* Socket functions                                                     */
/************************************************************************/

void getaddressconf(socker* sock) {
	uint8_t rxtop, txtop;
	uint8_t dist = (sock->base>>7)&0x6;
	
	uint8_t rmsrVal = read(RMSR);
	uint8_t tmsrVal = read(TMSR);
	uint8_t rxsize = 4<<((rmsrVal>>dist)&3);
	uint8_t rxoffs = 0;
	
	uint8_t txsize = 4<<((tmsrVal>>dist)&3);
	uint8_t txoffs = 0;
	
	for(; dist > 0; dist -= 2) {
		txoffs += 4<<((rmsrVal>>dist)&3);
		rxoffs += 4<<((tmsrVal>>dist)&3);
	}
	rxtop = rxsize + rxoffs;
	txtop = txsize + txoffs;
	
	if (rxtop > 8) rxsize -= rxtop - 0x20;
	if (txtop > 8) txsize -= txtop - 0x20;
	
	if (rxsize) {
		sock->rxmask = (((uint16_t)rxsize)<<H)-1;
		sock->rxbase = RX_BASE + (((uint16_t)rxoffs)<<H);
	} else {
		sock->rxmask = 0;
		sock->rxbase = (uint16_t)RX_BASE + 0x2000;
	}
	
	if (txsize) {
		sock->txmask = (((uint16_t)txsize)<<H)-1;
		sock->txbase = TX_BASE + (((uint16_t)txoffs)<<H);
	} else {
		sock->txmask = 0;
		sock->txbase = TX_BASE + 0x2000;
	}
}

void disconnect(socker* sock) {
	write(sock->base + S_CR, DISCON);
}

void closesocket(socker* sock) {
	write(sock->base + S_CR, CLOSE);
}

uint8_t opensocket(socker* sock) {
	uint8_t retry = 10, c;
	uint16_t base = sock->base;
	
opensock:
	if (!retry) return -1;
	write(base + S_MR, sock->mode);
	write(base + S_PORT0, sock->port>>H);
	write(base + S_PORT1, sock->port);
	write(base + S_MSSR0, 0x02);
	write(base + S_MSSR1, 0x18);
	write(base + S_CR, OPEN);
	for (c = 0xff; c > 0; c--);
	if (read(base + S_SR) != SOCK_INIT) {
		write(base + S_CR, CLOSE);
		retry--;
		goto opensock;
	}
	getaddressconf(sock);
	return 0;
}

uint8_t establish_block(uint16_t base) {

	/*wait for ESTABLISHED (blocking)*/
retry_ESTABLISH:
	switch (read(base + S_SR)) {
		case SOCK_CLOSED: return -1;
		case SOCK_ESTABLISHED: return 0; /*ESTABLISHED: return success*/
		default: goto retry_ESTABLISH;
	}
	return -68;
}

uint8_t connect_block(socker* sock, quad* ip, uint16_t port) {
	uint16_t base = sock->base;
	write(base + S_DIPR0, ip->b0);
	write(base + S_DIPR1, ip->b1);
	write(base + S_DIPR2, ip->b2);
	write(base + S_DIPR3, ip->b3);
	write(base + S_DPORT0, port>>H);
	write(base + S_DPORT1, port);
	
	write(base + S_CR, CONNECT);
	return establish_block(base);
}

uint8_t listen_block(socker* sock) {
	uint16_t base = sock->base;
	
	/*LISTEN*/
	write(base + S_CR, LISTEN);

	if (read(base + S_SR) != SOCK_LISTEN) {
		return -2;
	}
	
	return establish_block(base);
}

/************************************************************************/
/* Port functions                                                       */
/************************************************************************/

uint16_t putline(socker* sock, uint8_t* buffer, uint16_t size) {
	register uint16_t index = 0, base = sock->base;
	register uint16_t bwp = read(base + S_TX_WR0)<<H | read(base + S_TX_WR1);
	register uint16_t ubound = sock->txmask + 1;
	
	switch (read(base + S_SR)) {
	case SOCK_CLOSED: return -1;
	case SOCK_CLOSE_WAIT: return -2;
	}
	
	if (!sock->rxmask) goto end;
	
	for (; (index < ubound) && (index < size); index++) {
		write((bwp + index)%ubound + sock->txbase, buffer[index]);
	}
	
	bwp += index;
	write(base + S_TX_WR0, (uint8_t)(bwp>>H));
	write(base + S_TX_WR1, (uint8_t)bwp);
	write(base + S_CR, SEND);
end:
	return index;
}

uint16_t getline(socker* sock, uint8_t* buffer, uint16_t size) {
	register uint16_t s, index = 0, base = sock->base;
	register uint16_t brp = read(base + S_RX_RD0)<<H | read(base + S_RX_RD1);
	register uint16_t ubound = sock->rxmask + 1;
	
	if (!sock->rxmask) goto end;
	
	do {
		switch (read(base + S_SR)) {
		case SOCK_CLOSED: return -1;
		case SOCK_CLOSE_WAIT: return -2;
		}
		s = (read(base + S_RX_RSR0_RO)<<H | read(base + S_RX_RSR1_RO));
		for (; (index < s) && (index < (size - 1)); index++) {
			if ((buffer[index] = read((brp + index)%ubound + sock->rxbase)) == '\n') {
				if (buffer[index-1] == '\r') {
					index++; goto out;
				}
			}
		}
	} while (index < (size - 1));

out:
	brp += index;
	write(base + S_RX_RD0, (uint8_t)(brp>>H));
	write(base + S_RX_RD1, (uint8_t)brp);
	write(base + S_CR, RECV);
end:
	return index;
}

/************************************************************************/
/* Default setup functions                                              */
/************************************************************************/

void net_config_default(socker* sock) {
	write(MR, 0);
	
	write(IMR, 0); /*don't use interrupts at this time*/
	
	write(RTR0, 0x27); /*retry timeout in .1 microseconds (4000)*/
	write(RTR1, 0x10);
	
	write(RCR, 30); /*retry count*/
	
	write(GAR0, sock->gateway.b0); /*gateway*/
	write(GAR1, sock->gateway.b1);
	write(GAR2, sock->gateway.b2);
	write(GAR3, sock->gateway.b3);
	
	write(SHAR0, 'R'); /*MAC*/
	write(SHAR1, 'E');
	write(SHAR2, 'C');
	write(SHAR3, 'R');
	write(SHAR4, 'D');
	write(SHAR5, '0' + sock->ID);
	
	write(SUBR0, sock->netmask.b0); /*net mask*/
	write(SUBR1, sock->netmask.b1);
	write(SUBR2, sock->netmask.b2);
	write(SUBR3, sock->netmask.b3);
	
	write(SIPR0, sock->gateway.b0 & sock->netmask.b0); /*IP*/
	write(SIPR1, sock->gateway.b1 & sock->netmask.b1);
	write(SIPR2, sock->gateway.b2 & sock->netmask.b2);
	write(SIPR3, (sock->gateway.b3 & sock->netmask.b3) | (sock->ID & !sock->netmask.b3));

	write(sock->base + S_TOS, 1);
	write(sock->base + S_TTL, 0x80);
  
	write(RMSR, 0x03);
	
	write(TMSR, 0x03);
}