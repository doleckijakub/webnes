#include "ram.h"

#include "libc.h"

static uint8_t ram[64 * 1024];

void ram_init() {
    memset(ram, 0, sizeof(ram));
}

void ram_write_u8(uint16_t address, uint8_t value) {
	ram[address] = value;
}

uint8_t ram_read_u8(uint16_t address) {
	return ram[address];
}

void ram_write_u16(uint16_t address, uint16_t value) {
	UNIMPLEMENTED();
}

uint16_t ram_read_u16(uint16_t address) {
	uint16_t lo = ram_read_u8(address);
	uint16_t hi = ram_read_u8(address + 1);
	return (hi << 8) | lo;
}