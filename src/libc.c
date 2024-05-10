#include "libc.h"

#include <stdarg.h>

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

void snprintf(char *str, size_t size, const char *format, ...) {
	va_list va;
	va_start(va, format);

	int i = 0;

	while (*format) {
		if (*format == '%') {
			switch (*(++format)) {
				case 'd': {
					static char d[12];
					int n = va_arg(va, int);
					int j = 0, negative = 0;

					if (n < 0) {
						negative = 1;
						n = -n;
					}

					do {
						d[j++] = n % 10 + '0';
						n /= 10;
					} while (n);

					if (negative) d[j++] = '-';

					for (int k = i - 1; k >= 0; k--) {
						str[i++] = d[k];
					}
				} break;
				case 's': {

				} break;
				default: {
					str[i++] = *format;
				} break;
			}
		} else {
			str[i++] = *format;
		}

		++format;
	}

	str[i] = 0;

	va_end(va);
}

void printf(const char *format, ...) {
	static char buffer[1024];

	va_list va;
	va_start(va, format);

	snprintf(buffer, sizeof(buffer), format, va);

	va_end(va);

	puts(buffer);
}
