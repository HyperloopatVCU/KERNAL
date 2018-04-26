/*
 * base_includes.h
 *
 * Created: 3/30/2018 11:47:34 PM
 *  Author: Karats
 */ 

#ifndef BASE_INCLUDES_H_
#define BASE_INCLUDES_H_

#define MASTER

#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>

//#include <HardwareSerial.h>

#define com(X) ({ typeof(X) _ret = X;\
  __asm__ __volatile__ ("com %0":"+r"(_ret):);\
  _ret;})

#define CLR(X,Y) __asm__ __volatile__ ("\
  in  r16, %0\n\
  sbrc  r16, %1\n\
  sbi %0, %1\n\
  "\
: \
: "I"(_SFR_IO_ADDR(X)), "I"(Y)\
: "r16")

#define SET(X,Y) __asm__ __volatile__ ("\
  in  r16, %0\n\
  sbrs  r16, %1\n\
  sbi %0, %1\n\
  "\
: \
: "I"(_SFR_IO_ADDR(X)), "I"(Y)\
: "r16")

void kill(uint8_t);

#endif /* BASE_INCLUDES_H_ */