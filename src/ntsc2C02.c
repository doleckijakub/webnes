#include "ntsc2C02.h"

#include "libc.h"

static uint8_t framebuffer[FRAMEBUFFER_SIZE];

void ntsc2C02_init() {
	memset(framebuffer, 0xFF, FRAMEBUFFER_SIZE);
}

uint8_t *ntsc2C02_get_framebuffer() {
	return framebuffer;
}

void ntsc2C02_set_random() {
	for (int i = 0; i < FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT; i++) {
		framebuffer[4 * i + 0] = rand() % 256;
		framebuffer[4 * i + 1] = rand() % 256;
		framebuffer[4 * i + 2] = rand() % 256;
		framebuffer[4 * i + 3] = 0xFF;
	}
}
