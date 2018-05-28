/*
 * base_includes.h
 *
 * Created: 3/30/2018 11:47:34 PM
 *  Author: Karats
 */

#ifndef BASE_INCLUDES_H_
#define BASE_INCLUDES_H_

#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>

/* addresses */
#define H 8
#define L 0

/* errors */
#define ECLOSED -1
#define EDISCON -2
#define EOPEN -3
#define ELISTEN -4
#define EQUEUE -5
#define EACTIVITY -6

/* specialty functions */
#define com(X) ({ typeof(X) _ret = X;\
__asm__ __volatile__ ("com %0":"+r"(_ret):);\
_ret;});

#define CLR(X,Y) __asm__ __volatile__ ("\
	in		__tmp_reg__, %0\n\
	sbrc	__tmp_reg__, %1\n\
	sbi		%0, %1\n\
"\
: \
: "I"(_SFR_IO_ADDR(X)), "I"(Y)\
);

#define SET(X,Y) __asm__ __volatile__ ("\
	in		__tmp_reg__, %0\n\
	sbrs	__tmp_reg__, %1\n\
	sbi		%0, %1\n\
"\
: \
: "I"(_SFR_IO_ADDR(X)), "I"(Y)\
);

/*
#define INC_CARRYOVER(X) __asm__ __volatile__ ("\
	ld		__tmp_reg__, %a0\n\
	inc		__tmp_reg__\n\
	brvc	sk\n\
	st		%a0+, __tmp_reg__\n\
	ld		__tmp_reg__, %a0\n\
	inc		__tmp_reg__\n\
sk:	st		%a0, __tmp_reg__\n\
"\
: \
: "e"(X)\
: "memory");
*/

void whistle(uint16_t, uint8_t);
void beep(uint8_t);
void beepbeep(uint8_t);
void kill(uint8_t);
void marchofdoom();

#endif /* BASE_INCLUDES_H_ */