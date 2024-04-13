#include "buffer.h"

Buffer_Holder* buffer_create(Buffer_Option option, char* fname) {
  Buffer_Holder* buf = (Buffer_Holder*) cjalloc(sizeof(Buffer_Holder));
  buf->type = option;
  // Piece table
  if (option == BO_PT) {
    buf->buffer = pt_create(fname);
    buf->add = pt_add;
    buf->del = pt_del;
    buf->move_left = pt_move_left;
    buf->move_right = pt_move_right;
    buf->load_file = pt_load_file;
    buf->write_file = pt_write_file;
    buf->destroy = pt_destroy;
  }
  // Line table
  else if (option == BO_LT) {
    buf->buffer = lt_create(fname);
    buf->add = lt_add;
    buf->del = lt_del;
    buf->move_left = lt_move_left;
    buf->move_right = lt_move_right;
    buf->load_file = lt_load_file;
    buf->write_file = lt_write_file;
    buf->destroy = lt_destroy;
  }
  else {
    
  }
  return buf;
}

void buffer_destroy(Buffer_Holder* buf) {
  buf->destroy(buf);
  cjfree(buf);
}
