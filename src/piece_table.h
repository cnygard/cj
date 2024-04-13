#pragma once

#include <stdio.h>

#include "cjalloc.h"

enum _PT_Source {
  PT_original,
  PT_add
};
typedef enum _PT_Source PT_Source;

struct _Piece {
  int begin;
  int length;
  struct _Piece* next;
  struct _Piece* prev;
  PT_Source source;
};
typedef struct _Piece Piece;

struct _Piece_Table {
  char* original;
  char* add;
  Piece* first;
  int num_pieces;

  int add_size;
  int add_end;
};
typedef struct _Piece_Table Piece_Table;

struct _PT_Location {
  int loc;
  Piece* piece;
  int locInPiece;
};
typedef struct _PT_Location PT_Location;

struct _PT_Buffer {
  char* fname;

  Piece_Table* pt;

  int num_chars;
  int num_lines;
  
  PT_Location* point;
};
typedef struct _PT_Buffer PT_Buffer;

PT_Buffer* pt_create(char* fname);
void pt_destroy(void* buf);

void pt_add(char in);
int pt_del(void);

void pt_move_left(void);
void pt_move_right(void);

int pt_load_file(void* buffer);
int pt_write_file(void* buffer);
