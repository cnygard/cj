#pragma once

#include <stdlib.h>
#include <ncurses.h>
#include <math.h>

#include "buffer.h"

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

Screen* screen_init();
void screen_destroy(Screen* screen);

int screen_print();
