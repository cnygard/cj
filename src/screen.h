#pragma once

#include <stdlib.h>
#include <ncurses.h>
#include <math.h>

#include "cjalloc.h"
#include "piece_table.h"
#include "line_table.h"

struct _Window {
  int height;
  int width;
  WINDOW* win;
  
  Line* top_line;
  int top_line_ix;
};
typedef struct _Window Window;

struct _Screen {
  int height;
  int width;
  Window* inputwin;
  Window* infowin;
};
typedef struct _Screen Screen;

Screen* screen_init(void);
void screen_destroy(Screen* screen);

int screen_print(void);
int screen_print_pt(void);
int screen_print_lt(void);
