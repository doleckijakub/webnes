#include "r6502.h"

#include "libc.h"

#include "ram.h"

static uint8_t a, x, y;
static uint8_t sp, st;
static uint16_t pc;
static uint8_t fetched, opcode, cycles;
static uint16_t temp, abs_addr, rel_addr, clock_count;

void r6502_init() {
	r6502_reset();
}

void r6502_reset() {
	abs_addr = 0xFFFC;
	pc = ram_read_u16(abs_addr);

	a = 0;
	x = 0;
	y = 0;
	sp = 0;
	st = R6502_F_U;

	rel_addr = 0;
	abs_addr = 0;
	fetched = 0;

	cycles = 8;
}

void r6502_irq() {
	printfln("%s@%s:%d unimplemented", __func__, __FILE__, __LINE__);
}

void r6502_nmi() {
	printfln("%s@%s:%d unimplemented", __func__, __FILE__, __LINE__);
}

void r6502_clk() {
	printfln("%s@%s:%d unimplemented", __func__, __FILE__, __LINE__);
}

