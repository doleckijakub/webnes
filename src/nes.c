#include "nes.h"

#include "libc.h"

nes *nes_create() {
	nes *nes_p = malloc(sizeof(nes));
	nes_p->cpu = r6502_create(nes_p);
	memset(&nes_p->ram[0], 0, sizeof(nes_p->ram));

	return nes_p;
}

void nes_destroy(nes *nes_p) {
	free(nes_p);
}

void nes_write(nes *nes_p, uint16_t address, uint8_t value) {
	nes_p->ram[address] = value;
}

uint8_t nes_read(nes *nes_p, uint16_t address) {
	return nes_p->ram[address];
}
