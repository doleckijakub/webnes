#ifndef _R6502_H
#define _R6502_H

// All technical data taken from the official datasheet at http://archive.6502.org/datasheets/rockwell_r650x_r651x.pdf

#include "inttypes.h"

enum {
	R6502_F_C = 1 << 0,
	R6502_F_Z = 1 << 1,
	R6502_F_I = 1 << 2,
	R6502_F_D = 1 << 3,
	R6502_F_B = 1 << 4,
	R6502_F_U = 1 << 5,
	R6502_F_V = 1 << 6,
	R6502_F_N = 1 << 7
};

typedef struct r6502 {
	uint8_t a, x, y;
	uint8_t sp, st;
	uint16_t pc;
} r6502;

void r6502_reset(r6502*);
void r6502_clock(r6502*);

#endif
