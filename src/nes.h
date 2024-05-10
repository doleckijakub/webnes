#ifndef _NES_H
#define _NES_H

#include "inttypes.h"

#include "r6502.h"

typedef struct nes {
	r6502 cpu;
	// TODO: ntsc2C02 ppu;
	// TODO: ricoh2A03 apu;
	uint8_t ram[64 * 1024];
} nes;

nes *nes_create();
void nes_restroy(nes*);

void nes_write(nes*, uint16_t address, uint8_t value);
uint8_t nes_read(nes*, uint16_t address);

void emulate_nes_rom(void *rom_start, size_t rom_size);

#endif
