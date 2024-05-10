#ifndef _LIBC_H
#define _LIBC_H

#include "inttypes.h"

void *malloc(size_t);
void free(void*);
void memset(void*, char, size_t);

#endif
