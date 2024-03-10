#include "file.h"

int load_file(Buffer* buf) {
  if (buf->fname == NULL) {
    fprintf(stderr, "FILE LOAD ERROR: File name is NULL\n");
    return FILE_NULL;
  }

  FILE* file = fopen(buf->fname, "r");
  if (file == NULL) {
    fprintf(stderr, "FILE LOAD ERROR: File '%s' could not be opened\n",
      buf->fname);
    return FILE_CANT_OPEN;
  }

  char cur_char = '\0';
  Line* cur_line = buf->first_line;
  while (fscanf(file, "%c", &cur_char) != EOF) {
    if (cur_char == '\n') {
      cur_line->next = (Line*) malloc(sizeof(Line));
      cur_line->next->buf = (char*) malloc(sizeof(char) * LINE_LENGTH);
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

int write_file(Buffer* buf) {
  if (buf->fname == NULL) {
    fprintf(stderr, "FILE WRITE ERROR: File name is NULL\n");
    return FILE_NULL;
  }
  FILE* file = fopen(buf->fname, "w");
  if (file == NULL) {
    fprintf(stderr, "FILE WRITE ERROR: File '%s' could not be opened\n",
      buf->fname);
    return FILE_CANT_OPEN;
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
