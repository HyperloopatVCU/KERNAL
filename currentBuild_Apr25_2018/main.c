/*
 * Main routine
 *
 * Created: 3/30/2018 11:46:42 PM
 * Author : Karats
 */ 

#include "base_includes.h"
#include "NetWizard/NETWIZARD.h"

uint8_t configure(socker* sock) {
  uint8_t server[4] = {192,168,1,10};
  uint8_t netmask[4] = {255,255,255,0};

  sock->ID = 0;
  sock->mode = TCP;
  sock->port = 23;
  sock->gateway = (uint8_t*)&server;
  sock->netmask = (uint8_t*)&netmask;

  //asm("sei");

  /* Write default configuration */
  net_config_default(sock);
  
  /* OPEN socket */
  if (opensocket()) return -3; /* Socket won't OPEN */
  return 0;
  ///* Set up client */
  //
  //switch(r = connect_block(sock, &server, 8080)) {
    //case -1: return -2; /* Socket won't ESTABLISH */
    //default: return r; /*Something undefined happened*/
  //}
}

int main(void) {
  //uint16_t size;
  //uint8_t buffer[500] = {0};
  uint8_t c;
  socker socket;
  /*
  Serial.begin(1200);
  while (!Serial);
  Serial.write("Starting system config...\n");
  Serial.flush();
  */
  kill(configure(&socket));

  /*
  Serial.write("IP\t");
  Serial.write(String(readby(SIPR0)).c_str()); Serial.write('.');
  Serial.write(String(readby(SIPR1)).c_str()); Serial.write('.');
  Serial.write(String(readby(SIPR2)).c_str()); Serial.write('.');
  Serial.write(String(readby(SIPR3)).c_str()); Serial.write(" : ");
  Serial.write(String(socket.port).c_str()); Serial.write('\n');
  Serial.write("MAC\t");
  Serial.write(String(readby(SHAR0),HEX).c_str()); Serial.write('.');
  Serial.write(String(readby(SHAR1),HEX).c_str()); Serial.write('.');
  Serial.write(String(readby(SHAR2),HEX).c_str()); Serial.write('.');
  Serial.write(String(readby(SHAR3),HEX).c_str()); Serial.write('.');
  Serial.write(String(readby(SHAR4),HEX).c_str()); Serial.write('.');
  Serial.write(String(readby(SHAR5),HEX).c_str()); Serial.write('\n');
  Serial.write("Netmask\t");
  Serial.write(String(readby(SUBR0)).c_str()); Serial.write('.');
  Serial.write(String(readby(SUBR1)).c_str()); Serial.write('.');
  Serial.write(String(readby(SUBR2)).c_str()); Serial.write('.');
  Serial.write(String(readby(SUBR3)).c_str()); Serial.write('\n');
  Serial.write("Gateway\t");
  Serial.write(String(readby(GAR0)).c_str()); Serial.write('.');
  Serial.write(String(readby(GAR1)).c_str()); Serial.write('.');
  Serial.write(String(readby(GAR2)).c_str()); Serial.write('.');
  Serial.write(String(readby(GAR3)).c_str()); Serial.write("\n\n");
  */
  
lis:
  /*
  Serial.write("Listening for connection...\n");
  Serial.flush();
  */

  /* LISTEN for connection */
  kill(listen_block());
  /*
  Serial.write("Client IP\t");
  Serial.write(String(readby(S0 + S_DIPR0)).c_str()); Serial.write('.');
  Serial.write(String(readby(S0 + S_DIPR1)).c_str()); Serial.write('.');
  Serial.write(String(readby(S0 + S_DIPR2)).c_str()); Serial.write('.');
  Serial.write(String(readby(S0 + S_DIPR3)).c_str()); Serial.write(" : ");
  Serial.write(String(readw(S0 + S_PORT0)).c_str()); Serial.write('\n');
  Serial.write("MAC\t");
  Serial.write(String(readby(S0 + S_DHAR0),HEX).c_str()); Serial.write('.');
  Serial.write(String(readby(S0 + S_DHAR1),HEX).c_str()); Serial.write('.');
  Serial.write(String(readby(S0 + S_DHAR2),HEX).c_str()); Serial.write('.');
  Serial.write(String(readby(S0 + S_DHAR3),HEX).c_str()); Serial.write('.');
  Serial.write(String(readby(S0 + S_DHAR4),HEX).c_str()); Serial.write('.');
  Serial.write(String(readby(S0 + S_DHAR5),HEX).c_str()); Serial.write("\n\n");
  Serial.flush();
  */

  putline((uint8_t*)"\e[H\e[J\e[1;5;97;105m Welcome! \e[m\r\n", 34);

  while (1) {
    if (!(c = getby()))
      switch (readby(S0 + S_SR)) {
      case SOCK_CLOSED:
        //Serial.write("Port was forcibly closed.\n");
        //Serial.flush();
        opensocket();
        goto lis;
      case SOCK_CLOSE_WAIT:
discon: //Serial.write("Client wanted to disconnect.\n");
        //Serial.flush();
        putline((uint8_t*)"\e[1;5;97;105m Bye! \e[m\r\n", 24);
        disconnect();
        goto lis;
      }
    else {
      if (c == 4) goto discon;
      //Serial.write(c);
      //Serial.flush();
      putby(c);
    }
  }
  return 0;
}