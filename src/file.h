#pragma once

#include <stdio.h>

#include "cj.h"
#include "buffer.h"

enum _FileError {
  FILE_NULL = -1,
  FILE_SUCCESS = 0,
  FILE_CANT_OPEN
};
typedef enum _FileError FileError;

int load_file(Buffer* buf);
int write_file(Buffer* buf);
