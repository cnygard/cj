#include "world.h"

#include "cj.h"

int world_init() {
  world = (World*) malloc(sizeof(World));

  world->cur_buffer = NULL;
  world->buffer_chain = NULL;

  world->screen = screen_init();
  return 0;
}
