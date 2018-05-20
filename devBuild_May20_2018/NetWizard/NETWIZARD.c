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

static struct {
	volatile uint8_t A : 1; //answered a call / connected to server
	volatile uint8_t O : 1; //opened
	volatile uint8_t E : 1; //error generated
	volatile uint8_t errno; //error number
} netflags;

static void (*cont)() = NULL;

static void net_tc() {
	if (cont != NULL) cont();
}

/************************************************************************/
/* Socket functions                                                     */
/************************************************************************/

void disconnect() {
	uint8_t msg[1] = {DISCON};
	spi_pushdata(msg, 1);
	spi_process((COM_WRITE), 1, S0 + S_CR, 0);
	spi_exec();
}

void closesocket() {
	uint8_t msg[1] = {CLOSE};
	spi_pushdata(msg, 1);
	spi_process((COM_WRITE), 1, S0 + S_CR, 0);
	spi_exec();
}

//uint8_t connect_block(uint8_t* ip, uint16_t port) {
	//writes(S0 + S_DIPR0, ip, 4);
	//writew(S0 + S_DPORT0, port);
	//writeby(S0 + S_CR, CONNECT);
	//spi_process();
//
	//return establish_block();
//}

static void open();

static void open_continue() {
	uint8_t msg[1] = {CLOSE};
	cont = NULL;
	if (netflags.errno == 0) {
		netflags.E = 1;
		netflags.errno = EOPEN;
	} else if (spi_read(-1) != SOCK_INIT) {
		spi_pushdata(msg, 1);
		spi_process((COM_WRITE), 1, S0 + S_CR, 1);
		cont = open;
		spi_exec();
	} else {
		netflags.O = 1;
	}
}

static void open() {
	uint8_t msg[2] = {OPEN, 0xff};
	if (netflags.O) netflags.O = 0;
	spi_pushdata(msg, 2);
	spi_process((COM_WRITE), 1, S0 + S_CR, 0);
	spi_process((COM_READ), 1, S0 + S_SR, 1);
	cont = open_continue;
	netflags.errno--;
	spi_exec();
}

uint8_t open_block() {
	netflags.errno = 10;
	open();
	while (!netflags.O)
		if (netflags.E) return netflags.errno;
	return 0;
}

static uint8_t listening = 0;

static void establish() {
	uint8_t msg[1] = {0xff};

	switch (spi_read(-1)) {
	case SOCK_LISTEN:
		listening = 0;
		break;
	case SOCK_ESTABLISHED:
		netflags.A = 1;
		goto q;
	case SOCK_CLOSED:
		netflags.errno = ECLOSED;
		goto e;
	case SOCK_CLOSE_WAIT:
		netflags.errno = EDISCON;
	e:	netflags.E = 1;
	q:	cont = NULL;
		return;
	default:
		if (listening) {
			netflags.errno = ELISTEN;
			netflags.E = 1;
		}
	}
	spi_pushdata(msg, 1);
	spi_process((COM_READ), 1, S0 + S_SR, 1);
	spi_exec();
}

static void listen() {
	uint8_t msg[2] = {LISTEN, 0xff};
	listening = 1;
	netflags.errno = 0;
	if (netflags.A) netflags.A = 0;
	spi_pushdata(msg, 2);
	spi_process((COM_WRITE), 1, S0 + S_CR, 0);
	spi_process((COM_READ), 1, S0 + S_SR, 1);
	cont = establish;
	spi_exec();
}

uint8_t listen_block() {
	listen();
	while (!netflags.A)
		if (netflags.E) return netflags.errno;
	return 0;
}

/************************************************************************/
/* Port functions                                                       */
/************************************************************************/

static void putby_continue() {
	uint8_t msg[4] = {netflags.errno, spi_read(-2), spi_read(-1), SEND};
	(*((uint16_t*)(&msg[2])))++;
	spi_pushdata(msg, 3);
	spi_process(COM_WRITE, 1, (uint16_t)(msg[2])-1, 0);
	spi_process(COM_WRITE, 2, S0 + S_TX_WR0, 0);
	spi_process(COM_WRITE, 1, S0 + S_CR, 0);
	spi_exec();
}

void putby(uint8_t c) {
	uint8_t msg[2] = {255, 255};
	spi_pushdata(msg, 2);
	spi_process(COM_READ, 2, S0+S_TX_WR0, 1);
	cont = putby_continue;
	netflags.errno =  c;
	spi_exec();
}

/************************************************************************/
/* Default setup functions                                              */
/************************************************************************/

void net_config_default(uint8_t *gateway, uint8_t *netmask, uint8_t *ip, uint8_t *mac) {
	uint8_t msg[25] = {
		gateway[0], gateway[1], gateway[2], gateway[3],
		netmask[0], netmask[1], netmask[2], netmask[3],
		mac[0], mac[1], mac[2], mac[3], mac[4], mac[5],
		ip[0], ip[1], ip[2], ip[3],
		3,
		3,
		TCP,
		0, 23, //port number
		0x05, 0xb4 //TCP segment size
	};
	spi_init(net_tc);
	spi_pushdata(msg, 25);
	spi_process(COM_WRITE, 18, GWR0, 0);
	spi_process(COM_WRITE, 2, RMSR, 0);
	spi_process(COM_WRITE, 1, S0 + MR, 0);
	spi_process(COM_WRITE, 2, S0 + S_PORT0, 0);
	spi_process(COM_WRITE, 2, S0 + S_MSSR0, 0);
	spi_exec();
}