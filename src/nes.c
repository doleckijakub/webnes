#include "nes.h"

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

void nes_load_rom(void *rom, size_t rom_size) {
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

uint8_t *nes_get_framebuffer() {
	return ntsc2C02_get_framebuffer();
}

static uint8_t _was_key_pressed[2];

static bool was_key_pressed(uint8_t controller, Key key) {
	return _was_key_pressed[controller] & key;
}

void nes_emulate_frame() {
	if (was_key_pressed(0, KEY_A)) {
		do {
			r6502_clk();
		} while (!r6502_complete());

		// ntsc2C02_set_random();
	}

	memset(_was_key_pressed, 0, 2);
}

void nes_set_key_state(uint8_t controller, Key key, bool pressed) {
	if (pressed) _was_key_pressed[controller] |= key;
	controller_set_key_state(controller, key, pressed);
}