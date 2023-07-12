#include <klibtest.h>

uint8_t target[N];

void check_mem(int start, int end) {
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
			check_mem(i + 1, j);
		}
	}
}
