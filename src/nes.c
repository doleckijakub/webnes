#include "nes.h"

#include "inttypes.h"

void putd(int);

void nes_write(nes *ref, uint16_t address, uint8_t value) {
	ref->ram[address] = value;
}

uint8_t nes_read(nes *ref, uint16_t address) {
	return ref->ram[address];
}

void emulate_nes_rom(void *rom_start, size_t rom_size) {
	putd((int) rom_start);
	putd(rom_size);
}
