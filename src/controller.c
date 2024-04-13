#include "controller.h"

int handle_input(int cur) {
  Buffer_Holder* buffer = get_buffer_holder();
  Window* inputwin = world->screen->inputwin;
  Window* infowin = world->screen->infowin;
  void* buf = get_buffer();
  
  int exit = 0;
  switch (cur) {
    case '\t':
      exit = 1;
      break;
    case 3: // End of text
      exit = 1;
      break;
    case 19: // Ctrl-s (save)
      // write_file(buf);
      buffer->write_file(buf);
      break;
    case '\n': // Carriage return
      // new_line();
      buffer->add(cur);
      break;
    case KEY_LEFT: // Left arrow
      buffer->move_left();


      break;
    case KEY_RIGHT: // Right arrow
      buffer->move_right();

      break;
      /*
    case KEY_UP: // Up arrow
      // Top of text
      break;
    case KEY_DOWN: // Down arrow
      // Bottom of text
      break;
      */
    case KEY_RESIZE: // Window resize
      getmaxyx(stdscr, world->screen->height, world->screen->width);
      inputwin->height = world->screen->height - infowin->height;
      inputwin->width = world->screen->width;
      mvwin(infowin->win, world->screen->height - 1, 0);
      wresize(inputwin->win, inputwin->height, inputwin->width);
      break;
    case 127: // Delete
      buffer->del();
    default: // Anything else (typing)
      if (cur >= 32 && cur <= 126) {
        buffer->add(cur);
      }
      break;
  }
  return exit;
}
