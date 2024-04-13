#include "cj.h"

#include "piece_table.h"
#include "line_table.h"
#include "buffer.h"

Screen* get_screen(void) {
  return world->screen;
}

Buffer_Holder* get_buffer_holder(void) {
  return world->cur_buffer_holder;
}

void* get_buffer(void) {
  return world->cur_buffer_holder->buffer;
}

void* get_point(void) {
  PT_Buffer* pt = NULL;
  Line_Table* lt = NULL;
  Buffer_Holder* bh = get_buffer_holder();
  switch (bh->type) {
    case BO_PT:
      pt = (PT_Buffer*) get_buffer();
      return pt->point;
    case BO_LT:
      lt = (Line_Table*) get_buffer();
      return lt->point;
    default:
      return NULL;
  }
}

Line* get_line(void) {
  Buffer_Holder* bh = get_buffer_holder();
  if (bh->type != BO_LT) {
    return NULL;
  }
  Line_Table* lt = (Line_Table*) get_buffer();
  return lt->point->line;
}
