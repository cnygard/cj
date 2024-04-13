#pragma once

// #include <stdlib.h>
#include <memory.h>

#include "cjalloc.h"

#define LINE_LENGTH 128

struct _Line {
  struct _Line* prev;
  struct _Line* next;

  char* buf;
  int end;
  int size;
};
typedef struct _Line Line;

struct _LT_Location {
  Line* line;
  int col;
};
typedef struct _LT_Location LT_Location;

struct _Line_Table {
  char* fname;

  Line* first_line;
  LT_Location* point;
  int cur_line;

  int end;
  int num_lines;
  int num_chars;
};
typedef struct _Line_Table Line_Table;

Line_Table* lt_create(char* fname);
void lt_destroy(void* buf);

void lt_new_line(void);
void lt_delete_line(void);

void lt_add(char in);
int lt_del(void);

void lt_move_left(void);
void lt_move_right(void);
void lt_move_up(void);
void lt_move_down(void);

int lt_load_file(void* buffer);
int lt_write_file(void* buffer);

