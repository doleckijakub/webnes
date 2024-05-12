#ifndef _LIBC_H
#define _LIBC_H

#include "inttypes.h"

void *malloc(size_t);
void free(void*);

void memset(void*, char, size_t);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-non-prototype"

void console_log();
void puts(char *);

#pragma clang diagnostic pop

#endif
