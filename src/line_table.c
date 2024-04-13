#include "line_table.h"

#include "cj.h"

Line_Table* lt_create(char* fname) {
  Line_Table* buf = (Line_Table*) cjalloc(sizeof(Line_Table));
  buf->first_line = (Line*) cjalloc(sizeof(Line));
  buf->first_line->prev = NULL;
  buf->first_line->next = NULL;
  buf->first_line->buf = (char*) cjalloc(sizeof(char) * LINE_LENGTH);
  buf->first_line->size = LINE_LENGTH;
  buf->first_line->end = 0;
  buf->point = (LT_Location*) cjalloc(sizeof(LT_Location));
  buf->point->line = buf->first_line;
  buf->point->col = 0;
  buf->fname = NULL;
  if (fname) {
    buf->fname = fname;
  }
  buf->cur_line = 1;
  buf->end = 0;
  buf->num_lines = 0;
  buf->num_chars = 0;
  lt_load_file(buf);
  return buf;
}

void lt_destroy(void* buf) {
  Line_Table* buffer = (Line_Table*) buf;
  Line* cur = buffer->first_line;
  while (cur != NULL) {
    cjfree(cur->buf);
    Line* temp = cur;
    cjfree(temp);
    cur = cur->next;
  }
  cjfree(buffer->point);
  cjfree(buffer);
}

