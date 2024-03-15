#include "cj.h"

Screen* get_screen() {
  return world->screen;
}

Buffer* get_buffer() {
  return world->cur_buffer;
}

Location* get_point() {
  return world->cur_buffer->point;
}

Line* get_line() {
  return world->cur_buffer->point->line;
}
