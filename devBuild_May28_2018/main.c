/*
 * Main routine
 *
 * Created: 3/30/2018 11:46:42 PM
 * Author : Karats
 */

#include "base_includes.h"
#include "DEVICES/NetWizard/NETWIZARD.h"
#include "DATASERVICE.h"

const uint8_t gateway[4] = {192,168,1,1};

void configure_network() {
	uint8_t netmask[4] = {255,255,255,0};
	uint8_t ip[4] = {192,168,1,10};
	uint8_t mac[6] = {'M','E','T','E','R', 0};

	/* Write default configuration */
	net_config_default(gateway, netmask, ip, mac);
}

int main(void) {
	configure_network();

	kill(open_block());
	
	///* LISTEN for connection */
	//kill(listen_block());
	
	kill(connect_block(gateway, 23));
	
	return 0;
}