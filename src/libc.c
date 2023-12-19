#include "inttypes.h"

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
