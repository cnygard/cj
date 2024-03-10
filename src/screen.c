#include "screen.h"

#include "cj.h"

Screen* screen_init() {
  initscr();
  raw();
  noecho();
  Screen* screen = (Screen*) malloc(sizeof(Screen));
  screen->width = 0;
  screen->height = 0;
  getmaxyx(stdscr, screen->height, screen->width);

  screen->inputwin = (Window*) malloc(sizeof(Window));
  screen->inputwin->height = screen->height - 1;
  screen->inputwin->width = screen->width;
  screen->inputwin->win = newwin(screen->height - 1, 0, 0, 0);
  screen->inputwin->top_line = NULL;
  screen->inputwin->top_line_ix = 1;
  keypad(screen->inputwin->win, true);

  screen->infowin = (Window*) malloc(sizeof(Window));
  screen->infowin->height = screen->height - 1;
  screen->infowin->width = screen->width;
  screen->infowin->win = newwin(screen->infowin->height, 0,
    screen->inputwin->height, 0);
  screen->infowin->top_line = NULL;
  screen->inputwin->top_line_ix = 1;

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
  
  if (win->top_line_ix > world->cur_buffer->cur_line) {
    win->top_line = buf->point->line;
    win->top_line_ix = buf->cur_line;
  }
  
  while (win->top_line_ix + win->height-1 < world->cur_buffer->cur_line) {
    win->top_line = win->top_line->next;
    win->top_line_ix++;
  }

  Line* cur = win->top_line;
  int num_digits = floor(log10(world->cur_buffer->num_lines+1)+1);

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
  
  wclear(infowin->win);
  mvwprintw(infowin->win, 0, 0, "%s", buf->fname);
  mvwprintw(infowin->win, 0, 25, "%d %d",
    buf->point->line->end,
    world->screen->inputwin->top_line_ix);
  mvwprintw(infowin->win, 0, 46, "%d",
    world->screen->inputwin->height);
  mvwprintw(infowin->win, 0, 53, "%c %d", 
    buf->point->line->buf[buf->point->col],
    buf->point->line->buf[buf->point->col]);
  mvwprintw(infowin->win, 0, 70, "lc(%d,%d)",
    buf->cur_line,
    buf->point->col);
  wrefresh(infowin->win);

  wmove(win->win,
    world->cur_buffer->cur_line - win->top_line_ix,
    world->cur_buffer->point->col + num_digits + 1);
  wrefresh(win->win);
  return 0;
}
