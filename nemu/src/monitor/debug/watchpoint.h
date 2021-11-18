#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include <common.h>

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  int addr;
  char str[TOKEN_SIZE];
  word_t old_val;
  word_t new_val;
} WP;

WP* new_wp();
void free_wp(WP *wp);
WP* find_wp(char *args);
void print_wp();
void exec_wp();
bool check_wp();

#endif
