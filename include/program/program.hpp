#ifndef PROGRAM_H
#define PROGRAM_H

#include <stddef.h>

typedef struct {
	unsigned char *data;
	size_t size;
} program_t;

program_t *getProgram(const char *path);

void free_program(program_t *);

#endif
