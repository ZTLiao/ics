#include "watchpoint.h"
#include "expr.h"

#include <stdlib.h>

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;
  head = NULL;
  free_ = wp_pool;
}

/* Implement the functionality of watchpoint */


WP* new_wp() {
	Log("new watchpoint.");
	WP* prev = NULL;
	WP* node = head;
	while (node != NULL) {
		prev = node;
		node = node->next;
	}
	if (free_ == NULL) {
		panic("out of memory.");
	}
	WP* pWp = free_;
	free_ = free_->next;
	pWp->next = NULL;
	if (head == NULL) {
		head = pWp;
	} else {
		prev->next = pWp;
	}
	return pWp;
}

void free_wp(WP* wp) {
	Log("free watchpoint.");
	if (wp == NULL || head == NULL) {
		return;
	}
	wp->str[0] = '\0';
	wp->old_val = 0;
	wp->new_val = 0;
	if (!wp->NO) {
		head = wp->next;
	} else {
		WP* prev;
		WP* node = head;
		for (prev = NULL; node != NULL && node->NO != wp->NO; node = node->next) {
			prev = node;
		}
		prev->next = node->next;
	}
	wp->next = free_;
	free_ = wp;
}

WP* find_wp(char *args) {
	Log("find watchpoint.");
	if (args == NULL) {
		return NULL;
	}
	int NO = atoi(args);
	WP* node = head;
	while (node != NULL) {
		if (node->NO == NO) {
			return node;
		}
		node = node->next;
	}
	return NULL;
}

void print_wp() {
	printf("Num\tType\t\tWhat\n");
	WP* node = head;
	while (node != NULL) {
		printf("%d\twatchpoint\t%s\n", node->NO, node->str);
		node = node->next;
	}
}

void exec_wp() {
	Log("execute watchpoint.");
	WP* node = head;
	while (node != NULL) {
		bool b;
		word_t new_val = expr(node->str, &b);
		if (!b) {
			panic("express parse error");
		}
		word_t old_val = node->new_val;
		node->new_val = new_val;
		node->old_val = old_val;
		node = node->next;
	}	
}

bool check_wp() {
	Log("check watchpoint.");
	WP* node = head;
	while (node != NULL) {
		word_t old_val = node->old_val;
		word_t new_val = node->new_val;
		if (old_val != new_val) {
			printf("Hardware watchpoint %d: %s\n", node->NO, node->str);
			printf("Old value = %d\n", old_val);
			printf("New value = %d\n", new_val);
			return true;
		}	
		node = node->next;
	}	
	return false;
}

