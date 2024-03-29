#include "screen.h"

#include "cj.h"

Screen* screen_init() {
  // Screen init
  initscr();
  raw();
  noecho();
  Screen* screen = (Screen*) malloc(sizeof(Screen));
  screen->width = 0;
  screen->height = 0;
  getmaxyx(stdscr, screen->height, screen->width);

  // Input window init
  screen->inputwin = (Window*) malloc(sizeof(Window));
  screen->inputwin->height = screen->height - 1;
  screen->inputwin->width = screen->width;
  screen->inputwin->win = newwin(screen->height - 1, 0, 0, 0);
  screen->inputwin->top_line = NULL;
  screen->inputwin->top_line_ix = 1;
  keypad(screen->inputwin->win, true);

  // Info window init
  screen->infowin = (Window*) malloc(sizeof(Window));
  screen->infowin->height = 1;
  screen->infowin->width = screen->width;
  screen->infowin->win = newwin(screen->infowin->height, 0,
    screen->inputwin->height, 0);
  screen->infowin->top_line = NULL;
  screen->infowin->top_line_ix = 1;

  return screen;
}

void screen_destroy(Screen* screen) {
  endwin();
  free(screen);
}

int screen_print() {
  Window* win = world->screen->inputwin;
  Window* infowin = world->screen->infowin;
  Buffer* buf = world->cur_buffer;
  
  // Scroll up if point is above window bounds
  if (win->top_line_ix > world->cur_buffer->cur_line) {
    win->top_line = buf->point->line;
    win->top_line_ix = buf->cur_line;
  }
  
  // Scroll down if point is below window bounds
  while (win->top_line_ix + win->height-1 < world->cur_buffer->cur_line) {
    win->top_line = win->top_line->next;
    win->top_line_ix++;
  }

  Line* cur = win->top_line;
  // Line numbers on side
  int num_digits = floor(log10(world->cur_buffer->num_lines+1)+1);

  // Print screen
  wclear(win->win);
  for (int i = 0; i < win->height; i++) {
    if (cur == NULL) {
      break;
    }
    wprintw(win->win, "%*d ", num_digits, win->top_line_ix+i);
    wprintw(win->win, "%s",cur->buf);
    wprintw(win->win, "\n");
    cur = cur->next;
  }
  
  // Info window (needs improvement)
  wclear(infowin->win);
  mvwprintw(infowin->win, 0, 0, "%s", buf->fname);
  mvwprintw(infowin->win, 0, 25, "end:%d top:%d",
    buf->point->line->end,
    world->screen->inputwin->top_line_ix);
  mvwprintw(infowin->win, 0, 40, "h:%d",
    world->screen->inputwin->height);
  mvwprintw(infowin->win, 0, 46, "size:%d",
    buf->point->line->size);
  mvwprintw(infowin->win, 0, 60, "%c %d", 
    buf->point->line->buf[buf->point->col],
    buf->point->line->buf[buf->point->col]);
  mvwprintw(infowin->win, 0, 70, "lc(%d,%d)",
    buf->cur_line,
    buf->point->col);
  wrefresh(infowin->win);

  // Move cursor to correct spot in window
  wmove(win->win,
    world->cur_buffer->cur_line - win->top_line_ix,
    world->cur_buffer->point->col + num_digits + 1);
  wrefresh(win->win);
  return 0;
}
