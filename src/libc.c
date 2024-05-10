#include "libc.h"

extern char __heap_base;
static char *bump = &__heap_base;

void heap_reset(void) {
	bump = &__heap_base;
}

void *malloc(size_t s) {
	s += 3;
	s /= 4;
	s *= 4;

	void *p = bump;
	bump += s;

	return p;
}

void free(void *data) {
	(void) data;
}

void memset(void *data, char c, size_t sz) {
	for (size_t i = 0; i < sz; i++) {
		((uint8_t*) data)[i] = c;
	}
}
