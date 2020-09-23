/* Fill in your Name and GNumber in the following two comment fields
 * Name: Deep Kumar
 * GNumber: G01215145
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"
#include "hash.h"

// helper function
Symtab *hash_table_generator(int capacity);

/* Function to initialize the symtab
 * In this function, you will create a new Symtab struct.
 * Set all the element inside sytam->table to NULL
 * Return the pointer to the new symtab.
 * On any memory errors, return NULL
 */
Symtab *hash_initialize() {
  /* Implement this function */
  return hash_table_generator(HASH_TABLE_INITIAL);
}

/* Function to destroy the symtab and free all the memory blocks used
 * traverse through all the sybols inside symtab and free it.
 * Return on any memory errors.
 */
void hash_destroy(Symtab *symtab) {
  /* Implement this function */
  if(symtab != NULL && symtab->table != NULL) {
    Symbol **s = symtab->table;
     Symbol *reaper = NULL;
     Symbol *walker = NULL;

    for(int i=0; i<symtab->capacity; i++){
        walker = s[i];
        if(walker != NULL) {
            while(walker != NULL) {
                reaper = walker;
                walker = walker->next;
                free(reaper);
                reaper = NULL;
            }
        }
    }
  }
  free(symtab->table);
  symtab->table = NULL;
  free(symtab);
  symtab = NULL;
  return;
}

/* Function to get the capacity of symtab
 * Return the capacity of the table inside of symtab.
 * If symtab is NULL, return -1;
 */
int hash_get_capacity(Symtab *symtab) {
  /* Implement this function */
  if(symtab == NULL) {
    return -1;
  }
  return symtab->capacity;
}

/* Function to get the size of symtab
 * Return the number of used indexes in the table (size) inside of symtab.
 * If symtab is NULL, return -1;
 */
int hash_get_size(Symtab *symtab) {
  /* Implement this function */
  if(symtab == NULL) {
    return -1;
  }
  return symtab->size;
}

/* Function to add new symbol in hash table
 * works based on variable and value input data
 * performs a check, if the variable is already exists in the table
 * if exists then update the value in table
 * Also check if rehash is required before adding new symbol
 * Create new symbol and add it to hash table
 * Returns -1 if symtab is NULL or any malloc error
 * Returns 0 in case of successful execution
 */
int hash_put(Symtab *symtab, char *var, int val) {
  /* Implement this function */
  if(symtab == NULL || symtab->table == NULL) {
    return -1;
  }

  // update value if variable already exists in hash table
  Symbol *exist = hash_get(symtab, var);
  if(exist != NULL) {
    int index = hash_code(var) % symtab->capacity;
    Symbol *head = (symtab->table)[index];
    while(head != NULL) {
        if(!strcmp(head->variable, var)) {
            head->val = val;
            break;
        }
        head = head->next;
    }
    return 0;
  }

  // check load factor and rehash if required
  double load = symtab->size / symtab->capacity;
  if(load >= 2.0) {
    hash_rehash(symtab, symtab->capacity * 2);
    if(symtab->table == NULL) {
        return -1;
    }
  }

  // get index for new entry
  int index = hash_code(var) % symtab->capacity;
  Symbol *sym = symbol_create(var, val);
  if(sym == NULL) {
    return -1;
  }

  //add new symbol in hash table
  Symbol *head = (symtab->table)[index];
  if(head == NULL) {
    (symtab->table)[index] = sym;
    symtab->size++;
  } else {
        while(head->next != NULL) {
            head = head->next;
        }
        head->next = sym;
        symtab->size++;
  }
  return 0;
}

/* Function to get a symbol if its present in the hash table
 * performs search based on input variable
 * returns copy of symbol from hash table
 * returns NULL is symtab is NULL or variable not found
 */
Symbol *hash_get(Symtab *symtab, char *var) {
  //NULL check
  if(symtab==NULL || symtab->table == NULL){
    return NULL;
  }

  int index = hash_code(var) % symtab->capacity;
  Symbol *head = (symtab->table)[index];

  // loop to find if the variable in hash table
  while(head != NULL) {
    if(!strcmp(head->variable, var)) {
        return symbol_copy(head);
    }
    head = head->next;
  }
  return NULL;
}

/* Function to rehash the existing hast table
 * rehash will be done based on new capacity
 * Create new table array and set all indexes to NULL
 * Rehash all symbols from old symtab to new one
 * Free old array
 * Set symtab->table to NULL in case of memory errors
 *  return immediately if symtab in NULL
 */
void hash_rehash(Symtab *symtab, int new_capacity) {
  // NULL check
  if(symtab == NULL || symtab->table == NULL) {
    return;
  }

  // generate new hash table
  Symtab *new_symtab = hash_table_generator(new_capacity);
  if(new_symtab == NULL) {
    symtab->table = NULL;
    return;
  }

  // loop through each symbol and copy into new hash table
  // also free the old symbols.
  Symbol *reaper = NULL;
  Symbol *walker = NULL;
  for(int i=0; i<symtab->capacity; i++) {
    walker = (symtab->table)[i];
    while(walker != NULL) {
        hash_put(new_symtab, walker->variable, walker->val);
        reaper = walker;
        walker = walker->next;
        free(reaper);
        reaper = NULL;
    }
  }
  //update old symtab with new rehashed symtab properties
  symtab->table = new_symtab->table;
  symtab->size = new_symtab->size;
  symtab->capacity = new_symtab->capacity;
  return;
}

/* Helper function to create and initialize hash table
 * Based on provided capacity in input
 */
Symtab *hash_table_generator(int capacity) {
    Symtab *symtab = malloc(sizeof(Symtab));
    if(symtab == NULL) {
    return NULL;
  }

  symtab->table = malloc(sizeof(Symbol*)*capacity);
  if(symtab->table != NULL) {
    symtab->size = 0;
    symtab->capacity = capacity;

    for(int i=0; i<capacity; i++) {
        symtab->table[i] = NULL;
    }
    return symtab;
  }
    return NULL;
}

/* Implemented for you.
 * Provided function to print the symbol table */
void hash_print_symtab(Symtab *symtab) {
  if(symtab == NULL) {
    return;
  }
  printf("|-----Symbol Table [%d size/%d cap]\n", symtab->size, symtab->capacity);

  int i = 0;
  Symbol *walker = NULL;

  /* Iterate every index, looking for symbols to print */
  for(i = 0; i < symtab->capacity; i++) {
    walker = symtab->table[i];
    /* For each found linked list, print every symbol therein */
    while(walker != NULL) {
      printf("| %10s: %d \n", walker->variable, walker->val);
      walker = walker->next;
    }
  }
  return;
}

/* This function is written for you.
 * This computes the hash function for a String
 */
long hash_code(char *var) {
  long code = 0;
  int i;
  int size = strlen(var);

  for(i = 0; i < size; i++) {
    code = (code + var[i]);
    if(size == 1 || i < (size - 1)) {
      code *= 128;
    }
  }

  return code;
}
