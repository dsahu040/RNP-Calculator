/* Do NOT Edit This File */
#include <stdio.h>
#include <stdlib.h>

#include "rpn.h"
#include "stack.h"
#include "hash.h"

/* Main RPN Calculator Program */
int main(int argc, char *argv[]) {
  /* Create a new Stack and Symbol Table */
  Stack_head *stack = stack_initialize();
  Symtab *symtab = hash_initialize();
  /* Set up the filename with the default sample */
  char filename[100] = "sample1.txt";

  /* One argument is allowed, a filename of the file to open */
  if(argc == 2) {
    strncpy(filename, argv[1], 100);
  }

  /* Launch the rpn calculator */
  rpn(stack, symtab, filename);
  /* Clean up the calculator data structures */
  stack_destroy(stack);
  hash_destroy(symtab);
  return 0;
}
