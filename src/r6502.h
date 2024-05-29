#ifndef R6502_H
#define R6502_H

// All technical data taken from the official datasheet at http://archive.6502.org/datasheets/rockwell_r650x_r651x.pdf

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

void r6502_init();
void r6502_reset();
void r6502_irq();
void r6502_nmi();
void r6502_clk();

#endif
