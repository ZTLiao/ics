#include "watchpoint.h"
#include "expr.h"

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
	if (wp == NULL || head == NULL) {
		return;
	}
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

void print_wp() {
	printf("Num\tType\t\tWhat\n");
	WP* node = head;
	while (node != NULL) {
		printf("%d\twatchpoint\t%s\n", node->NO, node->str);
		node = node->next;
	}
}
