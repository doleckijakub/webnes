#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "inttypes.h"

void controller_init();

void controller_set_key_state(uint8_t key, bool pressed);
bool controller_get_key_state(uint8_t key);

#endif