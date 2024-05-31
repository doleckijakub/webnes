#include "nes.h"

#include "inttypes.h"
#include "libc.h"

#include "ram.h"
#include "r6502.h"
#include "ntsc2C02.h"
#include "ricoh2A03.h"
#include "controller.h"

void nes_init() {
	ram_init();
	r6502_init(); // cpu
	ntsc2C02_init(); // ppu
	ricoh2A03_init(); // apu
	controller_init();
}

void init_nes() { nes_init(); } // TODO: remove

void load_nes_rom(void *rom, size_t rom_size) {
	printfln(__func__);

	size_t offset = 0x8000;
	for (size_t i = 0; i < rom_size; i++) {
		ram_write_u8(offset++, ((uint8_t*) rom)[i]);
	}

	ram_write_u8(0xFFFC, 0x80);
	ram_write_u8(0xFFFD, 0x80);

	r6502_reset();

	printfln("~%s", __func__);
}

uint8_t *get_nes_framebuffer() {
	return ntsc2C02_get_framebuffer();
}

void emulate_nes_frame() {
	ntsc2C02_set_random();
}

void set_nes_key_state(uint8_t controller, Key key, bool pressed) {
	controller_set_key_state(controller, key, pressed);
}