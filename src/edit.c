#include "edit.h"

int run_editor(Buffer_Option type, char* fname) {
  // Initialize stuff
  World* world = world_init(1);
  // Gap_Buffer* buf = buffer_init(fname, type);
  Buffer_Holder* buffer = buffer_create(type, fname);
  world->cur_buffer_holder = buffer;
  // world->screen->inputwin->top_line = buf->point->line;
  Window* inputwin = world->screen->inputwin;

  // TODO:
  // Separate everything into functions and files
  // Names are so long
  // Info panel
  // For some reason has extra line that vim and emacs don't
  // Implement piece table
  // Abstraction
  // 
  // Add rpg

  screen_print();

  int exit = 0;
  int cur = 0;

  // Main loop
  while (!exit) {
    // Handle input
    cur = wgetch(inputwin->win);
    exit = handle_input(cur);
    if (exit) {
      break;
    }

    // // Point cant be past end of text
    // if (buf->point->col > buf->point->line->end) {
    //   buf->point->col = buf->point->line->end;
    // }
    screen_print();
    print_mem_usage(stderr, "Memory in use: ");
  }
  // Free stuff
  screen_destroy(world->screen);
  buffer_destroy(get_buffer_holder());
  cjfree(world);
  // world destroy
  return 0;
}
