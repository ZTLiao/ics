#include <tools/stack.h>

static PSTACK_FRAME TopFrame;

static PSTACK_FRAME CurrFrame;

static int StackSize = 0;

void push_stack(char* val) {
	PSTACK_FRAME pStackFrame = (PSTACK_FRAME) malloc(sizeof(STACK_FRAME));
	strcpy(pStackFrame->val, val);
	pStackFrame->prev = CurrFrame;
	if (TopFrame == NULL) {
		TopFrame = pStackFrame;
	}
	CurrFrame = pStackFrame;
	StackSize++;
}

void pop_stack(char* val) {
	PSTACK_FRAME pStackFrame = CurrFrame;
	if (pStackFrame == NULL) {
		return;
	}
	if (pStackFrame->prev != NULL) {
		CurrFrame = pStackFrame->prev;
	} else {
		CurrFrame = NULL;
	}
	strcpy(val, pStackFrame->val);
	StackSize--;
	if (StackSize == 0) {
		TopFrame = NULL;
		CurrFrame = NULL;	
	}
	if (pStackFrame != NULL) {
		free(pStackFrame);
	}
}

int get_stack_size() {
	return StackSize;
}
