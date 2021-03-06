#include "program/program.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

program_t *getProgram(const char *path) {
	FILE *fp = fopen(path, "rb");
	if (!fp) {
		perror("Err reading file");
		exit(1);
	}

	fseek(fp, 0, SEEK_END);
	size_t size = ftell(fp);
	rewind(fp);

	printf("Program size: %d bytes\n", size);

	program_t *result = (program_t *)malloc(sizeof(program_t));
	result->data = (unsigned char *) malloc(sizeof(char) * size);
	result->size = size;
	if (result->size != fread(result->data, 1, size, fp)) {
		perror("Err copying file to buffer");
		exit(2);
	}
	fclose(fp);

	return result;
}

void free_program(program_t *program) {
	free(program->data);
	free(program);
}
