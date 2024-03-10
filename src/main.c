#include "edit.h"

int main(int argc, char** argv) {
  // fprintf(stdout, "Loading...\n");
  // sleep(2);
  char* fname = NULL;
  if (argc > 1) {
    fname = argv[1];
  }
  int exit = run_editor(fname);
  return exit;
}
