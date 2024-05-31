#include "controller.h"

#include "libc.h"

static uint8_t controllers[2];

void controller_init() {
    memset(&controllers, 0, 2);
}

void controller_set_key_state(uint8_t controller, Key key, bool pressed) {
	printfln("%s: controller: %d, key: %d, pressed: %s", __func__, controller, key, pressed ? "true" : "false");
	
	if (pressed) {
		controllers[controller] |= key;
	} else {
		controllers[controller] &= ~key;
	}

	// printfln("controllers[%d] = %d", controller, controllers[controller]);
}
