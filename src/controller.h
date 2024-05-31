#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "inttypes.h"

typedef enum {
    KEY_A      = 0x80,
    KEY_B      = 0x40,
    KEY_SELECT = 0x20,
    KEY_START  = 0x10,
    KEY_UP     = 0x08,
    KEY_DOWN   = 0x04,
    KEY_LEFT   = 0x02,
    KEY_RIGHT  = 0x01
} Key;

void controller_init();

void controller_set_key_state(uint8_t controller, Key key, bool pressed);

#endif