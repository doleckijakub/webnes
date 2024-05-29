#ifndef _NES_H
#define _NES_H

#include "inttypes.h"

void nes_init();

void nes_write_u8(uint16_t address, uint8_t value);
uint8_t nes_read_u8(uint16_t address);

void nes_write_u16(uint16_t address, uint16_t value);
uint16_t nes_read_u16(uint16_t address);

#endif
