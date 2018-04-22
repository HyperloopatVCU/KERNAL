/*
 * Main routine
 *
 * Created: 3/30/2018 11:46:42 PM
 * Author : Karats
 */ 

#include "base_includes.h"
#include "NetWizard/NETWIZARD.h"

#define MASTER

uint8_t set_me_up(socker* sock) {
	uint16_t r;
	quad server = {192,168,1,1};
	
	sock->ID = 0;
	sock->base = S0;
	sock->mode = TCP;
	sock->port = 8080;
	sock->gateway = server;
	sock->netmask = (quad){255,255,255,0};

	/* Write default configuration */
	net_config_default(sock);
	
	/* OPEN socket */
	if (opensocket(sock)) return -1; /* Socket won't OPEN */
	
	#ifdef MASTER
	/* Set up master */
	
	/* LISTEN for connection */
	switch(r = listen_block(sock)) {
		case 0: break;
		case -1: return -2; /* Socket won't ESTABLISH */
		case -2: return -3; /* Socket won't LISTEN */
		default: return r; /*Something undefined happened*/
	}
	#else
	/* Set up client */
	
	switch(r = connect_block(sock, &server, 8080)) {
		case 0: break;
		case -1: return -2; /* Socket won't ESTABLISH */
		default: return r; /*Something undefined happened*/
	}
	#endif
	return 0;
}

int main(void)
{
	//uint16_t size;
	//uint8_t buffer[500] = {0};
	socker socket;
	set_me_up(&socket);
	
	putline(&socket, (uint8_t*)"\e[H\e[J\e[1;5;97;105m Welcome! \e[m\r\n", 34);
	while (1) 
	{
		//size = getline(&socket, buffer, 500);
		//switch (size) {
		//case -1: /* port closed somehow */
			//opensocket(&socket);
			//listen_block(&socket);
			//break;
		//case -2: /* Client wants to disconnect. */
			////putline(&socket, (uint8_t*)"\e[1;5;97;105m Bye! \e[m\r\n", 24);
			//disconnect(&socket);
			//listen_block(&socket);
		//}
		///* Echo line to client */
		//putline(&socket, buffer, size);
    }
}

