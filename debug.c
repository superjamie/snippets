#include <stdio.h>
#include <string.h>

// a couple of different ways to do debugging
// one with a precompiler macro at compile-time, requires recompilation to remove debugging
// one with a commandline switch at runtime, running the program like "./binaryname --debug"

#define DEBUG // do we want debugging? if not, comment this define out

#ifdef DEBUG  // if debugging is enabled
# define DEBUG_PRINT(x) printf x  // make DEBUG_PRINT do a printf
#else
# define DEBUG_PRINT(x) do {} while (0)  // otherwise make DEBUG_PRINT do nothing
#endif

// use this form of debugging like:
// DEBUG_PRINT(("Debugging here: %d\n", integer));

int main(int argc, char *argv[]) {

  int i = 0, debug_print = 0;  // initialise variables

  for (i=0; i < argc; i++) {  // run thru all commandline arguments
    if (strcmp("--debug", argv[i]) == 0 ) {  // if there is an argument "--debug"
      debug_print = 1;  // set this variable to 1. we'll truth check against this later
    }
  }

  printf("First standard line.\n");  // this prints every time the program runs
  if (debug_print) { printf("Debug line with --debug at runtime.\n"); }  // here's our runtime debug print
  DEBUG_PRINT(("Debug line with DEBUG at compile time.\n"));  // heres our macro debug print
  printf("Second standard line.\n");  // this prints every time the program runs

  return 0;
}
