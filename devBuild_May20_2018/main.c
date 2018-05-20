/*
 * Main routine
 *
 * Created: 3/30/2018 11:46:42 PM
 * Author : Karats
 */

#include "base_includes.h"
#include "NetWizard/NETWIZARD.h"

void configure() {
	uint8_t gateway[4] = {192,168,1,1};
	uint8_t netmask[4] = {255,255,255,0};
	uint8_t ip[4] = {192,168,1,10};
	uint8_t mac[6] = {'M','E','T','E','R', 0};

	/* Write default configuration */
	net_config_default(gateway, netmask, ip, mac);

	///* Set up client */
	//
	//switch(r = connect_block(sock, &server, 8080)) {
	//case -1: return -2; /* Socket won't ESTABLISH */
	//default: return r; /*Something undefined happened*/
	//}
}

int main(void) {
	int8_t ret = 0;

	configure();

	kill(open_block());
	/* LISTEN for connection */
	kill(listen_block());

	//putline((uint8_t*)"\e[H\e[J\e[1;5;97;105m Welcome! \e[m\r\n", 34);
	//
	//while (1) {
		//if (!(c = getby()))
		//switch (readby(S0 + S_SR)) {
		//case SOCK_CLOSED:
			//kill(-10);
			//opensocket();
			//goto lis;
		//case SOCK_CLOSE_WAIT:
//discon:			putline((uint8_t*)"\e[1;5;97;105m Bye! \e[m\r\n", 24);
			//whistle(0x0c30, 3);
			//whistle(0x0f00, 4);
			//for (c = 0x0ff; c--;);
			//disconnect();
			//goto lis;
		//}
		//else {
			//if (c == 4) goto discon;
			//buffer[i++] = c;
			//if ((i == 80) && (c != '\n')) {
				//buffer[i++] = '\r';
				//buffer[i++] = '\n';
				//goto pr;
			//}
			//if (c == '\n') {
				//buffer[i] = 0;
//pr:				putline(buffer, i);
				//i = 0;
			//}
		//}
	//}
	return 0;
}