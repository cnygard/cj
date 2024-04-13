#pragma once

#include <stdio.h>
// #include <stdlib.h>
#include <unistd.h>
#include <memory.h>

#include "cj.h"
#include "cjalloc.h"
#include "world.h"
#include "buffer.h"
#include "screen.h"
#include "controller.h"

// Needs to be a linked list
// kinda thing that makes it easy to get from line to
// line
int run_editor(Buffer_Option type, char* fname);
