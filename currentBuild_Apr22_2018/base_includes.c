/*
 * base_includes.c
 *
 * Created: 4/12/2018 2:19:02 PM
 *  Author: Karats
 */ 

#include "base_includes.h"
#include <avr/sfr_defs.h>

void kill(uint8_t r) {
	uint32_t x;
	uint8_t y;
	if (r == 0) return;
	r=-r;
	SET(DDRD, DDD1);
	CLR(PORTD, PORTD1);
	while (1) {
		for (y = 0x0a; y; y--)
		for (x = 0x0f0000; x; x--);
		for (y = r; y; y--) {
			SET(PIND, PIND1);
			for (x = 0x0f0000; x; x--);
			CLR(PIND, PIND1);
			for (x = 0x0f0000; x; x--);
		}
	}
}

void nothing() {}

/* //This is for use with CIDR decryption
uint8_t asli(uint8_t* hold, uint8_t* shift) {
	uint8_t val;
	asm volatile(
" \
	cpi	%2, 8\n\
	brsh	se\n\
	mov	r16, %2\n\
	eor	%2, %2\n\
	rjmp	la\n\
se:\n\
	subi	%2, 8\n\
	mov	r16, 8\n\
la:\n\
	asr	%0\n\
	ror	%1\n\
	dec	r16\n\
	brne	la\n\
	mov	r16, %0\n\
	mov	%0, %1\n\
	mov	%1, r0\n\
"
		: "+l"(*hold), "=l"(val), "+d"(*shift)
		:
	);
	return val;
}
*/