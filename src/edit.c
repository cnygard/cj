#include "edit.h"

#define BUF_SIZE 1024 

int run_editor(char* fname) {
  World* world = world_init();
  Buffer* buf = buffer_init(fname);
  world->cur_buffer = buf;
  world->screen->inputwin->top_line = buf->point->line;
  Window* inputwin = world->screen->inputwin;
  Window* infowin = world->screen->infowin;

  // TODO:
  // Realloc buffer if gets too big
  // Separate everything into functions and files
  // Scrolling needs to be separate from moving point
  // Names are so long
  // Info panel
  // New Line
  // Can't save?
  // scrolling correct but bounds are wrong?
  // 
  // Add rpg

  wmove(inputwin->win, 0, 0);

  mvwprintw(infowin->win, 0, 0, "INFO\n");
  wrefresh(infowin->win);

  screen_print();

  int exit = 0;
  int cur = 0;

  while (!exit) {
    cur = wgetch(inputwin->win);

    switch (cur) {
      case 3: // End of text
        exit = 1;
        break;
      case 19: // Ctrl-s
        wclear(infowin->win);
        mvwprintw(infowin->win, 0, 0, "Saving");
        wrefresh(infowin->win);
        int ret = write_file(buf);
        wclear(infowin->win);
        if (ret == FILE_SUCCESS) {
          mvwprintw(infowin->win, 0, 0, "Saved");
        } else {
          mvwprintw(infowin->win, 0, 0, "Not saved");
        }
        wrefresh(infowin->win);
        break;
      case '\n': // Carriage return
        new_line();
        break;
      case KEY_LEFT:
        if (buf->point->col == 0 &&
            buf->point->line->prev == NULL) {
          break;
        } else if (buf->point->col == 0) {
          buf->point->line = buf->point->line->prev;
          buf->point->col = buf->point->line->end;
          buf->cur_line--;
        } else {
          buf->point->col--;
        }
        break;
      case KEY_RIGHT:
        if (buf->point->col + 1 > buf->point->line->end &&
            buf->point->line->next == NULL) {
          break;
        } else if (buf->point->col == buf->point->line->end) {
          buf->point->line = buf->point->line->next;
          buf->point->col = 0;
          buf->cur_line++;
        } else {
          buf->point->col++;
        }
        break;
      case KEY_UP:
        if (buf->point->line->prev == NULL) {
          break;
        } else {
          buf->point->line = buf->point->line->prev;
          buf->cur_line--;
        }
        break;
      case KEY_DOWN:
        if (buf->point->line->next == NULL) {
          break;
        } else {
          buf->point->line = buf->point->line->next;
          buf->cur_line++;
        }
        break;
      case 127: // Delete
        if (buf->point->col == 0 && buf->point->line->prev == NULL) {
          wclear(infowin->win);
          mvwprintw(infowin->win, 0, 0, "Nothing to delete");
          wrefresh(infowin->win);
        } else if (buf->point->col == 0) {
          delete_line();
        } else {
          memmove(&buf->point->line->buf[buf->point->col - 1],
            &buf->point->line->buf[buf->point->col],
            buf->point->line->end - buf->point->col);
          buf->point->line->end--;
          buf->point->line->buf[buf->point->line->end] = '\0';
          buf->point->col--;
          buf->num_chars--;
        }
        break;
      default:
        if (cur >= 32 && cur <= 126) {
          if (buf->point->line->end >= buf->point->line->size) {
            buf->point->line->buf = (char*)
              realloc(buf->point->line->buf, buf->point->line->size * 2);
            buf->point->line->size *= 2;
            break;
          }
          if (buf->point->col <= buf->point->line->end) {
            memmove(&buf->point->line->buf[buf->point->col + 1],
              &buf->point->line->buf[buf->point->col],
              buf->point->line->end - buf->point->col);
            buf->point->line->buf[buf->point->col] = cur;
            buf->point->line->end++;
            buf->point->col++;
            buf->num_chars++;
          }
        }
        break;
    }
    if (buf->point->col > buf->point->line->end) {
      buf->point->col = buf->point->line->end;
    }
    screen_print();
  }
  screen_destroy(world->screen);
  buffer_destroy(buf);
  // world destroy
  return 0;
}
