#include "controller.h"

#include "libc.h"

static bool keys[8];

void controller_init() {
    memset(&keys, 0, 8);
}

void controller_set_key_state(uint8_t key, bool pressed) {
	keys[key] = pressed;
}

bool controller_get_key_state(uint8_t key) {
	return keys[key];
}