void lt_new_line(void) {
  LT_Location* point = (LT_Location*) get_point();
  Line_Table* buf = (Line_Table*) get_buffer();
  Line* temp = (Line*) cjalloc(sizeof(Line));
  temp->prev = point->line;
  temp->next = point->line->next;
  if (point->line->next != NULL) {
    point->line->next->prev = temp;
  }
  point->line->next = temp;
  point->line = temp;
  point->line->buf = (char*) cjalloc(sizeof(char) * LINE_LENGTH);
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

void lt_delete_line(void) {
  LT_Location* point = (LT_Location*) get_point();
  Line_Table* buf = (Line_Table*) get_buffer();
  Line* line = get_line();
  if (line->end + line->prev->end > line->prev->size) {
    // cjrealloc prev
    line->prev->buf = (char*) cjrealloc(line->prev->buf, line->prev->size * 2);
    if (line->prev->buf == NULL) {
      fprintf(stderr, "LINE TABLE COULD NOT REALLOC\n");
      exit(1);
    }
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
  cjfree(temp->buf);
  cjfree(temp);
  buf->num_chars--;
  buf->num_lines--;
  buf->cur_line--;
}

void lt_add(char in) {
  if (in == '\n') {
    lt_new_line();
    return;
  }
  Line_Table* buf = (Line_Table*) get_buffer();
  // Resize line if too much text
  if (buf->point->line->end >= buf->point->line->size) {
    buf->point->line->buf = (char*)
      cjrealloc(buf->point->line->buf, buf->point->line->size * 2);
    if (buf->point->line->buf == NULL) {
      fprintf(stderr, "LINE TABLE COULD NOT REALLOC\n");
      exit(1);
    }
    buf->point->line->size *= 2;
    return;
  }
  // Write characters
  if (buf->point->col <= buf->point->line->end) {
    memmove(&buf->point->line->buf[buf->point->col + 1],
        &buf->point->line->buf[buf->point->col],
        buf->point->line->end - buf->point->col);
    buf->point->line->buf[buf->point->col] = in;
    buf->point->line->end++;
    buf->point->col++;
    buf->num_chars++;
  }
}

int lt_del(void) {
  Line_Table* buf = (Line_Table*) get_buffer();
  // Beginning of text
  if (buf->point->col == 0 && buf->point->line->prev == NULL) {
    return 1;
  }
  // Beginnign of line combines with last
  else if (buf->point->col == 0) {
    lt_delete_line();
  }
  // Delete character
  else {
    memmove(&buf->point->line->buf[buf->point->col - 1],
        &buf->point->line->buf[buf->point->col],
        buf->point->line->end - buf->point->col);
    buf->point->line->end--;
    buf->point->line->buf[buf->point->line->end] = '\0';
    buf->point->col--;
    buf->num_chars--;
  }
  return 0;
}

void lt_move_left(void) {
  Line_Table* buf = (Line_Table*) get_buffer();
  // Beginning of text
  if (buf->point->col == 0 &&
      buf->point->line->prev == NULL) {
  }
  // Beginning of line underflows to previous
  else if (buf->point->col == 0) {
    buf->point->line = buf->point->line->prev;
    buf->point->col = buf->point->line->end;
    buf->cur_line--;
  }
  // Move left
  else {
    buf->point->col--;
  }
}
void lt_move_right(void) {
  Line_Table* buf = (Line_Table*) get_buffer();
  // End of text
  if (buf->point->col + 1 > buf->point->line->end &&
      buf->point->line->next == NULL) {
  }
  // End of line overflows to next
  else if (buf->point->col == buf->point->line->end) {
    buf->point->line = buf->point->line->next;
    buf->point->col = 0;
    buf->cur_line++;
  }
  // Move right
  else {
    buf->point->col++;
  }
}
void lt_move_up(void) {
  Line_Table* buf = (Line_Table*) get_buffer();
  if (buf->point->line->prev == NULL) {
  }
  // Move up
  else {
    buf->point->line = buf->point->line->prev;
    buf->cur_line--;
  }
}

void lt_move_down(void) {
  Line_Table* buf = (Line_Table*) get_buffer();
  if (buf->point->line->next == NULL) {
  }
  // Move down
  else {
    buf->point->line = buf->point->line->next;
    buf->cur_line++;
  }
}


int lt_load_file(void* buffer) {
  Line_Table* buf = (Line_Table*) buffer;
  if (buf->fname == NULL) {
    fprintf(stderr, "FILE LOAD ERROR: File name is NULL\n");
    return 1;
  }

  FILE* file = fopen(buf->fname, "r");
  if (file == NULL) {
    fprintf(stderr, "FILE LOAD ERROR: File '%s' could not be opened\n",
        buf->fname);
    return 1;
  }

  char cur_char = '\0';
  Line* cur_line = buf->first_line;
  while (fscanf(file, "%c", &cur_char) != EOF) {
    if (cur_line->end >= cur_line->size) {
      cur_line->buf = (char*) cjrealloc(cur_line->buf, cur_line->size * 2);
      if (cur_line->buf == NULL) {
        fprintf(stderr, "LINE TABLE COULD NOT REALLOC\n");
        exit(1);
      }
      cur_line->size *= 2;
    }
    if (cur_char == '\n') {
      cur_line->next = (Line*) cjalloc(sizeof(Line));
      cur_line->next->buf = (char*) cjalloc(sizeof(char) * LINE_LENGTH);
      cur_line->next->size = LINE_LENGTH;
      cur_line->next->end = 0;
      cur_line->next->prev = cur_line;
      cur_line->next->next = NULL;
      cur_line = cur_line->next;
      buf->num_lines++;
    } else {
      cur_line->buf[cur_line->end++] = cur_char;
    }
    buf->num_chars++;
  }
  fclose(file);
  return 0;
}

int lt_write_file(void* buffer) {
  Line_Table* buf = (Line_Table*) buffer;
  if (buf->fname == NULL) {
    fprintf(stderr, "FILE WRITE ERROR: File name is NULL\n");
    return 1;
  }
  FILE* file = fopen(buf->fname, "w");
  if (file == NULL) {
    fprintf(stderr, "FILE WRITE ERROR: File '%s' could not be opened\n",
        buf->fname);
    return 1;
  }

  Line* cur_line = buf->first_line;
  while (cur_line != NULL) {
    for (int i = 0; i < cur_line->end; i++) {
      fprintf(file, "%c", cur_line->buf[i]);
    }
    if (cur_line->next != NULL) {
      fprintf(file, "\n");
    }
    cur_line = cur_line->next;
  }
  fclose(file);
  return 0;
}
