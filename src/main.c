#include "cjalloc.h"
#include "edit.h"
#include "buffer.h"

int main(int argc, char** argv) {
  // fprintf(stdout, "Loading...\n");
  // sleep(2);
  char* fname = NULL;
  if (argc > 1) {
    fname = argv[1];
  }
  Buffer_Option type = -1;
  if (argc > 2) {
    if (strcmp("piece", argv[2]) == 0) {
      type = BO_PT; // piece table
    }
    else if (strcmp("line", argv[2]) == 0) {
      type = BO_LT; // line table
    }
  } else {
    type = BO_PT; // piece table
  }
  int tmp = 0;
  // scanf("%d", &tmp);
  int exit = run_editor(type, fname);
  print_mem_usage(stdout, "Leftover memory: ");
  return exit;
}
