#include "piece_table.h"

#include "cj.h"

#ifdef DEBUG_________DELETE_THIS_STUFF_________A
#define DEBUG_MSG(msg, pt) \
  do { \
    fprintf(stderr, "%s:\n", msg); \
    Piece* debugPiece = pt->first; \
    int debugIX = 0; \
    while (1) { \
      fprintf(stderr, "%d\t%d %d %d\n", \
          debugIX, \
          debugPiece->begin, \
          debugPiece->length, \
          debugPiece->source); \
      if (debugPiece->next != NULL) { \
        debugPiece = debugPiece->next; \
        debugIX++; \
      } else { \
        break; \
      } \
    } \
  } while (0)
#else
#define DEBUG_MSG(msg, pt)
#endif

PT_Buffer* pt_create(char* fname) {
  PT_Buffer* buf = (PT_Buffer*) cjalloc(sizeof(PT_Buffer));
  buf->pt = (Piece_Table*) cjalloc(sizeof(Piece_Table));
  buf->fname = NULL;
  if (fname) {
    buf->fname = fname;
  }
  buf->pt->add = (char*) cjalloc(sizeof(char) * LINE_LENGTH);
  buf->pt->add_end = 0;
  buf->pt->add_size = LINE_LENGTH;
  buf->point = (PT_Location*) cjalloc(sizeof(PT_Location));
  buf->point->loc = 0;
  buf->point->piece = (Piece*) cjalloc(sizeof(Piece));
  buf->pt->first = buf->point->piece;
  buf->pt->num_pieces = 1;
  int load_stat = pt_load_file(buf);
  // If file couldn't load, create an empty buffer
  if (load_stat != 0) {
    buf->pt->original = (char*) cjalloc(sizeof(char) * 0);
    if (buf->pt->original == NULL) {
      fprintf(stderr, "PIECE TABLE CREATE ERROR: PT_Buffer could not be allocated\n");
      return NULL;
    }
    buf->num_chars = 0;
    buf->pt->first->length = 0;
  }
  buf->point->piece->begin = 0;
  buf->point->piece->source = PT_original;
  buf->point->piece->next = NULL;
  buf->point->piece->prev = NULL;
  buf->point->locInPiece = 0;
  return buf;
}

void pt_destroy(void* buf) {
  // WRITE DESTROY FUNCTIONS FOR BUFFERS
}

void pt_add(char in) {
  PT_Buffer* buf = (PT_Buffer*) get_buffer();
  Piece_Table* pt = buf->pt;
  PT_Location* point = (PT_Location*) get_point();

  // Resize add buffer if needed
  if (pt->add_end >= pt->add_size) {
    pt->add = (char*) cjrealloc(pt->add, sizeof(char) * pt->add_size * 2);
    pt->add_size *= 2;
  }

  DEBUG_MSG("before add", pt);

  // WITH THESE FUNCTIONS CAN WRITE TESTS TO COMPARE THIS AND
  // THE OLD BUFFER

  // Beginning of piece
  if (point->locInPiece == 0) {
    // Add piece before
    point->piece->prev = (Piece*) cjalloc(sizeof(Piece));
    point->piece->prev->next = point->piece;
    pt->num_pieces++;
    point->piece = point->piece->prev;
    point->piece->begin = buf->pt->add_end;
    point->piece->length = 1;
    point->piece->source = PT_add;
    point->locInPiece++;
    if (point->piece->next == pt->first) {
      pt->first = point->piece;
    }
  }
  // End of piece
  else if (point->locInPiece == point->piece->length) {
    // Append to piece only if piece points to end of add buffer
    if (point->piece->source == PT_add
        && point->piece->begin + point->piece->length == pt->add_end) {
      point->piece->length++;
      point->locInPiece++;
    }
    // Can't add to middle of add buffer or end of original,
    // so must create new piece
    else {
      Piece* next = point->piece->next;
      point->piece->next = (Piece*) cjalloc(sizeof(Piece));
      point->piece->next->next = next;
      if (next != NULL) {
        next->prev = point->piece->next;
      }
      point->piece->next->prev = point->piece;
      pt->num_pieces++;
      point->piece = point->piece->next;
      point->piece->begin = pt->add_end;
      point->piece->length = 1;
      point->piece->source = PT_add;
      point->locInPiece = 1;
    }
  }
  // Middle of piece
  else {
    Piece* next = point->piece->next;
    point->piece->next = (Piece*) cjalloc(sizeof(Piece));
    point->piece->next->prev = point->piece;
    point->piece->next->next = (Piece*) cjalloc(sizeof(Piece));
    point->piece->next->next->prev = point->piece->next;
    point->piece->next->next->next = next;
    if (next != NULL) {
      next->prev = point->piece->next->next;
    }
    pt->num_pieces += 2;
    // Resize first piece
    int oldLen = point->piece->length;
    point->piece->length = point->locInPiece;
    // Second piece for new content
    point->piece = point->piece->next;
    point->piece->begin = pt->add_end;
    point->piece->length = 1;
    point->piece->source = PT_add;
    // Third piece for rest of old piece
    point->piece->next->begin = point->piece->prev->begin + point->locInPiece;
    point->piece->next->length = oldLen - point->piece->prev->length;
    point->piece->next->source = point->piece->prev->source;
    point->locInPiece = 1;
  }
  pt->add[pt->add_end] = in;
  pt->add_end++;
  buf->num_chars++;
  point->loc++;

  DEBUG_MSG("after add", pt);
}

