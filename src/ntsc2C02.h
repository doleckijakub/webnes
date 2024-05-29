#ifndef NTSC2C02_H
#define NTSC2C02_H

#include "inttypes.h"

#define FRAMEBUFFER_WIDTH 256
#define FRAMEBUFFER_HEIGHT 240
#define FRAMEBUFFER_SIZE (FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT * 4)

uint8_t *ntsc2C02_get_framebuffer();

void ntsc2C02_set_red();

#endif
