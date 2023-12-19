#include "nes.h"

void putd(int);

void emulate_nes_rom(void *rom_start, size_t rom_size) {
	putd((int) rom_start + rom_size);
}
