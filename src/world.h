#pragma once

#include "cjalloc.h"
#include "buffer.h"
#include "screen.h"

#define WO_SCR 1

struct _World {
  Buffer_Holder** buffer_holder_chain;
  Buffer_Holder* cur_buffer_holder;
  
  Screen* screen;
};
typedef struct _World World;

World* world_init(int options);
