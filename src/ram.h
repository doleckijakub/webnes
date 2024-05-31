#ifndef RAM_H
#define RAM_H

#include "inttypes.h"

void ram_init();

void ram_write_u8(uint16_t address, uint8_t value);
uint8_t ram_read_u8(uint16_t address);

void ram_write_u16(uint16_t address, uint16_t value);
uint16_t ram_read_u16(uint16_t address);

#endif