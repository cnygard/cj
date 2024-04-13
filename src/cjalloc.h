#pragma once

#include <stdlib.h>
#include <stdio.h>

void* cjalloc(size_t size);
void* cjrealloc(void* ptr, size_t size);
void cjfree(void* ptr);

void print_mem_usage(FILE* file, char* msg);
