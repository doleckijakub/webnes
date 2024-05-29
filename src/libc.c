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

static void append_char(char *str, size_t size, size_t *index, char c) {
	if (*index < size - 1) {
		str[*index] = c;
	}
	(*index)++;
}

static void vsnprintf(char *str, size_t size, const char *format, va_list args) {
	size_t index = 0;
	const char *fmt = format;

	while (*fmt) {
		if (*fmt == '%') {
			(void) *fmt++;
			switch (*fmt) {
				case 'd': {
					static char d[20];
					int i = 0;

					int n = va_arg(args, int);
					
					if (n < 0) {
						append_char(str, size, &index, '-');
						n = -n;
					}

					do {
						d[i++] = '0' + n % 10;
						n /= 10;
					} while (n > 0);

					for (int j = i - 1; j >= 0; j--) {
						append_char(str, size, &index, d[j]);
					}
				} break;
				case 'p': {
					static char p[20];
					static const char *x = "0123456789abcdef";

					intptr_t n = va_arg(args, intptr_t);

					append_char(str, size, &index, '0');
					append_char(str, size, &index, 'x');

					for (int i = 7; i >= 0; i--) {
						int h = (n >> (i * 4)) & 0xF;
						append_char(str, size, &index, x[h]);
					}
				} break;
				case 's': {
					const char *s = va_arg(args, const char *);
					while (*s) append_char(str, size, &index, *s++);
				} break;
				case '%': {
					append_char(str, size, &index, '%');
				} break;
				default: {
					append_char(str, size, &index, '%');
					append_char(str, size, &index, *fmt);
				} break;
			}
		} else {
			append_char(str, size, &index, *fmt);
		}

		++fmt;
	}

	if (index < size) {
		str[index] = 0;
	} else if (size > 0) {
		str[size - 1] = 0;
	}
}

void printfln(const char *format, ...) {
	static char buf[256];
	va_list args;
	va_start(args, format);
	vsnprintf(buf, 256, format, args);
	va_end(args);
	puts(buf);
}

int rand_n = 1;

int rand() {
	rand_n = rand_n * 1103515245 + 12345;
	return (unsigned int) (rand_n / 65536) % 32768;
}