int pt_del(void) {
  PT_Buffer* buf = (PT_Buffer*) get_buffer();
  Piece_Table* pt = buf->pt;
  PT_Location* point = (PT_Location*) get_point();

  if (point->loc == 0) {
    return 1;
  }
  DEBUG_MSG("before del", pt);

  // Beginning of piece (shouldn't happen)
  if (point->locInPiece == 0) {
    fprintf(stderr, "del at beginning?\n");
  }
  // End of piece
  else if (point->locInPiece == point->piece->length) {
    // Delete piece if it's empty unless it's the only piece
    if (point->piece->length == 1 && pt->num_pieces > 1) {
      Piece* curPiece = point->piece;
      if (curPiece->prev != NULL) {
        curPiece->prev->next = curPiece->next;
      }
      if (curPiece->next != NULL) {
        curPiece->next->prev = curPiece->prev;
      }
      // If at beginning of text, move to the next piece
      if (curPiece->prev == NULL) {
        pt->first = curPiece->next;
        point->piece = curPiece->next;
        point->locInPiece = 0;
      }
      // Otherwise, move to previous piece
      else {
        point->piece = curPiece->prev;
        point->locInPiece = point->piece->length;
      }
      pt->num_pieces--;
      cjfree(curPiece);
    }
    // Shrink piece by one
    else {
      point->piece->length--;
      point->locInPiece--;
    }
  }
  // Middle of piece
  else {
    // Move piece beginning forward if deleting the beginning
    if (point->locInPiece == 1) {
      point->piece->begin++;
      point->piece->length--;
      // Go to last piece unless at beginning of buffer
      if (point->piece->prev != NULL) {
        point->piece = point->piece->prev;
        point->locInPiece = point->piece->length;
      } else {
        point->locInPiece = 0;
      }
    }
    // Split current piece into two and shrink the first
    else {
      Piece* oldNext = point->piece->next;
      Piece* cur = point->piece;
      cur->next = (Piece*) cjalloc(sizeof(Piece));
      if (oldNext != NULL) {
        oldNext->prev = cur->next;
      }
      cur->next->prev = cur;
      cur->next->next = oldNext;
      pt->num_pieces++;
      cur->next->begin = cur->begin + point->locInPiece;
      cur->next->length = cur->length - point->locInPiece;
      cur->next->source = cur->source;
      cur->length = point->locInPiece - 1;
      point->locInPiece--;
    }
  }
  buf->num_chars--;
  point->loc--;
  DEBUG_MSG("after del", pt);
  return 0;
}

int pt_load_file(void* buffer) {
  PT_Buffer* buf = (PT_Buffer*) buffer;
  // Open file
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
  // Setup memory
  fseek(file, 0L, SEEK_END);
  int size = ftell(file);
  fseek(file, 0L, SEEK_SET);
  buf->pt->original = (char*) cjalloc(sizeof(char) * size);
  if (buf->pt->original == NULL) {
    fprintf(stderr, "FILE LOAD ERROR: PT_Buffer could not be allocated\n");
    return 1;
  }
  buf->num_chars = size;
  buf->pt->first->length = size;
  // Read to buffer
  fread(buf->pt->original, sizeof(char), size, file);

  fclose(file);
  return 0;
}

int pt_write_file(void* buffer) {
 PT_Buffer* buf = (PT_Buffer*) buffer;
 // Open file
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
  // Write to file
  Piece* curPiece = buf->pt->first;
  while (1) {
    for (int c = 0; c < curPiece->length; c++) {
      if (curPiece->source == PT_original) {
        fprintf(file, "%c",
            buf->pt->original[curPiece->begin + c]);
      } else {
        fprintf(file, "%c",
            buf->pt->add[curPiece->begin + c]);
      }
    }
    if (curPiece->next != NULL) {
      curPiece = curPiece->next;
    } else {
      break;
    }
  }
  fclose(file);
  return 0;
}

void pt_move_left(void) {
  PT_Location* point = (PT_Location*) get_point();
  // Move location in piece if not at first start of buffer
  if (point->loc > 0) {
    point->locInPiece--;
  }
  // Move overall location
  point->loc = point->loc > 0 ? point->loc-1 : 0;
  // Change pieces
  if (point->locInPiece <= 0 && point->piece->prev != NULL) {
    point->piece = point->piece->prev;
    point->locInPiece = point->piece->length;
  }
}

void pt_move_right(void) {
  PT_Buffer* buf = (PT_Buffer*) get_buffer();
  PT_Location* point = (PT_Location*) get_point();
  // Move location in piece if not at end of buffer
  if (point->loc < buf->num_chars) {
    point->locInPiece++;
  }
  // Move overall location
  point->loc = point->loc < buf->num_chars ? point->loc+1 : buf->num_chars;
  // Change pieces
  if (point->locInPiece > point->piece->length
      && point->loc <= buf->num_chars) {

    // ISNT CHANGIN CURRENT PIECE????

    point->piece = point->piece->next;
    point->locInPiece = 1;
  }

}
