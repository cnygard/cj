#include "file.h"

#include "buffer.h"

#include <cj.h>

Buffer* buffer_init(char* fname) {
  Buffer* buf = (Buffer*) malloc(sizeof(Buffer));
  buf->first_line = (Line*) malloc(sizeof(Line));
  buf->first_line->prev = NULL;
  buf->first_line->next = NULL;
  buf->first_line->buf = (char*) malloc(sizeof(char) * LINE_LENGTH);
  buf->first_line->size = LINE_LENGTH;
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

void new_line() {
  Location* point = get_point();
  Buffer* buf = get_buffer();
  Line* temp = (Line*) malloc(sizeof(Line));
  temp->prev = point->line;
  temp->next = point->line->next;
  if (point->line->next != NULL) {
    point->line->next->prev = temp;
  }
  point->line->next = temp;
  point->line = temp;
  point->line->buf = (char*) malloc(sizeof(char) * LINE_LENGTH);
  point->line->size = LINE_LENGTH;
  memmove(point->line->buf,
    point->line->prev->buf + point->col,
    point->line->prev->end - point->col + 1);
  memset(point->line->prev->buf + point->col,
    '\0', point->line->prev->end - point->col);
  point->line->end = point->line->prev->end - point->col;
  point->line->prev->end = point->col;
  point->col = 0;
  buf->num_lines++;
  buf->cur_line++;
  buf->num_chars++;
}

void delete_line() {
  Location* point = get_point();
  Buffer* buf = get_buffer();
  Line* line = get_line();
  if (line->end + line->prev->end > line->prev->size) {
    // realloc prev
    line->prev->buf = (char*) realloc(line->prev->buf, line->prev->size * 2);
    line->prev->size *= 2;
  }
  memmove(point->line->prev->buf + point->line->prev->end,
    point->line->buf,
    point->line->end);
  point->col = point->line->prev->end;
  point->line->prev->end += point->line->end;
  point->line->prev->next = point->line->next;
  if (point->line->next != NULL) {
    point->line->next->prev = point->line->prev;
  }
  Line* temp = point->line;
  point->line = point->line->prev;
  free(temp->buf);
  free(temp);
  buf->num_chars--;
  buf->num_lines--;
  buf->cur_line--;
}
