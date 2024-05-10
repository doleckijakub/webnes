#ifndef _R6502_H
#define _R6502_H

// All technical data taken from the official datasheet at http://archive.6502.org/datasheets/rockwell_r650x_r651x.pdf

#include "inttypes.h"

enum {
	R6502_F_C = 1 << 0, // carry
	R6502_F_Z = 1 << 1, // zero
	R6502_F_I = 1 << 2, // disable interrupts
	R6502_F_D = 1 << 3, // decimal mode
	R6502_F_B = 1 << 4, // break
	R6502_F_U = 1 << 5, // unused
	R6502_F_V = 1 << 6, // overflow
	R6502_F_N = 1 << 7  // negative
};

typedef struct nes nes;
uint8_t nes_read(nes*, uint16_t address);

typedef struct r6502 {
	uint8_t a, x, y;
	uint8_t sp, st;
	uint16_t pc;

	uint8_t fetched, opcode, cycles;
	uint16_t temp, abs_addr, rel_addr, clock_count;

	nes *nes_p;
} r6502;

r6502 r6502_create(nes*);
void r6502_destroy(r6502*);

uint8_t r6502_read(r6502*, uint16_t address);

void r6502_reset(r6502*);
void r6502_irq(r6502*);
void r6502_nmi(r6502*);
void r6502_clock(r6502*);

#endif
