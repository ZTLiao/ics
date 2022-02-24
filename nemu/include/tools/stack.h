#ifndef __STACK_H__
#define __STACK_H__

#include <stdlib.h>
#include <string.h>

typedef struct StackFrame {
	struct StackFrame *prev;
	char val[32];
} STACK_FRAME, *PSTACK_FRAME;

void push_stack(char*);

void pop_stack(char*);

int get_stack_size();

#endif
