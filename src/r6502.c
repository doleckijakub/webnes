#include "r6502.h"

r6502 r6502_create(nes *nes_p) {
	return (r6502) {
		.nes_p = nes_p
	};
}

void r6502_destroy(r6502 *cpu) {

}

uint8_t r6502_read(r6502 *cpu, uint16_t address) {
	return nes_read(cpu->nes_p, address);
}

void r6502_reset(r6502 *cpu) {
	cpu->abs_addr = 0xFFFC;
	uint16_t lo = r6502_read(cpu, cpu->abs_addr + 0);
	uint16_t hi = r6502_read(cpu, cpu->abs_addr + 1);
	cpu->pc = (hi << 8) | lo;

	cpu->a = 0;
	cpu->x = 0;
	cpu->y = 0;
	cpu->sp = 0;
	cpu->st = R6502_F_U;

	cpu->rel_addr = 0;
	cpu->abs_addr = 0;
	cpu->fetched = 0;

	cpu->cycles = 8;
}

void r6502_irq(r6502 *cpu) {

}

void r6502_nmi(r6502 *cpu) {

}

void r6502_clock(r6502 *cpu) {

}

