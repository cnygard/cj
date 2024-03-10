#include "file.h"

#include "buffer.h"
#include <cj.h>

Buffer* buffer_init(char* fname) {
  Buffer* buf = (Buffer*) malloc(sizeof(Buffer));
  buf->first_line = (Line*) malloc(sizeof(Line));
  buf->first_line->prev = NULL;
  buf->first_line->next = NULL;
  buf->first_line->buf = (char*) malloc(sizeof(char) * LINE_LENGTH);
  buf->first_line->end = 0;
  buf->point = (Location*) malloc(sizeof(Location));
  buf->point->line = buf->first_line;
  buf->point->col = 0;
  buf->fname = NULL;
  if (fname != NULL) {
    buf->fname = fname;
  }
  buf->cur_line = 1;
  buf->end = 0;
  buf->num_lines = 0;
  buf->num_chars = 0;
  load_file(buf);

  return buf;
}

int buffer_destroy(Buffer* buf) {
  Line* cur = buf->first_line;
  while (cur != NULL) {
    free(cur->buf);
    cur = cur->next;
  }
  free(buf->point);
  free(buf);

  return 0;
}
