#pragma once

#include <ncurses.h>

#include "buffer.h"
#include "screen.h"

struct _World {
  Buffer* buffer_chain;
  Buffer* cur_buffer;
  
  Screen* screen;
};
typedef struct _World World;

World* world_init();
