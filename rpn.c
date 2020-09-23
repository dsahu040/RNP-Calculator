/* Name: Deep Kumar
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "token.h"
#include "hash.h"

/* Local Function Declarations (that you need to finish implementing) */
static int read_file(char *filename, char *line);
static int parse_token(Symtab *symtab, Stack_head *stack, Token *tok);

/* Pre-Written Local Function Declarations */
static void print_header(char *filename, int step);
static void print_step_header(int step);
static void print_step_footer(Symtab *symtab, Stack_head *stack);
static void print_step_output(int val);

/* Helper functions */
void free_memory(Token *tok);
void arithmetic_oper(Symtab *symtab, Stack_head *stack, int oper);
int calculate(int val1, int val2, int oper);

/* Defines the largest line that can be read from a file */
#define MAX_LINE_LEN 255

/* This has been written for you.
 * Main function to run your program (written for you).
 * 1) Opens the file using the passed in filename. (your function)
 * -- If the file is not found (ie. fopen returns NULL), then exit(-1);
 * 2) Reads one line from the file.
 * -- The programs will all only be one line in size.
 * -- That line may be up to MAX_LINE_LEN long.
 * 3) Closes the file.
 * 4) Calls token_read_line(line, strlen(line))
 * -- This parses the line and prepares the tokens to be ready to get.
 * 5) While there are tokens remaining to parse: token_has_next() != 0
 * -- a) Get the next token: token_get_next()
 * 6) Parse the token (your function)
 * 7) Print out all of the relevant information
 */
int rpn(Stack_head *stack, Symtab *symtab, char *filename) {
  int step = 0; /* Used to track the program steps */
  int ret = 0;
  char line[MAX_LINE_LEN];
  Token *tok = NULL;

  /* Complete the read_file function that is defined later in this file. */
  ret = read_file(filename, line);
  if(ret != 0) {
    printf("Error: Cannot Read File %s.  Exiting\n", filename);
    exit(-1);
  }

  /* Pass the line into the tokenizer to initialize that system */
  token_read_line(line, strlen(line));

  /* Prints out the nice program output header */
  print_header(filename, step);

  /* Iterate through all tokens */
  while(token_has_next()) {
    /* Begin the next step of execution and print out the step header */
    step++; /* Begin the next step of execution */
    print_step_header(step);

    /* Get the next token */
    tok = token_get_next();
    /* Complete the implementation of this function later in this file. */
    ret = parse_token(symtab, stack, tok);
    if(ret != 0) {
      printf("Critical Error in Parsing.  Exiting Program!\n");
      exit(-1);
    }

    /* Prints out the end of step information */
    print_step_footer(symtab, stack);
  }

  return 0;
}

/* Local function to open a file or exit.
 * Open filename, read its contents (up to MAX_LINE_LEN) into line, then
 * close the file and return 0.
 * On any file error, return -1.
 */
static int read_file(char *filename, char *line) {
  //open file
  FILE *fp = fopen(filename, "r");
  if(fp == NULL) {
    return -1;
  }

  // fetch data from file
  while(feof(fp)== 0) {
    fgets(line, MAX_LINE_LEN, fp);
  }

  // close file
  fclose(fp);
  return 0;
}

/* Function to parse the Token to implement the rpn calculator features
 * Check the type of token and take decision based on it.
 * perform arithmetic operations
 * assign value to variables
 * push values or variables to the stack
 * print the calculation steps and result
 * If there are any memory errors, return -1.
 * On success, return 0
 */
static int parse_token(Symtab *symtab, Stack_head *stack, Token *tok) {
  /* Implement This Function */
    if(tok == NULL) {
        return -1;
    } else if(tok->type == TYPE_OPERATOR) {
        // for operator type input token
        //call helper method
        arithmetic_oper(symtab, stack, tok->oper);
    } else if(tok->type == TYPE_PRINT) {
        // print result from stack
        Token *result = stack_pop(stack);
        print_step_output(result->value);
    }  else if(tok->type == TYPE_ASSIGNMENT) {
        // pop 2 entries from stack
        Token *tok1 = stack_pop(stack);
        Token *tok2 = stack_pop(stack);

        // get value and variable from stack
        int val = tok1->type == TYPE_VALUE ? tok1->value : tok2->value;
        char *var = tok2->type == TYPE_VARIABLE ? tok2->variable : tok1->variable;

        // put new entry in hash table
        hash_put(symtab, var, val);
        token_free(tok1);
        token_free(tok2);
    } else {
        // push to stack if variable or value token
        stack_push(stack, tok);
  }
  return 0;
}

/* helper function to handle arithmetic operator type token
 * pop 2 tokens from stack
 * check if any of these are variable
 * if variable then get its value from hash_table
 * else fetch the values from tokens
 * call calculate method to perform the arithmetic operation on these values */
void arithmetic_oper(Symtab *symtab, Stack_head *stack, int oper) {
    Token *tok1 = stack_pop(stack);
    Token *tok2 = stack_pop(stack);

    int val1;
    int val2;

    //check if above tokens are variable and fetch value from token
    if(tok1->type == TYPE_VARIABLE && tok2->type == TYPE_VARIABLE) {
        val1 = (hash_get(symtab, tok1->variable))->val;
        val2 = (hash_get(symtab, tok2->variable))->val;
    } else if(tok1->type == TYPE_VARIABLE) {
        val1 = (hash_get(symtab, tok1->variable))->val;
        val2 = tok2->value;
    } else if(tok2->type == TYPE_VARIABLE) {
        val1 = tok1->value;
        val2 = (hash_get(symtab, tok2->variable))->val;
    } else {
        val1 = tok1->value;
        val2 = tok2->value;
    }

    // call helper method to perform arithmetic operation
    int result = calculate(val1, val2, oper);
    Token *val3 = token_create_value(result);
    stack_push(stack, val3);

    free_memory(tok1);
    free_memory(tok2);
}

/* Helper function to perform arithmetic operation on given input values
 * return the integer result.
*/
int calculate(int val1, int val2, int oper) {
    int result;
    switch(oper) {
      case OPERATOR_PLUS:
        result = val2 + val1;
        break;
      case OPERATOR_MINUS:
        result = val2 - val1;
        break;
      case OPERATOR_MULT:
        result = val2 * val1;
        break;
      case OPERATOR_DIV:
        result = val2 / val1;
        break;
      default:
        printf("Invalid operator!");
        break;
    }
    return result;
}

/* Helper function to free tokens */
void free_memory(Token *tok) {
  free(tok);
  tok = NULL;
}

/* This has been written for you.
 * Prints out the main output header
 */
static void print_header(char *filename, int step) {
  printf("######### Beginning Program (%s) ###########\n", filename);
  printf("\n.-------------------\n");
  printf("| Program Step = %2d\n", step);
  token_print_remaining();
  printf("o-------------------\n");
}

/* This has been written for you.
 * Prints out the information at the top of each step
 */
static void print_step_header(int step) {
  printf("\n.-------------------\n");
  printf("| Program Step = %2d\n", step++);
}

/* This has been written for you.
 * Prints out the output value (print token) nicely
 */
static void print_step_output(int val) {
  printf("|-----Program Output\n");
  printf("| %d\n", val);
}

/* This has been written for you.
 * Prints out the information at the bottom of each step
 */
static void print_step_footer(Symtab *symtab, Stack_head *stack) {
  hash_print_symtab(symtab);
  stack_print(stack);
  token_print_remaining();
  printf("o-------------------\n");
}
