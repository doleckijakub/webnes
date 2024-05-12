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

void emulate_nes_rom(void *rom, size_t rom_size) {
	puts("Hello, World!");
	puts("test");

	nes *nes_p = nes_create();

	size_t offset = 0x8000;
	for (size_t i = 0; i < rom_size; i++) {
		nes_p->ram[offset++] = ((uint8_t*) rom)[i];
	}

	nes_p->ram[0xFFFC] = 0x80;
	nes_p->ram[0xFFFD] = 0x80;

	r6502_reset(&nes_p->cpu);

	console_log(0, 69, 420);
}
