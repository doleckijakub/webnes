#ifndef _LIBC_H
#define _LIBC_H

#include "inttypes.h"

void *malloc(size_t);
void free(void*);

void memset(void*, char, size_t);

void puts(char *);
void wputs(char *);
void eputs(char *);
void tputs(char *);

void printfln(const char *, ...);
void wprintfln(const char *, ...);
void eprintfln(const char *, ...);
void tprintfln(const char *, ...);

void throw(const char *);
void throwf(const char *, ...);

#define UNIMPLEMENTED() eprintfln("%s:%d: unimplemented: %s", __FILE__, __LINE__, __func__)
#define TODO(fmt, ...)  eprintfln("%s:%d: %s: todo: " fmt, __FILE__, __LINE__, __func__, ...)

int rand();

#endif
