/*
 * base_includes.c
 *
 * Created: 4/12/2018 2:19:02 PM
 *  Author: Karats
 */

#include "base_includes.h"

void whistle(uint16_t freq, uint8_t dur) {
	uint16_t x;
	uint32_t y;
	SET(PORTD, PORTD2);
	SET(DDRD, DDD2);
	for (y = (dur * (0x80000 / (freq + 1))); y--;) {
		PIND ^= (1<<PIND2);
		for (x = freq; x--;);
	}
}

void beep(uint8_t n) {
	uint32_t y;
	for (y = 0x140000; y--;);
	for (; n--;) {
		for (y = 0x100000; y--;);
		whistle(0x0ff0, 2);
	}
}

void beepbeep(uint8_t n) {
	uint32_t y;
	for (y = 0x140000; y--;);
	for (; n--;) {
		for (y = 0x30000; y--;);
		whistle(0x0ff0, 1);
	}
}

void kill(uint8_t r) {
	uint32_t x;
	if (r == 0) return;
	cli();
	for (x = 10; x--;) {
		whistle(0x0f00, 1);
		whistle(0x0f40, 1);
	}
	whistle(0x0600, 30);
	r = -r;
	while (1) {
		for (x = 0x400000; x; x--);
		beep(r);
	}
}

#define G5 0x09f8
#define f5 0x0a8a
#define F5 0x0b30
#define E5 0x0bd7
#define d5 0x0c88
#define D5 0x0d4a
#define c5 0x0e14
#define C5 0x0ef0
#define B4 0x0fd0
#define a4 0x10c0
#define A4 0x11c0
#define g4 0x12d0
#define G4 0x13ea
#define f4 0x1519
#define F4 0x1660
#define E4 0x17b0
#define d4 0x1916
#define D4 0x1a98
#define c4 0x1c2e
#define C4 0x1dd5

void marchofdoom() {
	uint32_t x;
	whistle(G4, 4);
	for(x = 0x20000; x--;);
	whistle(G4, 4);
	for(x = 0x20000; x--;);
	whistle(G4, 4);
	for(x = 0x20000; x--;);
	whistle(d4, 3);
	for(x = 0x20000; x--;);
	whistle(a4, 1);
	for(x = 0x20000; x--;);
	whistle(G4, 4);
	for(x = 0x20000; x--;);
	whistle(d4, 3);
	for(x = 0x20000; x--;);
	whistle(a4, 1);
	for(x = 0x20000; x--;);
	whistle(G4, 8);
	for(x = 0x20000; x--;);

	whistle(D5, 4);
	for(x = 0x20000; x--;);
	whistle(D5, 4);
	for(x = 0x20000; x--;);
	whistle(D5, 4);
	for(x = 0x20000; x--;);
	whistle(d5, 3);
	for(x = 0x20000; x--;);
	whistle(a4, 1);
	for(x = 0x20000; x--;);
	whistle(f4, 4);
	for(x = 0x20000; x--;);
	whistle(d4, 3);
	for(x = 0x20000; x--;);
	whistle(a4, 1);
	for(x = 0x20000; x--;);
	whistle(G4, 8);
	for(x = 0x20000; x--;);

	whistle(G5, 4);
	for(x = 0x20000; x--;);
	whistle(G4, 3);
	for(x = 0x20000; x--;);
	whistle(G4, 1);
	for(x = 0x20000; x--;);
	whistle(G5, 4);
	for(x = 0x20000; x--;);
	whistle(f5, 3);
	for(x = 0x20000; x--;);
	whistle(F5, 1);
	for(x = 0x20000; x--;);
	whistle(E5, 1);
	for(x = 0x20000; x--;);
	whistle(d5, 1);
	for(x = 0x20000; x--;);
	whistle(E5, 2);
	for(x = 0x20000; x--;);
	for(x = 0x40000; x--;);
	whistle(g4, 2);
	for(x = 0x20000; x--;);
	whistle(c5, 4);
	for(x = 0x20000; x--;);

	whistle(C5, 3);
	for(x = 0x20000; x--;);
	whistle(B4, 1);
	for(x = 0x20000; x--;);
	whistle(a4, 1);
	for(x = 0x20000; x--;);
	whistle(A4, 1);
	for(x = 0x20000; x--;);
	whistle(a4, 2);
	for(x = 0x20000; x--;);
	for(x = 0x40000; x--;);
	whistle(d4, 2);
	for(x = 0x20000; x--;);
	whistle(f4, 4);
	for(x = 0x20000; x--;);

	whistle(d4, 3);
	for(x = 0x20000; x--;);
	whistle(f4, 1);
	for(x = 0x20000; x--;);
	whistle(a4, 4);
	for(x = 0x20000; x--;);
	whistle(G4, 3);
	for(x = 0x20000; x--;);
	whistle(a4, 1);
	for(x = 0x20000; x--;);
	whistle(D5, 8);
	for(x = 0x20000; x--;);

	whistle(G5, 4);
	for(x = 0x20000; x--;);
	whistle(G4, 3);
	for(x = 0x20000; x--;);
	whistle(G4, 1);
	for(x = 0x20000; x--;);
	whistle(G5, 4);
	for(x = 0x20000; x--;);
	whistle(f5, 3);
	for(x = 0x20000; x--;);
	whistle(F5, 1);
	for(x = 0x20000; x--;);
	whistle(E5, 1);
	for(x = 0x20000; x--;);
	whistle(d5, 1);
	for(x = 0x20000; x--;);
	whistle(E5, 2);
	for(x = 0x20000; x--;);
	for(x = 0x40000; x--;);
	whistle(g4, 2);
	for(x = 0x20000; x--;);
	whistle(c5, 4);
	for(x = 0x20000; x--;);

	whistle(C5, 3);
	for(x = 0x20000; x--;);
	whistle(B4, 1);
	for(x = 0x20000; x--;);
	whistle(a4, 1);
	for(x = 0x20000; x--;);
	whistle(A4, 1);
	for(x = 0x20000; x--;);
	whistle(a4, 2);
	for(x = 0x20000; x--;);
	for(x = 0x40000; x--;);
	whistle(d4, 2);
	for(x = 0x20000; x--;);
	whistle(f4, 4);
	for(x = 0x20000; x--;);

	whistle(d4, 3);
	for(x = 0x20000; x--;);
	whistle(f4, 1);
	for(x = 0x20000; x--;);
	whistle(G4, 4);
	for(x = 0x20000; x--;);
	whistle(d4, 3);
	for(x = 0x20000; x--;);
	whistle(a4, 1);
	for(x = 0x20000; x--;);
	whistle(G4, 8);
	for(x = 0x20000; x--;);
}

/* //This is for use with CIDR decryption
uint8_t asli(uint8_t* hold, uint8_t* shift) {
uint8_t val;
asm volatile(
" \
cpi %2, 8\n\
brsh  se\n\
mov r16, %2\n\
eor %2, %2\n\
rjmp  la\n\
se:\n\
subi  %2, 8\n\
mov r16, 8\n\
la:\n\
asr %0\n\
ror %1\n\
dec r16\n\
brne  la\n\
mov r16, %0\n\
mov %0, %1\n\
mov %1, r0\n\
"
: "+l"(*hold), "=l"(val), "+d"(*shift)
:
);
return val;
}
*/