#include "nes.h"

#include "inttypes.h"
#include "libc.h"

#include "r6502.h"
#include "ntsc2C02.h"

static uint8_t ram[64 * 1024];
// TODO: ricoh2A03 apu
static bool keys[8];

void nes_init() {
	r6502_init();
	memset(ram, 0, sizeof(ram));
}

// ram

void nes_write_u8(uint16_t address, uint8_t value) {
	ram[address] = value;
}

uint8_t nes_read_u8(uint16_t address) {
	return ram[address];
}

void nes_write_u16(uint16_t address, uint16_t value) {
	printfln("%s@%s:%d unimplemented", __func__, __FILE__, __LINE__);
}

uint16_t nes_read_u16(uint16_t address) {
	uint16_t lo = nes_read_u8(address);
	uint16_t hi = nes_read_u8(address + 1);
	return (hi << 8) | lo;
}

// keys

void nes_set_key_state(uint8_t key, bool pressed) {
	keys[key] = pressed;
}

bool nes_get_key_state(uint8_t key) {
	return keys[key];
}

//

void init_nes() {
	nes_init();
}

void load_nes_rom(void *rom, size_t rom_size) {
	printfln(__func__);

	size_t offset = 0x8000;
	for (size_t i = 0; i < rom_size; i++) {
		nes_write_u8(offset++, ((uint8_t*) rom)[i]);
	}

	printfln("%s: 1", __func__);
	nes_write_u8(0xFFFC, 0x80);
	nes_write_u8(0xFFFD, 0x80);

	printfln("%s: 2", __func__);
	r6502_reset();

	printfln("~%s", __func__);
}

uint8_t *get_nes_framebuffer() {
	return ntsc2C02_get_framebuffer();
}

void emulate_nes_frame() {
	ntsc2C02_set_red();
}
