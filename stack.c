/* Fill in your Name and GNumber in the following two comment fields
 * Name: Deep Kumar
 * GNumber: G01215145
 */
#include <stdio.h>
#include <stdlib.h>

#include "node.h"
#include "stack.h"

/* Function to initialize a stack
 * malloc a new stack head and initialize
 * set count to 0 and next pointer to NULL
 * On any malloc errors, return NULL
 * On success return pointer to Stack_head
 */
Stack_head *stack_initialize() {

  Stack_head *new_stack_head = malloc(sizeof(Stack_head));
  if(new_stack_head != NULL) {
    new_stack_head->count = 0;
    new_stack_head->top = NULL;
    return new_stack_head;
  }
  else {
    return NULL;
  }
}

/* Function to destroy a stack.
 * Call token_free() to free all tokens
 * free all the nodes
 * If head is NULL, you can just return.
 */
void stack_destroy(Stack_head *head) {
  /* Implement this function */
  Node *walker = NULL;
  Node *reaper = NULL;

  if(head == NULL) {
    return;
  }
  else {
    // loop through each node and free it
    walker = head->top;
    while(walker != NULL) {
      reaper = walker;
      walker = walker->next;
      token_free(reaper->tok);
      free(reaper);
      reaper = NULL;
    }

    free(head);
    head = NULL;
  }
  return;
}

/* Function to push a new Token on to the Stack.
 * create new node passing a token to it
 * insert this node at the top of your stack
 * increment the size of stack
 * On any malloc errors, return -1.
 * If there are no errors, return 0.
 */
int stack_push(Stack_head *stack, Token *tok) {
  /* Implement this function */
  Node *node = NULL;
  if(stack == NULL) {
    return 0;
  }

  node = node_create(tok);
  if(node != NULL) {
    // when stack is empty
    if(stack_is_empty(stack)) {
      stack->top = node;
      stack->count = 1;
      return 1;
    }
    else {
      // when stack is not empty
      node->next = stack->top;
      stack->top = node;
      stack->count++;
      // Node inserted
      return 1;
    }
  }
  else {
    // Failed to create node
    return 0;
  }
}

/* Function to pop a Token off of the Stack.
 * Remove the top node from the stack and return it
 * free the node containing the token
 * decrement the size of stack
 * If the stack was empty, return NULL.
 */
Token *stack_pop(Stack_head *stack) {
  /* Implement this function */
  Token *tok = NULL;
  Node *reaper = NULL;

  if(stack == NULL || stack_is_empty(stack)) {
    return NULL;
  }
  else {
    tok = stack->top->tok;
    reaper = stack->top;
    stack->top = stack->top->next;
    free(reaper);
    reaper = NULL;
    stack->count--;
    return tok;
  }

  return NULL;
}

/* Function to return the token in the stack node on the top of the stack
 * If the stack is NULL, return NULL.
 * If the stack is empty, return NULL.
 */
Token *stack_peek(Stack_head *stack) {
  /* Implement this function */
  if(stack == NULL || stack_is_empty(stack)) {
    return NULL;
  }
  else {
    return stack->top->tok;
  }
}

/* Function to check the empty status of stack
 * If stack is NULL, return -1.
 * Return 1 if the stack is empty or 0 otherwise.
 */
int stack_is_empty(Stack_head *stack) {
  /* Implement this function */
  if(stack == NULL) {
    return -1;
  }
  else if(stack->top == NULL) {
    return 1;
  }
  else {
    return 0;
  }
}

/* These two functions are written for you.
 * It recurses the stack and prints out the tokens in reverse order
 * eg. top->2->4->1->8 will print at Stack: 8 1 4 2
 * eg. stack_push(5) will then print Stack: 8 1 4 2 5
 */

/* This is implemented for you.
 * Recursive print. (Local function)
 * Base Case: node == NULL, return
 * Recursive Case: call print_node(node->next, print_data), then print node.
 */
static void print_node(Node *node) {
  if(node == NULL) {
    return;
  }
  token_print(node->tok);
  print_node(node->next);
  return;
}

/* This is implemented for you.
 * Setup function for the recursive calls.  Starts printing with stack->top
 */
void stack_print(Stack_head *stack) {
  if(stack == NULL) {
    return;
  }
  printf("|-----Program Stack\n");
  printf("| ");
  print_node(stack->top);
  printf("\n");
  return;
}
