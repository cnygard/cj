#include "screen.h"

#include "cj.h"

Screen* screen_init(void) {
  // Screen init
  initscr();
  raw();
  noecho();
  Screen* screen = (Screen*) cjalloc(sizeof(Screen));
  screen->width = 0;
  screen->height = 0;
  getmaxyx(stdscr, screen->height, screen->width);

  // Input window init
  screen->inputwin = (Window*) cjalloc(sizeof(Window));
  screen->inputwin->height = screen->height - 1;
  screen->inputwin->width = screen->width;
  screen->inputwin->win = newwin(screen->height - 1, 0, 0, 0);
  screen->inputwin->top_line = NULL;
  screen->inputwin->top_line_ix = 1;
  keypad(screen->inputwin->win, true);

  // Info window init
  screen->infowin = (Window*) cjalloc(sizeof(Window));
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
  cjfree(screen);
}

int screen_print(void) {
  Buffer_Holder* buffer = get_buffer_holder();
  switch (buffer->type) {
    case BO_PT:
      return screen_print_pt();
    case BO_LT:
      return screen_print_lt();
    default:
      return 1;
  }
}

int screen_print_pt(void) {
  Window* win = world->screen->inputwin;
  Window* infowin = world->screen->infowin;
  PT_Buffer* buf = (PT_Buffer*) get_buffer();


  Piece* curPiece = buf->pt->first;

  wclear(win->win);
  wmove(win->win, 0, 0);
  while (1) {
    for (int c = 0; c < curPiece->length; c++) {
      if (curPiece->source == PT_original) {
        wprintw(win->win, "%c",
            buf->pt->original[curPiece->begin + c]);
      } else {
        wprintw(win->win, "%c",
            buf->pt->add[curPiece->begin + c]);
      }
    }
    if (curPiece->next != NULL) {
      curPiece = curPiece->next;
    } else {
      break;
    }
  }
  mvwprintw(win->win, 12, 0, "o: %s", buf->pt->original);
  mvwprintw(win->win, 13, 0, "a: %s", buf->pt->add);

  Piece* pointP = buf->point->piece;
  int locInPiece = buf->point->locInPiece;
  if (pointP != NULL
      && buf->point->locInPiece == pointP->length
      && pointP->next != NULL) {
    pointP = pointP->next;
    locInPiece = 0;
  }


  // Info window (needs improvement)
  wclear(infowin->win);
  mvwprintw(infowin->win, 0, 0, "%s", buf->fname);
  mvwprintw(infowin->win, 0, 15, "pl:%d",
    buf->point->piece->length);
    // pointP->length);
  mvwprintw(infowin->win, 0, 22, "ps:%d",
    buf->pt->num_pieces);
  // mvwprintw(infowin->win, 0, 25, "end:%d top:%d",
  //   buf->point->line->end,
  //   world->screen->inputwin->top_line_ix);
  mvwprintw(infowin->win, 0, 28, "h:%d",
    world->screen->inputwin->height);
  // mvwprintw(infowin->win, 0, 34, "%d",
  //   buf->point->piece);
  mvwprintw(infowin->win, 0, 40, "lip:%d",
    buf->point->locInPiece);
  // mvwprintw(infowin->win, 0, 46, "cp:%d",
  //   point);
  mvwprintw(infowin->win, 0, 48, "loc:%d", buf->point->loc);
  if (pointP->source == PT_original) {
    mvwprintw(infowin->win, 0, 55, "%c",
      buf->pt->original[pointP->begin + locInPiece]);
    mvwprintw(infowin->win, 0, 60, "o");
  } else {
    mvwprintw(infowin->win, 0, 55, "%c",
      buf->pt->add[pointP->begin + locInPiece]);
    mvwprintw(infowin->win, 0, 60, "a");
  }
  
  // mvwprintw(infowin->win, 0, 46, "size:%d",
  //   buf->point->line->size);
  // mvwprintw(infowin->win, 0, 60, "%c %d", 
  //   buf->point->line->buf[buf->point->col],
  //   buf->point->line->buf[buf->point->col]);
  mvwprintw(infowin->win, 0, 65, "lc(%d,%d)",
    buf->num_lines,
    buf->point->loc);
  wrefresh(infowin->win);
  wmove(win->win, 0, buf->point->loc);

  wrefresh(win->win);
  return 0;
}

int screen_print_lt(void) {
  Window* win = world->screen->inputwin;
  Window* infowin = world->screen->infowin;
  Line_Table* buf = (Line_Table*) get_buffer();
  if (win->top_line == NULL) {
    win->top_line = buf->point->line;
  }

  // Scroll up if point is above window bounds
  if (win->top_line_ix > buf->cur_line) {
    win->top_line = buf->point->line;
    win->top_line_ix = buf->cur_line;
  }
  
  // Scroll down if point is below window bounds
  while (win->top_line_ix + win->height-1 < buf->cur_line) {
    win->top_line = win->top_line->next;
    win->top_line_ix++;
  }

  Line* cur = win->top_line;
  // Line numbers on side
  int num_digits = floor(log10(buf->num_lines+1)+1);

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
    win->top_line_ix);
  mvwprintw(infowin->win, 0, 40, "h:%d",
    win->height);
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
    buf->cur_line - win->top_line_ix,
    buf->point->col + num_digits + 1);
  wrefresh(win->win);
  return 0;
}

