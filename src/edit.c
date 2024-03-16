#include "edit.h"

#define BUF_SIZE 1024 

int run_editor(char* fname) {
  // Initialize stuff
  World* world = world_init();
  Buffer* buf = buffer_init(fname);
  world->cur_buffer = buf;
  world->screen->inputwin->top_line = buf->point->line;
  Window* inputwin = world->screen->inputwin;
  Window* infowin = world->screen->infowin;

  // TODO:
  // Separate everything into functions and files
  // Names are so long
  // Info panel
  // For some reason has extra line that vim and emacs don't
  // 
  // Add rpg

  screen_print();

  int exit = 0;
  int cur = 0;

  // Main loop
  while (!exit) {
    cur = wgetch(inputwin->win);

    switch (cur) {
      case 3: // End of text
        exit = 1;
        break;
      case 19: // Ctrl-s (save)
        write_file(buf);
        break;
      case '\n': // Carriage return
        new_line();
        break;
      case KEY_LEFT: // Left arrow
        // Beginning of text
        if (buf->point->col == 0 &&
            buf->point->line->prev == NULL) {
          break;
        // Beginning of line underflows to previous
        } else if (buf->point->col == 0) {
          buf->point->line = buf->point->line->prev;
          buf->point->col = buf->point->line->end;
          buf->cur_line--;
        // Move left
        } else {
          buf->point->col--;
        }
        break;
      case KEY_RIGHT: // Right arrow
        // End of text
        if (buf->point->col + 1 > buf->point->line->end &&
            buf->point->line->next == NULL) {
          break;
        // End of line overflows to next
        } else if (buf->point->col == buf->point->line->end) {
          buf->point->line = buf->point->line->next;
          buf->point->col = 0;
          buf->cur_line++;
        // Move right
        } else {
          buf->point->col++;
        }
        break;
      case KEY_UP: // Up arrow
        // Top of text
        if (buf->point->line->prev == NULL) {
          break;
        // Move up
        } else {
          buf->point->line = buf->point->line->prev;
          buf->cur_line--;
        }
        break;
      case KEY_DOWN: // Down arrow
        // Bottom of text
        if (buf->point->line->next == NULL) {
          break;
        // Move down
        } else {
          buf->point->line = buf->point->line->next;
          buf->cur_line++;
        }
        break;
      case KEY_RESIZE: // Window resize
        getmaxyx(stdscr, world->screen->height, world->screen->width);
        inputwin->height = world->screen->height - infowin->height;
        inputwin->width = world->screen->width;
        mvwin(infowin->win, world->screen->height - 1, 0);
        wresize(inputwin->win, inputwin->height, inputwin->width);
        break;
      case 127: // Delete
        // Beginning of text
        if (buf->point->col == 0 && buf->point->line->prev == NULL) {
          wclear(infowin->win);
          mvwprintw(infowin->win, 0, 0, "Nothing to delete");
          wrefresh(infowin->win);
        // Beginnign of line combines with last
        } else if (buf->point->col == 0) {
          delete_line();
        // Delete character
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
      default: // Anything else (typing)
        if (cur >= 32 && cur <= 126) {
          // Resize line if too much text
          if (buf->point->line->end >= buf->point->line->size) {
            buf->point->line->buf = (char*)
              realloc(buf->point->line->buf, buf->point->line->size * 2);
            buf->point->line->size *= 2;
            break;
          }
          // Write characters
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
    // Point cant be past end of text
    if (buf->point->col > buf->point->line->end) {
      buf->point->col = buf->point->line->end;
    }
    screen_print();
  }
  // Free stuff
  screen_destroy(world->screen);
  buffer_destroy(buf);
  free(world);
  // world destroy
  return 0;
}
