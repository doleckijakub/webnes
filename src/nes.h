#ifndef _NES_H
#define _NES_H

#include "inttypes.h"
#include "controller.h"

void nes_init();

void nes_load_rom(void *rom, size_t rom_size);
uint8_t *nes_get_framebuffer();
void nes_emulate_frame();
void nes_set_key_state(uint8_t controller, Key key, bool pressed);

#endif
