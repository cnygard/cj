#pragma once

#include <stdlib.h>
#include <memory.h>

#define LINE_LENGTH 128

struct _Line {
  struct _Line* prev;
  struct _Line* next;

  char* buf;
  int end;
  int size;
};
typedef struct _Line Line;

struct _Location {
  Line* line;
  int col;
};
typedef struct _Location Location;

struct _Buffer {
  char* fname;

  Line* first_line;
  Location* point;
  int cur_line;

  int end;
  int num_lines;
  int num_chars;
};
typedef struct _Buffer Buffer;

Buffer* buffer_init(char* fname);
int buffer_destroy(Buffer* buf);

void new_line();
void delete_line();
