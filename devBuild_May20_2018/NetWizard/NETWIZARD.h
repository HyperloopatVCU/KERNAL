/*
 * Network device initialization and functional wizard
 *  GLOBAL DEFINITIONS
 * Created: 3/30/2018 11:43:43 PM
 *  Author: Karats
 */

#ifndef NETWIZARD_H_
#define NETWIZARD_H_

#include "../base_includes.h"
#include "../SPIWizard/SPIWIZARD.h"

/* SPI OP-Codes */
#define COM_WRITE 0x0f0
#define COM_READ  0x0f

/* Addresses */
#define TX_BASE   0x4000
#define TX_SIZE   0x2000
#define RX_BASE   0x6000
#define RX_SIZE   0x2000
#define TOP   0x8000

/* important : http://www.iana.org/assignments/protocol-numbers/protocol-numbers.xhtml */

/* Constants */
/*mode*/
#define RST   0x80
#define PB    0x10
#define PPPOE_MR  0x08
#define AI    0x02
#define IND   0x01
/*port*/
#define CLOSED    0
#define TCP   1
#define UDP   2
#define RAW_IP    3
#define RAW_MAC   4
#define PPPOE_PORT  5
#define NO_DELAY_ACK  32
#define MULTICAST_1 160
#define MULTICAST_2 128

/*command*/
#define OPEN    1 /*open*/
#define LISTEN    2 /*listen*/
#define CONNECT   4 /*connect*/
#define DISCON    8 /*disconnect*/
#define CLOSE   16  /*close*/
#define SEND    32  /*send*/
#define SEND_MAC  33  /*(UDP)*/
#define SEND_KEEP 34  /*(TCP)*/
#define RECV    64  /*receive*/

/*status*/
#define SOCK_CLOSED 0
#define SOCK_INIT 0x13
#define SOCK_LISTEN 0x14
#define SOCK_ESTABLISHED 0x17
#define SOCK_CLOSE_WAIT 0x1c
#define SOCK_UDP  0x22
#define SOCK_IPRAW  0x32
#define SOCK_MACRAW 0x42
#define SOCK_PPPOE  0x5f

#define SOCK_SYNSENT  0x15
#define SOCK_SYNRECV  0x16
#define SOCK_FIN_WAIT 0x18
#define SOCK_CLOSING  0x1a
#define SOCK_TIME_WAIT  0x1b
#define SOCK_LAST_ACK 0x1d
#define SOCK_ARP_1  0x11
#define SOCK_ARP_2  0x21
#define SOCK_ARP_3  0x31

/* Common registers */
#define MR    0x0 /*mode*/
#define GWR0    0x1 /*gateway*/
#define GWR1    0x2
#define GWR2    0x3
#define GWR3    0x4
#define SUBR0   0x5 /*subnet mask*/
#define SUBR1   0x6
#define SUBR2   0x7
#define SUBR3   0x8
#define SHAR0   0x9 /*source hardware*/
#define SHAR1   0x0a
#define SHAR2   0x0b
#define SHAR3   0x0c
#define SHAR4   0x0d
#define SHAR5   0x0e
#define SIPR0   0x0f  /*source ip*/
#define SIPR1   0x10
#define SIPR2   0x11
#define SIPR3   0x12

#define IR    0x15  /*interrupt*/
#define IMR   0x16  /*interrupt mask*/
#define RTR0    0x17  /*retry time*/
#define RTR1    0x18
#define RCR   0x19  /*retry counter*/
#define RMSR    0x1a  /*rx mem size*/
#define TMSR    0x1b  /*tx mem size*/
#define PATR0   0x1c  /*auth type in pppoe*/
#define PATR1   0x1d

#define PTIMER    0x28  /*ppp lcp req timer*/
#define PMAGIC    0x29  /*ppp lcp magic num*/
#define UIPR0   0x2a  /*unreachable ip*/
#define UIPR1   0x2b
#define UIPR2   0x2c
#define UIPR3   0x2d
#define UPORT0    0x2e  /*unreachable port*/
#define UPORT1    0x2f

/* Socket registers */
#define S_MR    0x00  /*socket mode*/
#define S_CR    0x01  /*socket command*/
#define S_SR    0x03  /*socket status*/
#define S_PORT0   0x04  /*socket source port*/
#define S_PORT1   0x05
#define S_DHAR0   0x06  /*socket dest hardware*/
#define S_DHAR1   0x07
#define S_DHAR2   0x08
#define S_DHAR3   0x09
#define S_DHAR4   0x0a
#define S_DHAR5   0x0b
#define S_DIPR0   0x0c  /*socket dest ip*/
#define S_DIPR1   0x0d
#define S_DIPR2   0x0e
#define S_DIPR3   0x0f
#define S_DPORT0  0x10  /*socket dest port*/
#define S_DPORT1  0x11
#define S_MSSR0   0x12  /*socket max seg size*/
#define S_MSSR1   0x13
#define S_PROTO   0x14  /*socket protocol in ip raw mode*/
#define S_TOS   0x15  /*socket ip tos*/
#define S_TTL   0x16  /*socket ip ttl*/


#define S_TX_WR0  0x24  /*socket tx write ptr*/
#define S_TX_WR1  0x25
#define S_RX_RD0  0x28  /*socket rx read ptr*/
#define S_RX_RD1  0x29
/*read-only*/
#define S_IR_RO   0x02  /*socket interrupt*/

#define S_TX_FSR0_RO  0x20  /*socket tx free size*/
#define S_TX_FSR1_RO  0x21
#define S_TX_RR0_RO 0x22  /*socket tx read ptr*/
#define S_TX_RR1_RO 0x23
#define S_RX_RSR0_RO  0x26  /*socket rx received size*/
#define S_RX_RSR1_RO  0x27

/*IR mask*/
#define S_IR_CON  1
#define S_IR_DISCON 2
#define S_IR_RECV 4
#define S_IR_TIMEOUT  8
#define S_IR_SEND_OK  16

/*SOCKET 0*/
#define S0    0x400

/*SOCKET 1*/
#define S1    0x500

/*SOCKET 2*/
#define S2    0x600

/*SOCKET 3*/
#define S3    0x700

/*FUNCTIONS*/

uint8_t open_block();
uint8_t listen_block();

void putby(uint8_t);

void net_config_default(uint8_t*, uint8_t*, uint8_t*, uint8_t*);

#endif /* NETWIZARD_H_ */