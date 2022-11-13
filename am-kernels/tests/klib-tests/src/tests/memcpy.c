#include <string.h>
#include <assert.h>
typedef char uint8_t;
#define N 32
uint8_t data[N];
uint8_t target[N];

void reset() {
	int i;
	for (i = 0; i < N; i++) {
		data[i] = i + 1;
		target[i] = 0;
	}
}

void check_eq(int start, int end) {
	int i;
	for (i = start; i < end; i++) {
		assert(target[i] == data[i]);
	}
}

void test_memcpy() {
	int i, j;
	for (i = 0; i < N; i++) {
		for (j = i + 1; j <= N; j++) {
			reset();
			memcpy(target + i, data + i, j - i);
			check_eq(i + 1, j);
		}
	}
}
