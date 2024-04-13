#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "../src/cjalloc.h"
#include "../src/cj.h"
#include "../src/buffer.h"
#include "../src/world.h"

void test_buffer(Buffer_Option type, char* fname, int numiters, int seed) {
  clock_t start, end;
  double total_time = 0.0;

  srand(seed);

  int isScreen = 0;
  
  start = clock();
  World* world = world_init(isScreen);
  Buffer_Holder* buffer = buffer_create(type, fname);
  world->cur_buffer_holder = buffer;
  end = clock();
  double setup_time = ((double)(end - start)) / CLOCKS_PER_SEC;

  int num_chars = 0;
  
  PT_Buffer* pt = NULL;
  Line_Table* lt = NULL;
  switch (type) {
    case BO_PT:
      pt = (PT_Buffer*) get_buffer();
      num_chars = pt->num_chars;
      break;
    case BO_LT:
      lt = (Line_Table*) get_buffer();
      num_chars = lt->num_chars;
      break;
    default:
      break;
  }

  if (isScreen) { screen_print(); }
  // sleep(2);
  int curChar = 32;
  for (int i = 0; i < numiters; i++) {
    int k = 0;
    // while (k < 50000000) {k++;}
    int choice = rand() % 10;
    switch (choice) {
      case 0:
        choice = rand();
        if (choice % 2 == 0) {
          for (int i = 0; i < choice % 100; i++) {
            start = clock();
            buffer->move_left();
            end = clock();
            total_time += ((double)(end - start)) / CLOCKS_PER_SEC;
          }
        } else {
          for (int i = 0; i < choice % 100; i++) {
            start = clock();
            buffer->move_right();
            end = clock();
            total_time += ((double)(end - start)) / CLOCKS_PER_SEC;
          }
        }
        break;
      case 1: // add new line
        start = clock();
        buffer->add('\n');
        end = clock();
        total_time += ((double)(end - start)) / CLOCKS_PER_SEC;
        num_chars++;
        break;
      case 2: // del char
        start = clock();
        int success = buffer->del();
        end = clock();
        total_time += ((double)(end - start)) / CLOCKS_PER_SEC;
        if (success == 0) {
          num_chars--;
        }
        break;
      default: // add char
        start = clock();
        buffer->add(curChar);
        end = clock();
        total_time += ((double)(end - start)) / CLOCKS_PER_SEC;
        curChar++;
        if (curChar >= 127) {
          curChar = 32;
        }
        num_chars++;
        break;
    }
    if (isScreen) { screen_print(); }
  }

  start = clock();
  buffer->write_file(buffer->buffer);
  end = clock();
  double save_time = ((double)(end - start)) / CLOCKS_PER_SEC;
  // sleep(3);

  // Free stuff
  if (isScreen) { screen_destroy(world->screen); }
  // buffer_destroy(buf);
  free(world);

  printf("---------------------------------------------------------------\n");
  printf("%s\n", fname);
  // switch (type) {
  //   case BO_PT:
  //     pt = (PT_Buffer*) get_buffer();
  //     printf("buf numchars:%d\n", pt->num_chars);
  //     break;
  //   case BO_LT:
  //     lt = (Line_Table*) get_buffer();
  //     printf("buf numchars:%d\n", lt->num_chars);
  //     break;
  //   default:
  //     break;
  // }
  printf("counted numchars: %d\n", num_chars);
  printf("setup time: %f\n", setup_time);
  printf("save time: %f\n", save_time);
  printf("Total time taken for %d random inputs: %f seconds\n",
         numiters, total_time);
  print_mem_usage(stdout, "Total memory: ");
  printf("---------------------------------------------------------------\n");
}

int main(void) {
  int numiters = 1000000;
  int seed = time(NULL);
  seed = 5;

  char* pt_fname = "test/test_pt.txt";
  char* lt_fname = "test/test_lt.txt";
  test_buffer(BO_PT, pt_fname, numiters, seed);
  test_buffer(BO_LT, lt_fname, numiters, seed);

  return 0;
}
