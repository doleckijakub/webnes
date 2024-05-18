#include "libc.h"
#include "nes.h"

#define FRAMEBUFFER_WIDTH 256
#define FRAMEBUFFER_HEIGHT 240
#define FRAMEBUFFER_SIZE (FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT * 4)

uint8_t framebuffer[FRAMEBUFFER_SIZE];
nes *nes_p;

void init_nes() {
	printfln(__func__);
	nes_p = nes_create();
	printfln("nes_p: %p", nes_p);
}

void load_nes_rom(void *rom, size_t rom_size) {
	printfln(__func__);

	size_t offset = 0x8000;
	for (size_t i = 0; i < rom_size; i++) {
		nes_p->ram[offset++] = ((uint8_t*) rom)[i];
	}

	nes_p->ram[0xFFFC] = 0x80;
	nes_p->ram[0xFFFD] = 0x80;

	r6502_reset(&nes_p->cpu);
}

uint8_t *get_nes_framebuffer() {
	return framebuffer;
}

void emulate_nes_frame() {
	memset(framebuffer, 0xFF, FRAMEBUFFER_SIZE);
}

void set_nes_key_state(uint8_t key, bool pressed) {
	nes_p->keys[key] = pressed;
}
