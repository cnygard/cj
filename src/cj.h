#pragma once

#include "world.h"

World* world;

Screen* get_screen(void);
Buffer_Holder* get_buffer_holder(void);
void* get_buffer(void);
void* get_point(void);
Line* get_line(void);
