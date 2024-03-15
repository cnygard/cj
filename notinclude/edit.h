#pragma once

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>

#include "cj.h"
#include "world.h"
#include "file.h"
#include "buffer.h"
#include "screen.h"

// Needs to be a linked list
// kinda thing that makes it easy to get from line to
// line
int run_editor(char* fname);
