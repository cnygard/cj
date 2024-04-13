#include "world.h"

#include "cj.h"

World* world_init(int options) {
  world = (World*) cjalloc(sizeof(World));

  world->cur_buffer_holder = NULL;
  world->buffer_holder_chain = NULL;

  world->screen = NULL;
  if (options & WO_SCR) {
    world->screen = screen_init();
  }
  return world;
}
