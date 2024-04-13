#include "cjalloc.h"


size_t allmem = 0;
unsigned int sizeIndic = sizeof(unsigned int);
typedef unsigned int IndicType;


#ifdef DEBUG
void* cjalloc(size_t size) {
  allmem += size;
  void* actual = malloc(sizeIndic + size);
  void* usable = actual + sizeIndic;
  ((IndicType*)actual)[0] = (IndicType)size;
  fprintf(stderr, "allocing %zu bytes to %p\n", size, usable);
  return usable;
}

void* cjrealloc(void* ptr, size_t size) {
  void* actual = ptr - sizeIndic;
  allmem -= ((int*)actual)[0];
  allmem += size;
  actual = realloc(actual, sizeIndic + size);
  void* usable = actual + sizeIndic;
  ((IndicType*)actual)[0] = (IndicType)size;
  fprintf(stderr, "reallocing %zu bytes to %p\n", size, usable);
  return usable;
}

void cjfree(void* ptr) {
  void* actual = (ptr - sizeIndic);
  allmem -= ((IndicType*)actual)[0];
  fprintf(stderr, "freeing %d bytes from %p\n", ((IndicType*)actual)[0], ptr);
  free(actual); // THIS BREAKS EVERYRHING
}

void print_mem_usage(FILE* file, char* msg) {
  fprintf(file, "%s%zu\n", msg, allmem);
}

#else
void* cjalloc(size_t size) {
  return malloc(size);
}

void* cjrealloc(void* ptr, size_t size) {
  return realloc(ptr, size);
}

void cjfree(void* ptr) {
  free(ptr);
}

void print_mem_usage(FILE* file, char* msg) {}
#endif
