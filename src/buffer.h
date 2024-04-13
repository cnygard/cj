#pragma once

#include "cjalloc.h"
#include "piece_table.h"
#include "line_table.h"

enum _Buffer_Option {
  BO_PT = 0,
  BO_LT
};
typedef enum _Buffer_Option Buffer_Option;

typedef void (*Add_Func)(char in);
typedef int (*Del_Func)(void);

typedef void (*Move_Left_Func)(void);
typedef void (*Move_Right_Func)(void);

typedef int (*Load_File_Func)(void* buf);
typedef int (*Write_File_Func)(void* buf);

typedef void (*Destroy_Func)(void* buf);

struct _Buffer_Holder {
  Buffer_Option type;
  void* buffer;

  Add_Func add;
  Del_Func del;

  Move_Left_Func move_left;
  Move_Right_Func move_right;

  Load_File_Func load_file;
  Write_File_Func write_file;

  Destroy_Func destroy;
};
typedef struct _Buffer_Holder Buffer_Holder;

Buffer_Holder* buffer_create(Buffer_Option option, char* fname);
void buffer_destroy(Buffer_Holder* buf);

