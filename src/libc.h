#ifndef _LIBC_H
#define _LIBC_H

#include "inttypes.h"

void *malloc(size_t);
void free(void*);

void memset(void*, char, size_t);

#pragma clang diagnostic push
#pragma clang agnostic ignored "-Wdeprecated-non-prototype"
void console_log();
#pragma clang diagnostic pop

void puts(char *);
void printfln(const char *, ...);

#endif
